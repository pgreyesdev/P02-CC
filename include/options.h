/**
 * Complejidad Computacional - Práctica 2
 * Simulador de una máquina de Turing multicinta determinista.
 *
 * options.h: opciones de la línea de comandos.
 */

#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <string>

/**
 * Opciones del programa:
 *   -config <f> [-in <f>]
 */
class Options {
 public:
  // Lanza std::invalid_argument si falta una opción obligatoria o si una
  // opción no tiene valor, está repetida o no existe
  Options(int argc, char* argv[]);

  const std::string& config_file() const { return config_file_; }
  // Vacío si no se indicó -in: las cadenas se leen de teclado
  const std::string& input_file() const { return input_file_; }

  // Texto de ayuda con la forma de invocar el programa
  static std::string Usage(const std::string& program);

 private:
  std::string config_file_;
  std::string input_file_;
};

#endif  // OPTIONS_H_
