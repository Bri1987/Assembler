#include "ArrayList.h"

#include <stdlib.h>
#include <assert.h>


int size(struct ArrayList *list) {
  return list->size;
}

void resize(struct ArrayList *list) {
  int newcap = 2 * list->capacity;
  struct Instruction *newElements = malloc(newcap * sizeof(struct Instruction));
  for (int i=0; i<list->capacity; i++) {
    newElements[i] = list->elements[i]; 
  }
  list->capacity = newcap;
  free(list->elements);
  list->elements = newElements;
}

void addLast(struct ArrayList *list, struct Instruction elem) {
  if (list->size == list->capacity) {
    resize(list);
  }
  list->elements[list->size] = elem;
  list->size += 1;
}

struct Instruction get(struct ArrayList *list, int i) {
  assert(i < list->size);
  return list->elements[i];
}

struct ArrayList* newArrayList() {
  int startCap = 2;
  struct ArrayList *t = malloc(sizeof(struct ArrayList));   
  t->capacity = startCap;
  t->size = 0;
  t->elements = malloc(startCap * sizeof(struct Instruction));
  return t;
}

void freeList(struct ArrayList *list) {
  free(list->elements);
  free(list);
}

struct ArrayList* fromArray(struct Instruction *arr, int size) {
  struct ArrayList *l = newArrayList();
  for (int i=0; i<size; i++) {
    addLast(l, arr[i]);
  }
  return l;
}
