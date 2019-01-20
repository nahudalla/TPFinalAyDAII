#include <string>
#include <utility>
#include <functional>
#include <boost/filesystem.hpp>

#include <cli/init_utils.hpp>
#include <cli/constants.hpp>

#include <algor/Stack.hpp>

namespace fs = boost::filesystem;

std::string validate_input_file(std::string const& filename);
std::string validate_input_directory(std::string const& filename);
std::function<bool(CLI::results_t const&)> gen_file_input_processor(algor::List<fs::path> &);
std::function<bool(CLI::results_t const&)> gen_dir_input_processor(CLI::App &, algor::List<fs::path> &);

namespace cli {
    void translate_labels(CLI::App &app) {
        auto fmt = app.get_formatter();

        app.option_defaults()->group(constants::groups::OPTIONS);

        app.set_help_flag(constants::help::FLAGS, constants::help::DESCRIPTION)
                ->group(constants::help::GROUP);

        app.footer(constants::FOOTER);

        fmt->label("Usage", constants::labels::USAGE);
        fmt->label("OPTIONS", constants::labels::OPTIONS);
        fmt->label("SUBCOMMAND", constants::labels::SUBCOMMAND);
        fmt->label("SUBCOMMANDS", constants::labels::SUBCOMMANDS);
        fmt->label("Positionals", constants::labels::POSITIONALS);
        fmt->label("REQUIRED", constants::labels::REQUIRED);
        fmt->label("Env", constants::labels::ENV);
        fmt->label("Needs", constants::labels::NEEDS);
        fmt->label("Excludes", constants::labels::EXCLUDES);
    }

    void generate_algorithm_flags(CLI::App &app) {
        using namespace cli::constants;

        app.add_flag(graham_scan::FLAGS, graham_scan::DESCRIPTION)
                ->group(graham_scan::GROUP);

        app.add_flag(jarvis_march::FLAGS, jarvis_march::DESCRIPTION)
                ->group(jarvis_march::GROUP);

        app.add_flag(any_segments_intersect::FLAGS, any_segments_intersect::DESCRIPTION)
                ->group(any_segments_intersect::GROUP);

        app.add_flag(closest_pair_of_points::FLAGS, closest_pair_of_points::DESCRIPTION)
                ->group(closest_pair_of_points::GROUP);
    }

    void generate_input_options(CLI::App &app, algor::List<fs::path> &filesList) {
        using namespace constants;

        app.add_option(
                input_file::FLAGS,
                gen_file_input_processor(filesList),
                input_file::DESCRIPTION )

                ->type_size(-1)
                ->expected(-1)
                ->group(input_file::GROUP)
                ->check(validate_input_file)
                ->type_name(input_file::TYPE);

        auto dirOpt = app.add_option(
                input_directory::FLAGS,
                gen_dir_input_processor(app, filesList),
                input_directory::DESCRIPTION )

                ->type_size(-1)
                ->expected(-1)
                ->group(input_directory::GROUP)
                ->check(validate_input_directory)
                ->type_name(input_directory::TYPE);

        app.add_option(
                input_recursive::FLAGS,
                [](const CLI::results_t &){return true;},
                input_recursive::DESCRIPTION,
                true )

                ->group(input_recursive::GROUP)
                ->default_str(input_recursive::DEFAULT_STR)
                ->check(CLI::Range(input_recursive::MIN, input_recursive::MAX))
                ->needs(dirOpt)
                ->type_name(
                        std::string(input_recursive::TYPE)
                        +"["
                        +std::to_string(input_recursive::MIN)
                        +","
                        +std::to_string(input_recursive::MAX)
                        +"]"
                );
    }

    void generate_output_options(CLI::App &app, std::string &output_dir, std::string &output_file) {
        using namespace constants;

        auto dest = app.add_option(
                output_to_dir::FLAGS,
                output_dir,
                output_to_dir::DESCRIPTION )

                ->group(output_to_dir::GROUP)
                ->check(CLI::ExistingDirectory)
                ->type_name(output_to_dir::TYPE);

        auto combined = app.add_option(
                output_combined::FLAGS,
                output_file,
                output_combined::DESCRIPTION )

                ->group(output_combined::GROUP)
                ->check(CLI::NonexistentPath)
                ->type_name(output_combined::TYPE);

        dest->excludes(combined);
        combined->excludes(dest);
    }
}

std::string validate_input_file(std::string const& filename) {
    using namespace cli::constants::messages;
    if(!fs::exists(filename)) return genFileNotFoundError(filename);
    if(!fs::is_regular_file(filename)) return genNotAFileError(filename);

    {
        std::ifstream file(filename);
        if (!file) return genCouldNotOpenFileError(filename);
    }

    if(fs::is_empty(filename)) return genFileIsEmptyError(filename);

    return "";
}

std::string validate_input_directory(std::string const& filename) {
    using namespace cli::constants::messages;
    if(!fs::exists(filename)) return genDirectoryNotFoundError(filename);
    if(!fs::is_directory(filename)) return genNotADirectoryError(filename);
    if(fs::is_empty(filename)) return genDirectoryIsEmptyError(filename);

    return "";
}

std::function<bool(CLI::results_t const &)> gen_file_input_processor(algor::List<fs::path> & filesList) {
    return [&filesList](CLI::results_t const & res)->bool {
        for(const auto & filename : res) {
            filesList.add(fs::path(filename));
        }

        return true;
    };
}

std::function<bool(CLI::results_t const &)> gen_dir_input_processor(CLI::App &app, algor::List<fs::path> &filesList) {
    using namespace cli::constants;

    return [&filesList, &app](CLI::results_t const & res)->bool {
        auto recursive_opt = app.get_option(input_recursive::SHORT_FLAG);
        long long int recursion_depth = 0;

        {
            auto const &results = recursive_opt->results();

            if (!results.empty()) {
                recursion_depth = std::stoll(results.front());
            }
        }

        algor::Stack<std::pair<fs::path, long long int>> directories;

        for(const auto & directory : res) {
            directories.push({fs::path(directory), 0});
        }

        while(!directories.isEmpty()) {
            auto pair = directories.pop();
            auto & directory = pair.first;
            auto & depth = pair.second;

            for(auto & p : fs::directory_iterator(directory)) {
                if(fs::is_regular_file(p.path()) && p.path().extension().compare(fs::path(input_directory::EXTENSION)) == 0) {
                    filesList.add(p.path());
                } else if((recursion_depth == -1 || depth < recursion_depth) && fs::is_directory(p.path())) {
                    directories.push({p.path(), depth+1});
                }
            }
        }

        return true;
    };
}
