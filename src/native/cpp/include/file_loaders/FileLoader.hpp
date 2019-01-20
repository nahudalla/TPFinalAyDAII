#ifndef TPFINALAYDAI_FILELOADER_HPP
#define TPFINALAYDAI_FILELOADER_HPP

#include <file_loaders/InputParser.hpp>
#include <file_loaders/SolutionParser.hpp>

#include <algor/List.hpp>

#include <boost/filesystem.hpp>
#include <fstream>

namespace fs = boost::filesystem;

namespace file_loaders {
    namespace __detail_FileLoader {
        template <typename Parser = void, typename = void>
        struct is_supported_parser;

        template <>
        struct is_supported_parser<void, void> : std::false_type {};

        template <typename Algorithm>
        struct is_supported_parser<
                file_loaders::InputParser<Algorithm>,
                std::enable_if_t<!std::is_same_v<typename file_loaders::InputParser<Algorithm>::InputType, void>>
        > : std::true_type {};

        template <typename Algorithm>
        struct is_supported_parser<
                file_loaders::SolutionParser<Algorithm>,
                std::enable_if_t<!std::is_same_v<typename file_loaders::SolutionParser<Algorithm>::SolutionType, void>>
        > : std::true_type {};

        template <typename Parser>
        constexpr const auto is_supported_parser_v = std::is_base_of_v<std::true_type, is_supported_parser<Parser>>;
    }

    template <typename Parser, typename = std::enable_if_t<__detail_FileLoader::is_supported_parser_v<Parser>>>
    class FileLoader {
        algor::List<typename Parser::ResultType::value_type> results;
        algor::List<fs::path> loaded_files;

    public:
        FileLoader(FileLoader const&) = default;
        FileLoader(FileLoader &&) noexcept = default;
        FileLoader &operator=(FileLoader const&) = default;
        FileLoader &operator=(FileLoader &&) noexcept = default;
        ~FileLoader() = default;

        explicit FileLoader(algor::List<fs::path> const& filenames) {
            auto it = filenames.begin();
            auto end = filenames.end();
            for(; it != end; ++it) {
                std::ifstream file(it->string());

                auto result = Parser::parse(file);

                if(result.has_value()) {
                    this->results.add(std::move(*result));
                    this->loaded_files.add(*it);
                }

                file.close();
            }
        }

        explicit FileLoader(fs::path const& path) : FileLoader(algor::List<fs::path>{path}) {}

        const auto & getLoadedFiles() const {
            return this->loaded_files;
        }

        auto extractLoadedFiles() {
            return std::move(this->loaded_files);
        }

        const auto & getContents() const {
            return this->results;
        }

        auto extractContents() {
            return std::move(this->results);
        }
    };

}

#endif //TPFINALAYDAI_FILELOADER_HPP
