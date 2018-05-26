#include <stdint.h>

#pragma once

/****************************************************************************/
/*                                Tipos                                     */
/****************************************************************************/

// typedef struct czFILE {
//   char filename[11 + 1];
//   char  
// } czFILE;

struct bdatos;
typedef struct bdatos BDatos;

struct bdatos
{
  unsigned char datos[1024 + 1];
  int num_bloque;
};

struct indirecto;
typedef struct indirecto BIndirecto;

struct indirecto
{
  BDatos* datos[256];
  int num_bloque;
};

struct directorio;
typedef struct directorio Directorio;

struct directorio
{
  unsigned char valido[1 + 1];
  char nombre[11 + 1];
  unsigned char indice[4 + 1];
  Directorio* next_directorio;
};

struct b_directorio;
typedef struct b_directorio BDirectorio;

struct b_directorio
{
  Directorio* head;
  Directorio* tail;
  int count;
  unsigned int num_bloque;
};

struct bitmap;
typedef struct bitmap Bitmap;

struct bitmap
{
  // char bits[1024 + 1];
  unsigned char bits[1024 + 1];              //2^10 * 2^13
  Bitmap* next_bitmap;
  unsigned int num_bloque;
};

struct bitmaps;
typedef struct bitmaps Bitmaps;

struct bitmaps
{
  Bitmap* head;
  Bitmap* tail;
  int count;
};

struct index;
typedef struct index BIndice;

struct index
{
  unsigned char tamano[4 + 1];
  unsigned char creacion[4 + 1];
  unsigned char modificacion[4 + 1];
  BDatos* datos[252];
  BIndirecto* indirecto;
  int num_bloque;
};

typedef struct czFILE {
  char filename[11 + 1];
  BIndice* indice;
  int mode;             //0 si es 'r' y 1 si es 'w'
} czFILE;

// /****************************************************************************/
// /*                               Funciones                                  */
// /****************************************************************************/
//
czFILE* cz_open(char* filename, char mode);
int cz_exists(char* filename);
int cz_read(czFILE* file_desc, void* buffer, int nbytes);
int cz_write(czFILE* file_desc, void* buffer, int nbytes);
int cz_close(czFILE* file_desc);
int cz_mv(char* orig, char *dest);
int cz_cp(char* orig, char* dest);
int cz_rm(char* filename);
void cz_ls();

void cz_mount(char* diskfileName);

Directorio* directorio_init();
BDirectorio* bdirectorio_init();
void directorio_insert(BDirectorio* bdirectorio, Directorio* new);

Bitmap* bitmap_init(unsigned int num);
Bitmaps* bitmaps_init();
void bitmap_insert(Bitmaps* bitmaps, Bitmap* new);

BIndice* bindice_init();
BIndirecto* bindirecto_init();
BDatos* bdatos_init(int num_bloque);

czFILE* czfile_init(char* filename, int mode);

int buscar_espacio_en_bitmap();
void actualizar_directorio(int posicion_directorio, Directorio* directorio);
void actualizar_bitmap(Bitmap* bitmap);
void actualizar_indice(BIndice* bindice);
void actualizar_indirecto(BIndirecto* bindirecto);
void actualizar_datos(BDatos* bdatos);

void setear_bindice(int indice);
void setear_bindirecto(int indice);      
int directorio_lleno();
void agregar_direccion(czFILE* archivo);
int obtener_tamano(unsigned char* tamano);
int byte_a_decimal(unsigned char* tamano, int nbytes);

int escribir_un_bloque(czFILE* file_desc, void* buffer, int nbytes, int bytes_a_usar_en_bloque_actual, int posicion_bloque);
void actualizar_tamano(czFILE* file_desc, int j);

void cerrar_bloque_datos(BDatos* datos, FILE *fp);
void cerrar_bloque_indirecto(BIndirecto* indirecto, FILE* fp);


czFILE* setear_estructuras(char * filename, int mode);

void liberar_resto();
void leer_datos(BDatos* datos, FILE* fp);
void leer_bindirecto(BIndirecto* bindirecto, FILE* fp);

int vaciar_bitmap(int bloque);
void imprimir_bitmaps(int numero_bitmap);
void imprimir_bitmaps2();

void limpiar_rm(int posicion_indice);

void printear_dir();

int obtener_datos(BDatos* datos, void* buffer, int nbytes, int leidos, int voy_a_leer, int inicio_bloque);
void actualizar_disco();
void actualizar_tiempo(unsigned char* buff, int nuevo_tiempo);