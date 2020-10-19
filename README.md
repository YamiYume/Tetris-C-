# Tetris-CPP
A version of tetris in c++
## Basado en El video Code-It-Yourself! Tetris - Programming from Scratch `https://www.youtube.com/watch?v=8OK8_tHeCIA&ab_channel=javidx9`
## Funcionamiento
### Empecemos por la representacion.
- Si bien los tableros son una matriz de información, en aras de la ligereza y la sofisticación podemos representar cada uno de los datos necesarios en este caso como un string
basados en las siguientes formulas
  - Sea un tablero de `width` de ancho por `heigth` de alto podemos poner todas las filas una tras otra en una gran fila de tamaño `width*heidth` lo cual en si puede tranformarse en un string, de tal forma  que si queremos acceder a la posición `IxJ` notada de manera YxX y empezando ambas desde 0 podemos usar la formula `Posicion=I*width+J` devolviendonos la posición respectiva dentro del string; Basado en esta lógica hemos creado nuestro tablero que es un string compuesto de numeros del 1 al 9 los correspondiente a los 9 signos usados en nuestras graficas `" ABCDEFG=#"` , el tablero va a guardar únicamente la información de las piezas que estan fijas y de los bordes,la pantalla que es un string que representa literalmente los graficos del juego y tetrominos que son strings de longitud 16 que representan donde esta lleno o vacio el tetromino, adicionalmente cada string es un elemento de una lista la cual con sus índices nos va a permitir diferenciar entre los tetrominos y que al sumarle 65 a este indice nos devolvera el valos ASCCI correspondiente a la letra de la que deberian estar compuestos los tetrominos.
### Rotando.
- Para no complicar nuestro código ni tener que poner 4 strings por cada tetromino se hizo una función de rotación que dada una posicion `IxJ` que queremos saber del tetromino y la rotacion `Giro` de ese tetromino nos va a devolver la posicion dentro de cada string tetromino,  correspondiente a la que sería la posicion de entrada si el tetromino estuviera girado segun `Giro*90°` con respecto a la rotación horario.
```
int Rotation(int X, int Y, int G){
    switch(G % 4)
    {
        case 0: return Y*4+X;
        case 1: return 12+Y-X*4;
        case 2: return 15-Y*4-X;
        case 3: return 3-Y+X*4;
    }
    return 0;
}
```
### Validación.
- Para validar que nuestro movimiento y rotaciones, así como estados de game-over o la necesidad de fijar una pieza tenemos una función validación, para ello dados una posición de la esquina superior izquierda de nuestra pieza, su rotación y la clase de pieza que es,  iteramos sobre la pieza y usando nuestra función rotación accedemos a la posición de la pieza que queremos verificar en su rotación actual, con esa información verificamos si la pieza esta llena en esa posicion y si el tablero esta lleno en esa posicion conforme a la lógica mostrada anteriormente; en caso de que sea así, el movimiento es invalido, de otra forma si pasa las 16 comprobaciones el movimento es válido, a su vez los bloques `if` dentro de la función evitan errores y los límites ya puestos en el tablero mantendran la pieza dentro del tablero.
```
bool Valid(int T, int R, int X, int Y){
    for (int PX=0;PX<4;PX++)
        for (int PY=0;PY<4;PY++){
            int PI=Rotation(PX,PY,R);
            int FI=(Y+PY)*WidthF+X+PX;
            if (X+PX >=0 && X+PX<WidthF)
            {
                if (Y+PY >=0 && Y+PY<HeigthF)
                {
                    if((tetromino[T][PI]) == (L'X') && Field[FI]!=0)return false; 
                }     
            }   
        }
    return true;
}
```
### Movimiento,fijación,GameOver y rotación.
- El movimiento de los tetrominos esta completamente basado en las funciones anteriores, para el movimiento de las piezas lateralmente y hacia abajo solo se valida las pociones x+1,x-1 y Y+1 y en caso de ser validos estos poscionamientos se cambia el parametro Y o X de la esquina superior izquierda del tetromino, para la caída automática después de una cierta cantidad de ciclos la ficha cae automáticamente validando la colocación de la pieza en Y+1 y si es valida cambiando Y de su esquina superior izquierda a Y+1, en caso de no ser valida fijando la pieza en la posición en la que esta actualmente pasando la pieza a formar parte del tablero y generando una nueva pieza en la posición central superior del tablero validando si esta posición es valida pues en caso de no serlo es un GameOver pues el tablero estaria practicamente a tope, para rotar solamente se hace la verificación de la rotación `(Giro+1)%4` y en caso de ser válida se cambia a ese valor el `Giro` actual, cada uno de estos cambios estan comandados por las flechas del teclado que estan en una lista boleana y se verifica el valor de la posición de esta lista para generar los cambios.
### Eliminación de lineas
- Al fijar una piesa después de fallar en validar la cíida cada ciertos ciclos primero se revisan línea a línea las 4 posciones debajo de la última posición de la pieza, se supone que estan enteras y en caso de haber espacios se pasa a falso ese valor, en caso de no haber espacios, se agrega la posición a las líneas a elminar y se cambian los valores por 8 que al convertirlo en string mediante la lista `" ABCDEFG=#` en el `=` y corresponde al signo de ficha entera, posteriormente si se encontraron líneas enteras se genera la gráfica, del juego se para el proceso durante una cantidad de milisegundos y se bajan todas las posiciones encima de la posición a eliminar una unidad, así con las posiciones a eliminar desde la superior hasta la inferior, y se agrega una fila de ceros en la parte superior
```
if (!Deline.empty()){
            WriteConsoleOutputCharacter(hConsole,Screen,HeigthS*WidthS,{0,0},&dwBytesWritten);
            Sleep(300);
            for(auto &V : Deline)
                for(int PX=1;PX<WidthF-1;PX++){
                    for(int PY=V;PY>0;PY--)
                        Field[PY*WidthF+PX]=Field[(PY-1)*WidthF+PX];
                    Field[PX]=0;
                 }
            Deline.clear();    
        }     
```
# Aunque hay otros por menores acerca del juego estos son aquellos que fundamentan su funcionamiento y con ellos es más que suficiente para entender su implementación
# El programa ***NO*** Corre en linux, pues depende de archivos del sistema de windows para correr sus gráficos y no se encontró forma de hacer esto funcionar en linux.
