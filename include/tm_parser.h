/**
 * Complejidad Computacional - Práctica 2
 * Simulador de una máquina de Turing multicinta determinista.
 *
 * tm_parser.h: lectura de la definición de una máquina desde un fichero.
 */

#ifndef TM_PARSER_H_
#define TM_PARSER_H_

#include <cstddef>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "turing_machine.h"

/**
 * Lee la definición de una MT con el formato:
 *   # Comentarios
 *   q1 q2 q3 ...     # conjunto Q
 *   a1 a2 a3 ...     # conjunto Σ
 *   A1 A2 A3 ...     # conjunto Γ
 *   q1               # estado inicial
 *   b                # símbolo blanco
 *   q2 q3            # conjunto F
 *   k                # número de cintas
 *   q1 r1 ... rk q2 w1 m1 ... wk mk    # una transición por línea
 * donde ri es el símbolo leído en la cinta i, wi el que se escribe y mi el
 * movimiento de su cabeza (L, R o S).
 */
class TmParser {
 public:
  explicit TmParser(std::istream& input);

  // Lanza std::runtime_error si el formato no es correcto y
  // std::invalid_argument si la máquina leída no es válida
  TuringMachine Parse();

 private:
  // Lee la siguiente línea con contenido, sin comentarios y separada en
  // palabras. Devuelve false si se ha llegado al final de la entrada
  bool NextLine(std::vector<std::string>& tokens);

  // Como NextLine, pero es un error que no queden líneas. `what` describe lo
  // que se esperaba leer
  std::vector<std::string> RequireLine(const std::string& what);

  // Igual que RequireLine, pero la línea debe tener una única palabra
  std::string RequireSingle(const std::string& what);

  std::set<char> ToSymbols(const std::vector<std::string>& tokens) const;
  char ToSymbol(const std::string& token) const;
  std::size_t ToTapeCount(const std::string& token) const;
  Move ToMove(const std::string& token) const;
  Transition ToTransition(const std::vector<std::string>& tokens,
                          std::size_t tape_count) const;

  [[noreturn]] void Fail(const std::string& message) const;

  std::istream& input_;
  int line_number_ = 0;  // línea actual, para los mensajes de error
};

#endif  // TM_PARSER_H_
