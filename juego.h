#ifndef JUEGO_H
#define JUEGO_H
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
using namespace std;
const int PV_INICIAL = 100;
const int PH_INICIAL = 20;
const int PV_GOBLIN = 30;
const int PH_GOBLIN = 10;
const int PV_ARQUERO = 20;
const int PH_ARQUERO = 10;
const int PV_JEFE = 60;
const int PH_JEFE = 40;
struct Jugador
{
    char nombre[50];
    int fila;
    int columna;
    int PV;
    int PH;
    int oro;
};
struct Enemigo
{
    char tipo;
    int fila;
    int columna;
    int PV;
    int PH;
    int vivo;
    int descubierto;
};
struct Cofre
{
    int fila;
    int columna;
    char tipo;
    int abierto;
};
struct Estado
{
    int N;
    int cantidadE;
    int cantidadC;
    int turno;
    int terminado;
    int gano;
    int salidaFila;
    int salidaColumna;
};
struct Partida
{
    int N;
    char **tablero;
    Jugador jugador;
    Enemigo *enemigos;
    Cofre *cofres;
    int cantidadEnemigos;
    int cantidadCofres;
    int turno;
    int terminado;
    int gano;
    int salidaFila;
    int salidaColumna;
};
const char ARCHIVO_ESTADO[]="estado.bin";
const char ARCHIVO_TABLERO[]="tablero.bin";
const char ARCHIVO_JUGADOR[]="jugador.bin";
const char ARCHIVO_ENEMIGOS[]="enemigos.bin";
const char ARCHIVO_COFRES[]="cofres.bin";
const char ARCHIVO_REPORTE[]="reporte.txt";

void iniciarJuego();
void Tablero(char **&tablero,int N);
void liberarTablero(char **tablero,int N);
void iniciarPartida(Partida &partida);
void liberarPartida(Partida &partida);
void iniciarJugador(Partida &partida);
void generarSalida(Partida &partida);
void reiniciarTablero(Partida &partida);
void mostrarTablero();
void mostrarEstadisticas();
void moverJugador(const char *direccion);
void explorarCofre();
void atacar();
void actualizarTurno(Partida &partida);
void atacArqueros(Partida &partida);
void atacEnemigosMismaCasilla(Partida &partida);
void mJefe(Partida &partida);
void descubrirEnemigos(Partida &partida);
void comprobarFin(Partida &partida);
bool guardarPartida(Partida &partida);
bool cargarPartida(Partida &partida);
void generarEnemigos(Partida &partida,int cantidadGoblins,int cantidadArqueros);
void generarCofres(Partida &partida,int cantidad);
int Puntaje(Jugador &jugador,int gano);
void guardarReporte(Jugador &jugador,int gano);
bool dentroTablero(int fila,int columna,int N);
bool posicionOcupada(Partida &partida,int fila,int columna);
bool hayEnemigoVivo(Partida &partida,int fila,int columna);
bool hayCofre(Partida &partida,int fila,int columna);
int buscarEnemigo(Partida &partida,int fila,int columna);
int buscarCofre(Partida &partida,int fila,int columna);
#endif
