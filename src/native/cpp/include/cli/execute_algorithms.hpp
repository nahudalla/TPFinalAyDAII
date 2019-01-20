#ifndef TPFINALAYDAI_EXECUTE_ALGORITHM_HPP
#define TPFINALAYDAI_EXECUTE_ALGORITHM_HPP

#include <iostream>
#include <string>
#include <boost/filesystem.hpp>

#include <third_party/CLI11.hpp>

#include <cli/constants.hpp>
#include <cli/OutputWriter.hpp>
#include <file_loaders/FileLoader.hpp>
#include <file_loaders/InputParser.hpp>
#include <algor/List.hpp>

namespace fs = boost::filesystem;

namespace cli {
    template <typename Algorithm>
    using TFileLoader = file_loaders::FileLoader<file_loaders::InputParser<Algorithm>>;

    namespace __detail_execute_algorithms {
        template <typename Algorithm>
        void execute(OutputWriter & writer, TFileLoader<Algorithm> const& loader) {
            const auto & contents = loader.getContents();
            const auto & files = loader.getLoadedFiles();

            auto it_contents = contents.begin();
            auto it_files = files.begin();
            auto it_files_end = files.end();

            for(; it_files != it_files_end; ++it_files, ++it_contents) {
                logger::log(
                        logger::flags::INFO_2,
                        std::string("Procesando archivo: ") + it_files->string()
                );

                Algorithm alg(*it_contents);

                auto & out = writer.getOutputStream<Algorithm>(*it_files);

                if(!alg.run().has_value()) {
                    logger::log(logger::flags::WARNING, constants::messages::EMPTY_ALGORITHM_RESULT);
                }

                out << std::boolalpha;

                if(alg.run().has_value()) out << *(alg.run());
                else out << constants::messages::EMPTY_ALGORITHM_RESULT;

                out << std::endl;
            }
        }
    }

    void execute_algorithms(CLI::App const& app, OutputWriter & writer, algor::List<fs::path> const& filenames) {
        using namespace constants;

        using __detail_execute_algorithms::execute;

        bool nothing_to_do = true;

        if(*(app.get_option(graham_scan::SHORT_FLAG))) {
            nothing_to_do = false;
            logger::log(logger::flags::INFO, "Ejecutando algoritmo Graham Scan...");
            execute<algor::GrahamScan>(writer, TFileLoader<algor::GrahamScan>(filenames));
        }

        if(*(app.get_option(jarvis_march::SHORT_FLAG))) {
            nothing_to_do = false;
            logger::log(logger::flags::INFO, "Ejecutando algoritmo Jarvis' March...");
            execute<algor::JarvisMarch>(writer, TFileLoader<algor::JarvisMarch>(filenames));
        }

        if(*(app.get_option(any_segments_intersect::SHORT_FLAG))) {
            nothing_to_do = false;
            logger::log(logger::flags::INFO, "Ejecutando algoritmo interseccion de segmentos...");
            execute<algor::AnySegmentsIntersect>(writer, TFileLoader<algor::AnySegmentsIntersect>(filenames));
        }

        if(*(app.get_option(closest_pair_of_points::SHORT_FLAG))) {
            nothing_to_do = false;
            logger::log(logger::flags::INFO, "Ejecutando algoritmo puntos mas cercanos...");
            execute<algor::ClosestPairOfPoints>(writer, TFileLoader<algor::ClosestPairOfPoints>(filenames));
        }

        if(nothing_to_do) {
            logger::log(logger::flags::WARNING, "No se selecciono ningun algoritmo. Use --ayuda para obtener ayuda");
        }
    }
}

#endif //TPFINALAYDAI_EXECUTE_ALGORITHM_HPP
