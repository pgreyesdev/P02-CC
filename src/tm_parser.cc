/**
 * Complejidad Computacional - Práctica 2
 * Simulador de una máquina de Turing multicinta determinista.
 *
 * tm_parser.cc: implementación de la clase TmParser.
 */

#include "tm_parser.h"

#include <sstream>
#include <stdexcept>

namespace {

const char kComment = '#';

}  // namespace

TmParser::TmParser(std::istream& input) : input_(input) {}

TuringMachine TmParser::Parse() {
  const std::vector<std::string> state_tokens =
      RequireLine("el conjunto de estados Q");
  const std::set<std::string> states(state_tokens.begin(), state_tokens.end());
  const std::set<char> input_alphabet =
      ToSymbols(RequireLine("el alfabeto de entrada Sigma"));
  const std::set<char> tape_alphabet =
      ToSymbols(RequireLine("el alfabeto de la cinta Gamma"));
  const std::string initial_state = RequireSingle("el estado inicial");
  const char blank = ToSymbol(RequireSingle("el simbolo blanco"));
  const std::vector<std::string> final_tokens =
      RequireLine("el conjunto de estados finales F");
  const std::set<std::string> final_states(final_tokens.begin(),
                                           final_tokens.end());
  const std::size_t tape_count =
      ToTapeCount(RequireSingle("el numero de cintas"));

  std::vector<Transition> transitions;
  std::vector<std::string> tokens;
  while (NextLine(tokens)) {
    transitions.push_back(ToTransition(tokens, tape_count));
  }

  return TuringMachine(states, input_alphabet, tape_alphabet, initial_state,
                       blank, final_states, tape_count, transitions);
}

bool TmParser::NextLine(std::vector<std::string>& tokens) {
  std::string line;
  while (std::getline(input_, line)) {
    ++line_number_;
    line = line.substr(0, line.find(kComment));
    std::istringstream words(line);
    tokens.clear();
    std::string word;
    while (words >> word) tokens.push_back(word);
    if (!tokens.empty()) return true;
  }
  return false;
}

std::vector<std::string> TmParser::RequireLine(const std::string& what) {
  std::vector<std::string> tokens;
  if (!NextLine(tokens)) Fail("falta " + what);
  return tokens;
}

std::string TmParser::RequireSingle(const std::string& what) {
  const std::vector<std::string> tokens = RequireLine(what);
  if (tokens.size() != 1) Fail("se esperaba un unico elemento para " + what);
  return tokens.front();
}

std::set<char> TmParser::ToSymbols(
    const std::vector<std::string>& tokens) const {
  std::set<char> symbols;
  for (const std::string& token : tokens) symbols.insert(ToSymbol(token));
  return symbols;
}

char TmParser::ToSymbol(const std::string& token) const {
  if (token.size() != 1) {
    Fail("el simbolo '" + token + "' debe ser un unico caracter");
  }
  return token.front();
}

std::size_t TmParser::ToTapeCount(const std::string& token) const {
  // Como mucho tres cifras, para que la conversión no se desborde
  if (token.size() > 3 ||
      token.find_first_not_of("0123456789") != std::string::npos ||
      std::stoul(token) == 0) {
    Fail("el numero de cintas '" + token + "' debe ser un entero positivo");
  }
  return std::stoul(token);
}

Move TmParser::ToMove(const std::string& token) const {
  if (token == "L") return Move::kLeft;
  if (token == "R") return Move::kRight;
  if (token == "S") return Move::kStay;
  Fail("el movimiento '" + token + "' debe ser L, R o S");
}

Transition TmParser::ToTransition(const std::vector<std::string>& tokens,
                                  std::size_t tape_count) const {
  // Estado, k símbolos leídos, estado y k pares de símbolo y movimiento
  if (tokens.size() != 2 + 3 * tape_count) {
    Fail("una transicion con " + std::to_string(tape_count) +
         " cinta(s) debe tener " + std::to_string(2 + 3 * tape_count) +
         " elementos: q1 r1 ... rk q2 w1 m1 ... wk mk");
  }
  std::string read;
  for (std::size_t i = 0; i < tape_count; ++i) {
    read += ToSymbol(tokens[1 + i]);
  }
  std::string write;
  std::vector<Move> moves;
  for (std::size_t i = 0; i < tape_count; ++i) {
    write += ToSymbol(tokens[2 + tape_count + 2 * i]);
    moves.push_back(ToMove(tokens[3 + tape_count + 2 * i]));
  }
  return Transition(tokens[0], read, tokens[1 + tape_count], write, moves);
}

void TmParser::Fail(const std::string& message) const {
  throw std::runtime_error("linea " + std::to_string(line_number_) + ": " +
                           message);
}
