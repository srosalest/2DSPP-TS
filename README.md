# 2DSPP-TS
Algoritmo hibrido, Tabu Search  + BLF  para resolver 2DSPP

## Instrucciones de Compilación

Para compilar el programa, ingresar a la carpeta contenedora y ejecutar el
comando **make**

## Instrucciones de Ejecución

Una vez compilado, para ejecutar el programa se debe utilizar el siguiente
comando

(Las instancias se deben encontrar en la carpeta 2sp, para ejecutar solo es necesario ingresar el nombre de la instancia.)

~~~
make run INSTANCE=[nombredelainstancia.extension]
~~~

## Formato instancias

El formato de las instancias debe ser el mismo definido en la presentacion del problema realizado durante la catedra.

**IMPORTANTE:** Las instancias utilizadas no pueden contener objetos con un ancho mayor al ancho de la superficie.

**EJEMPLO**
~~~
20  //Cantidad de Objetos
25  //Ancho superficie
1 6 8   //id, ancho y largo del objeto
2 4 11
3 3 10
4 7 4
5 6 12
6 6 11
7 8 12
8 6 3
9 3 4
10 8 3
11 1 9
12 3 1
13 6 8
14 3 11
15 1 11
16 8 9
17 1 9
18 5 11
19 7 1
20 8 7
~~~

## Formato output

El output del programa se guarda en la carpeta output en un archivo del tipo [Nombre Instancia].OUTPUT.
El formato es el siguiente, primero se entrega la altura utilizada, segundo se entrega el área desperdiciada por la solución. Por último se entregan 4 números por objeto: ID del objeto, Coordenadas x de la esquina inferior izquierda, coordenadas y de la esquina inferior izquierda, si el objeto se encuentra rotado (1 == ROTADO)

**IMPORTANTE:** No se debe eliminar la carpeta output

**EJEMPLO**
~~~
33  //Altura obtenida
84  //Área desperdiciada
6 0 0 1 // ID, Coordenada x, Coordenada y, Rotacion
2 11 0 0
3 15 0 0
4 18 0 1
5 0 11 1
19 0 6 0
7 12 11 1
11 0 7 1
9 18 7 1
8 22 0 1
1 0 17 0
12 7 6 0
13 6 17 0
14 0 8 1
15 12 19 1
16 12 20 0
17 15 10 1
18 20 20 0
20 0 25 0
10 8 25 1
~~~


**IMPORTANTE:**  Se deben mantener las carpetas 2dsp y output.