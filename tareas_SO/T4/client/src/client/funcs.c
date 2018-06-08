#include <stdlib.h>
#include <stdio.h>
#include "funcs.h"
#include <math.h>
#include <unistd.h>

#include <string.h>

#include <stdbool.h>
#include <sys/types.h>  
#include <errno.h>  
#include <sys/wait.h>  
#include "sys/time.h"

int binary_to_decimal(char* binary){
	char* start;
	int total = 0;
    start = &binary[0];
    while (*start)
    {
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

void int_to_bits(char* resultado, int numero){
	int i = 7;
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
	resultado[8] = '\0';
}