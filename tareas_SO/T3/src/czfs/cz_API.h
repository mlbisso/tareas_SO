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
  char datos[1024 + 1];
};

struct indirecto;
typedef struct indirecto BIndirecto;

struct indirecto
{
  BDatos* datos[256];
};

struct directorio;
typedef struct directorio Directorio;

struct directorio
{
  char valido[1 + 1];
  char nombre[11 + 1];
  char indice[4 + 1];
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
  uint8_t bits[1024];              //2^10 * 2^13
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
  char tamano[4 + 1];
  char creacion[4 + 1];
  char modificacion[4 + 1];
  BDatos* datos[252];
  BIndirecto* indice;
};

typedef struct czFILE {
  char filename[11 + 1];
  BIndice* indice;
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
BDatos* bdatos_init();

int buscar_indice();
void actualizar_directorio(BDirectorio* bdirectorio);
void actualizar_bitmap(Bitmap* bitmap);
void actualizar_indice(BIndice* bindice);
void actualizar_indirecto(BIndirecto* bindirecto);
void actualizar_datos(BDatos* bdatos);