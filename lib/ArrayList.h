#pragma once
#include "../as/instruction.h"

struct ArrayList {
  int capacity;
  int size;
  struct Instruction *elements;
};

struct ArrayList* newArrayList();
struct ArrayList* fromArray(struct Instruction *arr, int size);
void freeList(struct ArrayList *list);

int size(struct ArrayList *list);
struct Instruction get(struct ArrayList *list, int i);

void addLast(struct ArrayList *list, struct Instruction elem);

