#pragma once

/****************************************************************************/
/*                                Tipos                                     */
/****************************************************************************/

struct param;
typedef struct param Param;

struct param
{
  char name[256];
  Param* next_param;
};

struct params;
typedef struct params Params;

struct params
{
  Param* head;
  Param* tail;
  int count;
};

struct tarea;
typedef struct tarea Tarea;

struct tarea
{
  char name[256];
  Tarea* next_tarea;
  Params* params;
};

struct tareas;
typedef struct tareas Tareas;

struct tareas
{
	Tarea* head;
	Tarea* tail;
	int count;
};


// /****************************************************************************/
// /*                               Funciones                                  */
// /****************************************************************************/
//
Tarea* tarea_init();
Tareas* tareas_init();

Param* param_init();
Params* params_init();

int reading_file(char* file, Tareas* tareas);
void tarea_insert(Tareas* tareas, Tarea* new);
void tarea_remove(Tareas* tareas);

void create_process(int start, int finish, pid_t pids[], struct timeval secuencial_i[], struct timeval secuencial_f[], Tareas* tareas, int es_el_padre, int m, int n, struct timeval real_i, struct timeval real_f, double tiempo_real, double tiempo_secuencial);
int es_fin(Tareas* tareas, pid_t pids[], int n);
void mostrar_estadisticas_globales (int procesos_ejecutados, int m, int n,double tiempo_real, double tiempo_secuencial);

void param_insert(Params* params, Param* new);