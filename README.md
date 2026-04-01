# TP1
## Información del estudiante

* **Nombre y Apellido**: Ramiro Carrattini
* **Padrón**: 113212
* **Mail**: ramirocarrattini@gmail.com / rcarrattini@fi.uba.ar

---

# Índice
- [TP1](#tp1)
  - [Información del estudiante](#información-del-estudiante)
- [Índice](#índice)
  - [1. Instrucciones](#1-instrucciones)
    - [1.1. Compilar el proyecto](#11-compilar-el-proyecto)
    - [1.2. Ejecutar las pruebas](#12-ejecutar-las-pruebas)
    - [1.3. Ejecutar el programa con Valgrind](#13-ejecutar-el-programa-con-valgrind)
  - [2. Funcionamiento](#2-funcionamiento)
    - [Estructuras](#estructuras)
        - [Pokédex (tp1)](#pokédex-tp1)
        - [Pokémon](#pokémon)
        - [Arquitectura final del programa](#arquitectura-final-del-programa)
    - [Primitivas](#primitivas)
    - [Flujo del programa](#flujo-del-programa)
  - [3. Respuestas a las preguntas teóricas](#3-respuestas-a-las-preguntas-teóricas)

## 1. Instrucciones
### 1.1. Compilar el proyecto
```bash
gcc main.c src/tp1.c -o tp1
gcc src/tp1.c pruebas/pruebas_alumno.c -o pruebas_tp1
```

### 1.2. Ejecutar las pruebas
```bash
./tp1 [archivo] [comando a ejecutar]
./pruebas_tp1
```

### 1.3. Ejecutar el programa con Valgrind
```bash
valgrind ./tp1 [archivo] [comando a ejecutar]
valgrind ./pruebas_tp1
```

## 2. Funcionamiento
### Estructuras
##### Pokédex (tp1)
Es la estructura principal del programa. Contiene un vector de punteros a Pokémon que viven en el heap, además de un tope que indíca la cantidad actual de Pokémon presentes. Se va rellenando a medida que se lee el archivo.
##### Pokémon
Es la estructura que rellena el vector dentro de la Pokédex. Contiene diferentes datos como su nombre, su tipo, su ataque, su defensa y su velocidad.
##### Arquitectura final del programa
Las estructuras del programa, en el instante que se finaliza la lectura de un archivo, quedan dispuestas dentro de la memoria de la siguiente manera:
![](https://i.imgur.com/udOnSyv.png)

### Primitivas
Existen varias operaciones para realizar sobre la Pokédex:

**`tp1_leer_archivo`**: Lee el archivo indicado y devuelve la Pokédex con los Pokémon correspondientes. Este proceso se realiza en varias fases diferentes:
1. Se abre el archivo indicado por `nombre` y se reserva memoria tanto para la estructura como la colección de Pokémon.
2. Se lee una línea y se determina si esta está vacía o es el final del archivo. Si lo és, se ignora y se repite este proceso con la siguiente línea del archivo. Internamente, se lee a partír del archivo y el texto se inserta en un buffer dinámico que se va llenando y escalando continuamente hasta poder contener a la línea completa. El final de una línea se reconoce por el marcador `'\n'`.
3. La línea leída se parsea para asegurar que esta tenga el formato correcto. Si no lo tiene, nuevamente se ignora y se continúa con la siguiente línea. Esto implíca que se respete la estructura pedida para cada línea, que la cantidad de campos sea la esperada y que los campos tengan valores lógicos (no se acepta que el Pokémon sea de un tipo fuera de los nombrados en la biblioteca, por ejemplo). Nuevamente si no se logra pasar este chequeo, el proceso se empieza desde cero.
4. Si se parsea sin errores, se escanea la colección de Pokémon hasta ahora para asegurarse de que el Pokémon que se está por agregar al vector no sea repetido (se identifica por nombre y sin distinguir por mayúsculas). En caso de serlo, se ignora la línea y se continúa con la siguiente del archivo.
5. Finalmente se agrega el Pokémon al vector con un chequo por si se debe hacer un ajuste al tamaño del vector. Internamente, este vector se maneja muy similarmente al buffer utilizado durante el proceso de lectura de línea.
6. Se repiten los pasos 2-5 hasta leer todas las líneas del archivo.
7. Si no se logra leer ningún Pokémon (el archivo está vacío o es inválido), se devolvera una Pokédex válida pero vacía.
8. Si la Pokédex no está vacía, se ordenan los Pokémon por nombre y se devuelve la estructura finalizada con todos los datos del archivo dentro de ella.

Siendo esta la función más compleja del proyecto, se adjunta un diagrama simplificado para poder visualizar su funcionamiento:

![](https://i.imgur.com/TdCviI5.png)

**`tp1_cantidad`**: Devuelve la cantidad actual de Pokémon dentro de la Pokédex.

**`tp1_guardar_archivo`**: Guarda en el archivo indicado los Pokémon contenidos en la Pokédex de manera tal que `tp1_leer_archivo` pueda volver a leerlo correctamente. Su funcionamiento es el siguiente:
1. Se crea o se sobreescribe el archivo con el `nombre` indicado.
2. Para cada Pokémon dentro de la Pokédex, se escribe una línea en el mismo formato que se utiliza para leer. Los tipos, antes almacenados como un número dentro de la estructura, son convertidos automáticamente a texto.
3. Se finaliza el guardado del archivo.

**`tp1_filtrar_tipo`**: Dado una Pokédex y un `tipo` específico, devuelve otra Pokédex conteniendo solamente los Pokémon de dicho tipo. Funciona así:
1. Se reserva memoria tanto para la nueva Pokédex como su colección de Pokémon.
2. Se crea un vector auxiliar para almacenar todos los Pokémon que se deberan copiar de la Pokédex original.
3. Se recorre la Pokédex original y se identifican los Pokémones a copiar (es decír, los que tienen el `tipo` pedido). Estos Pokémon se guardan en el vector auxiliar sin quitarlos de la Pokédex original.
4. A partir del vector auxiliar, se copian los Pokémon uno por uno y se almacenan dentro de la nueva Pokédex.
5. Se libera la memoria ocupada por el vector auxiliar y se devuelve la nueva Pokédex.

**`tp1_buscar_nombre`**: Busca un Pokémon por `nombre`. Se ejecuta una busqueda binaria clásica sobre la Pokédex, aprovechando el hecho de que los Pokémon estan ordenados por nombre.

**`tp1_buscar_orden`**: Devuelve el `n`-esimo pokemon por orden alfabetico (de menor a mayor). Nuevamente, esto resulta ser muy fácil gracias a que los Pokémon ya vienen ordenados así. Se accede al puntero a Pokémon en la posición `n` y se devuelve directamente.

**`tp1_con_cada_pokemon`**: Aplica la función `f` a cada Pokémon por orden alfabético (de menor a mayor). La función deja de aplicarse si `f` devuelve `false` o se terminaron los Pokémon. 

**`tp1_destruir`**: Libera toda la memoria asociada a la Pokédex. Estas liberaciones se hacen en un orden muy especifico para respetar la estructura de la Pokédex, y garantizan que no quede ningún dato colgando dentro del heap.
&nbsp;
### Flujo del programa
El programa empieza por imprimir un mensaje de bienvenida. Luego, controla que lo ingresado por línea de comando sea lo esperado. Si se llega a detectar una falla, se imprime un error detallando que sucedió mal, y se muestran las instrucciones por pantalla. 

Por ejemplo, el programa responde a la entrada `./tp1 a b c` de la siguiente manera:


![](https://i.imgur.com/mRzE029.png)


Si se pasan los controles iniciales, se empiezan a leer los Pokémon del archivo especificado, y se guardan dentro de un tp1_t utilizando `tp1_leer_archivo()` (proceso detallado en [la sección de primitivas del informe](#primitivas).)


Se obtiene la Pokédex a partír del archivo, y según la entrada del usuario se ejecutan uno de dos comandos:

**`ejecutar_comando_buscar`**: Busca un Pokémon con el nombre especificado e imprime su información por pantalla. Se realiza rápidamente con la primitiva `tp1_buscar_nombre()`. Si esta falla (es decir, el Pokémon buscado no está presente en la Pokédex cargada), se mostrará un mensaje de error.

**`ejecutar_comando_mostrar`**: Muestra todos los Pokémon por orden alfabético u ordenados por tipo. Si la Pokédex está vacía, se le avisará de esto al usuario.

La impresión por orden alfabético es mucho más sencilla: existe la primitiva `tp1_con_cada_pokemon` que resuelve el problema exacto que se propone. A esta primitiva se le pasa una función que muestra los datos de cada Pokémon por pantalla.

Para imprimir por orden de tipo, resultó necesario tener una manera de manipular la estructura opaca desde `main.c` para "ordenar" los Pokémon. De esto se encargó la función `mostrar_por_tipo()`. Lo que hace esta función se puede describir en diferentes pasos: 

1. Se crea una estructura auxiliar de vector dinámico (definida en `main.c`) y se reserva memoria para que quepa cada puntero a Pokémon de la Pokédex original dentro de ella.
2. Se inserta ordenadamente el puntero a cada Pokémon de la Pokédex original en el vector dinámico. El resultado final es un vector de punteros a Pokémon, ordenados según el tipo del Pokémon al que apuntan.
3. Se imprimen los datos de todos los Pokémon posición por posición.
4. Se libera la memoria que ocupó el vector auxiliar.

Luego de ejecutar el comando, se vuelve y se libera la memoria ocupada por la Pokédex. El programa termina su ejecución sin errores en este punto, y le avisa de esto al usuario.


&nbsp;
## 3. Respuestas a las preguntas teóricas
 - **Explicar la elección de la estructura para implementar la funcionalidad pedida. Justifique el uso de cada uno de los campos de la estructura.**

La naturaleza de los archivos que espera recibir el programa decidió mucho del diseño de la estructura: se supo que, por lo indicado, se requeriría rellenarla con cierta cantidad desconocida de Pokémon, a su vez con nombres de largo también desconocido.
Partiendo de esto, se decidió que lo más conveniente para este caso sería un vector dinámico. Por su flexibilidad en términos de tamaño y su adaptabilidad a la carga individual de cada archivo, esta estructura se utiliza tanto para la colección de Pokémon como para los strings que representan sus nombres.
Luego, se decidió agregar el campo *cantidad_pokemones* (el "tope" del vector) para garantizar la facilidad de implementación para las primitivas posteriores.
&nbsp;
- **Dar una definición de complejidad computacional y explique cómo se calcula.**

La complejidad computacional es una manera de representar la carga que tiene cierto algorítmo sobre la computadora que lo ejecuta a lo largo del tiempo.
Para calcularla se estudia como se comporta el algorítmo a medida que el tamaño de la entrada $n$ tiende a infinito. En el abstracto, este comportamiento se puede reducir muchas veces a un cálculo o fórmula matemática que le da una cota final. A partir de la cota calculada, se utiliza la notación *Big O* para categorizar a los algorítmos en diferentes tiempos de ejecución. Estos pueden ser constantes como $O(1)$, polinómicos como $O(n), O(n^2), ..., O(n^k)$, o descriptos por funciones más complejas como $O(n!), O(log(n)), etc...$
&nbsp;
- **Explicar con diagramas cómo quedan dispuestas las estructuras y elementos en memoria.**

La respuesta a este ítem se puede encontrar en [la sección de estructuras del informe](#estructuras).
&nbsp;
 - **Justificar la complejidad computacional temporal de  cada una de las funciones que se piden implementar.**

**`tp1_leer_archivo`**: Para una línea de lóngitud $n$, se estarían ejecutando las funciones `tp1_leer_linea`, `tp1_parsear_linea` y `pokemon_es_repetido`. Por lo tanto, la lectura de una línea y todos los procesos que implíca componen una tarea acotada en complejidad por la función más compleja de las 3. Posterior a la lectura, se ordenan los Pokémon con un *Bubble Sort*, también de complejidad $O(n^2)$.

Veamos las complejidades individuales de cada función:

`tp1_leer_linea`: Se invoca la función $O(1)$ `fgetc` una vez por cada carácter de una línea de $n$ carácteres. Para comprobar que la cota final es $O(n)$, necesito comprobar que el trabajo que realiza `realloc` durante la lectura no excede esa cota.

Se sabe que `realloc` se invocará $log_2(n)$ veces durante toda la lectura, ya que el tamaño del buffer se duplíca cada vez que se necesita ampliarlo. Entonces, matemáticamente, su trabajo es:

$T(n) = n + \frac{n}{2} + \frac{n}{4} + \frac{n}{8} + ... + 4 + 2 + 1$

$T(n) = \sum_{i=0}^{log_2(n)}\frac{n}{2^i}$

[Se puede verificar](https://www.wolframalpha.com/input?i=sum+n%2F2%5Ei%2C+i%3D0+to+log2%28n%29) que esta sumatoria tiene el resultado:

$T(n) = 2n - 1$

Entonces:

$T(n) \le O(n)$.

Por lo tanto, la complejidad final de la función entera es $O(n)$.

`tp1_parsear_linea`: Se cuentan los campos con `strchr` con una complejidad de $O(n)$ y luego se parsea la línea con `sscanf`, también $O(n)$. La complejidad final es de $O(n)$.

`pokemon_es_repetido`: Se itera linealmente sobre el vector de Pokémon para detectar Pokémon repetidos. Se usa `strcasecmp` $(O(n))$ un total de $n$ veces, haciendo que la función sea $O(n^2)$.

Como la lectura de una línea y el agregado de Pokémon es $O(n^2)$, la complejidad final de la función **`tp1_leer_archivo`** es de $O(n^3)$.

**`tp1_cantidad`**: Se extrae la cantidad almacenada en la estructura principal y se devuelve directamente. Su complejidad es de $O(1)$.

**`tp1_guardar_archivo`**: Asumiendo que `fprintf()` tiene una complejidad constante por la impresión de cada carácter, si se escriben un total de $n$ líneas de $n$ carácteres cada una, la complejidad final de la función resulta ser $O(n^2)$. Esto se puede visualizar con el siguiente gráfico:

![](https://i.imgur.com/bPV175q.png)

**`tp1_filtrar_tipo`**: Se busca linealmente sobre el vector de $n$ Pokémon para identificar cuales se deben copiar a la nueva Pokédex, comparando datos en cada iteración. En el peor caso, se ejecutaría además la función `copiar_pokemon` (de complejidad $O(1)$ ya que su funcionamiento consta de asignaciones simples de datos) un total de $n$ veces, llegando a una complejidad final de $O(n)$.

**`tp1_buscar_nombre`**: Se aprovecha el hecho de que los Pokémon siempre se reciben ordenados por nombre, y se realiza una busqueda binaria sobre el vector. La función tiene una complejidad final de $O(log(n))$.

**`tp1_buscar_orden`**: Se accede directamente al puntero al Pokémon en la posición $n$ dentro del vector y se devuelve. Su complejidad es de $O(1)$.

**`tp1_con_cada_pokemon`**: Su complejdad individual es de $O(n)$ ya que se itera sobre la cantidad total de $n$ Pokémon pertenecientes al vector. Sin embargo, la función `f` puede afectar este valor. Si se considera a la variable $x$ como la cota de la complejidad de `f`, una manera más precisa de representar la complejidad final del iterador es $O(n*x)$.

**`tp1_destruir`**: Se ejecuta la función $O(1)$, `free()`, por un total de $2n$ veces primero para los nombres de cada Pokémon, y luego para los punteros a cada estructura. Finalmente se libera el vector de Pokémon y la estructura principal. Se llega a una complejidad final de $O(n)$.
&nbsp;
- **Explique qué dificultades tuvo para implementar las funcionalidades pedidas en el main (si tuvo alguna) y explique si alguna de estas dificultades se podría haber evitado modificando la definición del .h.**

Las pruebas de caja negra podrían haber sido mucho más rigurosas con la estructura principal en `tp1.h` o pudiendo haber añadido algunas funciones auxiliares de control a la biblioteca. 

La parte de mostrar cada Pokémon ordenado por tipo fue muy dificultosa y se logró solamente después de mucha investigación acerca del tipo de dato `void*` y el funcionamiento de los punteros a función. La implementación que se hizo utilizando `tp1_con_cada_pokemon()` llevo mucho más esfuerzo y pensamiento que haber programado una función que directamente ordene la estructura por tipo en `tp1.h`.