#include <stdio.h>
#include <stdlib.h>
#include "cz_API.h"
#include <unistd.h>
#include <string.h>


int main(int argc, char *argv[])
{
	if (argc != 2){
		printf("Uso: %s <archivo> \nDonde\n", argv[0]);
		printf("archivo: es el archivo binario a leer\n");
		return 1;
	}

	cz_mount(argv[1]);

	// printf("%d\n", cz_exists("aaaaaa.txt"));

	// printf("%d\n", (int)1025%1024);

		// int tamano = 252 * 1024 +1;
		// int posicion_bloque_datos = (int)(tamano/1024);  		//donde quiero escribir
		// int resto = tamano - posicion_bloque_datos * 1024;		
		// int bytes_a_usar_en_bloque_actual = 1024 - resto;

		// //TODO ver bloque indirecto
		// printf("posicion_bloque_datos: %d\n", posicion_bloque_datos);		
		// printf("resto: %d\n", resto);
		// printf("bytes_a_usar_en_bloque_actual: %d\n", bytes_a_usar_en_bloque_actual);
	cz_ls();
	printf("existe: %d\n", cz_exists("texto.txs"));
	// czFILE* file = cz_open("aabaaa.txt", 'w');
	// if (file != NULL){
	// 	char texto[13] = "lacasadepapel";
	// 	int total = cz_write(file, texto, 13);
	// 	printf("total : %d\n", total);
	// }
	// cz_close(file);

	// printf("division %d\n", 7 % 2);
	// printf("%d\n", cz_exists("aaaaaa.txt"));

	// unsigned char buffer[10];
	// FILE *ptr;

	// ptr = fopen("simdiskformat.bin","rb");  // r for read, b for binary

	// fread(buffer,sizeof(buffer),1,ptr); // read 10 bytes to our buffer
	// for(int i = 0; i<10; i++){
	//     printf("%u ", buffer[i]); // prints a series of bytes
	// }
	// fclose(ptr);
	// fseek(fp, 10, SEEK_SET);  //que desde el inicio me mueva 10 bytes

	// fread(buffer, 20, pow(2,8), fp);  //que se lea archivo fp en posicion dada por seek, se guarde en buffer
	// 								   //se lean 20 bytes pow(2,8) veces---->bloques
	// printf("DIRECCIÓN FÍSICA: %u\n", buffer[0]);

	liberar_resto();
	return 0;
}
