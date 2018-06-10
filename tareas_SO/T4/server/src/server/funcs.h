#pragma once

struct carta;
typedef struct carta Carta;
struct carta
{
  int numero;
  int pinta;
  int valido;
  int agregada;
  int contada;
};

Carta* carta_init(int numero, int pinta);
Carta ** inicializar_mazo();
Carta ** inicializar_mano();

void barajar(Carta **array, int a, int b);

int  binary_to_decimal(char* binary, int tamano);
char * concatenate_two_strings(char* server_reply);
void int_to_bits(char* resultado, int numero, int tamano);
int obtener_tamano_bytes(int numero);

int repartir_cartas(Carta **mazo, Carta ** mano, int carta_superior);
int cambiar_carta(Carta ** mazo, Carta **mano, int carta, int pinta, int carta_superior);                        
int quien_empieza();
int quien_gano(Carta ** mano_j1, Carta ** mano_j2);

int obtener_bet(int bet);
int obtener_id_bet(int bet);
int actualizar_pot(int pot, int bet);

int puntaje(Carta ** mano);




Carta ** ordenar_mano(Carta ** mano);


// Manos
int escala_real(Carta ** mano);

int escala_color(Carta ** mano);
int puntos_escala_color(Carta ** mano);

int poker(Carta ** mano);
int puntos_poker(Carta ** mano);

int full(Carta ** mano);
int puntos_full(Carta ** mano);

int color(Carta ** mano);
int puntos_color(Carta ** mano);

int escala(Carta ** mano);
int puntos_escala(Carta ** mano);

int trio(Carta ** mano);
int puntos_trio(Carta ** mano);

int doble_par(Carta ** mano);
int puntos_doble_par(Carta ** mano);

int par(Carta ** mano);
int puntos_par(Carta ** mano);
