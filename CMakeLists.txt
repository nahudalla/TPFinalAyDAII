cmake_minimum_required(VERSION 3.12)

if(USE_EMSCRIPTEN_TOOLCHAIN)
    message("Looking for Emscripten path using SDK...")
    execute_process(
            COMMAND bash -c ". ~/emsdk/emsdk_env.sh > /dev/null && echo -n $EMSCRIPTEN"
            OUTPUT_VARIABLE EMSCRIPTEN_ROOT
    )
    if("${EMSCRIPTEN_ROOT}" STREQUAL "")
        message(FATAL_ERROR "Emscripten not found. Please install Emscripten SDK first.")
    else()
        message("Emscripten root found: ${EMSCRIPTEN_ROOT}")
        message("Setting up Emscripten toolchain...")
        set(CMAKE_TOOLCHAIN_FILE ${EMSCRIPTEN_ROOT}/cmake/Modules/Platform/Emscripten.cmake)
        string(CONCAT CMAKE_CXX_FLAGS
                " --bind"
                " -fno-exceptions"
                " -s WASM=1"
                " -s ENVIRONMENT=worker"
                " -s MODULARIZE=1"
                " -s EXIT_RUNTIME=0"
                " -s FILESYSTEM=0"
                " -s EXPORT_NAME=WASMModule"
                " -s ALLOW_MEMORY_GROWTH=1"
                " -s \"EXTRA_EXPORTED_RUNTIME_METHODS=['ccall']\""
        )
    endif()
endif()

project(TPFinalAyDAII)

if(NOT USE_EMSCRIPTEN_TOOLCHAIN)
    find_package( Boost COMPONENTS system filesystem REQUIRED )
    include_directories( ${Boost_INCLUDE_DIR} )
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

include_directories(
        src/native/cpp/include
)

set(SOURCE_FILES
        src/native/cpp/algor/Vector.cpp
        src/native/cpp/algor/ClosestPairOfPoints_impl/algorithm.cpp
        src/native/cpp/test_cases/TestCasesLoader_impl/ParserShared.cpp
        src/native/cpp/logger/Logger.cpp
        src/native/cpp/logger/Logger_impl/Message.cpp
)

set(TESTS_FILES
        src/native/cpp/tests/algor/Array.cpp
        src/native/cpp/tests/algor/List.cpp
        src/native/cpp/tests/algor/Stack.cpp
        src/native/cpp/tests/algor/Point.cpp
        src/native/cpp/tests/algor/Vector.cpp
        src/native/cpp/tests/algor/Segment.cpp
        src/native/cpp/tests/algor/RBTree.cpp

        src/native/cpp/tests/algor/AnySegmentsIntersect.cpp
        src/native/cpp/tests/algor/GrahamScan_JarvisMarch.cpp
        src/native/cpp/tests/algor/ClosestPairOfPoints.cpp
)

set(CLI_SOURCE_FILES
        src/native/cpp/cli/init_utils.cpp
        src/native/cpp/cli/constants.cpp
        src/native/cpp/cli/logger_setup.cpp
)

set(WASM_SOURCE_FILES
        src/native/cpp/wasm_bindings/data_input.cpp
        src/native/cpp/wasm_bindings/errors.cpp
        src/native/cpp/wasm_bindings/graham_scan.cpp
        src/native/cpp/wasm_bindings/utils.cpp
        src/native/cpp/wasm_bindings/jarvis_march.cpp
        src/native/cpp/wasm_bindings/closest_points.cpp
        src/native/cpp/wasm_bindings/any_segments_intersect.cpp
)

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Emscripten")
    set(WASM_MODULE_NAME WASMModule)

    string(TOLOWER "${CMAKE_BUILD_TYPE}" WASM_BUILD_TYPE)

    set(WASM_BUILD_DIR ${PROJECT_SOURCE_DIR}/web-build-${WASM_BUILD_TYPE}/wasm)

    add_executable(${WASM_MODULE_NAME} ${SOURCE_FILES} ${WASM_SOURCE_FILES})
    add_custom_command(
            TARGET ${WASM_MODULE_NAME}
            POST_BUILD
            COMMAND mkdir ARGS -p ${WASM_BUILD_DIR}
            COMMAND cp ARGS -f -t ${WASM_BUILD_DIR} ${CMAKE_BINARY_DIR}/${WASM_MODULE_NAME}.js ${CMAKE_BINARY_DIR}/${WASM_MODULE_NAME}.wasm
            COMMENT "Built files copied to: ${WASM_BUILD_DIR}"
    )

    if(WASM_BUILD_TYPE MATCHES "^(release|debug)$")
        add_custom_command(
                TARGET ${WASM_MODULE_NAME}
                POST_BUILD
                COMMAND npm ARGS run web-build-${WASM_BUILD_TYPE}
                COMMENT "Building web..."
        )
    endif()
else()
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")

    add_executable(TPFinalAyDAII ${SOURCE_FILES} ${CLI_SOURCE_FILES} src/native/cpp/cli.cpp)

    add_executable(Tests
            src/native/cpp/tests/impl/tests_init.cpp
            ${SOURCE_FILES}
            ${TESTS_FILES}
            src/native/cpp/tests/impl/tests.cpp
    )

    target_link_libraries(TPFinalAyDAII
            ${Boost_LIBRARIES}
    )
    target_link_libraries(Tests
            ${Boost_LIBRARIES}
    )
endif()