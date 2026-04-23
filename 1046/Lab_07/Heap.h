#pragma once

#include "Task.h"

typedef struct {
    Task* tasks;
    int size;
} Heap;

Heap initHeap();
void insertHeap(Heap* heap, Task task);
Task deleteFromHeap(Heap* heap);
void printHeap(Heap* heap);
Heap buildHeap(Task* tasks, int size);
void freeHeap(Heap* heap);
int loadTasks(const char* filename, Heap* heap);
