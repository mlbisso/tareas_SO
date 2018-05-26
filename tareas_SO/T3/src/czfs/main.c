#include <stdio.h>
#include <stdlib.h>
#include "cz_API.h"
#include <unistd.h>
#include <string.h>

#include <time.h>


int main(int argc, char *argv[])
{
	if (argc != 2){
		printf("Uso: %s <archivo> \nDonde\n", argv[0]);
		printf("archivo: es el archivo binario a leer\n");
		return 1;
	}

	cz_mount(argv[1]);

	// char * texto = "habia una vez, ........";


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

	// char orig[11 + 1] = "texto.txt";
	// char dest[11 + 1] = "holaa.txt";
	// // orig[11] = '\0';
	// // dest[11] = '\0';
	// // cz_cp(orig, dest);
	// cz_ls();
	cz_ls();
	// printf("%d\n", cz_exists("real.txt"));
	// printf("%d\n", cz_exists("teeee.txt"));
	// cz_ls();
	czFILE* file = cz_open("real.txt", 'r');

	int tamano = obtener_tamano(file -> indice -> tamano);
	unsigned char textos[tamano + 10 + 1];
	int leidos = cz_read(file, textos, tamano + 10);
	printf("leidos %d\n", leidos);
		printf("tamano %d\n", tamano);

	textos[tamano + 10] = '\0';
	printf("texto: %s\n", textos);
	cz_close(file);

	// file = cz_open("texto.txt", 'r');

	// int tamanos = obtener_tamano(file -> indice -> tamano);
	// unsigned char texto[tamanos + 1];
	// cz_read(file, texto, tamanos);
	// texto[tamanos] = '\0';
	// printf("texto: %s\n", texto);
	// cz_cp("real.txt", "texto.txt");
	// printf("\n");
	// cz_ls();



	// // cz_ls();
	// // // printf("existe: %d\n", cz_exists("texto.txs"));
	// // char or[11 + 1] = "holaa.txt";
	// // // // // char dest[11 + 1] = "newcam2.txt";
	// imprimir_bitmaps2();
	// czFILE* file = cz_open(dest, 'w');

	// unsigned char buf[2];
	// buf[0] = 'e';
	// buf[1] = '\0';
	// cz_write(file, buf, 1);

	// unsigned char bu[3];
	// // bu = "co";
	// bu[0] = 'c';
	// bu[1] = 'o';
	// bu[2] = '\0';
	// // bu[3] = '\0';

	// // sleep(5);

	// cz_write(file, bu, 2);

	// // printf("creacion %d\n", obtener_tamano(file -> indice -> creacion));
	// // printf("modificacion %d\n", obtener_tamano(file -> indice -> modificacion));
	// // // cz_ls();
	// cz_close(file);

	// imprimir_bitmaps2();
	// cz_ls();
	// cz_cp("holaa.txt", "chao.txt");
	// imprimir_bitmaps2();
	// cz_ls();
	// imprimir_bitmaps2();

	// cz_ls();
	// cz_rm("holaa.txt");
	// 	imprimir_bitmaps2();

	// cz_ls();
	// // liberar_resto();

	// czFILE* file_1 = cz_open(or, 'r');		//TODO problemas aqui
	// cz_ls();

	// // printf("human\n");

	// int tamano = obtener_tamano(file_1 -> indice -> tamano);



	// unsigned char buff[tamano + 1];
	// cz_read(file_1, buff, tamano);
	// buff[tamano] = '\0';
	// printf("buff %s\n", buff);
	// // cz_ls();

	// buff[1] = '\0';
	// printf("buff %s\n", buff);
	// cz_close(file_1);
	// cz_ls();

	liberar_resto();					//TODO tambien esta malo
	// printf("no me digas\n");
	// cz_ls();

	// printf("%s\n", buf);
	// // ]buf[1] = '\0';
	// cz_read(file, buf, 1);
	// buf[1] = '\0';

	// printf("buf:%s\n", buf);
	// unsigned char bufs[2];
	// buf[1] = '\0';
	// // printf("%s\n", buf);
	// // ]buf[1] = '\0';
	// cz_read(file, bufs, 1);
	// printf("buf:%s\n", bufs);
	// cz_close(file);
	// czFILE* file_1 = cz_open(or, 'w');
	// // ]unsigned char buf[13] = "123456789000";
	// // ]uf[12] = '\0';
	// unsigned char buff[13];
	// cz_read(file_1, buff, 12);
	// printf("buff %s\n", buff);
	// cz_close(file);
	// int tamano = obtener_tamano(file -> indice -> tamano);
	// printf("tamano: %d\n", tamano);
	// if (file != NULL){
	// 	char textos[13] = "lacasadepapel";
	// 	int total = cz_write(file, textos, 13);
	// 	printf("total : %d\n", total);
	// }
	// cz_close(file);
	// // czFILE* file_1 = cz_open(or, 'r');
	// unsigned char textos[1 + 1];
	// int leidos = cz_read(file, textos, 1);
	// textos[1] = '\0';

	// unsigned char texto[1 + 1];
	// cz_read(file, texto, 1);
	// texto[1] = '\0';

	// // printf("leidos: %d\n", leidos);
	// printf("texto: %s\n", texto);
	// if (leidos == tamano){
	// 	printf("OK\n");
	// }
	// cz_close(file);
	// cz_mv(or, dest);
					// imprimir_bitmaps2();
	// cz_rm(or);
					// imprimir_bitmaps2();

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

	return 0;
}
