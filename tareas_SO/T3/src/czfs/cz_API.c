// Importamos la librería estándar de C
#include <stdlib.h>
// Importamos el módulo estándar de I/O
#include <stdio.h>
// Importamos las definiciones de la lista
#include "cz_API.h"
#include <math.h>
#include <unistd.h>

#include <string.h>

#include <stdbool.h>


BDirectorio * bdirectorio;
Bitmaps * bitmaps;
czFILE* archivo;
char* filename_disco;
BIndice* in_bindice;

// Process* process_init(int PID)
// {
// 	Process* process = malloc(sizeof(Process));
// 	process->bursts = bursts_init();
// 	return process;
// }

// typedef struct czFILE {
//   char filename[11 + 1];
//   BIndice* indice;
// } czFILE;

// extern FILE *fp;

void cz_mount(char* diskfileName){
	bdirectorio = bdirectorio_init();
	bitmaps = bitmaps_init();	
	in_bindice = bindice_init();
	FILE *fp;
	filename_disco = diskfileName;
   	// char buffer[1024];
 //   	char valido[64];    //64 entradas por 1B   //TODO unsigned int?????
	// char nombre[704];  //64 entradas por 11B
	fp = fopen(filename_disco, "rb");
	Directorio* directorio_actual = bdirectorio->head;
	int i = 0;

	//BLOQUE DIRECTORIO
	while(directorio_actual != NULL){
		fseek(fp, i * 16, SEEK_SET); 
		fread(directorio_actual -> valido, 1, 1, fp);
		directorio_actual -> valido[1] = '\0';

		fseek(fp, 1 + i * 16, SEEK_SET);
		fread(directorio_actual-> nombre, 11, 1, fp);
		directorio_actual -> nombre[11] = '\0';
		// printf("%s\n", directorio_actual -> nombre);

		fseek(fp, 12 + i * 16, SEEK_SET);
		fread(directorio_actual-> indice, 4, 1, fp);
		directorio_actual -> indice[4] = '\0';
		// printf("indice\n");
		// printf("%s\n", directorio_actual -> indice);
		// printf("\n");
		directorio_actual = directorio_actual -> next_directorio;
		i += 1;
	}

	//BLOQUES BITMAPS
	i = 1;
	Bitmap* actual_bitmap = bitmaps -> head;
	for (i = 1; i < 9; i ++){
		fseek(fp, i * 1024, SEEK_SET);
		fread(actual_bitmap -> bits, 1024, 1, fp);
		// actual_bitmap -> bits[1024] = '\0';
		actual_bitmap = actual_bitmap -> next_bitmap;
	}
	fclose(fp);
}

Directorio* directorio_init(){
	Directorio* directorio = malloc(sizeof(Directorio));
	directorio -> next_directorio = NULL;
	return directorio;
}

BDirectorio* bdirectorio_init(){
	BDirectorio* Bdirectorio = malloc(sizeof(BDirectorio));
	Bdirectorio -> count = 0;
	Bdirectorio -> head = NULL;
	Bdirectorio -> tail = NULL;
	Bdirectorio -> num_bloque = 0;
	for (int i = 0; i < 64; i++){               //son 64 entradas
		Directorio* directorio = directorio_init();
		directorio_insert(Bdirectorio, directorio);
	}
	return Bdirectorio;
}

BIndice* bindice_init(){
	BIndice* bindice = malloc(sizeof(BIndice));
	return bindice;
}

BIndirecto* bindirecto_init(){
	BIndirecto* bindirecto = malloc(sizeof(BIndirecto));
	return bindirecto;
}

BDatos* bdatos_init(){
	BDatos* bdatos = malloc(sizeof(BDatos));
	return bdatos;
}

void directorio_insert(BDirectorio* Bdirectorio, Directorio* new){
	if(Bdirectorio -> count == 0){
		Bdirectorio -> tail = new;
		Bdirectorio -> head = new;
	}
	else{
		Bdirectorio -> tail -> next_directorio = new;            //el siguiente del ultimo actual es new
		Bdirectorio -> tail = new;                                //se agrega new al final
		Bdirectorio -> tail -> next_directorio = NULL;
	}
	Bdirectorio -> count++;	
}

Bitmap* bitmap_init(unsigned int num){
	Bitmap* bitmap = malloc(sizeof(Bitmap));
	bitmap -> num_bloque = num;
	return bitmap;
}

Bitmaps* bitmaps_init(){
	Bitmaps* bitymaps = malloc(sizeof(Bitmaps));
	bitymaps -> count = 0;
	bitymaps -> head = NULL;
	bitymaps -> tail = NULL;
	unsigned int i;
	for (i = 0; i < 8; i++){               //son 64 entradas
		Bitmap* bitmap = bitmap_init(i + 1);
		bitmap_insert(bitymaps, bitmap);
	}
	return bitymaps;
}
void bitmap_insert(Bitmaps* bitymaps, Bitmap* new){
	if(bitymaps -> count == 0){
		bitymaps -> tail = new;
		bitymaps -> head = new;
	}
	else{
		bitymaps -> tail -> next_bitmap = new;            //el siguiente del ultimo actual es new
		bitymaps -> tail = new;                                //se agrega new al final
		bitymaps -> tail -> next_bitmap = NULL;
	}
	bitymaps -> count++;	
}

	// 								   //se lean 20 bytes pow(2,8) veces---->bloques
	// int i;
	// for (i = 0; i < pow(2,6); i++){
	// 	printf("%u\n", buffer[i]);
	// }
	// fseek(fp, 10, SEEK_CUR);  //que desde el inicio me mueva 10 bytes
	// 	printf("FIN: %d\n", SEEK_CUR)

czFILE* cz_open(char* filename, char mode){
	// if (strcmp(mode, 'r') == 0){				//si modo es lectura
	if (mode == 'r'){
		if (cz_exists(filename) == 0){	//si no existe el nombre
			return NULL;
		}
		else{
			archivo = malloc(sizeof(czFILE));
			//TODO cargar archivo desde disco
			return archivo;
		}
	}
	if (mode == 'w'){				//si modo es escritura
		if (cz_exists(filename) == 1){	//si ya existe el nombre
			return NULL;
		}
		else{
			archivo = malloc(sizeof(czFILE));
			strcpy(archivo -> filename, filename);
			archivo -> filename[11] = '\0';
			int indice = buscar_indice();

			if (indice != -1){
				in_bindice -> tamano = 0x00;
				char tamano[4 + 1];
  char creacion[4 + 1];
  char modificacion[4 + 1];
  BDatos* datos[252];
  BIndirecto* indice;
			}
			else{
				return NULL;
			}
			//TODO  agregarle a archivo el nombre filename
			return archivo;
		}
	}
	return NULL;
}

int cz_exists(char* filename){
	Directorio* directorio_actual = bdirectorio -> head;
	while (directorio_actual != NULL){
		if (*directorio_actual -> valido == 1 && strcmp(directorio_actual->nombre, filename) == 0){
			return 1;
		}
		directorio_actual = directorio_actual -> next_directorio;
	}	
	return 0;
}

// int cz_read(czFILE* file_desc, void* buffer, int nbytes){

// }

// int cz_write(czFILE* file_desc, void* buffer, int nbytes){

// }

// int cz_close(czFILE* file_desc){

// }

// int cz_mv(char* orig, char *dest){

// }

int cz_cp(char* orig, char* dest){
	if (strcmp(orig, dest) == 0){   //los nombres son iguales 
		return 1;
	}
	if (cz_exists(dest) == 1){		//si dest ya existe
		return 1;
	}
	//TODO actualizar archivo
	return 0; 						//no hubo errores
}

// int cz_rm(char* filename){

// }

	// if (bdirectorio -> head != NULL){
	// 	printf("no es null\n");
	// }
	// else{
	// 	printf("es null\n");
	// }

void cz_ls(){
	Directorio* directorio_actual = bdirectorio -> head;
	while (directorio_actual != NULL){
		printf("actual %d\n", *directorio_actual -> valido);
		// printf("actual %s\n", directorio_actual -> valido);
		if (*directorio_actual -> valido == 1){
			printf("%s \n", directorio_actual -> nombre);
		}
		directorio_actual = directorio_actual -> next_directorio;
	}
}

int buscar_indice(){
	Bitmap* actual_bitmap = bitmaps -> head;
	while (actual_bitmap != NULL){
		for (int i = 0; i < 1024; i++){
			if (actual_bitmap -> bits[i] == 0){   //hay espacio aqui, entonces se asigna
				actual_bitmap -> bits[i] = 1;
				actualizar_bitmap(actual_bitmap);
				return ((actual_bitmap -> num_bloque - 1) * 1024 + i);
			}
		}
		actual_bitmap = actual_bitmap -> next_bitmap;
	}
	return -1;
}

void actualizar_directorio(BDirectorio* bdirectorio){

}

void actualizar_bitmap(Bitmap* bitmap){
	FILE *fp;
	fp = fopen(filename_disco, "wb");
	fseek(fp, bitmap -> num_bloque * 1024, SEEK_SET); 
	fwrite(bitmap -> bits, 1024, 1, fp);
}

void actualizar_indice(BIndice* bindice){

}

void actualizar_indirecto(BIndirecto* bindirecto){

}

void actualizar_datos(BDatos* bdatos){

}