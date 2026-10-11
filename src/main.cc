/**
 * Complejidad Computacional - Práctica 2
 * Simulador de una máquina de Turing multicinta determinista.
 *
 * main.cc: punto de entrada del programa.
 */

#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include "options.h"
#include "tm_parser.h"
#include "turing_machine.h"

namespace {

// Ejecuta la máquina con una cadena y muestra el resultado
void RunInput(const TuringMachine& machine, const std::string& input) {
  std::cout << "'" << input << "': ";
  try {
    const Execution execution = machine.Run(input);
    std::cout << (execution.accepted ? "aceptada" : "no aceptada")
              << " (parada en " << execution.state << " tras "
              << execution.steps << " pasos)\n";
    for (std::size_t i = 0; i < execution.tapes.size(); ++i) {
      std::cout << "  Cinta " << i + 1 << ": " << execution.tapes[i] << "\n";
    }
  } catch (const StepLimitExceeded& error) {
    std::cout << error.what() << "\n";
  } catch (const std::invalid_argument& error) {
    std::cout << "cadena no valida, " << error.what() << "\n";
  }
}

// Lee la máquina y la ejecuta con las cadenas de entrada. Devuelve el código
// de salida del programa
int Run(const Options& options) {
  std::ifstream config(options.config_file());
  if (!config.is_open()) {
    std::cerr << "Error: no se puede abrir el fichero '"
              << options.config_file() << "'\n";
    return 1;
  }

  // Las cadenas se leen del fichero de -in o, si no se indicó, de teclado
  std::ifstream input_file;
  std::istream* input = &std::cin;
  if (!options.input_file().empty()) {
    input_file.open(options.input_file());
    if (!input_file.is_open()) {
      std::cerr << "Error: no se puede abrir el fichero '"
                << options.input_file() << "'\n";
      return 1;
    }
    input = &input_file;
  }

  try {
    const TuringMachine machine = TmParser(config).Parse();

    if (input == &std::cin) {
      std::cout << "Introduzca las cadenas, una por linea (una linea vacia es "
                   "la cadena vacia).\nPara terminar: Ctrl+D en Linux, Ctrl+Z "
                   "y Enter en Windows.\n";
    }
    std::string line;
    while (std::getline(*input, line)) {
      // Ficheros con fin de línea de Windows leídos en Linux
      if (!line.empty() && line.back() == '\r') line.pop_back();
      RunInput(machine, line);
    }
  } catch (const std::exception& error) {
    std::cerr << "Error en '" << options.config_file() << "': " << error.what()
              << "\n";
    return 1;
  }
  return 0;
}

}  // namespace

int main(int argc, char* argv[]) {
  try {
    return Run(Options(argc, argv));
  } catch (const std::invalid_argument& error) {
    std::cerr << "Error: " << error.what() << "\n" << Options::Usage(argv[0]);
    return 1;
  }
}
