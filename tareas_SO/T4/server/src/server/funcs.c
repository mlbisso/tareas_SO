#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "funcs.h"
#include <string.h>
#include <stdint.h>

Tabla_final* tabla_final_init(int bits_last_level)
{
	Tabla_final* tabla_final = malloc(sizeof(Tabla_final));
	int i;
	int a = 1;
	tabla_final -> bits_optimo = bits_last_level;
	for (i=0; i < bits_last_level; i++)
	{
		a = a*2;
	}
	tabla_final -> tamano = a;
	for(i = 0; i < a; i++)
	{
		strcpy(tabla_final -> marcos[i], "zzzzzzzz000");
	}
  	tabla_final -> next_tabla = NULL;
  	return tabla_final;
}

TablaNDOS* tablandos_init(int bits_penlast_level, int bits_last_level)
{
	TablaNDOS* tablandos = malloc(sizeof(TablaNDOS));
	int i;
	int a = 1;
	for (i = 0; i< bits_penlast_level; i++)
	{
		a = a*2;
	}
	tablandos -> tamano = a;
	tablandos -> bits_optimo = bits_penlast_level;
	tablandos -> tablas_finales = (Tabla_final**)malloc(a*sizeof(Tabla_final*));
	for(i = 0; i < a; i++)
	{
		tablandos -> tablas_finales[i] = tabla_final_init(bits_last_level);
	}
	return tablandos;
}

TablaN3* tablan3_init(int bits_first_level, int bits_penlast_level, int bits_last_level)
{
	TablaN3* tablan3 = malloc(sizeof(TablaN3));
	int i;
	int a = 1;
	for (i = 0; i < bits_first_level; i++)
	{
		a = a*2;
	}
	tablan3-> tamano = a;
	tablan3 -> bits_optimo = bits_first_level;
	tablan3 -> tablas_n2 = (TablaNDOS**)malloc(a*sizeof(TablaNDOS*));
	for(i = 0; i < a; i++)
	{
		tablan3 -> tablas_n2[i] = tablandos_init(bits_penlast_level, bits_last_level);
	}
	return tablan3;
}

TablaN4* tablan4_init(int bits_first_level, int bits_second_level, int bits_penlast_level, int bits_last_level)
{
	TablaN4* tablan4 = malloc(sizeof(TablaN4));
	int i;
	int a = 1;
	for (i = 0; i < bits_first_level; i++)
	{
		a = a*2;
	}
	tablan4 -> tamano = a;
	tablan4 -> bits_optimo = bits_first_level;
	tablan4 -> tablas_n3 = (TablaN3**)malloc(a*sizeof(TablaN3*));
	for(i = 0; i < a; i++)
	{
		tablan4 -> tablas_n3[i] = tablan3_init(bits_second_level, bits_penlast_level, bits_last_level);
	}
	return tablan4;
}

TablaN5* tablan5_init(int bits_first_level, int bits_second_level, int bits_third_level, int bits_penlast_level, int bits_last_level)
{
	TablaN5* tablan5 = malloc(sizeof(TablaN5));
	int i;
	int a = 1;
	for (i = 0; i < bits_first_level; i++)
	{
		a = a*2;
	}
	tablan5 -> tablas_n4 = (TablaN4**)malloc(a*sizeof(TablaN4*));
	tablan5 -> tamano = a;
	tablan5 -> bits_optimo = bits_first_level;
	for(i = 0; i < a; i++)
	{
		tablan5 -> tablas_n4[i] = tablan4_init(bits_second_level, bits_third_level, bits_penlast_level, bits_last_level);
	}
	return tablan5;
}

Tlb* tlb_init()
{
	Tlb* tlb = malloc(sizeof(Tlb));
	int i;
	for(i = 0; i < 64; i++)
	{
		strcpy(tlb -> marcos[i], "zzzzzzzz000");
	}
	for(i = 0; i < 64; i++)
	{
		strcpy(tlb -> paginas[i], "zzzzzzzzzzzzzzzzzzzz");
	}
	for(i = 0; i < 64; i++)
	{
		strcpy(tlb -> lru[i], "000000000000000");
	}
	return tlb;
}

void imprimir_tlb(Tlb* tlb, int bits_por_nivel[], int niveles)
{
	printf("TLB\n");
	int i;
	printf("i     ");
	for (i= 0; i < niveles; i++)
	{
		printf("n%d_number     ", i+1);
	}
	printf("frame_number\n");

}

int character_bin_int(char binario[])
{
	int total = 0;
	int i;
	if (strlen(binario) == 1){
		return total;
	}
	for (i=0; i < strlen(binario)-3; i++)
	{
		if (binario[i] == 49) // 49 es un 1 en asci
		{
			total += pow(2, strlen(binario)-4-i);
		}
	}
	return total;
}

float optimizador(int niveles, int * puntero_to_bits_por_nivel)
{
	float tamano_utilizado;
	if (niveles == 1)
	{
		* puntero_to_bits_por_nivel = 20;
		return pow(2, 20)*11;
	}
	else if(niveles == 2)
	{
		int x;
		int i;
		double valor;
		double minimo = INFINITY;
		for (i= 1; i < 20; i++)
		{
			valor = pow(2, i)*(20 - i) + pow(2, 20 - i)*11;
			if (valor < minimo)
			{
				minimo = valor;
				x = i;
			}
		}
		* puntero_to_bits_por_nivel = x;
		puntero_to_bits_por_nivel++;
		* puntero_to_bits_por_nivel = 20 - x;
		puntero_to_bits_por_nivel--;
		tamano_utilizado = pow(2, x)*(20 - x) + 
						   pow(2, x)*pow(2, 20 - x)*11;
		return tamano_utilizado;
	}
	else if(niveles == 3)
	{	
		int x;
		int i;
		double valor;
		double minimo = INFINITY;
		int y;
		int j;
		for (i=1; i < 19; i++)
		{
			for (j=1; j < 19; j++)
			{
				if (i + j < 20)
				{
					valor = pow(2, i)*j + pow(2, j)*(20-i-j) + pow(2, 20-i-j)*11;
					if (valor < minimo)
					{
						minimo = valor;
						x = i;
						y = j;
					}
				}

			}
		}
		* puntero_to_bits_por_nivel = x;
		puntero_to_bits_por_nivel++;
		* puntero_to_bits_por_nivel = y;
		puntero_to_bits_por_nivel++;
		* puntero_to_bits_por_nivel = 20 - x - y;
		puntero_to_bits_por_nivel--;
		puntero_to_bits_por_nivel--;
		tamano_utilizado = pow(2, x)*y + 
					       pow(2, x)*pow(2, y)*(20-x-y) + 
					       pow(2, x)*pow(2, y)*pow(2, 20-x-y)*11;
		return tamano_utilizado;
	}
	else if(niveles == 4)
	{
		int x;
		int i;
		double valor;
		double minimo = INFINITY;
		int y;
		int j;
		int z;
		int k;
		for (i=1; i < 18; i++)
		{
			for (j=1; j < 18; j++)
			{
				for (k=1; k < 18; k++)
				{
					if (i + j + k < 20)
					{
						valor = pow(2, i)*j + pow(2, j)*k + pow(2, k)*(20-i-j-k) + pow(2, 20 - i - j - k)*11;
						if (valor < minimo)
						{
							minimo = valor;
							x = i;
							y = j;
							z = k;
						}
					}
				}	
			}
		}
		* puntero_to_bits_por_nivel = x;
		puntero_to_bits_por_nivel++;
		* puntero_to_bits_por_nivel = y;
		puntero_to_bits_por_nivel++;
		* puntero_to_bits_por_nivel = z;
		puntero_to_bits_por_nivel++;
		* puntero_to_bits_por_nivel = 20 - x - y - z;
		puntero_to_bits_por_nivel--;
		puntero_to_bits_por_nivel--;
		puntero_to_bits_por_nivel--;
		tamano_utilizado = pow(2, x)*y + 
						   pow(2, x)*pow(2, y)*z + 
						   pow(2, x)*pow(2, y)*pow(2, z)*(20-x-y-z) + 
						   pow(2, x)*pow(2, y)*pow(2, z)*pow(2, 20-x-y-z)*11;
		return tamano_utilizado;
	}
	else if(niveles == 5)
	{
		int x;
		int i;
		double valor;
		double minimo = INFINITY;
		int y;
		int j;
		int z;
		int k;
		int a;
		int l;
		for (i=1; i < 17; i++)
		{
			for (j=1; j < 17; j++)
			{
				for (k=1; k < 17; k++)
				{
					for (l=1; l < 17; l++)
					{
						if (i + j + k + l < 20)
						{
							valor = pow(2, i)*j + pow(2, j)*k + pow(2, k)*l + pow(2, l)*(20-i-j-k-l) + pow(2, 20-i-j-k-l)*11;
							if (valor < minimo)
							{
								minimo = valor;
								x = i;
								y = j;
								z = k;
								a = l;
							}
						}
					}	
				}	
			}
		}
		* puntero_to_bits_por_nivel = x;
		puntero_to_bits_por_nivel++;
		* puntero_to_bits_por_nivel = y;
		puntero_to_bits_por_nivel++;
		* puntero_to_bits_por_nivel = z;
		puntero_to_bits_por_nivel++;
		* puntero_to_bits_por_nivel = a;
		puntero_to_bits_por_nivel++;
		* puntero_to_bits_por_nivel = 20 - x - y - z - a;
		puntero_to_bits_por_nivel--;
		puntero_to_bits_por_nivel--;
		puntero_to_bits_por_nivel--;
		puntero_to_bits_por_nivel--;
		tamano_utilizado = pow(2, x)*y + 
					       pow(2, x)*pow(2, y)*z + 
					       pow(2, x)*pow(2, y)*pow(2, z)*a + 
					       pow(2, x)*pow(2, y)*pow(2, z)*pow(2, a)*(20-x-y-z-a) + 
					       pow(2, x)*pow(2, y)*pow(2, z)*pow(2, a)*pow(2, 20-x-y-z-a)*11;
	}
	return tamano_utilizado;	
} 

Input* input_init(int direccion_dec){
	Input* input = malloc(sizeof(Input));
	input->next_input = NULL;
	input -> direccion_dec = direccion_dec;
	input->direccion_bin[28] = '\0';
	itoa(direccion_dec, input->direccion_bin, 2, 28);
	return input;
}

Inputs* inputs_init(){
	Inputs* inputs = malloc(sizeof(Inputs));
	inputs -> count = 0;
	inputs -> head = NULL;
	inputs -> tail = NULL;
	return inputs;
}

void input_insert(Inputs* inputs, Input* new){
	if(inputs -> count == 0){
		inputs -> tail = new;
		inputs -> head = new;
	}
	else{
		inputs -> tail -> next_input = new;            //el siguiente del ultimo actual es new
		inputs -> tail = new;                                //se agrega new al final
		inputs -> tail -> next_input = NULL;
	}
	inputs -> count++;	
}

void input_remove(Inputs* inputs){
	if (inputs -> count > 0){
		inputs -> head = inputs -> head -> next_input;
		inputs -> count --;
	}
}

void recorrer_inputs(Inputs* inputs){
	Input* actual_input = inputs->head;
	while (actual_input != NULL){
		printf("input: %s\n", actual_input -> direccion_bin);
		actual_input = actual_input -> next_input;
	}
}


int reading_file(char* file, Inputs* inputs){
  char *p;
  FILE* fr = fopen(file, "r");
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  if (fr == NULL){
      return -1;
  }
  while ((read = getline(&line, &len, fr)) != -1) {
    p = strtok(line, "\n");        //p entrega el entero
    Input* input = input_init(atoi(p));
    input_insert(inputs, input);
  }
  return 0;
}

int esta_en_tlb(Tlb* tlb, Input* input, char frame[]){
	char no_offset[21];
	int esta = 0;
	strncpy(no_offset, input -> direccion_bin, 20);
	no_offset[20] = '\0';
	for(int i = 0; i < 64; i++){
		if (strcmp(tlb -> paginas[i], no_offset) == 0){
			strcpy(frame, tlb -> marcos[i]);
			frame[11] = '\0';
			esta = 1;
		}
	}
	return esta;
}

void imprimir_simulacion(Input* input, char frame[], char content[]){
	int f = character_bin_int(frame);
	int c = character_bin_int(content);
	printf("-%d-\n", input->direccion_dec);
	printf("DIRECCIÓN FÍSICA: %d\n", f);
	printf("CONTENIDO: %d\n", c);
}

int buscar_uno(Tabla_final* uno, char* input, Input* linea){
	char direccion[uno -> bits_optimo + 1];
	strncpy(direccion, input, uno -> bits_optimo);
	direccion[uno -> bits_optimo] = '\0';				//esto me entrega la dirección a la que tengo que ir en binario
	// int pasos = character_bin_int(direccion);
	char offset[strlen(input) + 1 - uno -> bits_optimo];
	memcpy(offset, &input[uno->bits_optimo], strlen(input) - uno->bits_optimo);
	offset[strlen(input)- uno -> bits_optimo] = '\0';
	char no_offset[21];
	memcpy(no_offset, &linea->direccion_bin[0], 20);
	no_offset[20] = '\0';
	int posicion = character_bin_int(no_offset);

	//leer
	FILE *fp;
   	char buffer[256];

	fp = fopen("data.bin", "rb");

	fseek(fp, posicion, SEEK_SET);

	fread(buffer, 20, pow(2,8), fp);
	int p_off = character_bin_int(offset);
	printf("-%d-\n", linea->direccion_dec);
	printf("DIRECCIÓN FÍSICA: %u\n", buffer[p_off]);
	printf("CONTENIDO: %u\n", buffer[p_off]);
	fclose(fp);
	return 1;
}

void buscar_dos(TablaNDOS* dos, char* input, Input* linea){
	char direccion[dos -> bits_optimo + 1];
	strncpy(direccion, input, dos -> bits_optimo);
	direccion[dos -> bits_optimo] = '\0';				//esto me entrega la dirección a la que tengo que ir en binario
	int pasos = character_bin_int(direccion);
	char dir_corta[strlen(input) + 1 - dos -> bits_optimo];
	memcpy(dir_corta, &input[dos->bits_optimo], strlen(input) - dos->bits_optimo);
	dir_corta[strlen(input)- dos -> bits_optimo] = '\0';
	buscar_uno(dos->tablas_finales[pasos], dir_corta, linea);
}

void buscar_tres(TablaN3* tres, char* input, Input* linea){
	char direccion[tres -> bits_optimo + 1];
	strncpy(direccion, input, tres -> bits_optimo);
	direccion[tres -> bits_optimo] = '\0';				//esto me entrega la dirección a la que tengo que ir en binario
	int pasos = character_bin_int(direccion);
	char dir_corta[strlen(input) + 1 - tres -> bits_optimo];
	memcpy(dir_corta, &input[tres->bits_optimo], strlen(input) - tres->bits_optimo);
	dir_corta[strlen(input)- tres -> bits_optimo] = '\0';
	buscar_dos(tres->tablas_n2[pasos], dir_corta, linea);
}
void buscar_cuatro(TablaN4* cuatro, char* input, Input* linea){
	char direccion[cuatro -> bits_optimo + 1];
	strncpy(direccion, input, cuatro -> bits_optimo);
	direccion[cuatro -> bits_optimo] = '\0';				//esto me entrega la dirección a la que tengo que ir en binario
	int pasos = character_bin_int(direccion);
	char dir_corta[strlen(input) + 1 - cuatro -> bits_optimo];
	memcpy(dir_corta, &input[cuatro->bits_optimo], strlen(input) - cuatro->bits_optimo);
	dir_corta[strlen(input)- cuatro -> bits_optimo] = '\0';
	buscar_tres(cuatro->tablas_n3[pasos], dir_corta, linea);
}
void buscar_cinco(TablaN5* cinco, char* input, Input* linea){
	char direccion[cinco -> bits_optimo + 1];
	strncpy(direccion, input, cinco -> bits_optimo);
	direccion[cinco -> bits_optimo] = '\0';				//esto me entrega la dirección a la que tengo que ir en binario
	int pasos = character_bin_int(direccion);
	char dir_corta[strlen(input) + 1 - cinco -> bits_optimo];
	memcpy(dir_corta, &input[cinco->bits_optimo], strlen(input) - cinco->bits_optimo);
	dir_corta[strlen(input)- cinco -> bits_optimo] = '\0';
	buscar_cuatro(cinco->tablas_n4[pasos], dir_corta, linea);
}

void simulacion(Inputs* inputs, Tlb* tlb, Tabla_final* uno, TablaNDOS* dos, TablaN3* tres, TablaN4* cuatro, TablaN5* cinco){
	int tlb_hits = 0;
	char frame[12];
	Input* input = inputs -> head;
	while(input != NULL){
		if (!esta_en_tlb(tlb, input, frame)){			//quiere decir que no esta
			if (uno != NULL){
				buscar_uno(uno, input -> direccion_bin, input);
			}
			if (dos != NULL){
				buscar_dos(dos, input -> direccion_bin, input);
			}
			if (tres != NULL){
				buscar_tres(tres, input -> direccion_bin, input);
			}
			if (cuatro != NULL){
				buscar_cuatro(cuatro, input -> direccion_bin, input);
			}
			if (cinco != NULL){
				buscar_cinco(cinco, input -> direccion_bin, input);
			}
		}   
		else{
			imprimir_simulacion(input, frame, "");   //TODO cambiaer
			tlb_hits += 1;
		}
		input = input -> next_input;
	}
}

//sacado de 
//http://www.techiedelight.com/implement-itoa-function-in-c/   (MODIFICADO)
void swap(char *x, char *y) {
  char t = *x; *x = *y; *y = t;
}

char* reverse(char *buffer, int i, int j)
{
  while (i < j)
    swap(&buffer[i++], &buffer[j--]);

  return buffer;
}

char* itoa(int value, char* buffer, int base, int largo_binario)
{

  int n = value;
  int i = 0;
  while (n)
  {
    int r = n % base;
    if (r >= 10){
      buffer[i++] = 65 + (r - 10);    	
    } 
    else{
      buffer[i++] = 48 + r;
    }
    n = n / base;
  }

  // si numero es 0
  if (i == 0)
    buffer[i++] = '0';

  for (i = 0; i < largo_binario; i++){
    if (buffer[i] != 48 && buffer[i] != 49){
        buffer[i] = 48; 
    }
  }
  return reverse(buffer, 0, i - 1);
}
