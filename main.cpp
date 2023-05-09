//
//  main.cpp
//
//  Created by Manuel Pastrana Blazquez on 30/3/23.
//


#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;

class Tablero {
  private:
    int filas;
    int columnas;
    int minas;
    char** tablero;
    bool** mina;
    bool** bandera;

  public:
    Tablero(int f, int c, int m) {
      filas = f;
      columnas = c;
      minas = m;

      // Inicializar matrices dinámicas
      tablero = new char*[filas];
      mina = new bool*[filas];
      bandera = new bool*[filas];

      for (int i = 0; i < filas; i++) {
        tablero[i] = new char[columnas];
        mina[i] = new bool[columnas];
        bandera[i] = new bool[columnas];

        for (int j = 0; j < columnas; j++) {
          tablero[i][j] = '_';
          mina[i][j] = false;
          bandera[i][j] = false;
        }
      }
    }

    ~Tablero() {
      // Liberar memoria de matrices dinámicas
      for (int i = 0; i < filas; i++) {
        delete[] tablero[i];
        delete[] mina[i];
        delete[] bandera[i];
      }

      delete[] tablero;
      delete[] mina;
      delete[] bandera;
    }

    void colocar_minas() {
      int cantidad = 0;
      while (cantidad < minas) {
        int x = rand() % filas;
        int y = rand() % columnas;

        if (!mina[x][y]) {
          mina[x][y] = true;
          cantidad++;
        }
      }
    }

    void imprimir_tablero() {
      cout << "  ";
      for (int i = 0; i < columnas; i++) {
        cout << i << " ";
      }
      cout << endl;

      for (int i = 0; i < filas; i++) {
        cout << i << " ";
        for (int j = 0; j < columnas; j++) {
          if (bandera[i][j]) {
            cout << 'B' << " ";
          } else {
            cout << tablero[i][j] << " ";
          }
        }
        cout << endl;
      }
    }

    bool descubrir_celda(int f, int c) {
      if (mina[f][c]) {
        cout << "" << endl;
        cout << " ‎ ‎ ‎ ‎ ‎ ‎¡Has encontrado una mina!" << endl;
        cout << "" << endl;
        cout << " ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎GAME OVER!" << endl;
        cout << "" << endl;
        return true;
      }

      if (tablero[f][c] == '_') {
        descubrir_celdas_adjacentes(f, c);
      } else {
        // Contar minas adyacentes
        int cantidad = 0;
        for (int i = f - 1; i <= f + 1; i++) {
          for (int j = c - 1; j <= c + 1; j++) {
            if (i >= 0 && i < filas && j >= 0 && j < columnas && mina[i][j]) {
              cantidad++;
            }
          }
        }

        tablero[f][c] = cantidad + '0';
      }

      // Imprimir tablero actualizado
      imprimir_tablero();

      return false;
    }

    bool colocar_bandera(int f, int c) {
      if (tablero[f][c] == '_') {
        tablero[f][c] = 'B';
        bandera[f][c] = true;
        return true;
      } else {
        char resp;
        cout << "" << endl;
        cout << "  ‎¿Desea eliminar la bandera de esta posición? S/N " << endl;
        cin >> resp;
        if (resp == 'N'|| resp == 'n'){
        return false;
        }else{
            tablero[f][c] = '_';
            bandera[f][c] = false;
            return true;}
        }
    }

    bool verificar_victoria(string nombre) {
      bool victoria = true;
      int minas_correctas = 0;
      for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
          if (tablero[i][j] == 'B' && mina[i][j]) {
            minas_correctas++;
          } else if (tablero[i][j] != 'B' && mina[i][j]) {
            victoria = false;
          }
        }
      }

      if (victoria && minas_correctas == minas) {
        cout << "" << endl;
        cout << " ‎ ‎ ‎ ‎ ‎ ‎¡Todas las minas han sido descubietas!" << endl;
        cout << " ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎¡" << nombre << " HA GANADO LA PARTIDA!" << endl;
        return true;
      }

      return false;
    }

  private:
    void descubrir_celdas_adjacentes(int f, int c) {
      if (f < 0 || f >= filas || c < 0 || c >= columnas || tablero[f][c] != '_') {
        return;
      }

      int cantidad = 0;
      for (int i = f - 1; i <= f + 1; i++) {
        for (int j = c - 1; j <= c + 1; j++) {
          if (i >= 0 && i < filas && j >= 0 && j < columnas && mina[i][j]) {
            cantidad++;
          }
        }
      }

      tablero[f][c] = cantidad + '0';

      if (cantidad == 0) {
        for (int i = f - 1; i <= f + 1; i++) {
          for (int j = c - 1; j <= c + 1; j++) {
            if (i >= 0 && i < filas && j >= 0 && j < columnas) {
              descubrir_celdas_adjacentes(i, j);
            }
          }
        }
      }
    }
};

const int MAX_FILAS = 10;
const int MAX_COLUMNAS = 10;

int main() {
  int nivel;
  bool seguir_jugando = true;

  srand(time(NULL));

  string nombre;
  int partidas_ganadas = 0;

  // Vector para almacenar las victorias de cada jugador
  vector<pair<string, int>> victorias_jugadores;

  while(seguir_jugando){
    cout << "" << endl;
    cout << "" << endl;
    cout << " ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎!Hola! ¿Cual es tu nombre? ";
    cin >> nombre;

    char respuesta = 'S';
    int victorias = 0;
    while(respuesta == 'S' || respuesta == 's'){
        while(true){
          cout << "" << endl;
          cout << " ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ Espero que te diviertas, " << nombre << endl;
          cout << "" << endl;
          cout << "" << endl;
          cout << "" << endl;
          cout << " ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎+------------------------------+" << endl;
          cout << "‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎| ‎ ‎!BIENVENIDO AL BUSCAMINAS! ‎ ‎|" << endl;
          cout << " ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎+------------------------------+" << endl;
          cout << "" << endl;
          cout << "‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎+---------------------------------------+" << endl;
          cout << "‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎| ‎ ‎ ‎Seleccione el nivel de dificultad ‎ ‎ ‎|" << endl;
          cout << "‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎+---------------------------------------+" << endl;
          cout << "‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎|  ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎Nivel fácil (1) ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎|" << endl;
          cout << "‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎+---------------------------------------+" << endl;
          cout << "‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎|  ‎ ‎ ‎ ‎ ‎ ‎  ‎ ‎ ‎ ‎Nivel experto (2) ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎|" << endl;
          cout << "‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎+---------------------------------------+" << endl;
          cout << "‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎|  ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎Nivel personalizado (3) ‎ ‎ ‎   ‎ |" << endl;
          cout << "‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎+---------------------------------------+" << endl;
          cout << "" << endl;
          cin >> nivel;

          // Inicializar tablero
          int filas, columnas, minas;
          if (nivel == 1) {
            filas = 6;
            columnas = 6;
            minas = 5;
          } else if (nivel == 2) {
            filas = 9;
            columnas = 9;
            minas = 10;
          } else {
            cout << "" << endl;
            cout << " ‎ ‎ ‎ ‎ ‎ ‎Ingrese el número de filas: ";
            cin >> filas;
            cout << "" << endl;
            cout << " ‎ ‎ ‎ ‎ ‎ ‎Ingrese el número de columnas: ";
            cin >> columnas;
            cout << "" << endl;
            cout << " ‎ ‎ ‎ ‎ ‎ ‎Ingrese la cantidad de minas: ";
            cout << "" << endl;
            cin >> minas;
          }

          if (filas > MAX_FILAS || columnas > MAX_COLUMNAS) {
            cout << "" << endl;
            cout << " ‎ ‎ ‎ ‎ ‎ ‎El tamaño del tablero no es válido." << endl;
            return 0;
          }

          Tablero t(filas, columnas, minas);

          // Colocar minas aleatoriamente
          t.colocar_minas();

          // Imprimir tablero
          t.imprimir_tablero();

          // Jugar
          while (true) {

            // Condiciones de victoria
            if (t.verificar_victoria(nombre)) {
              victorias++;
              partidas_ganadas++;
              break;
            }

            // Pedir jugada
            cout << "" << endl;
            cout << " ‎ ‎ ‎ ‎ ‎ ‎Ingresa fila y columna (B para poner o quitar una bandera): ";
            char opcion;
            cin >> opcion;

            if (opcion == 'B' || opcion == 'b') {
              cout << "" << endl;
              cout << " ‎ ‎ ‎ ‎ ‎ ‎Ingresa fila y columna para colocar bandera: ";
              cin >> filas >> columnas;

              if (t.colocar_bandera(filas, columnas)) {
                // Imprimir tablero actualizado
                t.imprimir_tablero();
              }

              continue;
            }

            filas = opcion - '0';
            cin >> columnas;

            if (t.descubrir_celda(filas, columnas)) {
              break;
            }

          }

          // Registrar victoria del jugador
          victorias_jugadores.push_back(make_pair(nombre, victorias));

          cout << "" << endl;
          cout << " ‎ ‎ ‎ ‎ ‎ ‎Jugador: " << nombre << endl;
          cout << "" << endl;
          cout << " ‎ ‎ ‎ ‎ ‎ ‎Partidas ganadas: " << victorias << endl;

          cout << "" << endl;
          cout << "" << endl;
          cout << " ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎+---------------------------------------------------+" << endl;
          cout << " ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎|¿Desea seguir jugando? (S) ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎|" << endl;
          cout << " ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎  ‎+---------------------------------------------------+" << endl;
          cout << " ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎|Cerrar Sesión y volver a selección de jugador (V) ‎ ‎|" << endl;
          cout << " ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎+---------------------------------------------------+" << endl;
          cout << " ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ |‎Historial de partidas (H) ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎|" << endl;
          cout << " ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎+---------------------------------------------------+" << endl;
            cout << "" << endl;
          cin >> respuesta;

          if(respuesta == 'V' || respuesta == 'v' || respuesta == 'H' || respuesta == 'h'){
              break;
          }
        }
        if(respuesta == 'V' || respuesta == 'v'){
            break;
        }
        else if(respuesta == 'H' || respuesta == 'h'){
            cout << "" << endl;
            cout << "" << endl;
            cout << " ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎+----------------------+" << endl;
            cout << " ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎|Historial de partidas:|" << endl;
            cout << " ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎ ‎+----------------------+" << endl;
            cout << "" << endl;
            cout << "" << endl;
            cout << " ‎ ‎ ‎ ‎ ‎--------------------------------------" << endl;
            for(int i=0; i<victorias_jugadores.size(); i++){
              cout << "" << endl;
              cout << " ‎ ‎ ‎ ‎ ‎Jugador: " << victorias_jugadores[i].first << endl;
              cout << " ‎ ‎ ‎ ‎ ‎Partidas ganadas: " << victorias_jugadores[i].second << endl;
              cout << "" << endl;
              cout << " ‎ ‎ ‎ ‎ --------------------------------------" << endl;
            }
        }
    }
  }

  return 0;
}

