/**
 * Complejidad Computacional - Práctica 2
 * Simulador de una máquina de Turing multicinta determinista.
 *
 * tape.h: una cinta de la máquina con su cabeza de lectura/escritura.
 */

#ifndef TAPE_H_
#define TAPE_H_

#include <cstddef>
#include <deque>
#include <iostream>
#include <string>

// Movimientos de la cabeza de L/E: izquierda (L), derecha (R) y parada (S)
enum class Move { kLeft, kRight, kStay };

/**
 * Cinta infinita en ambas direcciones. Solo se guardan las celdas visitadas o
 * escritas: el resto contiene el símbolo blanco.
 */
class Tape {
 public:
  // La cabeza queda sobre el primer símbolo de `content`, o sobre un blanco
  // si está vacío
  explicit Tape(char blank, const std::string& content = "");

  char Read() const { return cells_[head_]; }
  void Write(char symbol) { cells_[head_] = symbol; }
  void MoveHead(Move move);

  // Contenido de la cinta sin los blancos de los extremos y con el símbolo
  // bajo la cabeza entre corchetes, como "ab[c]d"
  std::string ToString() const;

 private:
  char blank_;
  std::deque<char> cells_;  // nunca está vacía
  std::size_t head_ = 0;    // posición de la cabeza dentro de cells_
};

std::ostream& operator<<(std::ostream& os, const Tape& tape);

#endif  // TAPE_H_
