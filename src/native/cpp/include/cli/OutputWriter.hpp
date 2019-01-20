#ifndef TPFINALAYDAI_OUTPUTWRITER_HPP
#define TPFINALAYDAI_OUTPUTWRITER_HPP

#include <optional>
#include <string>
#include <fstream>
#include <boost/filesystem.hpp>

#include <cli/constants.hpp>
#include <logger/Logger.hpp>

namespace fs = boost::filesystem;

namespace cli {
    namespace __detail_OutputWriter {
        class NullBuffer : public std::streambuf {
        public:
            int overflow(int c) override { return c; }
        };

        auto & getNullOstream() {
            static NullBuffer nullBuffer;
            static std::ostream null_stream(&nullBuffer);

            return null_stream;
        }
    }

    class OutputWriter {
        std::optional<std::ofstream> out_file;
        std::optional<std::string> out_dir;
        std::optional<std::ofstream> current_output_file;

    public:
        OutputWriter(
                std::string const& out_dir,
                std::string const& out_file
        ) {
            if(!out_dir.empty()) {
                this->out_dir = out_dir;
            } else if(!out_file.empty()) {
                std::ofstream file(out_file);

                if(!file) {
                    logger::log(
                            logger::flags::CRITICAL,
                            std::string("No se pudo abrir el archivo de salida: ") + out_file
                    );
                    return;
                }

                this->out_file = std::move(file);
            }
        }

        template <typename Algorithm>
        std::ostream & getOutputStream(fs::path const& input_file) {
            if(this->out_file.has_value()) {
                auto & out = *(this->out_file);

                logger::log(
                        logger::flags::INFO_3,
                        std::string("Escribiendo en salida combinada")
                );

                out << constants::output::genAlgorithmName<Algorithm>()
                    << " " << input_file << std::endl;

                return out;
            } else {
                fs::path out_path;

                if(this->out_dir.has_value()) {
                    out_path = *(this->out_dir);

                    out_path /= input_file.filename().stem().concat(
                            constants::output::genAlgorithmExtension<Algorithm>()
                    );
                } else {
                    out_path = input_file.parent_path() / input_file.stem().concat(
                            constants::output::genAlgorithmExtension<Algorithm>()
                    );
                }

                logger::log(
                        logger::flags::INFO_3,
                        std::string("Escribiendo salida en: ") + out_path.string()
                );

                if(fs::exists(out_path)) {
                    logger::log(
                            logger::flags::ERROR,
                            std::string("Archivo de salida ya existe: ") + out_path.string()
                    );

                    return __detail_OutputWriter::getNullOstream();
                }

                std::ofstream file(out_path.string());

                if(!file) {
                    logger::log(
                            logger::flags::ERROR,
                            std::string("No se pudo abrir el archivo para escritura: ") + out_path.string()
                    );

                    return __detail_OutputWriter::getNullOstream();
                }

                this->current_output_file = std::move(file);

                return *(this->current_output_file);
            }
        }
    };
}

#endif //TPFINALAYDAI_OUTPUTWRITER_HPP
