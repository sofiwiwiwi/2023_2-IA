# Risk Cash Transit Vehicle Routing problem

## Consideraciones
- Para la ejecución correcta del programa, los nombres de las carpetas donde se encuentran las instancias no deben tener espacios. Por ejemplo, en vez de `SET O` la carpeta debería llamarse `SET_O`.
- La carpeta con las instancias debe estar en la misma carpeta que `src`

## Compilación
Para compilar el código, escribir en la terminal en la misma carpeta donde está la carpeta `src` el comando:

`make compile`

## Ejecución
Para ejecutar el código, escribir en la terminal en la misma carpeta donde está la carpeta `src` el comando:

`make one_test FOLDER_INSTANCIAS=<nombre_carpeta> SET=<nombre_set> FILE_TEST=<nombre_instancia>`

donde "nombre_carpeta" es la carpeta que contiene todos los sets, "nombre_set" es el nombre del set de la instancia y "nombre instancia" es el nombre del archivo donde está la instancia. 

Por ejemplo:
`make one_test FOLDER_INSTANCIAS=Instancias-RCVRP SET=SET_O FILE_TEST=10.txt`

Ejecuta la instancia 10 del SET O

Si se escribe solamente 

`make one_test`

se ejecuta la instancia 10.txt del SET_O por defecto.


El archivo de salida se crea en la carpeta donde está el makefile