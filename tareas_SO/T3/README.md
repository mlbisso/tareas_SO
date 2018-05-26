# T3-SO

# **Alumnos**
Luis Ampuero	13637851
Mauro Bisso    14209519

# **Decisiones de diseño**
Se crearon tres archivos: cz_API.h corresponde al header, que contiene las declaraciones de structs y funciones utilizadas; cz_API.c, contiene las funciones que se usaron (la api); y main.c es el programa de prueba. 

Se crearon structs para un czFILE, que tiene el nombre del archivo, el modo (si es escritura o lectura), y un puntero al bloque indice que le corresponde. Además, se crearon structs para todos los tipos de bloques, los que están especificados en cz_API.h. De lo anterior es importante destacar que el bloque de directorio (BDirectorio) es una lista ligada de direcciones (Directorio), y que Bitmaps es una lista ligada de Bitmap.

**IMPORTANTE:** en main.c, antes de probar la API se debe colocar cz_mount(argv[1]). Esto permite que se lea el archivo binario y se monte el disco. Además, es importante que al final de main.c se coloque la función liberar_resto(). Esto permite que se libere la memoria que no se liberó con cz_close() para el bonus. Es decir, esto libera los bitmaps y las direcciones

Se utilizaron variables globales en cz_API. Estas corresponden al bloque de directorio, a la lista ligada de bitmpas, al nombre de archivo de disco (binario entregado como input), a un bloque índice, un bloque índice indirecto y a un arreglo, que se utiliza para manejar el disco desde el código. 

Se actualiza el disco (archivo binario) cada vez que se hace correctamente (no retorna un error) cz_close, un cz_mv, un cz_cp y un cz_rm.

El tiempo de creación se guarda cuando se hace cz_open en modo escritura a un archivo que no existe. Se realiza lo mismo con el tiempo de modificación de un archivo, pero, además, este valor cambia al final de cada ejecución de la función cz_write()
