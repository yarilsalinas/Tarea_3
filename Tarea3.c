#include <stdio.h>
#include <stdlib.h>
#include "tdas/list.h"
#include "tdas/heap.h"
#include "tdas/extra.h"
#include <string.h>
#include <time.h>
#define N 10
// Definición de la estructura para el estado del puzzle
typedef struct {
    int (*maze)[N]; // Matriz NxN que representa el tablero
    int x;    // Posición x del agente
    int y;    // Posición x del agente
    int steps; // Pasos realizados hasta la posición actual
    List* actions; //Secuencia de movimientos para llegar al estado
} State;

int distancia_L1(State* state) { //Distancia hasta la meta
    return abs(state->x - (N-1)) + abs(state->y - (N-1));
}

// Función para imprimir el estado actual
void imprimirEstado(const State *estado) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (estado->x==i && estado->y==j) printf(" A ");
            else if (i == 0 && j == 0) printf(" I "); 
            else if (i == N-1 && j == N-1) printf(" M ");
            else if (estado->maze[i][j] == 0)
                printf(" . "); // Imprime un espacio en blanco para el espacio vacío
            else
                printf("[X]");
        }
        printf("\n");
    }
}

void imprimirRuta(State *estado){
    for (int i = 0; i < N; i++) {
        for(int j = 0; j < N; j ++){
            int ruta = 0;
            State *paso = list_first(estado -> actions);
            while(paso != NULL) {
                if(paso -> x == i && paso -> y == j){
                    ruta = 1;
                    break;
                }
                paso = (State *)list_next(estado -> actions);
            }
            if(i == 0 && j == 0){
                printf(" I ");
            }
            else if(i == N - 1 && j == N - 1){
                printf(" M ");
            }
            else if(ruta){
                printf(" * ");
            }
            else if(estado -> maze[i][j] == 0){
                printf(" . ");
            }
            else{
                printf("[X]");
            }
        }
        printf("\n");
    }
}

State crearEstadoInicial(int maze[N][N], int dificultad){
    State estado;
    // Generar el laberinto en el arreglo real antes de asignar el puntero
    generate_maze(maze, dificultad);

    estado.maze = maze;
    estado.x = 0;
    estado.y = 0;
    estado.steps = 0;
    estado.actions = list_create();
    return estado;
}

int es_meta_mostrar(State *actual, int nodos_explorados) {
    if (actual->x == N - 1 && actual->y == N - 1) {
        printf("\n¡Ruta encontrada!\n");
        printf("Pasos tomados: %d\n", actual->steps);
        printf("Nodos explorados: %d\n", nodos_explorados);
        imprimirRuta(actual);
        return 1;
    }
    return 0;
}

int esValido(int x, int y, int maze[N][N]){ // ve si la posicion sigue dentro de la matriz
    if (x < 0 || x >= N || y < 0 || y >= N) {
        return 0;
    }
    if(maze[x][y] == 1){
        return 0;
    }
    return 1;
}

List *obtenerAdyacentes(State *actual){
    List *listaVecinos = list_create();
    int movX[] = {-1, 1, 0, 0}; // filas
    int movY[] = {0, 0, -1, 1}; // columnas
    //i = 0: arriba; i = 1 : abajo; i = 2 :izquiera; i = 3: derecha

    for(int i = 0; i < 4; i++){
        int nuevaX = (actual -> x) + movX[i];
        int nuevaY = (actual -> y) + movY[i];
        if(esValido(nuevaX, nuevaY, actual -> maze)){
            State *vecino = (State *)malloc(sizeof(State));
            vecino -> maze = actual -> maze; // apuntan a la misma matriz
            vecino -> x = nuevaX;
            vecino -> y = nuevaY;
            vecino -> steps = actual -> steps + 1;
            vecino -> actions = list_create(); // lista propia para cada vecino

            State *paso = (State *)list_first(actual -> actions);
            while(paso != NULL){
                State *copia = (State *)malloc(sizeof(State));
                copia -> x = paso -> x;
                copia -> y = paso -> y;
                copia -> maze = NULL;
                copia -> actions = NULL;
                copia -> steps = 0;

                list_pushBack(vecino -> actions, copia);
                paso = (State *)list_next(actual -> actions);
            }
            State *nuevaPos = (State *)malloc(sizeof(State));
            nuevaPos -> x = nuevaX;
            nuevaPos -> y = nuevaY;
            list_pushBack(vecino -> actions, nuevaPos);
            list_pushBack(listaVecinos, vecino);
        }
    }
    return listaVecinos;
}

void dfs(State estado_inicial) {
    printf("\nIniciando Búsqueda en Profundidad (DFS)...\n");
    // 1. Crear la Pila 
    List* stack = list_create();
    // 2. Crear una matriz de visitados 
    int visitados[N][N];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            visitados[i][j] = 0;
        }
    }
    State* inicial = (State*) malloc(sizeof(State));
    *inicial = estado_inicial; 
    inicial -> actions = list_create(); //Evita que 2 punteros apunten a los mismo, para que al momento de liberar no haya errores
    list_pushFront(stack, inicial);  
    int nodos_explorados = 0; //contador
   
    while (list_first(stack) != NULL) { // ver toda la pila
        State* actual = (State*) list_first(stack);
        list_popFront(stack);
        nodos_explorados++; //sumar al contador  
        if (es_meta_mostrar(actual, nodos_explorados)) {
            list_clean(actual -> actions);
            free(actual -> actions);
            free(actual);
            while (list_first(stack) != NULL) {
                State* obsoleto = (State*) list_first(stack);
                list_popFront(stack);
                list_clean(obsoleto -> actions);
                free(obsoleto -> actions);
                free(obsoleto); 
            }
            free(stack);
            return;
        }
        if (visitados[actual->x][actual->y] == 0) {
            visitados[actual->x][actual->y] = 1;
            List* adyacentes = obtenerAdyacentes(actual);
            State* vecino = (State*) list_first(adyacentes);
        
            while (vecino != NULL) {
                if (visitados[vecino->x][vecino->y] == 0) {
                    list_pushFront(stack, vecino);
                } else {
                    list_clean(vecino -> actions);
                    free(vecino -> actions);
                    free(vecino); 
                }
                vecino = (State*)list_next(adyacentes);
            }
            free(adyacentes);  
        }
    list_clean(actual -> actions);
    free(actual -> actions);
    free(actual);  
    }
    printf("\nNo se encontró ninguna ruta hacia la meta.\n");
}

void bfs(State estado_inicial) {
    printf("\nIniciando Búsqueda en Anchura (BFS)...\n");
    List* queue = list_create();
    int visitados[N][N];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            visitados[i][j] = 0;
        }
    }
    State* inicial = (State*) malloc(sizeof(State));
    *inicial = estado_inicial; //crear cola
    inicial -> actions = list_create();
    list_pushBack(queue, inicial);  
    visitados[inicial->x][inicial->y] = 1; //marcar el primero
    int nodos_explorados = 0; //contador

    while (list_first(queue) != NULL) { 
        State* actual = (State*) list_first(queue);
        list_popFront(queue); //sacar de la cola
        nodos_explorados++;
        if (es_meta_mostrar(actual, nodos_explorados)) {
            list_clean(actual->actions); 
            free(actual->actions); 
            free(actual); 

            while (list_first(queue) != NULL) {
                State* obsoleto = (State*) list_first(queue);
                list_popFront(queue);
                list_clean(obsoleto->actions); // Limpia los nodos de la lista interna
                free(obsoleto->actions);       // Libera el contenedor de la lista
                free(obsoleto);                // Libera el estado
            }
            free(queue);
            return;
        }
        List* adyacentes = obtenerAdyacentes(actual);
        State* vecino = (State*) list_first(adyacentes);

        while (vecino != NULL) {
            if (visitados[vecino->x][vecino->y] == 0) {
                visitados[vecino->x][vecino->y] = 1; //marcar como visitado
                list_pushBack(queue, vecino); //añadir a la cola
            } else {
                list_clean(vecino->actions);
                free(vecino->actions);
                free(vecino); 
            }
            vecino = (State*)list_next(adyacentes);
        }
        free(adyacentes);
        list_clean(actual->actions);
        free(actual->actions);
        free(actual);  
    }

    printf("\nNo se encontró ninguna ruta hacia la meta.\n");
    free(queue);
}

void best_first(State estado_inicial){
    printf("\nIniciando la busqueda de mejor primero(Best First)\n");
    Heap *heap = heap_create(); //cola de prioridad
    int visitados[N][N] = {0};
    State* inicial = (State *)malloc(sizeof(State));
    *inicial = estado_inicial;
    inicial -> actions = list_create();
    
    heap_push(heap, inicial, -(inicial->steps + distancia_L1(inicial))); //Segun la prioridad sera el orden de exploracion de los nodo
    visitados[inicial -> x][inicial -> y] = 1;
    int nodosExplorados = 0;
    
    while(heap_top(heap) != NULL){//Mientras hayan elementos en el heap
        
        State *actual = (State *) heap_top(heap); //Toma el de mayor prioridad)
        heap_pop(heap);
        nodosExplorados++; //contador de nodos ya explorados

        if(es_meta_mostrar(actual, nodosExplorados)){ // si esta en la meta
            list_clean(actual -> actions);
            free(actual -> actions);
            free(actual);
            while(heap_top(heap) != NULL){ // mientras existan elementos
                State *obsoleto = (State *) heap_top(heap); // se libera memoria del heap
                heap_pop(heap);
                list_clean(obsoleto -> actions);
                free(obsoleto -> actions);
                free(obsoleto);
            }
            free(heap);
            return;
        }
        List *adyacentes = obtenerAdyacentes(actual);
        State *vecino = (State *)list_first(adyacentes);
        while(vecino != NULL){
            if(visitados[vecino -> x][vecino -> y] == 0){ // si no fue visitado
                visitados[vecino -> x][vecino -> y] = 1;

                heap_push(heap, vecino, -(vecino->steps + distancia_L1(vecino)));
            }
            else{
                list_clean(vecino -> actions);
                free(vecino -> actions);
                free(vecino);
            }
            vecino = (State *)list_next(adyacentes);
        }
        free(adyacentes);
    
        list_clean(actual -> actions);// Se libera el nodo actual
        free(actual -> actions);
        free(actual);
    }
    printf("\nEl laberinto no tiene salida \n");
    free(heap);
    return;
}

int main() {
    // Inicializar la semilla de aleatoriedad
    srand(time(NULL));
    int maze[N][N];
    int dificultad;
    // Solicitar la dificultad al usuario con validación
    do {
        printf("Ingrese la dificultad del laberinto (porcentaje de obstáculos, 0 a 100): ");
        scanf("%d", &dificultad);
        if (dificultad < 0 || dificultad > 100) {
            printf("Error: Por favor ingrese un valor válido entre 0 y 100.\n");
        }
    } while (dificultad < 0 || dificultad > 100);
    // Estado inicial del puzzle
    // Laberinto generado con la dificultad ingresada por el usuario
    State estado_inicial = crearEstadoInicial(maze, dificultad);
    // Imprime el estado inicial
    printf("\nEstado inicial del puzzle:\n");
    imprimirEstado(&estado_inicial);
    printf("Distancia L1: %d\n", distancia_L1(&estado_inicial));
    char opcion;
    do {
        puts("========================================");
        puts("     Escoge método de búsqueda");
        puts("========================================");
        puts("1) Búsqueda en Profundidad");
        puts("2) Buscar en Anchura");
        puts("3) Buscar Mejor Primero");
        puts("4) Salir");
        printf("Ingrese su opción: ");
        scanf(" %c", &opcion);
        switch (opcion) {
        case '1':
            dfs(estado_inicial); //parece no ser la mejor
          break;
        case '2':
            bfs(estado_inicial);
          break;
        case '3':
          best_first(estado_inicial);
          break;
        }
        // Evitamos pausar y limpiar pantalla si el usuario eligió salir
        if (opcion != '4') {
            presioneTeclaParaContinuar();
            limpiarPantalla();
        }
  } while (opcion != '4');

  return 0;
}