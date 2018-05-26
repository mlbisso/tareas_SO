# T3-SO

#Alumnos
Luis Ampuero
Mauro Bisso    14209519

#Decisiones de diseño
Se crearon tres archivos: cz_API.h corresponde al header, que contiene las declaraciones de structs y funciones utilizadas; cz_API.c, contiene las funciones que se usaron (la api); y main.c es el programa de prueba. 

Se crearon strucst para un czFILE, que tiene el nombre del archivo, el modo (si es escritura o lectura), y un puntero al bloque indice que le corresponde. Además, se crearon structs para todos los tipos de bloques, los que están especificados en cz_API.h. De lo anterior es importante destacar que el bloque de directorio (BDirectorio) es una lista ligada de direcciones (Directorio), y que Bitmaps es una lista ligada de Bitmap.

IMPORTANTE: en main.c, antes de probar la API se debe colocar cz_mount(argv[1]). Esto permite que se lea el archivo binario y se monte el disco. Además, es importante que al final de main.c se coloque la función liberar_resto(). Esto permite que se libere la memoria que no se liberó con cz_close() para el bonus. Es decir, esto libera los bitmaps y las direcciones

Variables globales

El tiempo de creación se guarda cuando se hace cz_open en modo escritura a un archivo que no existe.


#Supuestos
Si un proceso está ejecutando en una cola que no tiene la mayor prioridad, y en ese instante llega otro proceso, se atiende a este último, pues tiene mayor prioridad.

#Comparación versiones
La versión 2 es mejor a la versión 1 en el sentido de que disminuye la inanición de los procesos. Es decir, evita que algún proceso no se llegue a ejecutar por encontrarse en una cola de baja prioridad. Por lo tanto, en la v2 el turnaround promedio debiese ser menor que en la v1; el response time no debiese tener mayores diferencias, así como tampoco el waiting time.

La versión 3 busca disminuir aún más la diferencia que existe en la preferencia que se le da a los procesos más cortos frente a los largos en este algoritmo. Los procesos más largos son los que llegarán a estar en una cola de prioridad baja. Entonces, al hacer que el quantum sea variable, se logra que estos procesos tengan más tiempo para utilizar la CPU, y por lo tanto puedan finalizar antes su ejecución. Por lo tanto, se disminuye el turnaround time de las versiones anteriores; el waiting time promedio debiese ser mayor (porque muchos procesos esperan a que un solo proceso largo ejecute una ráfaga); y el response time puede ser mayor, por lo anterior.

#Comentario
En caso de que que el comando /.mqlf (parametros) fallé ejecutar el comando >chmod +x mlfq 
# tareas_SO
