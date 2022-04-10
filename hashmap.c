#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets; //arreglo que apunta a pares
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

void insertMap(HashMap * map, char * key, void * value)
{
  Pair *dato= createPair(key,value);
  int index=hash(dato->key,map->capacity);
  map->current = index;
  
  while(map->buckets[index]!=NULL)
  {
    index++;
    if(index==map->capacity)
    {
      index=0;
    }
  }
  map->buckets[index] = dato;
  map->size++;
}

void enlarge(HashMap * map)
{
    enlarge_called = 1; //no borrar (testing purposes)
}

HashMap * createMap(long capacity) {
  HashMap* tabla=(HashMap*)malloc(sizeof(HashMap));
  tabla->buckets = (Pair **) malloc(sizeof(Pair*) * capacity);
  for(int i=0;i<capacity;i++)
     {
        tabla->buckets[i]=NULL;
     }
  tabla->capacity=capacity;
  tabla->size=0;
  tabla->current=-1;
  
  return tabla;
}
 
void eraseMap(HashMap * map,  char * key) 
{   
  void *dato=searchMap(map,key);
  if(dato!=NULL)
  {
    if(is_equal(key,map->buckets[map->current]->key)==1)
    {
      map->buckets[map->current]->key=NULL;
      map->size=map->size-1;
    }
  }
}

Pair * searchMap(HashMap * map,  char * key)
{   
  int index=hash(key,map->capacity);
  index=map->current;
  while(map->buckets[index]!=NULL)
  {
    if(is_equal(key,map->buckets[index]->key)==1)
    {
      index=map->current;
      //map->current=index;
      return map->buckets[index];
    }
    index++;
    if(index==map->capacity)
    {
      index=0;
    }
  }
  return NULL;
}

Pair * firstMap(HashMap * map) 
{
  for(int i=0;i<map->capacity;i++)
  {
    if(map->buckets[i] != NULL)
    {
      if(map->buckets[i]->key != NULL)
      {
        map->current=i;
        return map->buckets[i];
      }
    }
  }
  return NULL;
}

Pair * nextMap(HashMap * map) 
{
  for(int i=map->current+1;i<map->capacity;i++)
  {
    if(map->buckets[i] != NULL)
    {
      if(map->buckets[i]->key != NULL)
      {
        map->current=i;
        return map->buckets[i];
      }
    }
  }
  return NULL;
}