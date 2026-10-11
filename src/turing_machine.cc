/**
 * Complejidad Computacional - Práctica 2
 * Simulador de una máquina de Turing multicinta determinista.
 *
 * turing_machine.cc: implementación de la clase TuringMachine.
 */

#include "turing_machine.h"

StepLimitExceeded::StepLimitExceeded()
    : std::runtime_error("no se pudo decidir, se supero el limite de " +
                         std::to_string(TuringMachine::kMaxSteps) + " pasos") {}

TuringMachine::TuringMachine(const std::set<std::string>& states,
                             const std::set<char>& input_alphabet,
                             const std::set<char>& tape_alphabet,
                             const std::string& initial_state, char blank,
                             const std::set<std::string>& final_states,
                             std::size_t tape_count,
                             const std::vector<Transition>& transitions)
    : states_(states),
      input_alphabet_(input_alphabet),
      tape_alphabet_(tape_alphabet),
      initial_state_(initial_state),
      blank_(blank),
      final_states_(final_states),
      tape_count_(tape_count) {
  if (states_.empty()) {
    throw std::invalid_argument("el conjunto de estados Q esta vacio");
  }
  if (states_.count(initial_state_) == 0) {
    throw std::invalid_argument("el estado inicial '" + initial_state_ +
                                "' no pertenece a Q");
  }
  for (const std::string& state : final_states_) {
    if (states_.count(state) == 0) {
      throw std::invalid_argument("el estado final '" + state +
                                  "' no pertenece a Q");
    }
  }
  for (char symbol : input_alphabet_) {
    if (tape_alphabet_.count(symbol) == 0) {
      throw std::invalid_argument(std::string("el simbolo '") + symbol +
                                  "' de Sigma no pertenece a Gamma");
    }
  }
  if (tape_alphabet_.count(blank_) == 0) {
    throw std::invalid_argument(std::string("el simbolo blanco '") + blank_ +
                                "' no pertenece a Gamma");
  }
  if (input_alphabet_.count(blank_) != 0) {
    throw std::invalid_argument(std::string("el simbolo blanco '") + blank_ +
                                "' no puede pertenecer a Sigma");
  }
  if (tape_count_ == 0) {
    throw std::invalid_argument("el numero de cintas debe ser al menos 1");
  }

  for (const Transition& transition : transitions) {
    Validate(transition);
    const auto key = std::make_pair(transition.from(), transition.read());
    if (!transitions_.emplace(key, transition).second) {
      throw std::invalid_argument("la maquina no es determinista: hay mas de "
                                  "una transicion para (" + transition.from() +
                                  ", " + transition.read() + ")");
    }
  }
}

Execution TuringMachine::Run(const std::string& input) const {
  for (char symbol : input) {
    if (input_alphabet_.count(symbol) == 0) {
      throw std::invalid_argument(std::string("el simbolo '") + symbol +
                                  "' no pertenece a Sigma");
    }
  }

  // La entrada está solo en la primera cinta; las demás empiezan en blanco
  std::vector<Tape> tapes(tape_count_, Tape(blank_));
  tapes.front() = Tape(blank_, input);
  std::string state = initial_state_;
  std::size_t steps = 0;
  while (true) {
    std::string read;
    for (const Tape& tape : tapes) read += tape.Read();
    const auto found = transitions_.find({state, read});
    // La máquina se para cuando no hay transición aplicable
    if (found == transitions_.end()) break;
    if (steps == kMaxSteps) throw StepLimitExceeded();

    const Transition& transition = found->second;
    for (std::size_t i = 0; i < tapes.size(); ++i) {
      tapes[i].Write(transition.write()[i]);
      tapes[i].MoveHead(transition.moves()[i]);
    }
    state = transition.to();
    ++steps;
  }
  return {final_states_.count(state) != 0, state, steps, tapes};
}

void TuringMachine::Validate(const Transition& transition) const {
  for (const std::string& state : {transition.from(), transition.to()}) {
    if (states_.count(state) == 0) {
      throw std::invalid_argument("el estado '" + state +
                                  "' de una transicion no pertenece a Q");
    }
  }
  if (transition.read().size() != tape_count_ ||
      transition.write().size() != tape_count_ ||
      transition.moves().size() != tape_count_) {
    throw std::invalid_argument("una transicion desde '" + transition.from() +
                                "' no tiene un simbolo leido, un simbolo "
                                "escrito y un movimiento por cada cinta");
  }
  for (char symbol : transition.read() + transition.write()) {
    if (tape_alphabet_.count(symbol) == 0) {
      throw std::invalid_argument(std::string("el simbolo '") + symbol +
                                  "' de una transicion no pertenece a Gamma");
    }
  }
}
