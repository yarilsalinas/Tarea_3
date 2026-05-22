
// Ejemplo de heap (cola con prioridad)
printf("\n***** EJEMPLO USO DE HEAP ******\nCreamos un Heap e insertamos 3 elementos con distinta prioridad\n");
Heap* heap = heap_create();
char* data = strdup("Cinco");
printf("Insertamos el elemento %s con prioridad -5\n", data);
heap_push(heap, data, -5 /*prioridad*/);
data = strdup("Seis");
printf("Insertamos el elemento %s con prioridad -6\n", data);
heap_push(heap, data, -6 /*prioridad*/);
data = strdup("Siete");
printf("Insertamos el elemento %s con prioridad -7\n", data);
heap_push(heap, data, -7 /*prioridad*/);

printf("\nLos elementos salen del Heap ordenados de mayor a menor prioridad\n");
while (heap_top(heap) != NULL){
    printf("Top: %s\n", (char*) heap_top(heap));      
    heap_pop(heap);
}
printf("No hay más elementos en el Heap\n");