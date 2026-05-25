# Tarea 3
## objetivo:
El objetivo de este programa es resolver un laberinto generado aleatoriamente mediante distintos tipos de algoritmos de busqueda. El sistema modela el programa como un grafo implicito, donde cada celda representa una casilla del laberinto.
El programa permite comparar distintos caminos posibles segun el algoritmo de busqueda como DFS, BDS y Best First. Mostrando la ruta encontrada, cantidad de pasos y cantidad de nodos explorados.

## Funcionamiento del programa
En este programa el usuario puede seleccionar distintos metodos de busqueda para intentar encontrar una ruta desde el punto de inicio hasta la meta del laberinto.
El laberinto es generado segun la dificulta ingresada por el usuario, la cual representa la cantidad de obtaculos en el mapa.

## Opciones del laberinto
### Generar Laberinto
 El usuario ingresa una dificultad entre 0 a 100, el cual representa la cantidad de obstaculos en el mapa del laberinto se genera de manera automatica de tamaño 10x10.
### Busqueda en profundidad(DFS)
 El algoritmo intenta avanzar lo más profundo posible antes de retroceder, explorando caminos completos hasta encontrar salida o llegar a un callejón sin salida.
### Busqueda en anchura(BFS)
 El algoritmo explora primero todos los nodos cercanos antes de avanza a mayores profundindades.
### Busqueda de Best First
 El algoritmo explora primero los estaods más estados prometedores segun la prioridad.
      Prioridad = cantidad de pasos hechos + estimacion distancia faltante

      estimacion distancia faltante = |x - x(f)| + |y - y(f)|

      
## Problemas y limitaciones

### Funcion obtenerAdyacentes:
en esta funcion reservamos y liberamos constantemente memoria para los vecinos.

### redimiento:
en laberintos que sean mas grandes el rendimiento de este algoritmo puede sede ser lento Si un camino tiene 50 pasos, crear un vecino implica 50 reservas de memoria. Si ese vecino tiene 3 vecinos, son 150 reservas. Esto vuelve el código extremadamente lento y pesado.

## integrantes:
### sergio torres: 
funcion obtenerAdyacentes y busqueda de best First
### yaril salinas: 
funcion de BFS y DFS

### Subir cambios:
  ````
  git add .
  git commit -m "..."
  git push
  ````
  donde comit "..." es donde escribimos los cambios que subimos

### Actualizar repositorio local:
  ````
  git pull
  ````

  ## Instrucciones de Compilación y Ejecución
  
  ### Compilación
  Usa el siguiente comando en la terminal para compilar el programa junto con las librerías de TDAs:
   ````
   gcc tdas/*.c Tarea3.c -Wno-unused-result -o Tarea3
   ````

   ### Ejecución
  Una vez compilado, ejecuta el programa con:
   ````
   ./Tarea3
   ````