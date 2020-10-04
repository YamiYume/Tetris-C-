# Tetris-CPP
A version of tetris in c++
## Funcionamiento
### Empecemos por la representacion
- Si bien los tableros son una matriz de información, en aras de la ligereza y la sofisticación podemos representar cada uno de los datos necesarios en este caso como un string
basados en las siguientes formulas
  - Sea un tablero de `width` de ancho por `heigth` de alto podemos poner todas las filas una tras otra en una gran fila de tamaño `width*heidth` lo cual en si puede tranformarse en un string, de tal forma si queremos acede a la posición `IXJ` notada de manera YxX y empezando ambas desde 0 podemos podemos usar la formula `Posicion=I*width+J` devolviendonos la posición respectiva dentro del string; Basado en esta logica hemos creado nuestro tablero, pantalla y tetrominos que son strings de longitud 16.
### Rotando
- para no complicar nuestro codigo ni tenre que poner 4 strings por cada tetromino podemos hacer una funcion rotacion que dada una posicion
