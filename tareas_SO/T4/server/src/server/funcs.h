#pragma once
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

struct tabla_final;
typedef struct tabla_final Tabla_final;

struct tabla_final
{
  char marcos[2*2*2*2*2*2*2*2*2*2*2*2*2*2*2*2*2*2*2*2][11];
  Tabla_final* next_tabla;
  int tamano;
  int bits_optimo;
};

struct tablandos;
typedef struct tablandos TablaNDOS;
struct tablandos
{
  Tabla_final** tablas_finales;
  int tamano;
  int bits_optimo;
};

struct tablan3;
typedef struct tablan3 TablaN3;
struct tablan3
{
  TablaNDOS** tablas_n2;
  int tamano;
  int bits_optimo;
};

struct tablan4;
typedef struct tablan4 TablaN4;
struct tablan4
{
  TablaN3** tablas_n3;
  int tamano;
  int bits_optimo;
};

struct tablan5;
typedef struct tablan5 TablaN5;
struct tablan5
{
  TablaN4** tablas_n4;
  int tamano;
  int bits_optimo;
};

struct tlb;
typedef struct tlb Tlb;
struct tlb
{
  char marcos[64][11];
  char paginas[64][20];
  char lru[64][15];
};

struct input;
typedef struct input Input;

struct input
{
  int direccion_dec;
  char direccion_bin[29];
  Input* next_input;
};

struct inputs;
typedef struct inputs Inputs;

struct inputs
{
  Input* head;
  Input* tail;
  int count;
};

Tabla_final* tabla_final_init(int bits_ultimo_nivel);
TablaNDOS* tablandos_init(int bits_penlast_level, int bits_last_level);
TablaN3* tablan3_init(int bits_first_level, int bits_penlast_level, int bits_last_level);
TablaN4* tablan4_init(int bits_first_level, int bits_second_level, int bits_penlast_level, int bits_last_level);
TablaN5* tablan5_init(int bits_first_level, int bits_second_level, int bits_third_level, int bits_penlast_level, int bits_last_level);
Tlb* tlb_init();

Input* input_init(int direccion_dec);
Inputs* inputs_init();
void input_insert(Inputs* inputs, Input* new);
void input_remove(Inputs* inputs);

float optimizador(int niveles, int * puntero_to_bits_por_nivel);
void tabla_insert(Tabla_final* tabla_final, Tabla_final* new);
void imprimir_tlb(Tlb* tlb, int bits_por_nivel[], int niveles);

int character_bin_int(char* binario);
int reading_file(char* file, Inputs* inputs);
void recorrer_inputs(Inputs* inputs);

void swap(char *x, char *y);
char* reverse(char *buffer, int i, int j);
char* itoa(int value, char* buffer, int base, int largo_binario);

void simulacion(Inputs* inputs, Tlb* tlb, Tabla_final* uno, TablaNDOS* dos, TablaN3* tres, TablaN4* cuatro, TablaN5* cinco);
int esta_en_tlb(Tlb* tlb, Input* input, char frame[]);

void imprimir_simulacion(Input* input, char frame[], char content[]);

int buscar_uno(Tabla_final* uno, char* input, Input* linea);
void buscar_dos(TablaNDOS* dos, char* input, Input* linea);
void buscar_tres(TablaN3* tres, char* input, Input* linea);
void buscar_cuatro(TablaN4* cuatro, char* input, Input* linea);
void buscar_cinco(TablaN5* cinco, char* input, Input* linea);



