#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "funcs.h"
#include <string.h>
#include <stdint.h>
#include <time.h>


Carta* carta_init(int numero, int pinta)
{
	Carta* carta = malloc(sizeof(Carta));
	carta -> numero = numero;
	carta -> pinta = pinta;
  	return carta;
}

void barajar(Carta **array, int a, int b) {
    srand((unsigned)time(NULL));
    for (int i = 0; i < a; i++) {
        for (int z = 0; z < b; z++){
            size_t j = i + rand() / (RAND_MAX / (a - i) + 1);
            size_t f = z + rand() / (RAND_MAX / (b - z) + 1);
            Carta t = array[j][f];
            array[j][f] = array[i][z];
            array[i][z] = t;           
        }
    }
}

Carta ** inicializar_mazo(){
    int valores[13] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
    int pintas[4] = {1, 2, 3, 4};

    Carta **cartas = (Carta **) malloc(13 * sizeof(Carta *));

    for(int i = 0; i < 13; i++){
        cartas[i] = (Carta *) malloc(4 * sizeof(Carta));
    }

    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 4; j++){
            Carta *carta = carta_init(valores[i], pintas[j]);
            cartas[i][j] = *carta;
        }
    }

    barajar(cartas, 13, 4);
    // for (int i = 0; i < 13; i++) {
    //     for (int j = 0; j < 4; j++){
    //         printf("numero %d\n", cartas[i][j].numero);
    //         printf("pinta %d\n\n", cartas[i][j].pinta);
    //     }
    // }
    return cartas;
}