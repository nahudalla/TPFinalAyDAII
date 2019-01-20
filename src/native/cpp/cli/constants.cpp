#include <cli/constants.hpp>

namespace cli::constants {
    namespace messages {
        std::string genFileNotFoundError(std::string const& filename) {
            return std::string("Archivo no existente: ") + filename;
        }
        std::string genNotAFileError(std::string const& filename) {
            return std::string("No es un archivo: ") + filename;
        }
        std::string genFileIsEmptyError(std::string const& filename) {
            return std::string("El archivo esta vacio: ") + filename;
        }

        std::string genCouldNotOpenFileError(std::string const& filename) {
            return std::string("No se pudo abrir el archivo: ") + filename;
        }

        std::string genDirectoryNotFoundError(std::string const& filename) {
            return std::string("Directorio no existente: ") + filename;
        }
        std::string genNotADirectoryError(std::string const& filename) {
            return std::string("No es un directorio: ") + filename;
        }
        std::string genDirectoryIsEmptyError(std::string const& filename) {
            return std::string("El directorio esta vacio: ") + filename;
        }
    }
}
