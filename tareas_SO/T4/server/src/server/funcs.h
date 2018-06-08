#pragma once

struct carta;
typedef struct carta Carta;
struct carta
{
  int numero;
  int pinta;
};

Carta* carta_init(int numero, int pinta);
Carta ** inicializar_mazo();

void barajar(Carta **array, int a, int b);

int  binary_to_decimal(char* binary, int tamano);
char * concatenate_two_strings(char* server_reply);
void int_to_bits(char* resultado, int numero, int tamano);
int obtener_tamano_bytes(int numero);