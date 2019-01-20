#ifndef TPFINALAYDAI_INIT_UTILS_HPP
#define TPFINALAYDAI_INIT_UTILS_HPP

#include <string>
#include <boost/filesystem.hpp>

#include <third_party/CLI11.hpp>
#include <algor/List.hpp>

namespace fs = boost::filesystem;

namespace cli {
    void translate_labels(CLI::App & app);
    void generate_algorithm_flags(CLI::App & app);
    void generate_input_options(CLI::App &app, algor::List<fs::path> &filesList);
    void generate_output_options(CLI::App &app, std::string &output_dir, std::string &output_file);
}

#endif //TPFINALAYDAI_INIT_UTILS_HPP
