/**
 * Complejidad Computacional - Práctica 2
 * Simulador de una máquina de Turing multicinta determinista.
 *
 * transition.h: una transición de la función δ.
 */

#ifndef TRANSITION_H_
#define TRANSITION_H_

#include <string>
#include <vector>

#include "tape.h"

/**
 * Representa un elemento de la función de transición de una MT con k cintas:
 *   δ(from, r1 ... rk) = (to, w1 m1 ... wk mk)
 * En cada cinta la escritura y el movimiento se hacen en el mismo paso.
 */
class Transition {
 public:
  Transition(const std::string& from, const std::string& read,
             const std::string& to, const std::string& write,
             const std::vector<Move>& moves);

  const std::string& from() const { return from_; }
  const std::string& read() const { return read_; }
  const std::string& to() const { return to_; }
  const std::string& write() const { return write_; }
  const std::vector<Move>& moves() const { return moves_; }

 private:
  std::string from_;         // estado de partida
  std::string read_;         // símbolo de Γ leído en cada cinta
  std::string to_;           // estado de llegada
  std::string write_;        // símbolo de Γ que se escribe en cada cinta
  std::vector<Move> moves_;  // movimiento de la cabeza de cada cinta
};

#endif  // TRANSITION_H_
