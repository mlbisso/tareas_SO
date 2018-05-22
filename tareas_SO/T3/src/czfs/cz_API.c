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
char* filename_disco;
BIndice* bindice;
BIndirecto* bindirecto;

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

czFILE* czfile_init(char* filename){
	czFILE* archivo = malloc(sizeof(czFILE));
	strcpy(archivo -> filename, filename);
	archivo -> filename[11] = '\0';
	archivo -> indice = bindice;
	return archivo;
}

void cz_mount(char* diskfileName){
	bdirectorio = bdirectorio_init();
	bitmaps = bitmaps_init();	
	bindice = bindice_init();
	bindirecto = bindirecto_init();
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
	BIndice* bindices = calloc(1, sizeof(BIndice));
	// for (int i = 0; i < 252; i ++){
	// 	bindices -> datos[i] = NULL;
	// }
	return bindices;
}

BIndirecto* bindirecto_init(){
	BIndirecto* bindirecto = malloc(sizeof(BIndirecto));
	// for (int i = 0; i < 256; i ++){
	// 	  bindirecto -> datos[i] = NULL;
	// }
	return bindirecto;
}

BDatos* bdatos_init(int num_bloque){
	BDatos* bdatos = malloc(sizeof(BDatos));
	for (int i = 0; i < 1024; i ++){
		bdatos -> datos[i] = 0x00;
	}	
	bdatos -> datos[1024] = '\0';
	bdatos -> num_bloque = num_bloque;
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
	if (mode == 'r'){					//mode 'r'
		if (cz_exists(filename) == 0){	//si no existe el nombre
			return NULL;
		}
		else{
			czFILE* archivo = setear_estructuras(filename);
			return archivo;
		}
	}
	if (mode == 'w'){				//modo 'w'
		if (cz_exists(filename) == 1){	//si ya existe el nombre
			return NULL;
		}
		else{
			int indice = buscar_espacio_en_bitmap();
			if (indice != -1 && !directorio_lleno()){				//hay un espacio para guardar el indice de archivo
				setear_bindice(indice);		//hago que el bloque indice esté vacío
				czFILE * archivo = czfile_init(filename);
				agregar_direccion(archivo);
				return archivo;
			}
			else{
				return NULL;
			}
			//TODO  agregarle a archivo el nombre filename
		}
	}
	return NULL;
}

czFILE* setear_estructuras(char * filename){
	Directorio* directorio_actual = bdirectorio -> head;
	// unsigned char indice[4 + 1];
	while (directorio_actual != NULL){
		if (*directorio_actual -> valido == 1 && strcmp(directorio_actual->nombre, filename) == 0){
			break;
		}
		directorio_actual = directorio_actual -> next_directorio;
	}	
	//TODO usar directorio_actual -> indice
	czFILE* archivo = czfile_init(filename);
	return archivo;
}

void agregar_direccion(czFILE* archivo){
	Directorio* directorio_actual = bdirectorio -> head;
	int posicion_directorio = 0;
	while(directorio_actual != NULL){
		if (*directorio_actual -> valido == 0){			//quiere decir que hay espacio
			*directorio_actual -> valido = 1;
			strcpy(directorio_actual -> nombre, archivo -> filename);
			*directorio_actual -> indice = archivo -> indice -> num_bloque;
			break;
		}
		directorio_actual = directorio_actual -> next_directorio;
		posicion_directorio += 1;
	}
	actualizar_directorio(posicion_directorio, directorio_actual);
}

int directorio_lleno(){
	Directorio* directorio_actual = bdirectorio -> head;
	while(directorio_actual != NULL){
		if (*directorio_actual -> valido == 0){			//quiere decir que hay espacio
			return 0;									//no está lleno
		}
		directorio_actual = directorio_actual -> next_directorio;
	}
	return 1;											//está lleno
}

void setear_bindirecto(int indice){
	bindirecto -> num_bloque = indice;
	for (int i = 0; i < 256; i++){
		bindirecto -> datos[i] = NULL;
	}
}

void setear_bindice(int indice){
	int i;
	// unsigned char* byte_1[4] = {0x00, 0x00, 0x00, 0x00};
	// strcpy(bindice -> tamano, byte_1);
	for (i = 0; i < 4; i ++){
		// bindice -> tamano[i] = 0x00;
		bindice -> tamano[i] = 0x00;  				//TODO cambiar a 0x00
		bindice -> creacion[i] = 0x00;             //TODO cambiar a fecha actual
		bindice -> modificacion[i] = 0x00;			//TODO cambiar a fecha actual
	}
	// bindice -> tamano[3] = 0x01;
	bindice -> tamano[4] = '\0';
	bindice -> creacion[4] = '\0';
	bindice -> modificacion[4] = '\0';

	//TODO revisar
	for (i = 0; i < 252; i++){
		bindice -> datos[i] = NULL;
	}
	bindice -> indirecto = NULL;
	bindice -> num_bloque = indice;
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

int cz_write(czFILE* file_desc, void* buffer, int nbytes){
	//TODO cambiar timestamps
	int bytes_escritos = 0;
	int tamano = obtener_tamano(file_desc -> indice -> tamano);
	while (nbytes != 0 || tamano == 520192){					// 508 * 1024
		tamano = obtener_tamano(file_desc -> indice -> tamano);
		int posicion_bloque_datos = (int)(tamano/1024);  		//donde quiero escribir
		int resto = tamano - posicion_bloque_datos * 1024;		
		int bytes_a_usar_en_bloque_actual = 1024 - resto;
		// int t = 252 * 1024;
		// printf("posicion_bloque_datos: %d\n", (int)(t/1024));		
		// printf("resto: %d\n", t - posicion_bloque_datos * 1024);
		// printf("posicion_bloque_datos: %d\n", (int)(tamano/1024));

		//Obtener int de byte
		// int Int = file_desc -> indice -> tamano[0] | ( (int)file_desc -> indice -> tamano[1] << 8 ) | ( (int)file_desc -> indice -> tamano[2] << 16 ) | ( (int)file_desc -> indice -> tamano[3] << 24 );
		// printf("INT %d \n", Int);
		if (posicion_bloque_datos >= 252){			//hay que ir a bloque indirecto
			if (file_desc -> indice -> indirecto == NULL){  //hay que asignarle un indice indir
				int indice = buscar_espacio_en_bitmap();	//hay que buscar espacio para indir
				if (indice != -1){				//hay un espacio para guardar el indice de archivo
					setear_bindirecto(indice);			
				}
				file_desc -> indice -> indirecto = bindirecto;
			}
			if (file_desc -> indice -> indirecto -> datos[252 - posicion_bloque_datos] == NULL){
				int num_bloque = buscar_espacio_en_bitmap();
				if (num_bloque != -1){			//si hay espacio para un bloque
					BDatos* bloque_datos = bdatos_init(num_bloque);
					file_desc -> indice -> datos[252 - posicion_bloque_datos] = bloque_datos;
				}	
				else{
					return -1;
				}					
			}
		}

		if (file_desc -> indice -> datos[posicion_bloque_datos] == NULL){	//hay que crear un bloque de datos
			int num_bloque = buscar_espacio_en_bitmap();
			if (num_bloque != -1){			//si hay espacio para un bloque
				BDatos* bloque_datos = bdatos_init(num_bloque);
				file_desc -> indice -> datos[posicion_bloque_datos] = bloque_datos;
			}	
			else{
				return -1;
			}	
		}
		unsigned char *ptr = buffer;
		BDatos* bdatos;
		if (posicion_bloque_datos < 252){
			bdatos = file_desc -> indice -> datos[posicion_bloque_datos];
		}
		else{
			bdatos = file_desc -> indice -> indirecto -> datos[252 - posicion_bloque_datos];
		}
		int bytes_a_escribir;
		if (nbytes <= bytes_a_usar_en_bloque_actual){
			bytes_a_escribir = nbytes;
		}
		else{
			bytes_a_escribir = bytes_a_usar_en_bloque_actual;
		}
		int j = 0;
		for (int i = 1024 - bytes_a_usar_en_bloque_actual; i < 1024 - bytes_a_usar_en_bloque_actual + bytes_a_escribir; i++){
			bdatos -> datos[i] = ptr[j];
			j += 1;
		}
		actualizar_tamano(file_desc, j);
		nbytes -= j;
		bytes_escritos += j;
		tamano = obtener_tamano(file_desc -> indice -> tamano);
	}
	// for (int i = 0; i < 252; i ++){
	// 	if (file_desc -> indice -> datos[i] == NULL){

	// 	}
	// }
	return bytes_escritos;
}

void actualizar_tamano(czFILE* file_desc, int j){
	// unsigned char bytes[4];

	int tamano = obtener_tamano(file_desc -> indice -> tamano);
	tamano += j;
	file_desc -> indice -> tamano[0] = (tamano >> 24) & 0xFF;
	file_desc -> indice -> tamano[1] = (tamano >> 16) & 0xFF;
	file_desc -> indice -> tamano[2] = (tamano >> 8) & 0xFF;
	file_desc -> indice -> tamano[3] = tamano & 0xFF;
	// int* intArray = malloc(sizeof(int[4]));

	// for (int i=0; i<4; i++){
	//    intArray[i] = file_desc -> indice -> tamano[i];
	// }
	// printf("tamano %d\n", *intArray);
}

int escribir_un_bloque(czFILE* file_desc, void* buffer, int nbytes, int bytes_a_usar_en_bloque_actual, int posicion_bloque){
	unsigned char *ptr = buffer;
	BDatos* bdatos = file_desc -> indice -> datos[posicion_bloque];
	int bytes_a_escribir;
	if (nbytes <= bytes_a_usar_en_bloque_actual){
		bytes_a_escribir = nbytes;
	}
	else{
		bytes_a_escribir = bytes_a_usar_en_bloque_actual;
	}
	int j = 0;
	for (int i = 1024 - bytes_a_usar_en_bloque_actual; i < 1024 - bytes_a_usar_en_bloque_actual + bytes_a_escribir; i++){
		bdatos -> datos[i] = ptr[j];
		j += 1;
	}
	return nbytes - j; 				//cuantos bytes quedan por escribir
}

int obtener_tamano(unsigned char* tamano){
	int tamano_real_bytes = byte_a_decimal(tamano, 4);
	return tamano_real_bytes;
}

int byte_a_decimal(unsigned char* tamano, int nbytes){
	unsigned char *ptr = tamano;
	unsigned char bits[nbytes * 8 + 1];
	int numero;
	int posicion_actual = nbytes * 8 - 1;
	int asignaciones = 0;
	for (int i = nbytes - 1; i >= 0; i--){
		numero = ptr[i];
		while (numero != 0){
			if (numero % 2 == 0){    //es par
				bits[posicion_actual] = '0';
			}
			if (numero % 2 == 1){
				bits[posicion_actual] = '1';
			}
			asignaciones += 1;
			numero = (int)(numero / 2);
			posicion_actual -= 1;
		}
		while (asignaciones != 8){
			bits[posicion_actual] = '0';
			posicion_actual -= 1;
			asignaciones += 1;
		}
		asignaciones = 0;
	}
	bits[nbytes * 8] = '\0';
	int resultado = 0;
	for (int i = 0; i < 32; i++){
		if (bits[i] == 48){   //es cero
			if (resultado == 0){
				continue;
			}
			resultado = resultado * 2;
		}
		if (bits[i] == 49){    // es uno
			if (resultado == 0){
				resultado = 1;
				continue;
			}
			resultado = resultado * 2 + 1;
		}
	}
	return resultado;
}

int cz_close(czFILE* file_desc){
	if (cz_exists(file_desc -> filename) == 0){			//el archivo no existe en el sistema
		return 1;
	}
	unsigned char sin_bloque[4];
	for (int i = 0; i < 4; i++){
		sin_bloque[i] = 0x00;
	}
	FILE *fp;
	fp = fopen(filename_disco, "wb");

	int num_bloque = file_desc -> indice -> num_bloque;
	fseek(fp, num_bloque * 1024, SEEK_SET); 
	fwrite(file_desc -> indice -> tamano, 4, 1, fp);

	fseek(fp, num_bloque * 1024 + 4, SEEK_SET);
	fwrite(file_desc -> indice -> creacion, 11, 1, fp);

	fseek(fp, num_bloque * 1024 + 8, SEEK_SET);
	fwrite(file_desc -> indice -> modificacion, 4, 1, fp);

	for (int i = 0; i < 252; i ++){
		fseek(fp, num_bloque * 1024 + 12 + 4 * i, SEEK_SET);
		if (file_desc -> indice -> datos[i] == NULL){
			fwrite(sin_bloque, 4, 1, fp);
		}
		else{
			unsigned char input[4];
			int num = file_desc -> indice -> datos[i] -> num_bloque;
			input[0] = (num >> 24) & 0xFF;
			input[1] = (num >> 16) & 0xFF;
			input[2] = (num >> 8) & 0xFF;
			input[3] = num & 0xFF;
			fwrite(input, 4, 1, fp);
			cerrar_bloque_datos(file_desc -> indice -> datos[i], fp);
		}
	}
	fseek(fp, num_bloque * 1024 + 12 + 4 * 252, SEEK_SET);
	if (file_desc -> indice -> indirecto == NULL){
		fwrite(sin_bloque, 4, 1, fp);
	}
	else{
		unsigned char input[4];
		int num = file_desc -> indice -> indirecto -> num_bloque;
		input[0] = (num >> 24) & 0xFF;
		input[1] = (num >> 16) & 0xFF;
		input[2] = (num >> 8) & 0xFF;
		input[3] = num & 0xFF;
		fwrite(input, 4, 1, fp);
		cerrar_bloque_indirecto(file_desc -> indice -> indirecto, fp);
	}

	fclose(fp);
	//TODO actualizar el coso
	// file_desc -> indice
	free(file_desc -> indice);
	free(file_desc);
	return 0;
}

void cerrar_bloque_datos(BDatos* datos, FILE* fp){
	int num_bloque = datos->num_bloque;
	fseek(fp, num_bloque * 1024, SEEK_SET); 
	fwrite(datos -> datos, 1024, 1, fp);
	free(datos);
}

void cerrar_bloque_indirecto(BIndirecto* indirecto, FILE* fp){
	int num_bloque = indirecto->num_bloque;
	unsigned char sin_bloque[4];
	for (int i = 0; i < 4; i++){
		sin_bloque[i] = 0x00;
	}
	for (int i = 0; i < 256; i ++){
		fseek(fp, num_bloque * 1024 + 4 * i, SEEK_SET);
		if (indirecto -> datos[i] == NULL){
			fwrite(sin_bloque, 4, 1, fp);
		}
		else{
			unsigned char input[4];
			int num = indirecto -> datos[i] -> num_bloque;
			input[0] = (num >> 24) & 0xFF;
			input[1] = (num >> 16) & 0xFF;
			input[2] = (num >> 8) & 0xFF;
			input[3] = num & 0xFF;
			fwrite(input, 4, 1, fp);
			cerrar_bloque_datos(indirecto -> datos[i], fp);
		}	
	}
	free(indirecto);
}
int cz_mv(char* orig, char *dest){
	if (strcmp(orig, dest) == 0){   //los nombres son iguales 
		return 1;
	}
	if (cz_exists(dest) == 1){		//si dest ya existe
		return 1;
	}
	if (cz_exists(orig) == 1){
		Directorio* directorio_actual = bdirectorio -> head;
		int posicion_directorio = 0;
		//printf("LS ANTES:\n");
		//cz_ls();
		//printf("REVISANDO NOMBRES DE BLOQUES DIRECTORIO\n");
		while (directorio_actual != NULL){
			unsigned char no_nulo;
			no_nulo = '\x01';
			if (*directorio_actual -> valido == no_nulo){
				if (strcmp(directorio_actual -> nombre, orig) == 0){
					//printf("Nombre origen : %s encontrado en bloque directorio! \n", orig);
					strcpy(directorio_actual -> nombre, dest);
					//printf("LS DESPUES:\n");
					//cz_ls();
					actualizar_directorio(posicion_directorio, directorio_actual);
					return 0;
				}
			}
			directorio_actual = directorio_actual -> next_directorio;
			posicion_directorio += 1;
		}
	}
	else {
		return 1;
	}
}

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
		unsigned char nulo;
		nulo = '\x01';
		if (*directorio_actual -> valido == nulo){
			printf("%s \n", directorio_actual -> nombre);
		}
		directorio_actual = directorio_actual -> next_directorio;
	}
}

int buscar_espacio_en_bitmap(){
	Bitmap* actual_bitmap = bitmaps -> head;
	while (actual_bitmap != NULL){
		for (int i = 0; i < 1024; i++){			//recorrer cada byte del bitmap
			for (int j = 0; j < 8; j ++){
				if (!((actual_bitmap -> bits[i] & ( 1 << j )) >> j)){        //si encuentro un bit de cero
					int resultado = i * 8 + (7 - j);
					actual_bitmap -> bits[i] |= 1UL << j;				//cambiar un bit a 1 en actual_bitmap
					actualizar_bitmap(actual_bitmap);
					// printf("i: %d\n", i);
					// printf("j: %d\n", j);
					// printf("resultado: %d\n", resultado);
					return resultado;
				}
			}
		}
		actual_bitmap = actual_bitmap -> next_bitmap;
	}
	return -1;
}

void actualizar_directorio(int posicion_directorio, Directorio* directorio){
	FILE *fp;
	fp = fopen(filename_disco, "wb");

	fseek(fp, posicion_directorio * 16, SEEK_SET); 
	fwrite(directorio -> valido, 1, 1, fp);

	fseek(fp, 1 + posicion_directorio * 16, SEEK_SET);
	fwrite(directorio -> nombre, 11, 1, fp);

	fseek(fp, 12 + posicion_directorio * 16, SEEK_SET);
	fwrite(directorio-> indice, 4, 1, fp);

	fclose(fp);
}

void actualizar_bitmap(Bitmap* bitmap){
	FILE *fp;
	fp = fopen(filename_disco, "wb");
	fseek(fp, bitmap -> num_bloque * 1024, SEEK_SET); 
	fwrite(bitmap -> bits, 1024, 1, fp);
	fclose(fp);
}

void liberar_resto(){
	Bitmap* actual_bitmap = bitmaps -> head;
	while (actual_bitmap != NULL){
		Bitmap* bitmap_a_borrar = actual_bitmap;
		actual_bitmap = actual_bitmap -> next_bitmap;
		free(bitmap_a_borrar);
	}
	free(bitmaps);

	Directorio* directorio_actual = bdirectorio -> head;
	while (directorio_actual != NULL){
		Directorio* directorio_a_borrar = directorio_actual;
		directorio_actual = directorio_actual -> next_directorio;
		free(directorio_a_borrar);
	}
	free(bdirectorio);	
	// free(bindice);						//TODO revisar
	free(bindirecto);
}

void actualizar_indice(BIndice* bindice){

}

void actualizar_indirecto(BIndirecto* bindirecto){

}

void actualizar_datos(BDatos* bdatos){

}