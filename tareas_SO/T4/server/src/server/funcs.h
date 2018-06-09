#pragma once

struct carta;
typedef struct carta Carta;
struct carta
{
  int numero;
  int pinta;
  int valido;
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

int quien_empieza();
int quien_gano(Carta ** mano_j1, Carta ** mano_j2);