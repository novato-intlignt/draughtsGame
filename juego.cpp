#include "./lib/juego.h"
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <string.h>

using namespace std;

bool salto =
    false; // switch que avisa a las otras funciones que se realiza un salto.
bool mnsj_err = false; // switch que permite imprimir mensajes de error solo
                       // cuando el humano mueve.
bool ia = false;       // switch para evitar acciones no deseadas cuando juega
                       // minimax.

Juego::Juego() {

  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {

      tablero[i][j] = ' ';
    }
  }

  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {

      if (j % 2 == 1) {
        tablero[0][j] = iaPeon;
        tablero[2][j] = iaPeon;
      }
      if (j % 2 == 0) {
        tablero[1][j] = iaPeon;
      }
      if (j % 2 == 0) {
        tablero[7][j] = humanoPeon;
        tablero[9][j] = humanoPeon;
      }
      if (j % 2 == 1) {
        tablero[8][j] = humanoPeon;
      }
    }
  }
}

bool Juego::chequearMovimiento(Jugador jugador, Movimiento mv,
                               char TABLERO[10][10]) {

  char dama, peon;
  // No le permite al jugador mover una pieza contraria.
  if (jugador == IA) {
    dama = iaDama;
    peon = iaPeon;
  } else if (jugador == HUMANO) {
    dama = humanoDama;
    peon = humanoPeon;
  }
  // Se asegura que los parametros sean validos.
  if (mv.f1 < 0 || mv.f1 > 9 || mv.c1 < 0 || mv.c1 > 9) {
    if (mnsj_err == true) {
      cout << "Parametros incorrectos.\n";
    }
    return false;
  }
  if (mv.f2 < 0 || mv.f2 > 9 || mv.c2 < 0 || mv.c2 > 9) {
    if (mnsj_err == true) {
      cout << "Parametros incorrectos.\n";
    }
    return false;
  }

  // Retorna falso si la casilla destino está ocupada.
  if (TABLERO[mv.f2][mv.c2] != ' ') {
    if (mnsj_err == true) {
      cout << "La casilla destino está ocupada.\n";
    }
    return false;
  }

  // Retorna falso si la pieza a mover es vacio.
  if (TABLERO[mv.f1][mv.c1] == ' ') {
    if (mnsj_err == true) {
      cout << "La pieza a mover no existe\n";
    }
    return false;
  }

  // Retorna falso si la pieza se está moviendo vertical u horizontalmente.
  if (mv.c1 == mv.c2 || mv.f1 == mv.f2) {
    if (mnsj_err == true) {
      cout << "No puede mover las piezas ni vertical ni horizontalmente\n";
    }
    return false;
  }

  // Reglas para peones
  if (TABLERO[mv.f1][mv.c1] == peon) {

    // Retorna falso si un peon retrocede.
    if ((jugador == IA && mv.f2 < mv.f1) ||
        (jugador == HUMANO && mv.f2 > mv.f1)) {
      if (mnsj_err == true) {
        cout << "No puedes retroceder\n";
      }
      return false;
    }

    // Retorna verdadero si se mueve solo una casilla.
    if ((mv.f2 == mv.f1 + 1 || mv.f2 == mv.f1 - 1) &&
        (mv.c2 == mv.c1 + 1 || mv.c2 == mv.c1 - 1)) {
      return true;
    }

    else { // Reglas para saltos con peones.

      // Retorna falso si el peón no se esta moviendo exactamente dos filas.
      //(solo necesita que se cumpla una igualdad para NO retornar falso)
      if (mv.f2 != mv.f1 + 2 && mv.f2 != mv.f1 - 2) {
        if (mnsj_err == true) {
          cout << "No puedes moverte mas de dos filas \n";
        }
        return false;
      }

      // Retorna falso si el peón no se esta moviendo exactamente dos columnas.
      //(solo necesita que se cumpla una igualdad para NO retornar falso)
      if (mv.c2 != mv.c1 + 2 && mv.c2 != mv.c1 - 2) {
        if (mnsj_err == true) {
          cout << "No puedes moverte mas de dos columnas\n";
        }
        return false;
      }

      // Retorna falso si intenta saltar sin haber una pieza que se este
      // saltando.
      if (mv.f2 > mv.f1 && mv.c2 > mv.c1) {
        if (TABLERO[mv.f2 - 1][mv.c2 - 1] == ' ') {
          if (mnsj_err == true) {
            cout << "No puedes moverte mas de una casillas sin saltar\n";
          }
          return false;
        }
      }

      else if (mv.f2 > mv.f1 && mv.c2 < mv.c1) {
        if (TABLERO[mv.f2 - 1][mv.c2 + 1] == ' ') {
          if (mnsj_err == true) {
            cout << "No puedes moverte mas de una casillas sin saltar\n";
          }
          return false;
        }
      }

      else if (mv.f2 < mv.f1 && mv.c2 > mv.c1) {
        if (TABLERO[mv.f2 + 1][mv.c2 - 1] == ' ') {
          if (mnsj_err == true) {
            cout << "No puedes moverte mas de una casillas sin saltar\n";
          }
          return false;
        }
      }

      else if (mv.f2 < mv.f1 && mv.c2 < mv.c1) {
        if (TABLERO[mv.f2 + 1][mv.c2 + 1] == ' ') {
          if (mnsj_err == true) {
            cout << "No puedes moverte mas de una casillas sin saltar\n";
          }
          return false;
        }
      }

      // Retorna falso si salta a uno de sus compañeros.
      if (mv.f2 > mv.f1 && mv.c2 > mv.c1) {
        if ((TABLERO[mv.f2 - 1][mv.c2 - 1] == dama) ||
            (TABLERO[mv.f2 - 1][mv.c2 - 1] == peon)) {
          if (mnsj_err == true) {
            cout << "No puedes saltar a un compañero\n";
          }
          return false;
        }
      }

      else if (mv.f2 > mv.f1 && mv.c2 < mv.c1) {
        if ((TABLERO[mv.f2 - 1][mv.c2 + 1] == dama) ||
            (TABLERO[mv.f2 - 1][mv.c2 + 1] == peon)) {
          if (mnsj_err == true) {
            cout << "No puedes saltar a un compañero\n";
          }
          return false;
        }
      }

      else if (mv.f2 < mv.f1 && mv.c2 > mv.c1) {
        if ((TABLERO[mv.f2 + 1][mv.c2 - 1] == dama) ||
            (TABLERO[mv.f2 + 1][mv.c2 - 1] == peon)) {
          if (mnsj_err == true) {
            cout << "No puedes saltar a un compañero\n";
          }
          return false;
        }
      }

      else if (mv.f2 < mv.f1 && mv.c2 < mv.c1) {
        if ((TABLERO[mv.f2 + 1][mv.c2 + 1] == dama) ||
            (TABLERO[mv.f2 + 1][mv.c2 + 1] == peon)) {
          if (mnsj_err == true) {
            cout << "No puedes saltar a un compañero\n";
          }
          return false;
        }
      }

      // Si no rompe las reglas, entonces el salto es legal.
      salto = true;
      return true;
    }
  }
  // Reglas de saltos para damas
  if (TABLERO[mv.f1][mv.c1] == dama) {

    int piezas = 0; // Numero de piezas en la diagonal.

    // Retorna falso si no se mueve en una diagonal perfecta.
    if (abs(mv.f1 - mv.f2) != abs(mv.c1 - mv.c2)) {
      if (mnsj_err == true) {
        cout << "Debes moverte sobre una diagonal perfecta\n";
      }
      return false;
    }

    // Retorna verdadero si solo me muevo una casilla.
    if ((abs(mv.f1 - mv.f2) == 1 && abs(mv.c1 - mv.c2) == 1)) {
      return true;
    }

    // Analiza el tipo de movimiento en el caso de dirigirse a la esquina
    // inferior derecha.
    if (mv.f2 > mv.f1 && mv.c2 > mv.c1) {

      int j = mv.c1 + 1;

      for (int i = mv.f1 + 1; i < mv.f2; i++) {

        if (TABLERO[i][j] != ' ') {
          // Cuenta el numero de piezas dentro de la diagonal.
          piezas++;
          // Retorna falso si salta a un compañero.
          if ((TABLERO[i][j] == dama) || (TABLERO[i][j] == peon)) {
            if (mnsj_err == true) {
              cout << "No puedes saltar a un compañero\n";
            }
            return false;
          }
        }
        j++;
      }
      // Retorna verdadero si es un movimiento sin salto.
      if (piezas == 0) {
        return true;
      }

      // retorna falso si salta a mas de un enemigo.
      if (piezas > 1) {
        if (mnsj_err == true) {
          cout << "No puedes saltar a más de un enemigo en un salto\n";
        }
        return false;
      }
    } else

      // Analiza el tipo de movimiento en el caso de dirigirse a la esquina
      // inferior izquierda.
      if (mv.f2 > mv.f1 && mv.c2 < mv.c1) {

        int j = mv.c1 - 1;

        for (int i = mv.f1 + 1; i < mv.f2; i++) {

          if (TABLERO[i][j] != ' ') {
            // Cuenta el numero de piezas dentro de la diagonal.
            piezas++;
            // Retorna falso si salta a un compañero.
            if ((TABLERO[i][j] == dama) || (TABLERO[i][j] == peon)) {
              if (mnsj_err == true) {
                cout << "No puedes saltar a un compañero\n";
              }
              return false;
            }
          }
          j--;
        }
        // Retorna verdadero si es un movimiento sin salto.
        if (piezas == 0) {
          return true;
        }

        // retorna falso si salta a mas de un enemigo.
        if (piezas > 1) {
          if (mnsj_err == true) {
            cout << "No puedes saltar a más de un enemigo en un salto\n";
          }
          return false;
        }
      } else

        // Analiza el tipo de movimiento en el caso de dirigirse a la esquina
        // superior derecha.
        if (mv.f2 < mv.f1 && mv.c2 > mv.c1) {

          int j = mv.c1 + 1;

          for (int i = mv.f1 - 1; i > mv.f2; i--) {

            if (TABLERO[i][j] != ' ') {
              // Cuenta el numero de piezas dentro de la diagonal.
              piezas++;
              // Retorna falso si salta a un compañero.
              if ((TABLERO[i][j] == dama) || (TABLERO[i][j] == peon)) {
                if (mnsj_err == true) {
                  cout << "No puedes saltar a un compañero\n";
                }
                return false;
              }
            }
            j++;
          }
          // Retorna verdadero si es un movimiento sin salto.
          if (piezas == 0) {
            return true;
          }

          // retorna falso si salta a mas de un enemigo.
          if (piezas > 1) {
            if (mnsj_err == true) {
              cout << "No puedes saltar a más de un enemigo en un salto\n";
            }
            return false;
          }
        } else

          // Analiza el tipo de movimiento en el caso de dirigirse a la esquina
          // superior izquierda.
          if (mv.f2 < mv.f1 && mv.c2 < mv.c1) {

            int j = mv.c1 - 1;

            for (int i = mv.f1 - 1; i > mv.f2; i--) {

              if (TABLERO[i][j] != ' ') {
                // Cuenta el numero de piezas dentro de la diagonal.
                piezas++;
                // Retorna falso si salta a un compañero.
                if ((TABLERO[i][j] == dama) || (TABLERO[i][j] == peon)) {
                  if (mnsj_err == true) {
                    cout << "No puedes saltar a un compañero\n";
                  }
                  return false;
                }
              }
              j--;
            }
            // Retorna verdadero si es un movimiento sin salto.
            if (piezas == 0) {
              return true;
            }

            // retorna falso si salta a mas de un enemigo.
            if (piezas > 1) {
              if (mnsj_err == true) {
                cout << "No puedes saltar a más de un enemigo en un salto\n";
              }
              return false;
            }
          }
    // Si no rompe las reglas, el salto es legal.
    salto = true;
    return true;
  }
  return true;
}

void Juego::mover(Movimiento mv, char TABLERO[10][10]) {

  if (salto == true) {

    // Si juego con un peon, remueve la pieza capturada despues del salto.
    if (TABLERO[mv.f1][mv.c1] == iaPeon ||
        TABLERO[mv.f1][mv.c1] == humanoPeon) {

      if (mv.f2 > mv.f1 && mv.c2 > mv.c1) {
        TABLERO[mv.f2 - 1][mv.c2 - 1] = ' ';
        if (ia == false) {
          cout << "Se capturo la pieza " << mv.f2 - 1 << " - " << mv.c2 - 1
               << " \n";
        }
      } else if (mv.f2 > mv.f1 && mv.c2 < mv.c1) {
        TABLERO[mv.f2 - 1][mv.c2 + 1] = ' ';
        if (ia == false) {
          cout << "Se capturo la pieza " << mv.f2 - 1 << " - " << mv.c2 + 1
               << " \n";
        }
      } else if (mv.f2 < mv.f1 && mv.c2 > mv.c1) {
        TABLERO[mv.f2 + 1][mv.c2 - 1] = ' ';
        if (ia == false) {
          cout << "Se capturo la pieza " << mv.f2 + 1 << " - " << mv.c2 - 1
               << " \n";
        }
      } else if (mv.f2 < mv.f1 && mv.c2 < mv.c1) {
        TABLERO[mv.f2 + 1][mv.c2 + 1] = ' ';
        if (ia == false) {
          cout << "Se capturo la pieza " << mv.f2 + 1 << " - " << mv.c2 + 1
               << " \n";
        }
      }
    }

    // Si juego con una dama, remueve la pieza capturada despues del salto.
    if (TABLERO[mv.f1][mv.c1] == humanoDama ||
        TABLERO[mv.f1][mv.c1] == iaDama) {

      if (mv.f2 > mv.f1 && mv.c2 > mv.c1) {

        int j = mv.c1 + 1;

        for (int i = mv.f1 + 1; i < mv.f2; i++) {

          if (TABLERO[i][j] != ' ') {
            TABLERO[i][j] = ' ';
            if (ia == false) {
              cout << "Se capturo la pieza: ( " << i << " , " << j << " )\n";
            }
          }
          j++;
        }

      } else

          if (mv.f2 > mv.f1 && mv.c2 < mv.c1) {

        int j = mv.c1 - 1;

        for (int i = mv.f1 + 1; i < mv.f2; i++) {

          if (TABLERO[i][j] != ' ') {
            TABLERO[i][j] = ' ';
            if (ia == false) {
              cout << "Se capturo la pieza: ( " << i << " , " << j << " )\n";
            }
          }
          j--;
        }
      } else

          if (mv.f2 < mv.f1 && mv.c2 < mv.c1) {

        int j = mv.c1 - 1;

        for (int i = mv.f1 - 1; i > mv.f2; i--) {

          if (TABLERO[i][j] != ' ') {
            TABLERO[i][j] = ' ';
            if (ia == false) {
              cout << "Se capturo la pieza: ( " << i << " , " << j << " )\n";
            }
          }
          j--;
        }
      } else

          if (mv.f2 < mv.f1 && mv.c2 > mv.c1) {

        int j = mv.c1 + 1;

        for (int i = mv.f1 - 1; i > mv.f2; i--) {

          if (TABLERO[i][j] != ' ') {
            TABLERO[i][j] = ' ';
            if (ia == false) {
              cout << "Se capturo la pieza: ( " << i << " , " << j << " )\n";
            }
          }
          j++;
        }
      }
    }
  }

  if (TABLERO[mv.f1][mv.c1] == humanoPeon ||
      TABLERO[mv.f1][mv.c1] ==
          humanoDama) { // Funcion sopladita debe ser llamada antes de realizar
                        // los cambios,para asi evaluar incluso a una unica
                        // pieza que se mueva y no coma teniendo la oportunidad.

    if (salto == false && ia == false) {
      sopladita(mv, TABLERO);
    } // Si se realizo un salto, no llamar a la funcion sopladita.
  }

  // Borra la pieza en la posicion inicial y la escribe en la nueva posicion.
  TABLERO[mv.f2][mv.c2] = TABLERO[mv.f1][mv.c1];
  TABLERO[mv.f1][mv.c1] = ' ';

  // Si un peon llega al otro extremo, se escribe una dama.
  for (int i = 0; i < 8; i++) {

    if (TABLERO[0][i] == humanoPeon) {
      TABLERO[0][i] = humanoDama;
    }

    if (TABLERO[9][i] == iaPeon) {
      TABLERO[9][i] = iaDama;
    }
  }

  if (TABLERO[mv.f2][mv.c2] == humanoPeon ||
      TABLERO[mv.f2][mv.c2] == humanoDama) {

    if (salto == true && ia == false) {
      system("clear"); /// limpia la pantalla
      imprimirTablero(
          TABLERO); // imprimer el tablero despues de haber realizado un salto.
      multisalto(mv, TABLERO); // analiza la opcion de multisalto.
      system("clear");         // limpia la pantalla
    }
  }
  salto = false;
}

void Juego::multisalto(
    Movimiento mv,
    char TABLERO[10][10]) { // Funcion mltisalto cuando juega el humano.

  salto = false;    // devuelve la variable salto a su valor por defecto
  char resp[2];     // guarda la respuesta si o no.
  mnsj_err = false; // no imprime mensajes de error.
  Movimiento aux;   // guarda movimiento ingresado por el usuario.
  Movimiento copia_buena =
      mv; // guarda una copia del ultimo salto correcto para utilizarlo como
          // argumento en la llamada recursiva

  cout << "¿Desea continuar capturando?('si' o 'no')\n";
  cin >> resp;
  // No avanza hasta responder una de las dos opciones.
  while (strcmp("si", resp) != 0 && strcmp("no", resp) != 0) {
    cout << "¿Desea continuar capturando?('si' o 'no')\n";
    cin >> resp;
  }

  if (strcmp("no", resp) == 0) {
    mv.c1 = mv.c2;
    mv.f1 = mv.f2;
    sopladita(mv, TABLERO); // Cuando termina de realizar multisaltos, soplara
                            // de ser posible.
  } else if (strcmp("si", resp) == 0) {

    bool sw = false;

    cout << "Mover pieza: (" << mv.f2 << " , " << mv.c2 << " ) \n";
    cout << "A la casilla: \n";
    cout << "Fila: ";
    cin >> aux.f2;
    cout << "Columna: ";
    cin >> aux.c2;

    aux.f1 = mv.f2;
    aux.c1 = mv.c2; // Guarda la posicion actual como origen del siguiente salto
                    // solicitado a evaluar.
    mnsj_err = true;
    if ((chequearMovimiento(HUMANO, aux, TABLERO) == true) && (salto == true)) {
      sw = true;
      copia_buena = aux;
    }
    mnsj_err = false;

    if (sw == true) {
      mover(aux, TABLERO); // Si el salto es legal, lo realiza, y llama
                           // nuevamente a multisalto.
    } else {
      cout << "Movimiento ilegal. Por favor intente otra vez.\n";
      multisalto(copia_buena, TABLERO);
    } // En caso que no existan mas saltos, llama una vez mas a la funcion
      // multisalto para darle la oportunidad de responder 'no'.
  }
}

void Juego::sopladita(
    Movimiento mv,
    char TABLERO[10][10]) { // funcion para soplar las piezas del humano en caso
                            // que no capture teniendo la posibilidad.

  mnsj_err = false; // cuando se llama a la funcion sopladita, todavia no ha
                    // terminado la funcion humano mueve, por lo tanto mnsj_err
                    // es true cuando entra en esta funcion.

  Movimiento aux, origen, peon, dama;
  origen.f1 = -1;
  origen.c1 = -1; // valiable auxilar para guardar coordenadas en caso de que la
                  // pieza a soplar sea la del origen de movimiento.
  peon.f1 = -1;
  peon.c1 = -1; // valiable auxilar para guardar coordenadas en caso de que la
                // pieza a soplar sea un peon.
  dama.f1 = -1;
  dama.c1 = -1; // valiable auxilar para guardar coordenadas en caso de que la
                // pieza a soplar sea una dama.

  bool sw = false;  // Si ya capturo una dama, no sigue capurando mas.
  bool cap = false; // Si ya capturo una pieza, no sigue capturando mas.

  for (int i = 0; i < 10; i++) { // Busca en toda la matriz.

    for (int j = 0; j < 10; j++) {

      if (TABLERO[i][j] == humanoPeon) { // Analiza solo a los peones.

        if (cap == true) {
        } else { // Analiza a todos los peones.

          aux.c1 = j;
          aux.f1 = i;
          aux.c2 = aux.c1 + 2;
          aux.f2 =
              aux.f1 - 2; // Analiza si exite posible captura por la derecha.

          if (chequearMovimiento(HUMANO, aux, TABLERO) == true &&
              salto == true) {
            salto = false;
            if (i == mv.f1 && j == mv.c1) {
              origen.f1 = i;
              origen.c1 = j;
              cap = true;
            } else {
              peon.f1 = i;
              peon.c1 = j;
            }

          } else {
            aux.c1 = j;
            aux.f1 = i;
            aux.c2 = aux.c1 - 2;
            aux.f2 = aux.f1 -
                     2; // Analiza si existe posible capturapor la izquierda.

            if (chequearMovimiento(HUMANO, aux, TABLERO) == true &&
                salto == true) {
              salto = false;
              if (i == mv.f1 && j == mv.c1) {
                origen.f1 = i;
                origen.c1 = j;
                cap = true;
              } else {
                peon.f1 = i;
                peon.c1 = j;
              }
            }
          }
        }
      }
    }
  }

  for (int i = 0; i < 10; i++) { // Busca en toda la matriz.

    for (int j = 0; j < 10; j++) {

      if (TABLERO[i][j] == humanoDama) { // Analiza solo a las damas.

        if (cap == true) {
        } else { // Analiza a todas las damas.

          if (cap == false) { // Busca un peon o dama enemiga dentro de la
                              // diagonal inferior derecha.

            int c = j + 1, f = i + 1; // Toma a la casilla siguiente como
                                      // primera casila a analizar.

            aux.f1 = i;
            aux.c1 = j; // Guarda la pieza como origen del movimiento

            while (c < 9 && f < 9) { // Limita la busqueda hasta la penultima
                                     // casilla del borde del tablero.

              if ((TABLERO[f][c] == iaPeon) ||
                  (TABLERO[f][c] == iaDama)) { // Guarda la casilla siguiente
                                               // como la casilla destino.
                aux.f2 = f + 1;
                aux.c2 = c + 1;

                if ((chequearMovimiento(HUMANO, aux, TABLERO) == true) &&
                    (salto == true)) {
                  salto = false;
                  if (i == mv.f1 && j == mv.c1) {
                    origen.f1 = i;
                    origen.c1 = j;
                    cap = true;
                  } else {
                    dama.f1 = i;
                    dama.c1 = j;
                  }
                }
              }
              c++;
              f++;
            }
          }

          if (cap == false) { // Busca un peon o dama enemiga dentro de la
                              // diagonal inferior izquierda.

            int c = j - 1, f = i + 1; // Toma a la casilla siguiente como
                                      // primera casilla a analizar.

            aux.f1 = i;
            aux.c1 = j; // Guarda la pieza como origen del movimiento

            while (c > 0 && f < 9) { // Limita la busqueda hasta la penultima
                                     // casilla del borde del tablero.

              if ((TABLERO[f][c] == iaPeon) ||
                  (TABLERO[f][c] == iaDama)) { // Guarda la casilla siguiente
                                               // como la casilla destino.
                aux.f2 = f + 1;
                aux.c2 = c - 1;

                if ((chequearMovimiento(HUMANO, aux, TABLERO) == true) &&
                    (salto == true)) {
                  salto = false;
                  if (i == mv.f1 && j == mv.c1) {
                    origen.f1 = i;
                    origen.c1 = j;
                    cap = true;
                  } else {
                    dama.f1 = i;
                    dama.c1 = j;
                  }
                }
              }
              c--;
              f++;
            }
          }

          if (cap == false) { // Busca un peon o dama enemiga dentro de la
                              // diagonal superior izquierda.

            int c = j - 1, f = i - 1; // Toma a la casilla siguiente como
                                      // primera casilla a analizar.

            aux.f1 = i;
            aux.c1 = j; // Guarda la pieza como origen del movimiento

            while (c > 0 && f > 0) { // Limita la busqueda hasta la penultima
                                     // casilla del borde del tablero.

              if ((TABLERO[f][c] == iaPeon) ||
                  (TABLERO[f][c] == iaDama)) { // Guarda la casilla siguiente
                                               // como la casilla destino.
                aux.f2 = f - 1;
                aux.c2 = c - 1;

                if ((chequearMovimiento(HUMANO, aux, TABLERO) == true) &&
                    (salto == true)) {
                  salto = false;
                  if (i == mv.f1 && j == mv.c1) {
                    origen.f1 = i;
                    origen.c1 = j;
                    cap = true;
                  } else {
                    dama.f1 = i;
                    dama.c1 = j;
                  }
                }
              }
              c--;
              f--;
            }
          }

          if (cap == false) { // Busca un peon o dama enemiga dentro de la
                              // diagonal superior derecha.

            int c = j + 1, f = i - 1; // Toma a la casilla siguiente como
                                      // primera casilla a analizar.

            aux.f1 = i;
            aux.c1 = j; // Guarda la pieza como origen del movimiento.

            while (c < 9 && f > 0) { // Limita la busqueda hasta la penultima
                                     // casilla del borde del tablero.

              if ((TABLERO[f][c] == iaPeon) ||
                  (TABLERO[f][c] == iaDama)) { // Guarda la casilla siguiente
                                               // como la casilla destino.
                aux.f2 = f - 1;
                aux.c2 = c + 1;

                if ((chequearMovimiento(HUMANO, aux, TABLERO) == true) &&
                    (salto == true)) {
                  salto = false;
                  if (i == mv.f1 && j == mv.c1) {
                    origen.f1 = i;
                    origen.c1 = j;
                    cap = true;
                  } else {
                    dama.f1 = i;
                    dama.c1 = j;
                  }
                }
              }
              c++;
              f--;
            }
          }
        }
      }
    }
  }
  if (origen.f1 != -1 &&
      origen.c1 != -1) { // Si la pieza que no realizo la captura es la misma
                         // que se movio, la soplara.

    TABLERO[origen.f1][origen.c1] = ' ';
    cout << "Se le ha soplado la pieza: ( " << origen.f1 << " , " << origen.c1
         << " )\n";

  } else if (dama.f1 != -1 &&
             dama.c1 != -1) { // En segundo lugar buscara soplar una dama ya que
                              // es mas valiosa.

    TABLERO[dama.f1][dama.c1] = ' ';
    cout << "Se le ha soplado la pieza: ( " << dama.f1 << " , " << dama.c1
         << " )\n";

  } else if (peon.f1 != -1 &&
             peon.c1 != -1) { // por ultimo eliminara un peon de ser el caso.

    TABLERO[peon.f1][peon.c1] = ' ';
    cout << "Se le ha soplado la pieza: ( " << peon.f1 << " , " << peon.c1
         << " )\n";
  }
  salto = false; // Regresa al valor de salto por defecto.
}

void Juego::humanoMueve() {

  mnsj_err = true;

  Movimiento mv;
  bool sw = false;
  char f1, f2, c1, c2;

  while (sw == false) {

    cout << "Mover pieza\n";
    cout << "Fila: ";
    cin >> f1;
    mv.f1 = (int)(f1 - 48);
    cout << "Columna: ";
    cin >> c1;
    mv.c1 = (int)(c1 - 48);

    // Chequea si el usuario se equivoca al ingresar el movimiento.
    while (mv.f1 < 0 || mv.f1 > 9 || mv.c1 < 0 || mv.c1 > 9) {
      cout << "Ingresar numeros entre 0 y 9.\n";
      cout << "Mover pieza:\n";
      cout << "Fila: ";
      cin >> f1;
      mv.f1 = (int)(f1 - 48);
      cout << "Columna: ";
      cin >> c1;
      mv.c1 = (int)(c1 - 48);
    }

    cout << "A la casilla:\n";
    cout << "Fila: ";
    cin >> f2;
    mv.f2 = (int)(f2 - 48);
    cout << "Columna: ";
    cin >> c2;
    mv.c2 = (int)(c2 - 48);

    while (mv.f2 < 0 || mv.f2 > 9 || mv.c2 < 0 || mv.c2 > 9) {
      cout << "Ingresar numeros entre 0 y 9.\n";
      cout << "A la casilla\n";
      cout << "Fila: ";
      cin >> f2;
      mv.f2 = (int)(f2 - 48);
      cout << "Columna: ";
      cin >> c2;
      mv.c2 = (int)(c2 - 48);
    }

    sw = chequearMovimiento(HUMANO, mv, tablero);

    if (sw == false) {
      cout << "Movimiento ilegal. Por favor intente otra vez.\n";
    } else {
      system("clear"); // limpia la pantalla
      cout << "***************************************************************"
           << endl;
      cout << "=> El humano ha movido la pieza:(" << mv.f1 << "," << mv.c1
           << ") a la posicion (" << mv.f2 << "," << mv.c2 << ")\n";

      mover(mv, tablero);
    }
  }
  mnsj_err = false;
}

void Juego::jugar() {

  int turno = 0;
  cout << "***************************************************************"
       << endl;
  imprimirTablero(tablero);
  cout << "***************************************************************"
       << endl;

  while (!chequearGanador(HUMANO, tablero) && !chequearGanador(IA, tablero)) {

    if (turno % 2 == 0) {
      cout << endl << "TURNO DEL HUMANO:" << endl;

      humanoMueve();

      if (chequearGanador(HUMANO, tablero) == true) {
        cout << "El humano ha ganado!" << endl;
      }
      if (chequearGanador(IA, tablero) == true) {
        cout << "La maquina ha ganado!" << endl;
      }
      turno++;

    } else {

      ia = true;
      Movimiento IAmovi = minimax(
          tablero); // Despues de llamar a minimax, salto es igual a false.(esto
                    // se debe a que cada vez que se llama a la funcion puntaje,
                    // se debe devolver el valor de salto a 'false' para que
                    // esto no interfiera con los siguientes analisis)
      ia = false;
      chequearMovimiento(
          IA, IAmovi, tablero); // con esto volvemos el valor de salto a true en
                                // caso de ser salto, para eliminar la pieza en
                                // caso de salto al llamar a la funcion mover.
      Movimiento multi;         // varialble auxiliar

      if (salto == true) { // Solo ejecutara multisaltos si el primer movimiento
                           // fue un salto.

        do { // ejecuta multisaltos en caso de ser posible.
          cout << "=> La maquina ha movido la pieza:(" << IAmovi.f1 << ","
               << IAmovi.c1 << ") a la posicion (" << IAmovi.f2 << ","
               << IAmovi.c2 << ")\n";
          cout << "************************************************************"
                  "***"
               << endl;
          mover(IAmovi, tablero); // Actualiza el tablero.(mover vuelve el valor
                                  // de salto a falso)

          multi.f1 = IAmovi.f2;
          multi.c1 =
              IAmovi.c2; // Guardo el destino del ultimo movimiento en multi.

          ia = true;
          IAmovi = minimax(tablero); // genera nuevo movimiento.
          ia = false;

        } while (IAmovi.f1 == multi.f1 && IAmovi.c1 == multi.c1 &&
                 chequearMovimiento(IA, IAmovi, tablero) == true &&
                 salto ==
                     true); // comparo el ultimo destino con el proximo origen
      } else {
        cout << "=> La maquina ha movido la pieza:(" << IAmovi.f1 << ","
             << IAmovi.c1 << ") a la posicion (" << IAmovi.f2 << ","
             << IAmovi.c2 << ")\n";
        cout
            << "***************************************************************"
            << endl;
        mover(IAmovi, tablero);
      }

      if (chequearGanador(IA, tablero) == true) {
        cout << "La maquina ha ganado!" << endl;
      }
      turno++;
      imprimirTablero(tablero);
      cout << "***************************************************************"
           << endl;
    }
  }
}

bool Juego::chequearGanador(Jugador jugador, char TABLERO[10][10]) {

  int piezas = 0;
  // Retorna verdadero si solo hay un tipo de piezas sobre el tablero
  if (jugador == HUMANO) {

    for (int i = 0; i < 10; i++) {

      for (int j = 0; j < 10; j++) {
        if ((TABLERO[i][j] == iaPeon) || (TABLERO[i][j] == iaDama)) {
          piezas++;
        }
      }
    }
  } else if (jugador == IA) {

    for (int i = 0; i < 10; i++) {
      for (int j = 0; j < 10; j++) {
        if ((TABLERO[i][j] == humanoPeon) || (TABLERO[i][j] == humanoDama)) {
          piezas++;
        }
      }
    }
  }
  if (piezas > 0) {
    return false;
  } else
    return true;

  // Implementar triunfo por bloqueo.
}

void Juego::imprimirTablero(char tableros[10][10]) {

  cout << "***************************************************************"
       << endl;
  cout << "    0     1     2     3     4     5     6     7     8     9  \n";
  cout << " ------------------------------------------------------------";
  for (int i = 0; i < 10; i++) {
    cout << '\n' << i << "|";
    for (int j = 0; j < 10; j++) {
      cout << "  " << tableros[i][j] << " " << " |";
    }
    cout << "\n"
         << " ------------------------------------------------------------";
  }
  cout << endl;
}

bool Juego::profundidad(int pro) {
  // pro = 1: Si tiene la oportunidad para saltar lo hara, pero no evitara que
  // saltes. pro = 2: Tomara la oportunidad de saltar y evitara que saltes si es
  // evidente, pero con buenos movimiento le puedes ganar. pro = 3: De aqui en
  // adelante se vuelve mas inteligente .
  if (pro == 3) {
    return true;
  } else {
    return false;
  }
}

Movimiento Juego::minimax(char IAtablero[10][10]) {

  int mejorPuntaje = 1001; //  si o si me guarda la primera jugada.
  Movimiento mejorJugada;
  mejorJugada.p = 1000;        // va guardando el mejor movimiento y puntaje
  Movimiento mejorMovi;        // Mejor movimiento a retornar.
  Movimiento mv;               // Variable para enviar posiciones a evaluar.
  bool SD = false, SI = false; // switch para guardar verdadero en caso de tener
                               // la posibilidad de capturar, para no seguir
                               // evaluando con otros movimientos.
  int piezasIA = 0; // variable para contar numero de piezas que tiene la
                    // computadora a partir de la segunda fila.
  int piezasH = 0;  // variable para contar numero de piezas que tiene el humano
                    // en su ultima fila.
  int def = 1;      // Por defecto las piezas se evaluan a partir de la
                    // fila 1.(estrategia defensiva)

  char copiaTablero[10][10]; // tablero auxiliar.

  for (int i = 0; i < 10; i++) { // crea una copia del original
    for (int j = 0; j < 10; j++) {
      copiaTablero[i][j] = IAtablero[i][j];
    }
  }

  for (int i = 0; i < 10;
       i++) { // Si ya no tengo mas piezas para mover a partir de la fila 1,
              // entonces habilito las piezas de la fila 0 para ser usadas.
    for (int j = 0; j < 10; j++) {
      if ((IAtablero[i][j] == iaPeon) || (IAtablero[i][j] == iaDama)) {
        piezasIA++;
      }
    }
  }
  for (int j = 0; j < 10; j++) {
    if (IAtablero[9][j] == humanoPeon) {
      piezasH++;
    }
  }

  if (piezasIA < 6) {
    def = 0;
  } // Si no tengo mas piezas que la fila 0, entonces la habilito.
  if (piezasH < 5) {
    def = 0;
  } // Si el humano mueve su filia 9, entonces habilita la fila 0.

  for (int i = def; i < 10; i++) {
    for (int j = 0; j < 10; j++) {

      if (IAtablero[i][j] == iaDama) { // evalua los movimietos con damas

        int puntaje;

        // Busca un peon o dama enemiga dentro de la DIAGONAL INFERIOR DERECHA.

        int c = j + 1, f = i + 1; // Toma a la casilla siguiente como primera
                                  // casila a analizar.

        mv.c1 = j;
        mv.f1 = i; // El origen del movimiento a evaluar es la posicion del peon
                   // encontrado.

        while (c < 9 && f < 9) { // Limita la busqueda hasta la penultima
                                 // casilla del borde del tablero.

          if ((IAtablero[f][c] == humanoPeon) ||
              (IAtablero[f][c] == humanoDama)) {

            mv.f2 = f + 1;
            mv.c2 =
                c + 1; // Guarda la casilla siguiente como la casilla destino.

            if ((chequearMovimiento(IA, mv, IAtablero) == true) &&
                (salto == true)) {

              salto = false; // devuelve el valor de salto a defecto.

              puntaje = -1 + max(IAtablero, mv,
                                 1); // le sumamos menos uno para que prefiera
                                     // comer con una dama en vez de un peon.

              for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                  IAtablero[i][j] = copiaTablero[i][j];
                }
              } // devuelve el valor del original.

              if (puntaje < mejorJugada.p) {
                mejorJugada.p = puntaje;
                mejorJugada.f1 = mv.f1;
                mejorJugada.c1 = mv.c1;
                mejorJugada.f2 = mv.f2;
                mejorJugada.c2 = mv.c2;
              }
            }
          }
          c++;
          f++;
        }

        // Busca un peon o dama enemiga dentro de la DIAGONAL INFERIOR
        // IZQUIERDA.

        c = j - 1;
        f = i +
            1; // Toma a la casilla siguiente como primera casilla a analizar.

        mv.f1 = i;
        mv.c1 = j; // Guarda la pieza como origen del movimiento

        while (c > 0 && f < 9) { // Limita la busqueda hasta la penultima
                                 // casilla del borde del tablero.

          if ((IAtablero[f][c] == humanoPeon) ||
              (IAtablero[f][c] == humanoDama)) {

            mv.f2 = f + 1;
            mv.c2 =
                c - 1; // Guarda la casilla siguiente como la casilla destino.

            if ((chequearMovimiento(IA, mv, IAtablero) == true) &&
                (salto == true)) {

              salto = false; // devuelve el valor de salto a defecto.

              puntaje = -1 + max(IAtablero, mv, 1);

              for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                  IAtablero[i][j] = copiaTablero[i][j];
                }
              } // devuelve el valor del original.

              if (puntaje < mejorJugada.p) {
                mejorJugada.p = puntaje;
                mejorJugada.f1 = mv.f1;
                mejorJugada.c1 = mv.c1;
                mejorJugada.f2 = mv.f2;
                mejorJugada.c2 = mv.c2;
              }
            }
          }
          c--;
          f++;
        }

        // Busca un peon o dama enemiga dentro de la DIAGONAL SUPERIOR
        // IZQUIERDA.

        c = j - 1;
        f = i -
            1; // Toma a la casilla siguiente como primera casilla a analizar.

        mv.f1 = i;
        mv.c1 = j; // Guarda la pieza como origen del movimiento

        while (c > 0 && f > 0) { // Limita la busqueda hasta la penultima
                                 // casilla del borde del tablero.

          if ((IAtablero[f][c] == humanoPeon) ||
              (IAtablero[f][c] == humanoDama)) { // Guarda la casilla siguiente
                                                 // como la casilla destino.

            mv.f2 = f - 1;
            mv.c2 = c - 1;

            if ((chequearMovimiento(IA, mv, IAtablero) == true) &&
                (salto == true)) {

              salto = false; // devuelve el valor de salto a defecto.

              puntaje = -1 + max(IAtablero, mv, 1);

              for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                  IAtablero[i][j] = copiaTablero[i][j];
                }
              } // devuelve el valor del original.

              if (puntaje < mejorJugada.p) {
                mejorJugada.p = puntaje;
                mejorJugada.f1 = mv.f1;
                mejorJugada.c1 = mv.c1;
                mejorJugada.f2 = mv.f2;
                mejorJugada.c2 = mv.c2;
              }
            }
          }
          c--;
          f--;
        }

        // Busca un peon o dama enemiga dentro de la DIAGONAL SUPERIOR DERECHA.

        c = j + 1,
        f = i -
            1; // Toma a la casilla siguiente como primera casilla a analizar.

        mv.f1 = i;
        mv.c1 = j; // Guarda la pieza como origen del movimiento.

        while (c < 9 && f > 0) { // Limita la busqueda hasta la penultima
                                 // casilla del borde del tablero.

          if ((IAtablero[f][c] == humanoPeon) ||
              (IAtablero[f][c] == humanoDama)) { // Guarda la casilla siguiente
                                                 // como la casilla destino.

            mv.f2 = f - 1;
            mv.c2 = c + 1;

            if ((chequearMovimiento(IA, mv, IAtablero) == true) &&
                (salto == true)) {

              salto = false; // devuelve el valor de salto a defecto.

              puntaje = -1 + max(IAtablero, mv, 1);

              for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                  IAtablero[i][j] = copiaTablero[i][j];
                }
              } // devuelve el valor del original.

              if (puntaje < mejorJugada.p) {
                mejorJugada.p = puntaje;
                mejorJugada.f1 = mv.f1;
                mejorJugada.c1 = mv.c1;
                mejorJugada.f2 = mv.f2;
                mejorJugada.c2 = mv.c2;
              }
            }
          }
          c++;
          f--;
        }

        if (mejorJugada.p <
            mejorPuntaje) { // Guarda el mejor movimiento entre todas las damas.

          mejorPuntaje = mejorJugada.p;
          mejorMovi.f1 = mejorJugada.f1;
          mejorMovi.c1 = mejorJugada.c1;
          mejorMovi.f2 = mejorJugada.f2;
          mejorMovi.c2 = mejorJugada.c2;
        }
      }

      if (IAtablero[i][j] ==
          iaPeon) { // Evalua todos los movimientos con peones.

        int puntaje;

        mv.c1 = j;
        mv.f1 = i; // El origen del movimiento a evaluar es la posicion del peon
                   // encontrado.

        mv.c2 = j + 2;
        mv.f2 = i + 2; // COORDENADAS PARA COMPROBAR SALTO POR LA DERECHA.

        if (mv.f1 < 8 && mv.c1 < 8 &&
            chequearMovimiento(IA, mv, IAtablero) == true &&
            salto == true) { // Evalua primero el salto, de no ser posible hacer
                             // un movimiento simple.

          salto = false; // devuelve el valor de salto a defecto.

          puntaje = max(IAtablero, mv, 1);

          for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
              IAtablero[i][j] = copiaTablero[i][j];
            }
          } // devuelve el valor del original.

          // puntaje = min(IAtablero,mv,1);//para multisaltos

          if (puntaje < mejorJugada.p) {
            mejorJugada.p = puntaje;
            mejorJugada.f1 = mv.f1;
            mejorJugada.c1 = mv.c1;
            mejorJugada.f2 = mv.f2;
            mejorJugada.c2 = mv.c2;
          }
        }

        mv.c2 = j - 2;
        mv.f2 = i + 2; // COORDENADAS PARA COMPROBAR SALTO POR LA IZQUIERDA.

        if (mv.f1 < 8 && mv.c1 > 1 &&
            chequearMovimiento(IA, mv, IAtablero) == true &&
            salto == true) { // Evalua primero el salto, de no ser posible hacer
                             // un movimiento simple.

          salto = false; // devuelve el valor de salto a defecto.

          puntaje = max(IAtablero, mv, 1);

          for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
              IAtablero[i][j] = copiaTablero[i][j];
            }
          } // devuelve el valor del original.

          if (puntaje < mejorJugada.p) {
            mejorJugada.p = puntaje;
            mejorJugada.f1 = mv.f1;
            mejorJugada.c1 = mv.c1;
            mejorJugada.f2 = mv.f2;
            mejorJugada.c2 = mv.c2;
          }
        }

        if (SD == false &&
            SI == false) { // Evaluara movimiento simplo solo si no existe
                           // ninguna posibilidad de saltar

          mv.c2 = j + 1;
          mv.f2 = i + 1; // COORDENADAS PARA COMPROBAR MOVIMIENTO POR LA
                         // DERECHA.

          if (mv.f1 < 9 && mv.c1 < 9 &&
              chequearMovimiento(IA, mv, IAtablero) == true) {

            puntaje = max(IAtablero, mv, 1);

            for (int i = 0; i < 10; i++) {
              for (int j = 0; j < 10; j++) {
                IAtablero[i][j] = copiaTablero[i][j];
              }
            } // devuelve el valor del original.

            if (puntaje < mejorJugada.p) {

              mejorJugada.p = puntaje;
              mejorJugada.f1 = mv.f1;
              mejorJugada.c1 = mv.c1;
              mejorJugada.f2 = mv.f2;
              mejorJugada.c2 = mv.c2;
            }
          }

          mv.c2 = j - 1;
          mv.f2 =
              i + 1; // COORDENADAS PARA COMPROBAR MOVIMIENTO POR LA IZQUIERDA.

          if (mv.f1 < 9 && mv.c1 > 0 &&
              chequearMovimiento(IA, mv, IAtablero) == true) {

            puntaje = max(IAtablero, mv, 1);

            for (int i = 0; i < 10; i++) {
              for (int j = 0; j < 10; j++) {
                IAtablero[i][j] = copiaTablero[i][j];
              }
            } // devuelve el valor del original.

            if (puntaje < mejorJugada.p) {

              mejorJugada.p = puntaje;
              mejorJugada.f1 = mv.f1;
              mejorJugada.c1 = mv.c1;
              mejorJugada.f2 = mv.f2;
              mejorJugada.c2 = mv.c2;
            }
          }
        }
        SD = false;
        SI = false;

        if (mejorJugada.p <
            mejorPuntaje) { // Guarda el mejor movimiento entre sus cuatro
                            // posibilidades entre todos los peones.

          mejorPuntaje = mejorJugada.p;
          mejorMovi.f1 = mejorJugada.f1;
          mejorMovi.c1 = mejorJugada.c1;
          mejorMovi.f2 = mejorJugada.f2;
          mejorMovi.c2 = mejorJugada.c2;
        }
      }
    }
  }
  return mejorMovi;
}

int Juego::max(char IAtablero[10][10], Movimiento mvAnterior, int pro) {

  if (profundidad(pro)) {
    return 0;
  } // Si se alcanza la profundidad retorna 0.

  if ((chequearMovimiento(IA, mvAnterior, IAtablero) == true &&
       salto == true)) {
    mover(mvAnterior, IAtablero);
    salto = false;
    return (
        pro -
        10); // Si la computadora captura una pieza enemiga gana el valor de
             // "pro"(profundidad) menos 10 puntos. Incluso si la computadora
             // sabe que va a perder siguiendo cualquier camino, escogera el mas
             // largo y  tratara de alargar el juego lo mas que pueda, buscando
             // la oportunidad para que el humano se equivoque.
  } else {
    mover(mvAnterior, IAtablero);
  }

  // if(chequearGanador(IA,IAtablero)==true){return (pro -100);}

  int mejorPuntaje = -1001; //  si o si me guarda la primera jugada.
  Movimiento mejorJugada;
  mejorJugada.p = -1000; // va guardando el mejor movimiento y puntaje
  Movimiento mejorMovi;  // Mejor movimiento a retornar.
  Movimiento mv;         // Variable para enviar posiciones a evaluar.
  bool SD = false, SI = false;

  char copiaTablero[10][10]; // tablero auxiliar.

  for (int i = 0; i < 10; i++) { // crea una copia del original
    for (int j = 0; j < 10; j++) {
      copiaTablero[i][j] = IAtablero[i][j];
    }
  }

  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {

      if (IAtablero[i][j] == humanoDama) { // evalua los movimietos con damas

        int puntaje;

        // Busca un peon o dama enemiga dentro de la DIAGONAL INFERIOR DERECHA.

        int c = j + 1, f = i + 1; // Toma a la casilla siguiente como primera
                                  // casila a analizar.

        mv.c1 = j;
        mv.f1 = i; // El origen del movimiento a evaluar es la posicion del peon
                   // encontrado.

        while (c < 9 && f < 9) { // Limita la busqueda hasta la penultima
                                 // casilla del borde del tablero.

          if ((IAtablero[f][c] == iaPeon) || (IAtablero[f][c] == iaDama)) {

            mv.f2 = f + 1;
            mv.c2 =
                c + 1; // Guarda la casilla siguiente como la casilla destino.

            if ((chequearMovimiento(HUMANO, mv, IAtablero) == true) &&
                (salto == true)) {

              salto = false; // devuelve el valor de salto a defecto.

              puntaje = 1 + min(IAtablero, mv,
                                1); // le sumamos uno para que prefiera comer
                                    // con una dama en vez de un peon.

              for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                  IAtablero[i][j] = copiaTablero[i][j];
                }
              } // devuelve el valor del original.

              if (puntaje > mejorJugada.p) {
                mejorJugada.p = puntaje;
              }
            }
          }
          c++;
          f++;
        }

        // Busca un peon o dama enemiga dentro de la DIAGONAL INFERIOR
        // IZQUIERDA.

        c = j - 1;
        f = i +
            1; // Toma a la casilla siguiente como primera casilla a analizar.

        mv.f1 = i;
        mv.c1 = j; // Guarda la pieza como origen del movimiento

        while (c > 0 && f < 9) { // Limita la busqueda hasta la penultima
                                 // casilla del borde del tablero.

          if ((IAtablero[f][c] == iaPeon) || (IAtablero[f][c] == iaDama)) {

            mv.f2 = f + 1;
            mv.c2 =
                c - 1; // Guarda la casilla siguiente como la casilla destino.

            if ((chequearMovimiento(HUMANO, mv, IAtablero) == true) &&
                (salto == true)) {

              salto = false; // devuelve el valor de salto a defecto.

              puntaje = 1 + min(IAtablero, mv, 1);

              for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                  IAtablero[i][j] = copiaTablero[i][j];
                }
              } // devuelve el valor del original.

              if (puntaje > mejorJugada.p) {
                mejorJugada.p = puntaje;
              }
            }
          }
          c--;
          f++;
        }

        // Busca un peon o dama enemiga dentro de la DIAGONAL SUPERIOR
        // IZQUIERDA.

        c = j - 1;
        f = i -
            1; // Toma a la casilla siguiente como primera casilla a analizar.

        mv.f1 = i;
        mv.c1 = j; // Guarda la pieza como origen del movimiento

        while (c > 0 && f > 0) { // Limita la busqueda hasta la penultima
                                 // casilla del borde del tablero.

          if ((IAtablero[f][c] == iaPeon) ||
              (IAtablero[f][c] == iaDama)) { // Guarda la casilla siguiente como
                                             // la casilla destino.

            mv.f2 = f - 1;
            mv.c2 = c - 1;

            if ((chequearMovimiento(HUMANO, mv, IAtablero) == true) &&
                (salto == true)) {

              salto = false; // devuelve el valor de salto a defecto.

              puntaje = 1 + min(IAtablero, mv, 1);

              for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                  IAtablero[i][j] = copiaTablero[i][j];
                }
              } // devuelve el valor del original.

              if (puntaje > mejorJugada.p) {
                mejorJugada.p = puntaje;
              }
            }
          }
          c--;
          f--;
        }

        // Busca un peon o dama enemiga dentro de la DIAGONAL SUPERIOR DERECHA.

        c = j + 1,
        f = i -
            1; // Toma a la casilla siguiente como primera casilla a analizar.

        mv.f1 = i;
        mv.c1 = j; // Guarda la pieza como origen del movimiento.

        while (c < 9 && f > 0) { // Limita la busqueda hasta la penultima
                                 // casilla del borde del tablero.

          if ((IAtablero[f][c] == iaPeon) ||
              (IAtablero[f][c] == iaDama)) { // Guarda la casilla siguiente como
                                             // la casilla destino.

            mv.f2 = f - 1;
            mv.c2 = c + 1;

            if ((chequearMovimiento(HUMANO, mv, IAtablero) == true) &&
                (salto == true)) {

              salto = false; // devuelve el valor de salto a defecto.

              puntaje = 1 + min(IAtablero, mv, 1);

              for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                  IAtablero[i][j] = copiaTablero[i][j];
                }
              } // devuelve el valor del original.

              if (puntaje > mejorJugada.p) {
                mejorJugada.p = puntaje;
              }
            }
          }
          c++;
          f--;
        }

        if (mejorJugada.p >
            mejorPuntaje) { // Guarda el mejor movimiento entre todas las damas.

          mejorPuntaje = mejorJugada.p;
        }
      }

      if (IAtablero[i][j] ==
          humanoPeon) { // Evalua todos los movimientos con peones.

        int puntaje; // Guarda los puntajes de los cuatro movimientos posibles
                     // con peones.

        mv.c1 = j;
        mv.f1 = i; // El origen del movimiento a evaluar es la posicion del peon
                   // encontrado.

        mv.c2 = j + 2;
        mv.f2 = i - 2; // COORDENADAS PARA COMPROBAR SALTO POR LA DERECHA.

        if (mv.f1 > 1 && mv.c1 < 8 &&
            chequearMovimiento(HUMANO, mv, IAtablero) == true &&
            salto == true) { // Evalua primero el salto, de no ser posible hacer
                             // un movimiento simple.

          salto = false; // devuelve el valor de salto a defecto.

          puntaje = min(
              IAtablero, mv,
              pro + 1); // Llama a la funcion recursiva para obtener un puntaje.

          for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
              IAtablero[i][j] = copiaTablero[i][j];
            }
          } // devuelve el valor del original.

          if (puntaje > mejorJugada.p) {
            mejorJugada.p = puntaje;
          } // Si el puntaje obtenido es mejor que el actual mejor, actualiza el
            // mejor puntaje y movimiento.

          SD = true;
        }

        mv.c2 = j - 2;
        mv.f2 = i - 2; // COORDENADAS PARA COMPROBAR SALTO POR LA IZQUIERDA.

        if (mv.f1 > 1 && mv.c1 > 1 &&
            chequearMovimiento(HUMANO, mv, IAtablero) == true &&
            salto == true) { // Evalua primero el salto, de no ser posible hacer
                             // un movimiento simple.

          salto = false; // devuelve el valor de salto a defecto.

          puntaje = min(IAtablero, mv, pro + 1);

          for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
              IAtablero[i][j] = copiaTablero[i][j];
            }
          } // devuelve el valor del original.

          if (puntaje > mejorJugada.p) {
            mejorJugada.p = puntaje;
          }

          SI = true;
        }

        if (SD == false &&
            SI == false) { // analizara un movimiento simple, solo si no se
                           // puede saltar, ya que saltar es obligatorio.

          mv.c2 = j + 1;
          mv.f2 = i - 1; // COORDENADAS PARA COMPROBAR MOVIMIENTO POR LA
                         // DERECHA.

          if (mv.f1 > 0 && mv.c1 < 9 &&
              chequearMovimiento(HUMANO, mv, IAtablero) == true) {

            puntaje = min(IAtablero, mv, pro + 1);

            for (int i = 0; i < 10; i++) {
              for (int j = 0; j < 10; j++) {
                IAtablero[i][j] = copiaTablero[i][j];
              }
            } // devuelve el valor del original.

            if (puntaje > mejorJugada.p) {
              mejorJugada.p = puntaje;
            }
          }

          mv.c2 = j - 1;
          mv.f2 =
              i - 1; // COORDENADAS PARA COMPROBAR MOVIMIENTO POR LA IZQUIERDA.

          if (mv.f1 > 0 && mv.c1 > 0 &&
              chequearMovimiento(HUMANO, mv, IAtablero) == true) {

            puntaje = min(IAtablero, mv, pro + 1);

            for (int i = 0; i < 10; i++) {
              for (int j = 0; j < 10; j++) {
                IAtablero[i][j] = copiaTablero[i][j];
              }
            } // devuelve el valor del original.

            if (puntaje > mejorJugada.p) {
              mejorJugada.p = puntaje;
            }
          }
        }
        SD = false;
        SI = false;

        if (mejorJugada.p >
            mejorPuntaje) { // Guarda el mejor puntaje entre sus cuatro
                            // posibilidades entre todos los peones.

          mejorPuntaje = mejorJugada.p;
        }
      }
    }
  }
  return mejorPuntaje;
}

int Juego::min(char IAtablero[10][10], Movimiento mvAnterior, int pro) {

  if (profundidad(pro)) {
    return 0;
  } // Si se alcanza la profundidad retorna 0.

  if ((chequearMovimiento(HUMANO, mvAnterior, IAtablero) == true &&
       salto == true)) {
    mover(mvAnterior, IAtablero);
    salto = false;
    return (
        10 -
        pro); // Si el humano captura una pieza enemiga gana el valor de 10
              // menos "pro"(profundidad) puntos. Incluso si la computadora sabe
              // que va a perder siguiendo cualquier camino, escogera el mas
              // largo y tratara de alargar el juego lo mas que pueda, buscando
              // la oportunidad para que el humano se equivoque.
  } else {
    mover(mvAnterior, IAtablero);
  }

  // if(chequearGanador(HUMANO, IAtablero)==true){return (100 - pro); }//si
  // humano gana, retorna 100

  int mejorPuntaje = 1001; //  si o si me guarda la primera jugada.
  Movimiento mejorJugada;
  mejorJugada.p = 1000; // va guardando el mejor movimiento y puntaje
  Movimiento mejorMovi; // Mejor movimiento a retornar.
  Movimiento mv;        // Variable para enviar posiciones a evaluar.
  bool SD = false, SI = false;

  char copiaTablero[10][10]; // tablero auxiliar.

  for (int i = 0; i < 10; i++) { // crea una copia del original
    for (int j = 0; j < 10; j++) {
      copiaTablero[i][j] = IAtablero[i][j];
    }
  }

  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {

      if (IAtablero[i][j] == iaDama) { // evalua los movimietos con damas

        int puntaje;

        // Busca un peon o dama enemiga dentro de la DIAGONAL INFERIOR DERECHA.

        int c = j + 1, f = i + 1; // Toma a la casilla siguiente como primera
                                  // casila a analizar.

        mv.c1 = j;
        mv.f1 = i; // El origen del movimiento a evaluar es la posicion del peon
                   // encontrado.

        while (c < 9 && f < 9) { // Limita la busqueda hasta la penultima
                                 // casilla del borde del tablero.

          if ((IAtablero[f][c] == humanoPeon) ||
              (IAtablero[f][c] == humanoDama)) {

            mv.f2 = f + 1;
            mv.c2 =
                c + 1; // Guarda la casilla siguiente como la casilla destino.

            if ((chequearMovimiento(IA, mv, IAtablero) == true) &&
                (salto == true)) {

              salto = false; // devuelve el valor de salto a defecto.

              puntaje = -1 + max(IAtablero, mv,
                                 1); // le sumamos menos uno para que prefiera
                                     // comer con una dama en vez de un peon.

              for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                  IAtablero[i][j] = copiaTablero[i][j];
                }
              } // devuelve el valor del original.

              if (puntaje < mejorJugada.p) {
                mejorJugada.p = puntaje;
                mejorJugada.f1 = mv.f1;
                mejorJugada.c1 = mv.c1;
                mejorJugada.f2 = mv.f2;
                mejorJugada.c2 = mv.c2;
              }
            }
          }
          c++;
          f++;
        }

        // Busca un peon o dama enemiga dentro de la DIAGONAL INFERIOR
        // IZQUIERDA.

        c = j - 1;
        f = i +
            1; // Toma a la casilla siguiente como primera casilla a analizar.

        mv.f1 = i;
        mv.c1 = j; // Guarda la pieza como origen del movimiento

        while (c > 0 && f < 9) { // Limita la busqueda hasta la penultima
                                 // casilla del borde del tablero.

          if ((IAtablero[f][c] == humanoPeon) ||
              (IAtablero[f][c] == humanoDama)) {

            mv.f2 = f + 1;
            mv.c2 =
                c - 1; // Guarda la casilla siguiente como la casilla destino.

            if ((chequearMovimiento(IA, mv, IAtablero) == true) &&
                (salto == true)) {

              salto = false; // devuelve el valor de salto a defecto.

              puntaje = -1 + max(IAtablero, mv, 1);

              for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                  IAtablero[i][j] = copiaTablero[i][j];
                }
              } // devuelve el valor del original.

              if (puntaje < mejorJugada.p) {
                mejorJugada.p = puntaje;
                mejorJugada.f1 = mv.f1;
                mejorJugada.c1 = mv.c1;
                mejorJugada.f2 = mv.f2;
                mejorJugada.c2 = mv.c2;
              }
            }
          }
          c--;
          f++;
        }

        // Busca un peon o dama enemiga dentro de la DIAGONAL SUPERIOR
        // IZQUIERDA.

        c = j - 1;
        f = i -
            1; // Toma a la casilla siguiente como primera casilla a analizar.

        mv.f1 = i;
        mv.c1 = j; // Guarda la pieza como origen del movimiento

        while (c > 0 && f > 0) { // Limita la busqueda hasta la penultima
                                 // casilla del borde del tablero.

          if ((IAtablero[f][c] == humanoPeon) ||
              (IAtablero[f][c] == humanoDama)) { // Guarda la casilla siguiente
                                                 // como la casilla destino.

            mv.f2 = f - 1;
            mv.c2 = c - 1;

            if ((chequearMovimiento(IA, mv, IAtablero) == true) &&
                (salto == true)) {

              salto = false; // devuelve el valor de salto a defecto.

              puntaje = -1 + max(IAtablero, mv, 1);

              for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                  IAtablero[i][j] = copiaTablero[i][j];
                }
              } // devuelve el valor del original.

              if (puntaje < mejorJugada.p) {
                mejorJugada.p = puntaje;
                mejorJugada.f1 = mv.f1;
                mejorJugada.c1 = mv.c1;
                mejorJugada.f2 = mv.f2;
                mejorJugada.c2 = mv.c2;
              }
            }
          }
          c--;
          f--;
        }

        // Busca un peon o dama enemiga dentro de la DIAGONAL SUPERIOR DERECHA.

        c = j + 1,
        f = i -
            1; // Toma a la casilla siguiente como primera casilla a analizar.

        mv.f1 = i;
        mv.c1 = j; // Guarda la pieza como origen del movimiento.

        while (c < 9 && f > 0) { // Limita la busqueda hasta la penultima
                                 // casilla del borde del tablero.

          if ((IAtablero[f][c] == humanoPeon) ||
              (IAtablero[f][c] == humanoDama)) { // Guarda la casilla siguiente
                                                 // como la casilla destino.

            mv.f2 = f - 1;
            mv.c2 = c + 1;

            if ((chequearMovimiento(IA, mv, IAtablero) == true) &&
                (salto == true)) {

              salto = false; // devuelve el valor de salto a defecto.

              puntaje = -1 + max(IAtablero, mv, 1);

              for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                  IAtablero[i][j] = copiaTablero[i][j];
                }
              } // devuelve el valor del original.

              if (puntaje < mejorJugada.p) {
                mejorJugada.p = puntaje;
                mejorJugada.f1 = mv.f1;
                mejorJugada.c1 = mv.c1;
                mejorJugada.f2 = mv.f2;
                mejorJugada.c2 = mv.c2;
              }
            }
          }
          c++;
          f--;
        }

        if (mejorJugada.p <
            mejorPuntaje) { // Guarda el mejor movimiento entre todas las damas.

          mejorPuntaje = mejorJugada.p;
          mejorMovi.f1 = mejorJugada.f1;
          mejorMovi.c1 = mejorJugada.c1;
          mejorMovi.f2 = mejorJugada.f2;
          mejorMovi.c2 = mejorJugada.c2;
        }
      }

      if (IAtablero[i][j] ==
          iaPeon) { // Evalua todos los movimientos con peones.

        int puntaje;

        mv.c1 = j;
        mv.f1 = i; // El origen del movimiento a evaluar es la posicion del peon
                   // encontrado.

        mv.c2 = j + 2;
        mv.f2 = i + 2; // COORDENADAS PARA COMPROBAR SALTO POR LA DERECHA.

        if (mv.f1 < 8 && mv.c1 < 8 &&
            chequearMovimiento(IA, mv, IAtablero) == true &&
            salto == true) { // Evalua primero el salto, de no ser posible hacer
                             // un movimiento simple.

          salto = false; // devuelve el valor de salto a defecto.

          puntaje = max(
              IAtablero, mv,
              pro + 1); // Llama a la funcion recursiva para obtener un puntaje.

          for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
              IAtablero[i][j] = copiaTablero[i][j];
            }
          } // devuelve el valor del original.

          if (puntaje < mejorJugada.p) {
            mejorJugada.p = puntaje;
          } // Si el puntaje obtenido es mejor que el actual mejor, actualiza el
            // mejor puntaje y movimiento.

          SD = true;
        }

        mv.c2 = j - 2;
        mv.f2 = i + 2; // COORDENADAS PARA COMPROBAR SALTO POR LA IZQUIERDA.

        if (mv.f1 < 8 && mv.c1 > 1 &&
            chequearMovimiento(IA, mv, IAtablero) == true &&
            salto == true) { // Evalua primero el salto, de no ser posible hacer
                             // un movimiento simple.

          salto = false; // devuelve el valor de salto a defecto.

          puntaje = max(IAtablero, mv, pro + 1);

          for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
              IAtablero[i][j] = copiaTablero[i][j];
            }
          } // devuelve el valor del original.

          if (puntaje < mejorJugada.p) {
            mejorJugada.p = puntaje;
          }

          SI = true;
        }

        if (SD == false &&
            SI == false) { // Evaluara movimiento simplo solo si no existe
                           // ninguna posibilidad de saltar

          mv.c2 = j + 1;
          mv.f2 = i + 1; // COORDENADAS PARA COMPROBAR MOVIMIENTO POR LA
                         // DERECHA.

          if (mv.f1 < 9 && mv.c1 < 9 &&
              chequearMovimiento(IA, mv, IAtablero) == true) {

            puntaje = max(IAtablero, mv, pro + 1);

            for (int i = 0; i < 10; i++) {
              for (int j = 0; j < 10; j++) {
                IAtablero[i][j] = copiaTablero[i][j];
              }
            } // devuelve el valor del original.

            if (puntaje < mejorJugada.p) {
              mejorJugada.p = puntaje;
            }
          }

          mv.c2 = j - 1;
          mv.f2 =
              i + 1; // COORDENADAS PARA COMPROBAR MOVIMIENTO POR LA IZQUIERDA.

          if (mv.f1 < 9 && mv.c1 > 0 &&
              chequearMovimiento(IA, mv, IAtablero) == true) {

            puntaje = max(IAtablero, mv, pro + 1);

            for (int i = 0; i < 10; i++) {
              for (int j = 0; j < 10; j++) {
                IAtablero[i][j] = copiaTablero[i][j];
              }
            } // devuelve el valor del original.

            if (puntaje < mejorJugada.p) {
              mejorJugada.p = puntaje;
            }
          }
        }

        SD = false;
        SI = false;

        if (mejorJugada.p <
            mejorPuntaje) { // Guarda el mejor movimiento entre sus cuatro
                            // posibilidades entre todos los peones.

          mejorPuntaje = mejorJugada.p;
        }
      }
    }
  }
  return mejorPuntaje;
}
/*
int Juego::puntaje(Jugador jugador, Movimiento mv,char TABLERO[10][10]){

    if(jugador == HUMANO){

        if(chequearMovimiento(HUMANO,mv,TABLERO) == true && salto == true)
{salto=false; return 10; }

    }else if(jugador == IA){

        if(chequearMovimiento(IA,mv,TABLERO) == true && salto == true)
{salto=false; return -10; }

    }else return 0;
}
*/
int main(int argc, char **argv) {

  Juego damas;

  damas.jugar();

  return 0;
}
