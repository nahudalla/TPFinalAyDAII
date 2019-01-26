#include <string>
#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>

using namespace emscripten;

val EMSCRIPTEN_KEEPALIVE run_graham_scan(int coordinates_ptr, int length, val const& progress) {
    auto coordinates = reinterpret_cast<int*>(coordinates_ptr);

    if (progress.typeOf().as<std::string>() != "function") {
        return val::global("Error").new_(val("Invalid progress callback, must be a function."));
    }

    auto msg = std::string("Received ") + std::to_string(length / 2) + " elements.\n";
    for (unsigned long i = 0; i < length - 1; i += 2) {
        msg += std::string("(") + std::to_string(coordinates[i]) + "," + std::to_string(coordinates[i + 1]) + ") ";
    }

    progress(msg);

    int * ptr = (int*) malloc(sizeof(int)*2);

    if(ptr == nullptr) return val::global("Error").new_(val("Could not allocate memory for return value!"));

    ptr[0] = coordinates[0];
    ptr[1] = coordinates[1];

    auto retObj = val::object();
    retObj.set(val("ptr"), val(reinterpret_cast<long>(ptr)));
    retObj.set(val("length"), val(2));
    retObj.set(val("algorithm"),val("graham_scan"));

    return std::move(retObj);
}

val EMSCRIPTEN_KEEPALIVE run_jarvis_march(int coordinates, int length, val const& progress) {
    auto ret = run_graham_scan(coordinates, length, progress);
    ret.set(val("algorithm"), val("jarvis_march"));
    return std::move(ret);
}

val EMSCRIPTEN_KEEPALIVE run_closest_points(int coordinates, int length, val const& progress) {
    auto ret = run_graham_scan(coordinates, length, progress);
    ret.set(val("algorithm"), val("closest_points"));
    return std::move(ret);
}

val EMSCRIPTEN_KEEPALIVE run_any_segments_intersect(int coordinates, int length, val const& progress) {
    auto ret = run_graham_scan(coordinates, length, progress);
    ret.set(val("algorithm"), val("any_segments_intersect"));
    return std::move(ret);
}

EMSCRIPTEN_BINDINGS(algorithm_exports) {
    function("run_graham_scan", &run_graham_scan, allow_raw_pointers());
    function("run_jarvis_march", &run_jarvis_march, allow_raw_pointers());
    function("run_closest_points", &run_closest_points, allow_raw_pointers());
    function("run_any_segments_intersect", &run_any_segments_intersect, allow_raw_pointers());
}
