# T2-SO

#Alumnos
Luis Ampuero
Mauro Bisso    14209519

#Parte 1

En esta parte de la tarea, se ejecutan las m tareas entregadas en input.txt, con la restricción de que se pueden ejecutar n tareas a la vez (es decir, dentro de estos n procesos, no se considera el proceso padre, solo los que hagan execvp). Las estadísticas, output y exit code se muestran en consola (para mostrar de forma correcta el output, se usó pipe()). Si se hace CTRL+C, el programa finaliza, y se muestran las estadísticas que se tienen hasta ese momento. En este caso, el tiempo secuencial total no considerará los procesos que no alcanzaron a terminar de ejecutar. El archivo de input que el programa usa, se ubicará en la carpeta que contiene el Makefile.

#Parte 2

Se lee el archivo de input; se guardan en una lista ligada las líneas. Se realizan los cambios de bases necesarios para poder leer direcciones y offset. Se procede con la primera línea, se va al TLB, y como no hay nada, se va por los distintos niveles (eso sí, antes se calcularon los valores óptimos por nivel). Se llega a la última tabla, se ve que no hay nada y se procede a leer el data.bin. Los datos se guardan en un buffer, y se puede también obtener la dirección buscada. Se imprime en consola este valor dos veces, ya que no pudimos pasar este valor a binario - con los otros valores sí pudimos hacer un cambio de base, no sabemos por qué con este no.
