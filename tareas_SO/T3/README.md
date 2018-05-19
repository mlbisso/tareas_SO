# T1-SO

#Alumnos
Luis Ampuero
Mauro Bisso

#Decisiones de diseño
Se crearon tres archivos: funcs.h corresponde al header, que contiene las declaraciones de structs y funciones utilizadas; funcs.c, contiene las funciones que se usaron; y main.c contiene el programa principal. Las colas se crearon con listas ligadas; y los procesos son estructuras que contienen sus datos, las conexiones entre procesos para las colas y la información requerida como output. Además de estas estructuras, se tiene un Burst (una ráfaga), que se coloca en la lista ligada de Bursts de cada proceso.

Se creó una cola new, que se completa al momento de leer el archivo de input. Contiene todos los procesos,los que todavía no están en estado ready. También se tiene una cola finished, que es donde llegan a parar los procesos que completan todas sus ráfagas. Por último, se tiene un conjunto de colas ready. El scheduler se encarga de recorrer estas últimas colas (desde mayor a menor prioridad); y cuando encuentra que una cola tiene un proceso en el head, toma este proceso y lo lleva a estado running (porque es el proceso con mayor prioridad, el programa creado los ordena para que se dé esta situación).

#Supuestos
Si un proceso está ejecutando en una cola que no tiene la mayor prioridad, y en ese instante llega otro proceso, se atiende a este último, pues tiene mayor prioridad.

#Comparación versiones
La versión 2 es mejor a la versión 1 en el sentido de que disminuye la inanición de los procesos. Es decir, evita que algún proceso no se llegue a ejecutar por encontrarse en una cola de baja prioridad. Por lo tanto, en la v2 el turnaround promedio debiese ser menor que en la v1; el response time no debiese tener mayores diferencias, así como tampoco el waiting time.

La versión 3 busca disminuir aún más la diferencia que existe en la preferencia que se le da a los procesos más cortos frente a los largos en este algoritmo. Los procesos más largos son los que llegarán a estar en una cola de prioridad baja. Entonces, al hacer que el quantum sea variable, se logra que estos procesos tengan más tiempo para utilizar la CPU, y por lo tanto puedan finalizar antes su ejecución. Por lo tanto, se disminuye el turnaround time de las versiones anteriores; el waiting time promedio debiese ser mayor (porque muchos procesos esperan a que un solo proceso largo ejecute una ráfaga); y el response time puede ser mayor, por lo anterior.

#Comentario
En caso de que que el comando /.mqlf (parametros) fallé ejecutar el comando >chmod +x mlfq 
# tareas_SO
