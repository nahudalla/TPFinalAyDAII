#ifndef TPFINALAYDAI_TESTSLOADERCOMMON_HPP
#define TPFINALAYDAI_TESTSLOADERCOMMON_HPP

#include <file_loaders/TestCasesLoader.hpp>

template <typename Algorithm>
auto * & __getLoaderPointer() {
    static const file_loaders::TestCasesLoader<Algorithm> * TESTS_LOADER = nullptr;

    return TESTS_LOADER;
}

template <typename Algorithm>
const auto & getTestsLoader() {
    auto * & TESTS_LOADER = __getLoaderPointer<Algorithm>();

    if(TESTS_LOADER == nullptr) {
        TESTS_LOADER = new file_loaders::TestCasesLoader<Algorithm>();
    }

    return *TESTS_LOADER;
}

template <typename Algorithm>
bool cleanupTestsLoader() {
    auto * & TESTS_LOADER = __getLoaderPointer<Algorithm>();

    if(TESTS_LOADER != nullptr) {
        delete TESTS_LOADER;
        TESTS_LOADER = nullptr;
    }

    return true;
}

#endif //TPFINALAYDAI_TESTSLOADERCOMMON_HPP
