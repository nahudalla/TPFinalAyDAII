#ifndef TPFINALAYDAI_CONSTANTS_HPP
#define TPFINALAYDAI_CONSTANTS_HPP

#include <string>

#include <algor/ClosestPairOfPoints.hpp>
#include <algor/AnySegmentsIntersect.hpp>
#include <algor/GrahamScan.hpp>
#include <algor/JarvisMarch.hpp>

#define TCONST_NON_STATIC constexpr const char *
#define TCONST static constexpr const char *

namespace cli::constants {
    namespace groups {
        TCONST ALGORITHMS = "Algoritmos";
        TCONST HELP = "Ayuda";
        TCONST OPTIONS = "Opciones";
        TCONST INPUT = "Entrada de datos";
        TCONST OUTPUT = "Salida de resultados";
    }

    namespace types {
        TCONST DIRECTORY = "DIRECTORIO";
        TCONST FILE = "ARCHIVO";
        TCONST INTEGER = "ENTERO";
    }

    namespace help {
        TCONST FLAGS = "-h,--help,--ayuda";
        TCONST DESCRIPTION = "Imprime este mensaje de ayuda";
        TCONST GROUP = groups::HELP;
    }

    namespace graham_scan {
        TCONST FLAGS = "-g,--graham_scan";
        TCONST SHORT_FLAG = "-g";
        TCONST DESCRIPTION = "Aplica el algoritmo Graham Scan al conjunto de entrada";
        TCONST GROUP = groups::ALGORITHMS;
    }

    namespace jarvis_march {
        TCONST FLAGS = "-j,--jarvis_march";
        TCONST SHORT_FLAG = "-j";
        TCONST DESCRIPTION = "Aplica el algoritmo Jarvis March al conjunto de entrada";
        TCONST GROUP = groups::ALGORITHMS;
    }

    namespace any_segments_intersect {
        TCONST FLAGS = "-i,--interseccion_segmentos";
        TCONST SHORT_FLAG = "-i";
        TCONST DESCRIPTION = "Aplica el algoritmo de deteccion de interseccion de segmentos al conjunto de entrada";
        TCONST GROUP = groups::ALGORITHMS;
    }

    namespace closest_pair_of_points {
        TCONST FLAGS = "-p,--puntos_cercanos";
        TCONST SHORT_FLAG = "-p";
        TCONST DESCRIPTION = "Aplica el algoritmo de deteccion de puntos mas cercanos al conjunto de entrada";
        TCONST GROUP = groups::ALGORITHMS;
    }

    namespace input_file {
        TCONST FLAGS = "-a,--archivo";
        TCONST DESCRIPTION = "Lee datos de entrada del archivo";
        TCONST GROUP = groups::INPUT;
        TCONST TYPE = types::FILE;
    }

    namespace input_directory {
        TCONST FLAGS = "-d,--directorio";
        TCONST DESCRIPTION = "Busca archivos de entrada de datos (*.txt) en el directorio";
        TCONST GROUP = groups::INPUT;
        TCONST TYPE = types::DIRECTORY;
        TCONST EXTENSION = ".txt";
    }

    namespace input_recursive {
        TCONST FLAGS = "-r,--recursivo";
        TCONST SHORT_FLAG = "-r";
        TCONST DESCRIPTION = "Profundidad de la busqueda en directorios. Sin recursion: 0 (defecto). Infinita: -1";
        TCONST DEFAULT_STR = "0";
        TCONST GROUP = groups::INPUT;
        TCONST TYPE = types::INTEGER;
        static constexpr const int MIN = -1;
        static constexpr const int MAX = 300;
    }

    namespace output_to_dir {
        TCONST FLAGS = "-D,--destino";
        TCONST DESCRIPTION = "Indica el directorio en el que se deben crear los archivos de soluciones (*.[algoritmo].sol.txt)\n"
                             "\t\t\t\t\t\t\t  Por defecto se crean en el mismo directorio que cada archivo de entrada";
        TCONST GROUP = groups::OUTPUT;
        TCONST TYPE = types::DIRECTORY;
    }

    namespace output_combined {
        TCONST FLAGS = "-c,--combinado";
        TCONST DESCRIPTION = "Escribe en el archivo indicado las salidas de todos los algoritmos, para todas las entradas.\n";
        TCONST GROUP = groups::OUTPUT;
        TCONST TYPE = types::FILE;
    }

    namespace labels {
        TCONST USAGE = "Uso";
        TCONST OPTIONS = "OPCIONES";
        TCONST SUBCOMMAND = "SUBCOMANDO";
        TCONST SUBCOMMANDS = "SUBCOMANDOS";
        TCONST POSITIONALS = "Posicionales";
        TCONST REQUIRED = "REQUERIDO";
        TCONST ENV = "Ent";
        TCONST NEEDS = "Necesita";
        TCONST EXCLUDES = "; No combinable con";
    }

    TCONST FOOTER = "\nDesarrollado por Nahuel Dallacamina Ortea";

    namespace messages {
        TCONST EMPTY_ALGORITHM_RESULT = "No se produjo ningun resultado.";

        std::string genFileNotFoundError(std::string const& filename);
        std::string genNotAFileError(std::string const& filename);
        std::string genFileIsEmptyError(std::string const& filename);

        std::string genCouldNotOpenFileError(std::string const& filename);

        std::string genDirectoryNotFoundError(std::string const& filename);
        std::string genNotADirectoryError(std::string const& filename);
        std::string genDirectoryIsEmptyError(std::string const& filename);
    }

    namespace output {
        TCONST EXTENSION = ".sol.txt";

        template <typename Algorithm = void>
        TCONST_NON_STATIC genAlgorithmName();

        template <typename Algorithm>
        std::string genAlgorithmExtension() {
            return std::string(".") + genAlgorithmName<Algorithm>() + EXTENSION;
        }

        template <>
        TCONST_NON_STATIC genAlgorithmName<algor::GrahamScan>() {
            return "graham_scan";
        }
        template <>
        TCONST_NON_STATIC genAlgorithmName<algor::JarvisMarch>() {
            return "jarvis_march";
        }
        template <>
        TCONST_NON_STATIC genAlgorithmName<algor::AnySegmentsIntersect>() {
            return "segmentos_intersecan";
        }
        template <>
        TCONST_NON_STATIC genAlgorithmName<algor::ClosestPairOfPoints>() {
            return "puntos_mas_cercanos";
        }
    }
}

#undef TCONST
#undef TCONST_NON_STATIC

#endif //TPFINALAYDAI_CONSTANTS_HPP
