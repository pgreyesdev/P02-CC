/**
 * Complejidad Computacional - Práctica 2
 * Simulador de una máquina de Turing multicinta determinista.
 *
 * turing_machine.h: definición formal de la máquina de Turing.
 */

#ifndef TURING_MACHINE_H_
#define TURING_MACHINE_H_

#include <cstddef>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "tape.h"
#include "transition.h"

/**
 * Error que lanza TuringMachine::Run cuando la máquina no se ha parado después
 * de TuringMachine::kMaxSteps pasos.
 */
class StepLimitExceeded : public std::runtime_error {
 public:
  StepLimitExceeded();
};

/**
 * Resultado de ejecutar la máquina con una cadena de entrada.
 */
struct Execution {
  bool accepted;            // la máquina se paró en un estado de F
  std::string state;        // estado en el que se paró
  std::size_t steps;        // transiciones aplicadas
  std::vector<Tape> tapes;  // cintas al parar; el resultado está en la primera
};

/**
 * Máquina de Turing multicinta determinista:
 *   M = (Q, Σ, Γ, s, b, F, δ)
 * Escritura y movimiento simultáneos, movimientos L, R y S, y cintas infinitas
 * en ambas direcciones. Solo se para cuando no hay transición aplicable.
 */
class TuringMachine {
 public:
  // Máximo de pasos que ejecuta Run para una cadena. Evita que la simulación
  // no termine cuando la máquina no se para
  static constexpr std::size_t kMaxSteps = 1000000;

  // Lanza std::invalid_argument si los elementos no cumplen las restricciones
  // de la definición formal o si δ no es determinista
  TuringMachine(const std::set<std::string>& states,
                const std::set<char>& input_alphabet,
                const std::set<char>& tape_alphabet,
                const std::string& initial_state, char blank,
                const std::set<std::string>& final_states,
                std::size_t tape_count,
                const std::vector<Transition>& transitions);

  const std::set<std::string>& states() const { return states_; }
  const std::set<char>& input_alphabet() const { return input_alphabet_; }
  const std::set<char>& tape_alphabet() const { return tape_alphabet_; }
  const std::string& initial_state() const { return initial_state_; }
  char blank() const { return blank_; }
  const std::set<std::string>& final_states() const { return final_states_; }
  std::size_t tape_count() const { return tape_count_; }

  // Ejecuta la máquina con `input` en la primera cinta y la cabeza sobre su
  // primer símbolo. Lanza std::invalid_argument si `input` tiene símbolos que
  // no son de Σ y StepLimitExceeded si se supera kMaxSteps
  Execution Run(const std::string& input) const;

 private:
  void Validate(const Transition& transition) const;

  std::set<std::string> states_;
  std::set<char> input_alphabet_;
  std::set<char> tape_alphabet_;
  std::string initial_state_;
  char blank_;
  std::set<std::string> final_states_;
  std::size_t tape_count_;
  // δ indexada por (estado, símbolos leídos): al ser determinista hay como
  // mucho una transición para cada par
  std::map<std::pair<std::string, std::string>, Transition> transitions_;
};

#endif  // TURING_MACHINE_H_
