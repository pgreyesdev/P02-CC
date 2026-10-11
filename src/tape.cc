/**
 * Complejidad Computacional - Práctica 2
 * Simulador de una máquina de Turing multicinta determinista.
 *
 * tape.cc: implementación de la clase Tape.
 */

#include "tape.h"

#include <algorithm>

Tape::Tape(char blank, const std::string& content)
    : blank_(blank), cells_(content.begin(), content.end()) {
  if (cells_.empty()) cells_.push_back(blank_);
}

void Tape::MoveHead(Move move) {
  switch (move) {
    case Move::kLeft:
      // La cinta crece por la izquierda y la cabeza sigue en la posición 0
      if (head_ == 0) {
        cells_.push_front(blank_);
      } else {
        --head_;
      }
      break;
    case Move::kRight:
      ++head_;
      if (head_ == cells_.size()) cells_.push_back(blank_);
      break;
    case Move::kStay:
      break;
  }
}

std::string Tape::ToString() const {
  // Tramo que se muestra: desde el primer símbolo no blanco hasta el último,
  // ampliado si hace falta para incluir la celda de la cabeza
  std::size_t first = head_;
  std::size_t last = head_;
  for (std::size_t i = 0; i < cells_.size(); ++i) {
    if (cells_[i] == blank_) continue;
    first = std::min(first, i);
    last = std::max(last, i);
  }

  std::string text;
  for (std::size_t i = first; i <= last; ++i) {
    if (i == head_) {
      text += std::string("[") + cells_[i] + "]";
    } else {
      text += cells_[i];
    }
  }
  return text;
}

std::ostream& operator<<(std::ostream& os, const Tape& tape) {
  return os << tape.ToString();
}
