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
    carta -> valido = 1;
    carta -> agregada = 0;
    carta -> contada = 0;
  	return carta;
}

int obtener_id_bet(int bet){
    int id = 1;
    if (bet == 0){
        id = 2;
    }
    if (bet == 100){
        id = 3;
    }
    if (bet == 200){
        id = 4;
    }
    if (bet == 500){
        id = 5;
    }
    return id;

}

int obtener_bet(int bet){
    if (bet == 1){
        return -1;
    }
    if (bet == 2){
        return 0;
    }
    if (bet == 3){
        return 100;
    }
    if (bet == 4){
        return 200;
    }
    if (bet == 5){
        return 500;
    }
    return -1;
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

// Carta ** inicializar_mano(){

// }

int cambiar_carta(Carta ** mazo, Carta **mano, int carta, int pinta, int carta_superior){
    int z;
    for (z = 0; z < 5; z++){
        if (mano[z][0].numero == carta && mano[z][0].pinta == pinta){
            break;
        }
    }
    int listo = 0;
    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 4; j++){
            // printf("%d %d\n", mazo[i][j].numero, mazo[i][j].pinta);
            if (mazo[i][j].valido == 1){
                mazo[i][j].valido = 0;
                Carta *carta = carta_init(mazo[i][j].numero, mazo[i][j].pinta);
                mano[z][0] = *carta;
                printf("%d %d\n", mano[z][0].numero, mano[z][0].pinta);
                listo = 1;
                break;
            }
        }
        if (listo == 1){
            break;
        }
    }
    return carta_superior + 1;
}


int repartir_cartas(Carta **mazo, Carta ** mano, int carta_superior){
    // int fila = (int)(carta_superior/13);
    // int columna = (int)(carta_superior/4) + 1;
    int z = 0;
    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 4; j++){
            if (z == 5){
                break;
            }
            // printf("%d %d\n", mazo[i][j].numero, mazo[i][j].pinta);
            if (mazo[i][j].valido == 1){
                mazo[i][j].valido = 0;
                Carta *carta = carta_init(mazo[i][j].numero, mazo[i][j].pinta);
                mano[z][0] = *carta;
                // printf("%d %d\n", mano[z][0].numero, mano[z][0].pinta);
                z ++;                
            }
        }
        if (z == 5){
            break;
        }
    }
    // printf("\n");
    return carta_superior + 5;
}

int quien_empieza(){
    srand(time(NULL));  
    int r = rand() % 2;
    return r;
}


int actualizar_pot(int pot, int bet){
    return pot - bet;
}

Carta ** ordenar_mano(Carta ** mano){
    Carta ** nueva_mano = inicializar_mano();
    int minimo = 15;
    for (int i = 0; i < 5; i++){
        for (int j = 0; j < 5; j++){
            if (mano[j][0].numero < minimo && mano[j][0].agregada == 0){
                minimo = mano[j][0].numero;
            }
        }
        for (int k = 0; k < 5; k++){
            if (mano[k][0].numero == minimo && mano[k][0].agregada == 0){
                Carta * nueva_carta = carta_init(mano[k][0].numero, mano[k][0].pinta);
                nueva_mano[i][0] = *nueva_carta;
                mano[k][0].agregada = 1;
                break;
            }
        }
        minimo = 15;
    }
    for (int i = 0; i < 5; i++){
        mano[i][0].agregada = 0;
    }
    return nueva_mano;
}


int escala_real_color(Carta ** mano){
    int pinta = mano[0][0].pinta;
    for (int i = 1; i < 5; i++){
        if (mano[i][0].pinta != pinta){
            return 0;
        }
    }
    if (mano[0][0].numero == 1 && mano[1][0].numero == 10 && mano[2][0].numero == 11 && mano[3][0].numero == 12 && mano[4][0].numero == 13){
        return 1;
    }
    return 0;
}

int escala_color(Carta ** mano){
    int pinta = mano[0][0].pinta;
    for (int i = 1; i < 5; i++){
        if (mano[i][0].pinta != pinta){
            return 0;
        }
    }
    int numero = mano[0][0].numero;
    if (numero + 1 == mano[1][0].numero && numero + 2 == mano[2][0].numero && numero + 3 == mano[3][0].numero && numero + 4 == mano[4][0].numero){
        return 1;
    }
    return 0;
}

int puntos_escala_color(Carta ** mano){
    int puntos = mano[4][0].numero;
    return puntos;
}

int poker(Carta ** mano){
    int cartas_iguales = 0;
    for (int i = 0; i < 5; i++){
        int numero_a_comparar = mano[i][0].numero;
        for (int j = 0; j < 5; j++){
            if (mano[j][0].numero == numero_a_comparar){
                cartas_iguales += 1;
            }
        }
        if (cartas_iguales == 4){
            return 1;
        }
        cartas_iguales = 0;
    }
    return 0;
}

int puntos_poker(Carta ** mano){
    int puntos;
    int cartas_iguales = 0;
    int numero_poker;
    int kicker;
    for (int i = 0; i < 5; i++){
        int numero_a_comparar = mano[i][0].numero;
        for (int j = 0; j < 5; j++){
            if (mano[j][0].numero == numero_a_comparar){
                cartas_iguales += 1;
            }
        }
        if (cartas_iguales == 4){
            numero_poker = numero_a_comparar;
            if (numero_poker == 1){
                puntos = 14;
            } else {
                puntos = numero_poker;
            }
            break;
        }
        cartas_iguales = 0;
    }
    for (int i = 0; i < 5; i++){
        if (mano[i][0].numero != numero_poker){
            kicker = mano[i][0].numero;
            puntos += kicker;
            break;
        }
    }
    return puntos;
}

int full(Carta ** mano){
    int cartas_iguales = 0;
    int numero_trio = 0;
    int numero_par = 0;
    for (int i = 0; i < 5; i++){
        int numero_a_comparar = mano[i][0].numero;
        for (int j = 0; j < 5; j++){
            if (mano[j][0].numero == numero_a_comparar && mano[j][0].contada == 0){
                cartas_iguales += 1;
            }
        }
        if (cartas_iguales == 3){
            numero_trio = numero_a_comparar;
            for (int k = 0; k < 5; k++){
                if (mano[k][0].numero == numero_trio){
                    mano[k][0].contada = 1;
                }
            }
        } else if (cartas_iguales == 2){
            numero_par = numero_a_comparar;
            for (int k = 0; k < 5; k++){
                if (mano[k][0].numero == numero_par){
                    mano[k][0].contada = 1;
                }
            }
        }
        cartas_iguales = 0;
    }
    for (int i = 0; i < 5; i++){
        mano[i][0].contada = 0;
    }
    if (numero_par && numero_trio){
        return 1;
    } else {
        return 0;
    }
}

int puntos_full(Carta ** mano){
    int puntos = 0;
    int cartas_iguales = 0;
    int numero_trio = 0;
    int numero_par = 0;
    for (int i = 0; i < 5; i++){
        int numero_a_comparar = mano[i][0].numero;
        for (int j = 0; j < 5; j++){
            if (mano[j][0].numero == numero_a_comparar && mano[j][0].contada == 0){
                cartas_iguales += 1;
            }
        }
        if (cartas_iguales == 3){
            numero_trio = numero_a_comparar;
            for (int k = 0; k < 5; k++){
                if (mano[k][0].numero == numero_trio){
                    mano[k][0].contada = 1;
                }
            }
        } else if (cartas_iguales == 2){
            numero_par = numero_a_comparar;
            for (int k = 0; k < 5; k++){
                if (mano[k][0].numero == numero_par){
                    mano[k][0].contada = 1;
                }
            }
        }
        cartas_iguales = 0;
    }
    for (int i = 0; i < 5; i++){
        mano[i][0].contada = 0;
    }
    if (numero_trio == 1){
        puntos += 14*3;
    } else {
        puntos += numero_trio*3;
    }
    if (numero_par == 1){
        puntos += 14*2;
    } else {
        puntos += numero_par*2;
    }
    return puntos;
}

int color(Carta ** mano){
    int pinta = mano[0][0].pinta;
    for (int i = 1; i < 5; i++){
        if (mano[i][0].pinta != pinta){
            return 0;
        }
    }
    return 1;
}

int puntos_color(Carta ** mano){
    int puntos = mano[4][0].numero;
    return puntos;
}

int escala(Carta ** mano){
    int numero = mano[0][0].numero;
    if (numero + 1 == mano[1][0].numero && numero + 2 == mano[2][0].numero && numero + 3 == mano[3][0].numero && numero + 4 == mano[4][0].numero){
        return 1;
    }
    if (mano[0][0].numero == 1 && mano[1][0].numero == 10 && mano[2][0].numero == 11 && mano[3][0].numero == 12 && mano[4][0].numero == 13){
        return 1;
    }
    return 0;
}

int puntos_escala(Carta ** mano){
    int puntos = 0;
    if (mano[0][0].numero == 1){
        puntos += 14;
    } else {
        puntos += mano[4][0].numero;
    }
    return puntos;
}

int trio(Carta ** mano){
    int cartas_iguales = 0;
    for (int i = 0; i < 5; i++){
        int numero_a_comparar = mano[i][0].numero;
        for (int j = 0; j < 5; j++){
            if (mano[j][0].numero == numero_a_comparar){
                cartas_iguales += 1;
            }
        }
        if (cartas_iguales == 3){
            return 1;
        }
        cartas_iguales = 0;
    }

    return 0;
}

int puntos_trio(Carta ** mano){
    int cartas_iguales = 0;
    for (int i = 0; i < 5; i++){
        int numero_a_comparar = mano[i][0].numero;
        for (int j = 0; j < 5; j++){
            if (mano[j][0].numero == numero_a_comparar){
                cartas_iguales += 1;
            }
        }
        if (cartas_iguales == 3){
            return numero_a_comparar;
        }
        cartas_iguales = 0;
    }
    return 0;
}

int doble_par(Carta ** mano){
    int cartas_iguales = 0;
    int pares = 0;
    for (int i = 0; i < 5; i++){
        int numero_a_comparar = mano[i][0].numero;
        for (int j = 0; j < 5; j++){
            if (mano[j][0].numero == numero_a_comparar && mano[j][0].contada == 0){
                cartas_iguales += 1;
            }
        }
        if (cartas_iguales == 2){
            pares += 1;
            int numero_par = numero_a_comparar;
            for (int k = 0; k < 5; k++){
                if (numero_par == mano[k][0].numero){
                    mano[k][0].contada = 1;
                }
            }
        }
        cartas_iguales = 0;
    }
    for (int i = 0; i < 5; i++){
        mano[i][0].contada = 0;
    }
    if (pares == 2){
        return 1;
    } 
    return 0;
}

int puntos_doble_par(Carta ** mano){
    int puntos = 0;
    int cartas_iguales = 0;
    int contador = 0;
    int numero_par_1;
    int numero_par_2;
    for (int i = 0; i < 5; i++){
        int numero_a_comparar = mano[i][0].numero;
        for (int j = 0; j < 5; j++){
            if (mano[j][0].numero == numero_a_comparar && mano[j][0].contada == 0){
                cartas_iguales += 1;
            }
        }
        if (cartas_iguales == 2){
            int numero_par = numero_a_comparar;
            if (contador == 0){
                numero_par_1 =  numero_par;
                contador = 1;
            } else {
                numero_par_2 = numero_par;
            }
            for (int k = 0; k < 5; k++){
                if (numero_par == mano[k][0].numero){
                    mano[k][0].contada = 1;
                }
            }
        }
        cartas_iguales = 0;
    }
    for (int i = 0; i < 5; i++){
        mano[i][0].contada = 0;
    }
    if (numero_par_1 == 1){
        puntos += 14*2;
    }
    else {
        puntos += numero_par_1*2;
    }
    if (numero_par_2 == 1){
        puntos += 14*2;
    }
    else {
        puntos += numero_par_2*2;
    }
    return puntos;
}

int par(Carta ** mano){
    int cartas_iguales = 0;
    for (int i = 0; i < 5; i++){
        int numero_a_comparar = mano[i][0].numero;
        for (int j = 0; j < 5; j++){
            if (mano[j][0].numero == numero_a_comparar){
                cartas_iguales += 1;
            }
        }
        if (cartas_iguales == 2){
            return 1;
        }
        cartas_iguales = 0;
    }
    return 0;
}

int puntos_par(Carta ** mano){
    int cartas_iguales = 0;
    for (int i = 0; i < 5; i++){
        int numero_a_comparar = mano[i][0].numero;
        for (int j = 0; j < 5; j++){
            if (mano[j][0].numero == numero_a_comparar){
                cartas_iguales += 1;
            }
        }
        if (cartas_iguales == 2){
            return numero_a_comparar;
        }
        cartas_iguales = 0;
    }
    return 0;
}

int puntaje(Carta ** mano){
    int puntos;
    Carta ** mano_ordenada;
    mano_ordenada = ordenar_mano(mano);
    if (escala_real_color(mano_ordenada)){
        printf("Tiene escala real color\n");
        puntos = 1000;
    } else if (escala_color(mano_ordenada)){
        printf("Tiene escala color\n");
        puntos = 900;
        puntos += puntos_escala_color(mano_ordenada);
    } else if (poker(mano_ordenada)){
        printf("Tiene poker\n");
        puntos = 800;
        puntos += puntos_poker(mano_ordenada);
    } else if (full(mano_ordenada)){
        printf("Tiene full\n");
        puntos = 700;
        puntos += puntos_full(mano_ordenada);
    } else if (color(mano_ordenada)){
        printf("Tiene color\n");
        puntos = 600;
        puntos += puntos_color(mano_ordenada);
    } else if (escala(mano_ordenada)){
        printf("Tiene escala\n");
        puntos = 500;
        puntos += puntos_escala(mano_ordenada);
    } else if (trio(mano_ordenada)){
        printf("Tiene trio\n");
        puntos = 400;
        puntos += puntos_trio(mano_ordenada);
    } else if (doble_par(mano_ordenada)){
        printf("Tiene doble par\n");
        puntos = 300;
        puntos += puntos_doble_par(mano_ordenada);
    } else if (par(mano_ordenada)){
        printf("Tiene par\n");
        puntos = 200;
        puntos += puntos_par(mano_ordenada);
    } else {
        printf("Tiene carta alta\n");
        if (mano_ordenada[0][0].numero == 1){
            puntos = 14;
        } else {
            puntos = mano_ordenada[4][0].numero;
        }
    }
    return puntos;
}

int quien_gano(Carta ** mano_j1, Carta ** mano_j2){
    printf("FUNCION QUIEN GANO\n");
    int puntos_j1 = puntaje(mano_j1);
    printf("Puntos j1: %d\n", puntos_j1);
    int puntos_j2 = puntaje(mano_j2);
    printf("Puntos j2: %d\n", puntos_j2);
    if (puntos_j1 > puntos_j2){
        return 1;
    } else if (puntos_j2 > puntos_j1){
        return 2;
    } else {
        return 0;
    } 
}


Carta ** inicializar_mano(){
    Carta **cartas = (Carta **) malloc(5 * sizeof(Carta *));

    for(int i = 0; i < 5; i++){
        cartas[i] = (Carta *) malloc(sizeof(Carta));
    }
    return cartas;
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