#include <stdlib.h>
#include <stdio.h>
#include "cz_API.h"
#include <math.h>
#include <unistd.h>

#include <string.h>

#include <stdbool.h>

#include <time.h>



BDirectorio * bdirectorio;
Bitmaps * bitmaps;
char* filename_disco;
BIndice* bindice;
BIndirecto* bindirecto;
unsigned char disco_total[64 * 1048576 + 1];

int posicion_read = 0;		//en que posicion se quedó para el read

czFILE* czfile_init(char* filename, int mode){
	czFILE* archivo = malloc(sizeof(czFILE));
	strcpy(archivo -> filename, filename);
	archivo -> filename[11] = '\0';
	archivo -> indice = bindice;
	archivo -> mode = mode;
	return archivo;
}

void cz_mount(char* diskfileName){
	bdirectorio = bdirectorio_init();
	bitmaps = bitmaps_init();	
	filename_disco = diskfileName;
	FILE *fp;
	fp = fopen(filename_disco, "rb");
	Directorio* directorio_actual = bdirectorio->head;
	int i = 0;

	//BLOQUE DIRECTORIO
	fseek(fp, 0, SEEK_SET);
	fread(disco_total, 64 * 1048576, 1, fp);
	disco_total[64 * 1048576] = '\0';
	while(directorio_actual != NULL){
		fseek(fp, i * 16, SEEK_SET); 
		fread(directorio_actual -> valido, 1, 1, fp);
		directorio_actual -> valido[1] = '\0';

		fseek(fp, 1 + i * 16, SEEK_SET);
		fread(directorio_actual-> nombre, 11, 1, fp);
		directorio_actual -> nombre[11] = '\0';

		fseek(fp, 12 + i * 16, SEEK_SET);
		fread(directorio_actual-> indice, 4, 1, fp);
		directorio_actual -> indice[4] = '\0';
		directorio_actual = directorio_actual -> next_directorio;
		i += 1;
	}
	//BLOQUES BITMAPS
	i = 1;
	Bitmap* actual_bitmap = bitmaps -> head;
	for (i = 1; i < 9; i ++){
		fseek(fp, i * 1024, SEEK_SET);
		fread(actual_bitmap -> bits, 1024, 1, fp);
		actual_bitmap -> bits[1024] = '\0';
		actual_bitmap = actual_bitmap -> next_bitmap;
	}
	fclose(fp);
}

Directorio* directorio_init(){
	Directorio* directorio = calloc(1, sizeof(Directorio));
	directorio -> next_directorio = NULL;
	return directorio;
}

BDirectorio* bdirectorio_init(){
	BDirectorio* Bdirectorio = calloc(1, sizeof(BDirectorio));
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
	for (int i = 0; i < 252; i ++){
		bindices -> datos[i] = NULL;
	}
	bindices -> indirecto = NULL;
	return bindices;
}

BIndirecto* bindirecto_init(){
	BIndirecto* bindirecto = calloc(1, sizeof(BIndirecto));
	for (int i = 0; i < 256; i ++){
		  bindirecto -> datos[i] = NULL;
	}
	return bindirecto;
}

BDatos* bdatos_init(int num_bloque){
	BDatos* bdatos = calloc(1, sizeof(BDatos));
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
	Bitmap* bitmap = calloc(1, sizeof(Bitmap));
	bitmap -> num_bloque = num;
	return bitmap;
}

Bitmaps* bitmaps_init(){
	Bitmaps* bitymaps = calloc(1, sizeof(Bitmaps));
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

czFILE* cz_open(char* filename, char mode){
	bindice = bindice_init();
	bindirecto = bindirecto_init();
	if (mode == 'r'){					//mode 'r'
		if (cz_exists(filename) == 0){	//si no existe el nombre
			return NULL;
		}
		else{
			czFILE* archivo = setear_estructuras(filename, 0);		
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
				czFILE * archivo = czfile_init(filename, 1);
				agregar_direccion(archivo);
				int nuevo_tiempo = (unsigned)time(NULL);
				actualizar_tiempo(archivo -> indice -> creacion, nuevo_tiempo);		//modificar tiempo creacion
				actualizar_tiempo(archivo -> indice -> modificacion, nuevo_tiempo);
				return archivo;
			}
			else{
				return NULL;
			}
		}
	}
	return NULL;
}

czFILE* setear_estructuras(char * filename, int mode){
	Directorio* directorio_actual = bdirectorio -> head;
	while (directorio_actual != NULL){
		if (*directorio_actual -> valido == 1 && strcmp(directorio_actual->nombre, filename) == 0){
			break;
		}
		directorio_actual = directorio_actual -> next_directorio;
	}	

	//Seteo indice
	int indice = byte_a_decimal(directorio_actual -> indice, 4);
	setear_bindice(indice);
	FILE *fp;
	fp = fopen(filename_disco, "rb");

	// fseek(fp, indice * 1024, SEEK_SET); 	
	// fread(bindice -> tamano, 4, 1, fp);

	memcpy(bindice -> tamano, disco_total + indice * 1024, 4);
	bindice -> tamano[4] = '\0';

	// fseek(fp, indice * 1024 + 4, SEEK_SET); 	
	// fread(bindice -> creacion, 4, 1, fp);
	memcpy(bindice -> creacion, disco_total + indice * 1024 + 4, 4);
	bindice -> creacion[4] = '\0';

	// fseek(fp, indice * 1024 + 8, SEEK_SET); 	
	// fread(bindice -> modificacion, 4, 1, fp);
	memcpy(bindice -> modificacion, disco_total + indice * 1024 + 8, 4);
	bindice -> modificacion[4] = '\0';

	for (int i = 0; i < 252; i ++){
		unsigned char buffer[4 + 1];
		// fseek(fp, indice * 1024 + 12 + 4 * i, SEEK_SET);
		// fread(buffer, 4, 1, fp);
		memcpy(buffer, disco_total + indice * 1024 + 12 + 4 * i, 4);
		buffer[4] = '\0';
		if (buffer[0] == 0x00 && buffer[1] == 0x00 && buffer[2] == 0x00 && buffer[3] == 0x00){
			bindice -> datos[i] = NULL;
		}
		else{
			int num_bloque = byte_a_decimal(buffer, 4);
			BDatos* bloque_datos = bdatos_init(num_bloque);
			leer_datos(bloque_datos, fp);
			bindice -> datos[i] = bloque_datos;
		}
	}

	// fseek(fp, indice * 1024 + 12 + 4 * 252, SEEK_SET);
	unsigned char buffer[4 + 1];
	// fread(buffer, 4, 1, fp);
	memcpy(buffer, disco_total + indice * 1024 + 12 + 4 * 252, 4);
	buffer[4] = '\0';	

	if (buffer[0] == 0x00 && buffer[1] == 0x00 && buffer[2] == 0x00 && buffer[3] == 0x00){
		bindice -> indirecto = NULL;
	}
	else{
		int num_bloque = byte_a_decimal(buffer, 4);
		bindirecto -> num_bloque = num_bloque;
	}

	leer_bindirecto(bindirecto, fp);		

	bindice -> indirecto = bindirecto;	
	fclose(fp);

	czFILE* archivo = czfile_init(filename, mode);

	return archivo;
}

void leer_datos(BDatos* datos, FILE* fp){
	// fseek(fp, datos -> num_bloque * 1024, SEEK_SET);
	// fread(datos -> datos, 1024, 1, fp);
	memcpy(datos -> datos, disco_total + datos -> num_bloque * 1024, 1024);		//TODO malo
	datos -> datos[1024] = '\0';	
}

void leer_bindirecto(BIndirecto* bindirecto, FILE* fp){
	int indice = bindirecto -> num_bloque;
	for (int i = 0; i < 256; i ++){
		unsigned char buffer[4 + 1];
		// fseek(fp, indice * 1024 + 4 * i, SEEK_SET);
		// fread(buffer, 4, 1, fp);
		memcpy(buffer, disco_total + indice * 1024 + 4 * i, 4);	
		buffer[4] = '\0';
		if (buffer[0] == 0x00 && buffer[1] == 0x00 && buffer[2] == 0x00 && buffer[3] == 0x00){
			bindirecto -> datos[i] = NULL;
		}
		else{

			int num_bloque = byte_a_decimal(buffer, 4);

			if (num_bloque <= 0 || num_bloque >= 65536){
				bindirecto -> datos[i] = NULL;  	
				continue;
			}

			BDatos* bloque_datos = bdatos_init(num_bloque);

			leer_datos(bloque_datos, fp);		

			bindirecto -> datos[i] = bloque_datos;
		}
	}	
}


void agregar_direccion(czFILE* archivo){
	Directorio* directorio_actual = bdirectorio -> head;
	int posicion_directorio = 0;
	while(directorio_actual != NULL){
		if (*directorio_actual -> valido == 0){			//quiere decir que hay espacio
			*directorio_actual -> valido = 1;
			strcpy(directorio_actual -> nombre, archivo -> filename);

			directorio_actual -> indice[0] = (archivo -> indice -> num_bloque >> 24) & 0xFF;
			directorio_actual -> indice[1] = (archivo -> indice -> num_bloque >> 16) & 0xFF;
			directorio_actual -> indice[2] = (archivo -> indice -> num_bloque >> 8) & 0xFF;
			directorio_actual -> indice[3] = archivo -> indice -> num_bloque & 0xFF;
			directorio_actual -> indice[4] = '\0';
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

	for (i = 0; i < 4; i ++){
		bindice -> tamano[i] = 0x00;  			
		bindice -> creacion[i] = 0x00;            
		bindice -> modificacion[i] = 0x00;
	}
	bindice -> tamano[4] = '\0';
	bindice -> creacion[4] = '\0';
	bindice -> modificacion[4] = '\0';

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

int obtener_datos(BDatos* datos, void* buffer, int nbytes, int leidos, int voy_a_leer, int inicio_bloque){
	if (voy_a_leer > 1024){			//TODO sigue incompleto, falta posicion
		voy_a_leer = 1024;
	}
    char *mid = (char*)buffer;
    char *ptr = (char*)datos->datos;
	memcpy(mid + leidos, ptr + inicio_bloque, voy_a_leer);
	leidos += voy_a_leer;
	return voy_a_leer;
}

int cz_read(czFILE* file_desc, void* buffer, int nbytes){
	if (file_desc -> mode == 1){		//si es modo write y quieres leer
		return -1;
	}

	int leidos = 0;
	int voy_a_leer = nbytes;
	int voy_a_leer_oficial = nbytes;
	int maximo = (obtener_tamano(file_desc -> indice -> tamano)) - posicion_read; //TODO malo?
	if (voy_a_leer > maximo){
		voy_a_leer = maximo;
		voy_a_leer_oficial = maximo;
	}

	int lectura_un_bloque = 0;
	int inicio_bloque = 0;
	for (int i = 0; i < 252; i++){
		//TODO hacer un continue que setee posicion actual read
		if(posicion_read <= 1024 * (i + 1)){
			if (posicion_read <= 1024 * i){
				inicio_bloque = 0;
			}
			else{
				inicio_bloque = posicion_read - 1024 * i;
			}
			if (bindice -> datos[i] != NULL){
				lectura_un_bloque = obtener_datos(bindice -> datos[i], buffer, nbytes, leidos, voy_a_leer, inicio_bloque);
				leidos += lectura_un_bloque;
				voy_a_leer -= lectura_un_bloque;
			}
			if (leidos == voy_a_leer_oficial){
				break;
			}
		}
	}

	for (int i = 0; i < 256; i++){
		//TODO hacer un continue que setee posicion actual read
		if(posicion_read <= 1024 * (i + 1)){
			if (posicion_read <= 1024 * i){
				inicio_bloque = 0;
			}
			else{
				inicio_bloque = posicion_read - 1024 * i;
			}
			if (bindice -> indirecto -> datos[i] != NULL){
				lectura_un_bloque = obtener_datos(bindice -> indirecto -> datos[i], buffer, nbytes, leidos, voy_a_leer, inicio_bloque);
				leidos += lectura_un_bloque;
				voy_a_leer -= lectura_un_bloque;
			}
			if (leidos == voy_a_leer_oficial){
				break;
			}
		}
	}
	posicion_read += leidos;
	
	return leidos;
}

int cz_write(czFILE* file_desc, void* buffer, int nbytes){
	if (file_desc -> mode == 0){		//si es modo read y quieres escribir
		return -1;
	}
	int bytes_escritos = 0;
	int tamano = obtener_tamano(file_desc -> indice -> tamano);
	while (nbytes != 0 || tamano == 520192){					// 508 * 1024
		tamano = obtener_tamano(file_desc -> indice -> tamano);
		int posicion_bloque_datos = (int)(tamano/1024);  		//donde quiero escribir
		int resto = tamano - posicion_bloque_datos * 1024;		
		int bytes_a_usar_en_bloque_actual = 1024 - resto;

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
	int nuevo_tiempo = (unsigned)time(NULL);				//cambio tiempo modificacion
	actualizar_tiempo(file_desc -> indice -> modificacion, nuevo_tiempo);
	return bytes_escritos;
}

void actualizar_tamano(czFILE* file_desc, int j){
	int tamano = obtener_tamano(file_desc -> indice -> tamano);
	tamano += j;
	file_desc -> indice -> tamano[0] = (tamano >> 24) & 0xFF;
	file_desc -> indice -> tamano[1] = (tamano >> 16) & 0xFF;
	file_desc -> indice -> tamano[2] = (tamano >> 8) & 0xFF;
	file_desc -> indice -> tamano[3] = tamano & 0xFF;
}

void actualizar_tiempo(unsigned char* buff, int nuevo_tiempo){
	buff[0] = (nuevo_tiempo >> 24) & 0xFF;
	buff[1] = (nuevo_tiempo >> 16) & 0xFF;
	buff[2] = (nuevo_tiempo >> 8) & 0xFF;
	buff[3] = nuevo_tiempo & 0xFF;
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
	posicion_read = 0;
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
	// fseek(fp, num_bloque * 1024, SEEK_SET); 
	// fwrite(file_desc -> indice -> tamano, 4, 1, fp);

	memcpy(disco_total + num_bloque * 1024, file_desc -> indice -> tamano, 4);


	// fseek(fp, num_bloque * 1024 + 4, SEEK_SET);
	// fwrite(file_desc -> indice -> creacion, 11, 1, fp);

	memcpy(disco_total + num_bloque * 1024 + 4, file_desc -> indice -> creacion, 11);


	// fseek(fp, num_bloque * 1024 + 8, SEEK_SET);
	// fwrite(file_desc -> indice -> modificacion, 4, 1, fp);

	memcpy(disco_total + num_bloque * 1024 + 8, file_desc -> indice -> modificacion, 4);


	for (int i = 0; i < 252; i ++){
		// fseek(fp, num_bloque * 1024 + 12 + 4 * i, SEEK_SET);
		if (file_desc -> indice -> datos[i] == NULL){
			// fwrite(sin_bloque, 4, 1, fp);
			memcpy(disco_total + num_bloque * 1024 + 12 + 4 * i, sin_bloque, 4);
		}
		else{
			unsigned char input[4];
			int num = file_desc -> indice -> datos[i] -> num_bloque;
			input[0] = (num >> 24) & 0xFF;
			input[1] = (num >> 16) & 0xFF;
			input[2] = (num >> 8) & 0xFF;
			input[3] = num & 0xFF;
			// fwrite(input, 4, 1, fp);
			memcpy(disco_total + num_bloque * 1024 + 12 + 4 * i, input, 4);
			cerrar_bloque_datos(file_desc -> indice -> datos[i], fp);
		}
	}
	// fseek(fp, num_bloque * 1024 + 12 + 4 * 252, SEEK_SET);
	if (file_desc -> indice -> indirecto == NULL){
		// fwrite(sin_bloque, 4, 1, fp);
		memcpy(disco_total + num_bloque * 1024 + 12 + 4 * 252, sin_bloque, 4);
	}
	else{
		unsigned char input[4];
		int num = file_desc -> indice -> indirecto -> num_bloque;
		input[0] = (num >> 24) & 0xFF;
		input[1] = (num >> 16) & 0xFF;
		input[2] = (num >> 8) & 0xFF;
		input[3] = num & 0xFF;
		fwrite(input, 4, 1, fp);
		memcpy(disco_total + num_bloque * 1024 + 12 + 4 * 252, input, 4);
		cerrar_bloque_indirecto(file_desc -> indice -> indirecto, fp);
	}

	fclose(fp);
	// file_desc -> indice
	free(file_desc -> indice);
	free(file_desc);
	bindice = bindice_init();
	actualizar_disco();
	free(bindirecto);
	free(bindice);
	// bindirecto = bindirecto_init();
	return 0;
}

void cerrar_bloque_datos(BDatos* datos, FILE* fp){
	// fseek(fp, , SEEK_SET); 
	// fwrite(, 1024, 1, fp);
	memcpy(disco_total + datos -> num_bloque * 1024, datos -> datos, 1024);
	free(datos);
	// actualizar_disco();
}

void cerrar_bloque_indirecto(BIndirecto* indirecto, FILE* fp){
	int num_bloque = indirecto->num_bloque;
	unsigned char sin_bloque[4];
	for (int i = 0; i < 4; i++){
		sin_bloque[i] = 0x00;
	}
	for (int i = 0; i < 256; i ++){
		// fseek(fp, num_bloque * 1024 + 4 * i, SEEK_SET);
		if (indirecto -> datos[i] == NULL){
			// fwrite(sin_bloque, 4, 1, fp);
			memcpy(disco_total + num_bloque * 1024 + 4 * i, sin_bloque, 4);
		}
		else{
			unsigned char input[4];
			int num = indirecto -> datos[i] -> num_bloque;
			input[0] = (num >> 24) & 0xFF;
			input[1] = (num >> 16) & 0xFF;
			input[2] = (num >> 8) & 0xFF;
			input[3] = num & 0xFF;
			// fwrite(input, 4, 1, fp);
			memcpy(disco_total + num_bloque * 1024 + 4 * i, input, 4);
			cerrar_bloque_datos(indirecto -> datos[i], fp);
		}	
	}
	// free(indirecto);
	// 
	// actualizar_disco();
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
 		while (directorio_actual != NULL){
 			unsigned char no_nulo;
 			no_nulo = '\x01';
			if (*directorio_actual -> valido == no_nulo){
 				if (strcmp(directorio_actual -> nombre, orig) == 0){
 					strcpy(directorio_actual -> nombre, dest);
 					strcpy(directorio_actual -> nombre, dest);
 					actualizar_directorio(posicion_directorio, directorio_actual);
 					actualizar_disco();
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
 	return 1;
}

int cz_cp(char* orig, char* dest){
	if (strcmp(orig, dest) == 0){   //los nombres son iguales 
		return 1;
	}
	if (cz_exists(dest) == 1){		//si dest ya existe
		return 1;
	}
	czFILE* origen = cz_open(orig, 'r');
	int tamano_origen = obtener_tamano(origen -> indice ->tamano);
	unsigned char orig_texto[tamano_origen + 1];
	cz_read(origen, orig_texto, tamano_origen);
	orig_texto[tamano_origen] = '\0';
	cz_close(origen);
	czFILE* destino = cz_open(dest, 'w');
	cz_write(destino, orig_texto, tamano_origen);
	cz_close(destino);
	actualizar_disco();
	return 0; 						//no hubo errores
}

void imprimir_bitmaps(int numero_bitmap){			//TODO borrar
	Bitmap* actual_bitmap = bitmaps -> head;
	while (actual_bitmap != NULL){
		for (int i = 0; i < 1024; i++){			//recorrer cada byte del bitmap
			for (int j = 7; j >= 0; j --){
				int resultado = i * 8 + (7 - j);
				if (resultado == numero_bitmap){
					if (!((actual_bitmap -> bits[i] & ( 1 << j )) >> j)){ 
						printf("0");
					}
					else{
						printf("1");
					}
					break;
				}
			}
		}
		actual_bitmap = actual_bitmap -> next_bitmap;
	}
	printf("\n");
}

void imprimir_bitmaps2(){			//TODO borrar
	Bitmap* actual_bitmap = bitmaps -> head;
	int unos = 0;
	while (actual_bitmap != NULL){
		for (int i = 0; i < 1024; i++){			//recorrer cada byte del bitmap
			for (int j = 7; j >= 0; j --){
				if (!((actual_bitmap -> bits[i] & ( 1 << j )) >> j)){ 
					
				}
				else{
					unos += 1;
				}
			}
		}
		actual_bitmap = actual_bitmap -> next_bitmap;
	}
	printf("unos: %d\n", unos);
	printf("\n");
}

int vaciar_bitmap(int bloque){			//TODO MALO MALO
	Bitmap* actual_bitmap = bitmaps -> head;
	while (actual_bitmap != NULL){
		for (int i = 0; i < 1024; i++){			//recorrer cada byte del bitmap
			for (int j = 7; j >= 0; j--){
				int resultado = i * 8 + (7 - j);
				if (resultado == bloque){						
					actual_bitmap -> bits[i] &= ~(1UL << j);
					actualizar_bitmap(actual_bitmap);
					return 0;
				}
			}
		}
		actual_bitmap = actual_bitmap -> next_bitmap;
	}
	return -1;
}

void limpiar_rm(int posicion_indic){
	int posicion_indice = posicion_indic;
	vaciar_bitmap(posicion_indice);			//saca del bitmap el bloque indice
	FILE *fp;
	fp = fopen(filename_disco, "rb");
	unsigned char buffer[4 + 1];
	buffer[0] = 0x00;
	buffer[1] = 0x00;
	buffer[2] = 0x00;
	buffer[3] = 0x00;
	buffer[4] = '\0';

	int num_bloque;
	int i;
	for (i = 0; i < 252; i ++){
		// fseek(fp, posicion_indice * 1024 + 12 + 4 * i, SEEK_SET);
		// fread(buffer, 4, 1, fp);				//lee el número de un bloque de datos
	
		memcpy(buffer, disco_total + posicion_indice * 1024 + 12 + 4 * i, 4);
		buffer[4] = '\0';
		num_bloque = byte_a_decimal(buffer, 4);		
		if (num_bloque != 0){					//si no es cero, hay un bloque de datos
			vaciar_bitmap(num_bloque);
		}
	}
	// fseek(fp, posicion_indice * 1024 + 12 + 4 * i, SEEK_SET);
	// fread(buffer, 4, 1, fp);
	memcpy(buffer, disco_total + posicion_indice * 1024 + 12 + 4 * i, 4);
	buffer[4] = '\0';
	num_bloque = byte_a_decimal(buffer, 4);	
	
	if (num_bloque != 0){					//si no es cero, hay un bloque indirecto
		vaciar_bitmap(num_bloque);			//saca de bitmap el indirecto
		int otro_numero = 0;
		for (int i = 0; i < 256; i ++){
			// fseek(fp, num_bloque * 1024 + 4 * i, SEEK_SET);
			// fread(buffer, 4, 1, fp);				//lee el número de un bloque de datos
			memcpy(buffer, disco_total + num_bloque * 1024 + 4 * i, 4);
			buffer[4] = '\0';
			otro_numero = byte_a_decimal(buffer, 4);		
			if (otro_numero != 0){					//si no es cero, hay un bloque de datos
				vaciar_bitmap(otro_numero);			//saca de bitmap el bloque de datos
			}
	}
	
	}	
	fclose(fp);
}

void printear_dir(){
	Directorio* directorio_actual = bdirectorio -> head;	
	while (directorio_actual != NULL){
		printf("[0] %d\n", directorio_actual -> indice[0]);
		printf("[1] %d\n", directorio_actual -> indice[1]);
		printf("[2] %d\n", directorio_actual -> indice[2]);
		printf("[3] %d\n", directorio_actual -> indice[3]);	
		directorio_actual = directorio_actual -> next_directorio;
	
	}
}

int cz_rm(char* filename){
	if (cz_exists(filename) == 1){
		Directorio* directorio_actual = bdirectorio -> head;
		int posicion_directorio = 0;
		int posicion_indice = 0;
		while (directorio_actual != NULL){
			unsigned char no_nulo;
			no_nulo = '\x01';
			if (*directorio_actual -> valido == no_nulo){
				if (strcmp(directorio_actual -> nombre, filename) == 0){ //encontré el archivo
					posicion_indice = byte_a_decimal(directorio_actual -> indice, 4);
					limpiar_rm(posicion_indice);
					*directorio_actual -> valido = 0x00;		//se saca archivo de directorio
					actualizar_directorio(posicion_directorio, directorio_actual);
					actualizar_disco();
					return 0;
				}
			}
			directorio_actual = directorio_actual -> next_directorio;
			posicion_directorio += 1;
		}
	}
	// else{
	// 	actualizar_disco();
	// 	return 0;
	// }
	return 1;   			//TODO revisar
}

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
			for (int j = 7; j >= 0; j --){
				if (!((actual_bitmap -> bits[i] & ( 1 << j )) >> j)){        //si encuentro un bit de cero
					int resultado = i * 8 + (7 - j);
					actual_bitmap -> bits[i] |= 1UL << j;				//cambiar un bit a 1 en actual_bitmap
					actualizar_bitmap(actual_bitmap);
					return resultado;
				}
			}
		}
		actual_bitmap = actual_bitmap -> next_bitmap;
	}
	return -1;
}

void actualizar_directorio(int posicion_directorio, Directorio* directorio){
	// FILE *fp;
	// fp = fopen(filename_disco, "wb");

	// fseek(fp, posicion_directorio * 16, SEEK_SET); 
	// fwrite(directorio -> valido, 1, 1, fp);

	// fseek(fp, 1 + posicion_directorio * 16, SEEK_SET);
	// fwrite(directorio -> nombre, 11, 1, fp);

	// fseek(fp, 12 + posicion_directorio * 16, SEEK_SET);
	// fwrite(directorio-> indice, 4, 1, fp);

	// fclose(fp);

	memcpy(disco_total + posicion_directorio * 16, directorio -> valido, 1);
	memcpy(disco_total + 1 + posicion_directorio * 16, directorio -> nombre, 11);
	memcpy(disco_total + 12 + posicion_directorio * 16, directorio -> indice, 4);

	// actualizar_disco();
}

void actualizar_disco(){
	FILE *fp;
	fp = fopen(filename_disco, "wb");
	fseek(fp, 0, SEEK_SET); 
	fwrite(disco_total, 64 * 1048576, 1, fp);
	fclose(fp);	
}

void actualizar_bitmap(Bitmap* bitmap){
	// FILE *fp;
	// fp = fopen(filename_disco, "wb");
	// printf("num_bloque: %d\n", bitmap -> num_bloque);
	// fseek(fp, bitmap -> num_bloque * 1024, SEEK_SET); 
	// printf("bitmap_bits %s\n", bitmap -> bits);
	// fwrite(bitmap -> bits, 1024, 1, fp);
	// fclose(fp);
	memcpy(disco_total + bitmap->num_bloque * 1024, bitmap->bits, 1024);
	// actualizar_disco();
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
	// free(bindirecto);
}
