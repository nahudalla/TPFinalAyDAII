#ifndef TPFINALAYDAI_TESTCASESLOADER_HPP
#define TPFINALAYDAI_TESTCASESLOADER_HPP

#include <iostream>

#include <type_traits>
#include <fstream>
#include <boost/filesystem.hpp>

#include <file_loaders/TestCasesLoader_impl/SolutionDirNames.hpp>
#include <file_loaders/InputParser.hpp>
#include <file_loaders/SolutionParser.hpp>
#include <file_loaders/FileLoader.hpp>

#include <algor/List.hpp>

namespace fs = boost::filesystem;

namespace file_loaders {
#if !defined DEBUG && !defined NDEBUG
    static constexpr const char * DEFAULT_TEST_CASES_DIRECTORY = "./test_cases";
#else
    static constexpr const char * DEFAULT_TEST_CASES_DIRECTORY = "../test_cases";
#endif

#ifdef __EMSCRIPTEN__
    static_assert(false, "Emscripten target not supported for TestCasesLoader class!");
#endif

    template <typename Algorithm>
    class TestCasesLoader {
        using TestCaseParser = InputParser<Algorithm>;
        using SolutionParser = SolutionParser<Algorithm>;

        using TestCaseType = typename TestCaseParser::InputType;
        using SolutionType = typename SolutionParser::SolutionType;

        static constexpr const char * SOLUTIONS_DIR_NAME = __detail_TestCasesLoader::SolutionDirName<Algorithm>::DIR_NAME;

        static_assert(
                !std::is_same_v<TestCaseType , void>
                && !std::is_same_v<SolutionType , void>
                && SOLUTIONS_DIR_NAME != nullptr
                , "Unrecognized algorithm type!"
        );

        algor::List<typename TestCaseType::value_type> test_cases;
        algor::List<typename SolutionType::value_type> solutions;

    public:
        explicit TestCasesLoader(const char * directory) {
            if(directory == nullptr) directory = DEFAULT_TEST_CASES_DIRECTORY;

            fs::path dir_path(directory);

            if(!fs::exists(dir_path)) {
                std::cerr << "ERROR: Test case directory does not exist!: " << directory;
                return;
            }

            algor::List<fs::path> test_case_paths;

            for (auto & p : fs::directory_iterator(directory)) {
                const auto & test_case_path = p.path();
                if(!fs::is_regular_file(p) || test_case_path.extension().compare(fs::path(".txt")) != 0) continue;

                test_case_paths.add(test_case_path);
            }

            FileLoader<TestCaseParser> loader(test_case_paths);

            auto loaded = loader.extractLoadedFiles();
            this->test_cases = loader.extractContents();

            using ItTestCases = typename decltype(this->test_cases)::Iterator;

            auto it_loaded = loaded.begin();
            this->test_cases.removeIf([this, &it_loaded](const ItTestCases &, const ItTestCases &)->bool {
                fs::path solution_path = it_loaded->parent_path() / SOLUTIONS_DIR_NAME / it_loaded->stem() . concat(".sol.txt");

                ++it_loaded;

                FileLoader<SolutionParser> loader1(solution_path);
                auto res = loader1.extractContents();

                if(res.isEmpty()) return true;

                this->solutions.add(res.remove(res.begin()));

                return false;
            });
        }

        TestCasesLoader() : TestCasesLoader(DEFAULT_TEST_CASES_DIRECTORY)
        {}

        TestCasesLoader(TestCasesLoader &&) noexcept = default;
        TestCasesLoader &operator=(TestCasesLoader &&) noexcept = default;
        TestCasesLoader(TestCasesLoader const&) = default;
        TestCasesLoader &operator=(TestCasesLoader const&) = default;
        ~TestCasesLoader() = default;

        const auto & getTestCases() const {
            return this->test_cases;
        }

        const auto & getSolutions() const {
            return this->solutions;
        }
    };
}

#endif //TPFINALAYDAI_TESTCASESLOADER_HPP
