#include <string>
#include <boost/filesystem.hpp>

#include <third_party/CLI11.hpp>

#include <algor/List.hpp>

#include <cli/init_utils.hpp>
#include <cli/OutputWriter.hpp>
#include <cli/execute_algorithms.hpp>
#include <cli/logger_setup.hpp>

namespace fs = boost::filesystem;

int main(int argc, char * argv[]) {
    cli::setup_logger();

    CLI::App app {"Esta aplicacion fue desarrollada como trabajo final para la materia Analisis y Diseño de Algoritmos I,\n"
                  "en la carrera Ingenieria de Sistemas, Universidad Nacional del Centro de la Provincia de Buenos Aires,\n"
                  "Argentina. Noviembre/Diciembre de 2018.\n",
                  "TPFinalAyDAI"};

    algor::List<fs::path> input_files;

    cli::translate_labels(app);
    cli::generate_algorithm_flags(app);
    cli::generate_input_options(app, input_files);

    std::string output_dir, output_file;

    cli::generate_output_options(app, output_dir, output_file);

    app.callback([&](){
        cli::OutputWriter writer(output_dir, output_file);

        cli::execute_algorithms(app, writer, input_files);
    });

    CLI11_PARSE(app, argc, argv);

    return 0;
}
