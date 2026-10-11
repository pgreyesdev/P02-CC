/**
 * Complejidad Computacional - Práctica 2
 * Simulador de una máquina de Turing multicinta determinista.
 *
 * transition.cc: implementación de la clase Transition.
 */

#include "transition.h"

Transition::Transition(const std::string& from, const std::string& read,
                       const std::string& to, const std::string& write,
                       const std::vector<Move>& moves)
    : from_(from), read_(read), to_(to), write_(write), moves_(moves) {}
