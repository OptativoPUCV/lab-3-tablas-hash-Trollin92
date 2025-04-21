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
    Pair **parsitosViejos = map->buckets;      // Cree una variable auxiliar de tipo Pair** para matener el arreglo map->buckets
    map->capacity *=2;                         // Duplique el valor de la variable capacity
    map->buckets = (Pair**)malloc(sizeof(Pair*) * map->capacity);   // Asigne a map->buckets un nuevo arreglo con la nueva capacidad
    map->size = 0;                                                  // Inicialice size a 0.
    for (size_t i = 0; i<map->capacity; i++){  // Inicializando los buckets en NULL  
        map->buckets[i] = NULL;                                        
    }
    for (size_t i = 0; i<map->capacity/2 ; i++){
        if (parsitosViejos[i] != NULL && parsitosViejos[i]->key != NULL){   
            insertMap(map,parsitosViejos[i]->key,parsitosViejos[i]->value); // Inserte los elementos del arreglo antiguo al nuevo
        }
    }
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
    map->size -=1;                              //actualizamos el size
    long index = hash(key,map->capacity);       // buscamos la ubicacion de la clave
    for (size_t i = index; i<map->capacity;i++){ //desde index en adelante buscamos el bucket a anular
        if(map->buckets[i] == NULL) return;      //si dato no existe, return null
        if (is_equal(map->buckets[i]->key,key)){ //si encontramos el dato, anulamos su key con null
        map->buckets[i]->key = NULL;
        return;
        }
    }
}

Pair * searchMap(HashMap * map,  char * key) {
    long index = hash(key,map->capacity); // calculamos la posicion donde podría estar el par
    for (size_t i = index; i<map->capacity; i++){ //desde 'index' hacia delante buscamos la key;
        if (map->buckets[i] == NULL) return NULL;//si el bucket es vacio no hace falta seguir buscando
        if (is_equal(map->buckets[i]->key , key)){
            map->current = i;                   // actualizo el current
            return map->buckets[i];             //retornamos el parsito encontrado
        }
    }
}

Pair * firstMap(HashMap * map) {
    for (size_t i = 0; i<map->capacity; i++){    //desde la posicion 0 del array, buscamos hasta encontrar dato valido
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL){
            map->current = i;                   //actualizamos el current
            return map->buckets[i];             //retorna el primer par del array
        }
    }
    return NULL;
}

Pair * nextMap(HashMap * map) {
    for (size_t i = map->current+1; i<map->capacity; i++){    //desde la posicion 0 del array, buscamos hasta encontrar dato valido
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL){
            map->current = i;                   //actualizamos el current nuevo
            return map->buckets[i];             //retorna el siguiente par valido al current
        }
    }
    return NULL;
}
