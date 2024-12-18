#include <cstdlib>
#include <iostream>
using namespace std;

const char humanoPeon = 'b';
const char iaPeon = 'n';
const char humanoDama = 'B';
const char iaDama = 'N';

enum Jugador { HUMANO, IA };

struct Movimiento {
  int f1; // fila de origen.
  int c1; // columna de origen.
  int f2; // fila de destino.
  int c2; // columna de destino.
  int p;  // guarda el puntaje de capturas y movimientos simples de la jugada en
          // la funcion minimax.
};

class Juego {
  char tablero[10][10] = {
      {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
      {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
      {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
      {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
      {' ', ' ', 'b', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
      {' ', ' ', ' ', ' ', ' ', 'n', ' ', ' ', ' ', 'B'},
      {' ', ' ', ' ', ' ', 'n', ' ', ' ', ' ', ' ', ' '},
      {' ', ' ', ' ', 'n', ' ', ' ', ' ', ' ', ' ', ' '},
      {' ', ' ', 'n', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
      {' ', 'N', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
  };

public:
  Juego();
  void imprimirTablero(char tablero[10][10]); //
  // Builder
  void humanoMueve();                                                 //
  bool chequearGanador(Jugador, char tablero[10][10]);                //
  bool chequearMovimiento(Jugador, Movimiento, char tablero[10][10]); //
  void mover(Movimiento, char tablero[10][10]);                       //
  bool profundidad(int pro);
  // Composite
  void multisalto(Movimiento, char tablero[10][10]); //
  void sopladita(Movimiento, char tablero[10][10]);  //
  // Strategy
  Movimiento minimax(char tablero[10][10]);              //
  int min(char tablero[10][10], Movimiento mv, int pro); //
  int max(char tablero[10][10], Movimiento mv, int pro); //
  void jugar();                                          //
};

#ifndef JUEGO_H
#define JUEGO_H

#endif /* JUEGO_H */
