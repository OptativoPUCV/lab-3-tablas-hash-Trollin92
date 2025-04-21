#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    map->size +=1;
    long index = hash(key,map->capacity); // calculamos la posicion donde se va a insertar el nuevo par
    for (size_t i = index; i<map->capacity; i++){
        if (map->buckets[i] == NULL){ //si el bucket está vacío, se inserta el nuevo par
            map->buckets[i] = createPair(key,value);
            return;
        }
        else if (is_equal(map->buckets[i]->key,key)){ //si el bucket no está vacío, se verifica si la key es igual a la del par ya existente
            map->buckets[i]->value = value; //si es igual, se reemplaza el valor del par existente por el nuevo valor
            return;
        }
        else if (i == map->capacity-1){ //como es circular, si llega al final del hashmap vuelve al principio
            i = -1;
        }
    }
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)


}


HashMap * createMap(long capacity) {                    //se crea la variable hashmap con capacidad = 'capacity'; y la retorna
    HashMap * Hmap = (HashMap *)malloc(sizeof(HashMap)); //reservar memoria para la creacion de una tabla hashmap
    Hmap->buckets = (Pair **)malloc(sizeof(Pair *) * capacity); //inicializa buckets al reservar memoria para una cantidad de buckets = capacity
    for (size_t i = 0; i<capacity; i++) {
        Hmap->buckets[i] = NULL; // inicializa cada bucket en NULL
    }
    Hmap->size = 0; // No hay ningún par dentro del hashmap, asi que la cant. de buckets es 0
    Hmap->capacity = capacity; // Se asigna la capacidad de la tabla como la variable leida por la función
    Hmap->current = -1; //petición del ejercicio
    return Hmap;
}

void eraseMap(HashMap * map,  char * key) {    


}

Pair * searchMap(HashMap * map,  char * key) {
    long index = hash(key,map->capacity); // calculamos la posicion donde podría estar el par
    for (size_t i = index; i<map->capacity; i++){
        if (map->buckets[i] == NULL) return NULL;
        if (is_equal(map->buckets[i]->key , key)){
            map->current = i;
            return buckets[i];
        }
    }
    return NULL;
}

Pair * firstMap(HashMap * map) {

    return NULL;
}

Pair * nextMap(HashMap * map) {

    return NULL;
}
