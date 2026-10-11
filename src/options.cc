/**
 * Complejidad Computacional - Práctica 2
 * Simulador de una máquina de Turing multicinta determinista.
 *
 * options.cc: implementación de la clase Options.
 */

#include "options.h"

#include <map>
#include <stdexcept>

Options::Options(int argc, char* argv[]) {
  // Cada opción va seguida de su valor
  std::map<std::string, std::string> values = {{"-config", ""}, {"-in", ""}};
  for (int i = 1; i < argc; i += 2) {
    const std::string option = argv[i];
    const auto found = values.find(option);
    if (found == values.end()) {
      throw std::invalid_argument("opcion desconocida '" + option + "'");
    }
    if (i + 1 >= argc) {
      throw std::invalid_argument("falta el valor de la opcion " + option);
    }
    if (!found->second.empty()) {
      throw std::invalid_argument("la opcion " + option + " esta repetida");
    }
    found->second = argv[i + 1];
    if (found->second.empty()) {
      throw std::invalid_argument("el valor de la opcion " + option +
                                  " esta vacio");
    }
  }

  if (values["-config"].empty()) {
    throw std::invalid_argument("falta la opcion obligatoria -config");
  }

  config_file_ = values["-config"];
  input_file_ = values["-in"];
}

std::string Options::Usage(const std::string& program) {
  return "Uso: " + program +
         " -config <f> [-in <f>]\n"
         "  -config <f>   fichero con la definicion de la maquina de Turing\n"
         "  -in <f>       fichero con las cadenas, una por linea "
         "(por defecto, teclado)\n";
}
