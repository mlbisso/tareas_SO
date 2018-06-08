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
