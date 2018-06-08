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

int binary_to_decimal(char* binary, int tamano){
	char* start;
	int total = 0;
	int i = 0;
    start = &binary[0];
    while (*start && tamano != i)
    {
    	i ++;
        total *= 2;
        if (*start++ == '1') total += 1;
    }
    return total;
}

char * concatenate_two_strings(char* server_reply){
    char* name_with_extension;
    char otherString[8];
    char otherString_2[8]; 
    strncpy(otherString, server_reply + 16, 8);
    strncpy(otherString_2, server_reply + 24, 8);
    otherString[8] = '\0'; // place the null terminator
    otherString_2[8] = '\0'; // place the null terminator
    name_with_extension = malloc(8+1+8); /* make space for the new string (should check the return value ...) */
    strcpy(name_with_extension, otherString); /* copy name into the new var */
    strcat(name_with_extension, otherString_2); /* add the extension */
    return name_with_extension;
}

void int_to_bits(char* resultado, int numero, int tamano){
	int i = tamano - 1;
	while(numero != 0){
		if (numero % 2 == 0){			//hay que agregar un cero al final
			memcpy(resultado + i, "0", 1);
		}
		else if(numero % 2 == 1){
			memcpy(resultado + i, "1", 1);
		}	
		numero = (int)(numero / 2);
		i --;	
	}
	for (int j = i; j >= 0; j --){
		memcpy(resultado + j, "0", 1);
	}
	resultado[tamano] = '\0';
}

int obtener_tamano_bytes(int numero){
	int i = 0;
	while(numero != 0){	
		numero = (int)(numero / 2);
		i ++;	
	}
	int resto = 8 - (i % 8);
	if (resto != 8){
		i += resto;
	}
	return i;
}