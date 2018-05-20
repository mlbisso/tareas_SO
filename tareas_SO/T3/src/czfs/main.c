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

	// cz_open("hola.txt", 'w');
	// int n= cz_exists("polloyarroz");
	// printf("n: %d", n);
	// FILE *fp;
 //   	char buffer[256];

	// fp = fopen("simdiskformat.bin", "rb");
	cz_ls();
	printf("%d\n", cz_exists("nonono.txt"));
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


	return 0;
}
