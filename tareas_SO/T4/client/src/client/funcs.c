#include <stdlib.h>
#include <stdio.h>
#include "funcs.h"
#include <math.h>
#include <unistd.h>

#include <string.h>

#include <stdbool.h>
#include <sys/types.h>  
#include <errno.h>  
#include <sys/wait.h>  
#include "sys/time.h"

#define die(e) do { fprintf(stderr, "%s\n", e); exit(EXIT_FAILURE); } while (0);


Param* param_init()
{
	Param* param = malloc(sizeof(Param));
	param->next_param = NULL;
	return param;
}

Params* params_init()
{
	Params* params = malloc(sizeof(Params));
	params -> count = 0;
	params -> head = NULL;
	params -> tail = NULL;
	return params;
}

void param_insert(Params* params, Param* new){
	if(params -> count == 0){
		params -> tail = new;
		params -> head = new;
	}
	else{
		params -> tail -> next_param = new;            //el siguiente del ultimo actual es new
		params -> tail = new;                                //se agrega new al final
		params -> tail -> next_param = NULL;
	}
	params -> count++;	
}


Tarea* tarea_init()
{
	Tarea* tarea = malloc(sizeof(Tarea));
	tarea->next_tarea = NULL;
	tarea-> params = params_init();
	return tarea;
}

Tareas* tareas_init()
{
	Tareas* tareas = malloc(sizeof(Tareas));
	tareas -> count = 0;
	tareas -> head = NULL;
	tareas -> tail = NULL;
	return tareas;
}

void tarea_insert(Tareas* tareas, Tarea* new){
	if(tareas -> count == 0){
		tareas -> tail = new;
		tareas -> head = new;
	}
	else{
		tareas -> tail -> next_tarea = new;            //el siguiente del ultimo actual es new
		tareas -> tail = new;                                //se agrega new al final
		tareas -> tail -> next_tarea = NULL;
	}
	tareas -> count++;	
}

void tarea_remove(Tareas* tareas){
	if (tareas -> count > 0){
		tareas -> head = tareas -> head -> next_tarea;
		tareas -> count --;
	}
}

int reading_file(char* file, Tareas* tareas){
    char *p;
	FILE* fr = fopen(file, "r");
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	int m = 0;
  	if (fr == NULL){
    	return -1;
	}
	while ((read = getline(&line, &len, fr)) != -1) {
		m += 1;
		p = strtok(line, "\n");        //esto entrega una linea de comando
	    if(p)
	    {
			Tarea* tarea = tarea_init();
			strcpy(tarea -> name, p);
			int z;
			int inicio = 0;            //para obtener substrings
			int blocked = 0;  //1 si estamos dentro de un " "
			for (z = 0; z < strlen(p); z++){
				if (p[z] == 32){ 					//si es espacio en blanco
					if (blocked == 0){					//o sea, si no es un espacio blanco dentro de un " "
						Param* param = param_init();
						strcpy(param -> name, strndup(p+inicio, z - inicio));
						param_insert(tarea -> params, param);
						inicio = z + 1;
					}
				}
				if (p[z] == 34){ 					//si es "
					if (blocked == 0){
						blocked = 1;				//encontré el primer "
					}
					else{							//si encontré el segundo "	
						Param* param = param_init();
						strcpy(param -> name, strndup(p+inicio + 1, z - inicio -1));
						param_insert(tarea -> params, param);
						inicio = z + 2;
						z += 1;
						blocked = 0;
					}
				}
				else if (z + 1 == strlen(p)){
					Param* param = param_init();
					strcpy(param -> name, strndup(p+inicio, z - inicio));
					param_insert(tarea -> params, param);
				}
			}
			tarea_insert(tareas,tarea);
	    }
	}
	fclose(fr);
	return m;
}

void create_process(int start, int finish, pid_t pids[], struct timeval secuencial_i[], struct timeval secuencial_f[], Tareas* tareas, int es_el_padre, int m, int n, struct timeval real_i, struct timeval real_f, double tiempo_real, double tiempo_secuencial){
    pid_t pid;
    pid_t pid_dos;
    int i;
	int link[2];
    char foo[4096];
    for(i = start; i<finish; i++)
    {
        pid = fork();
        if (pid > 0)
        {
            pids[i] = pid;
			gettimeofday(&secuencial_i[i], NULL);
            char *argv[tareas -> head -> params -> count + 1];
            int x = 0;
            Param* actual_param = tareas -> head -> params -> head;
            while (actual_param != NULL){
            	argv[x] = actual_param -> name;
            	x++;
            	actual_param = actual_param -> next_param;
            }
            argv[x] = NULL;
            printf("Proceso %d creado\n", pid);
	        tarea_remove(tareas);
			if (pipe(link)==-1)
			  die("pipe");

			if ((pid_dos = fork()) == -1)
			  die("fork");

            if (pid_dos > 0){
			    dup2 (link[1], STDOUT_FILENO);
			    close(link[0]);
			    close(link[1]);
            	execvp(argv[0], argv);
            	die("execvp");
            }
            else{
                close(link[1]);
    			int nbytes = read(link[0], foo, sizeof(foo));
    			printf("Output: \n %.*s\n", nbytes, foo);
            }
           
        }
        else
        {
        	if (m == 1){
        		int status;
        		int procesos_ejecutados = 1;
        		printf("Proceso padre\n");
		        waitpid(pids[0], &status, 0);                            //espera por un hijo
		        if (WIFEXITED(status)){
		        	printf("Proceso %d terminado con exit code %d\n", pids[0], WEXITSTATUS(status));	
		        }
		        else{
		        	printf("Ocurrió un término fallido en proceso %d\n", pids[0]);
		        }
				gettimeofday(&secuencial_f[0], NULL);
				tiempo_secuencial += (secuencial_f[0].tv_usec - secuencial_i[0].tv_usec) * pow(10,-6);
				gettimeofday(&real_f, NULL);
				tiempo_real = (real_f.tv_usec - real_i.tv_usec) * pow(10,-6);
				mostrar_estadisticas_globales(procesos_ejecutados, m, n, tiempo_real, tiempo_secuencial);		
        	}
        	exit(0);
        }
    }
}

int es_fin(Tareas* tareas, pid_t pids[], int n){
	int terminado = 1;									 //ver si quedan tareas en proceso
	int z;
	for (z = 0; z < n; z ++){
		if (pids[z] != -1){
			terminado = 0;
			break;
		}
	}
	return terminado;
}

void mostrar_estadisticas_globales (int procesos_ejecutados, int m, int n,double tiempo_real, double tiempo_secuencial){
	if (tiempo_real < 0){
		tiempo_real = -1 * tiempo_real;
	}
	if (tiempo_secuencial < 0){
		tiempo_secuencial = -1 * tiempo_secuencial;
	}
	printf("Tiempo real: %g segundos\n", tiempo_real);
	printf("Tiempo secuencial: %g segundos\n", tiempo_secuencial);
	printf("m = %d \n", m);
	printf("n = %d \n", n);
	printf("total de procesos ejecutados = %d\n", procesos_ejecutados);
}