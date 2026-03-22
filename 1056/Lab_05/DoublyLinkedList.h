#pragma once

#include "Movie.h"

typedef struct Node {
    Movie* data;
    struct Node* prev;
    struct Node* next;
} Node;

typedef void (*InsertListFn)(Node**, Movie*);

int loadMovies(const char* filename, Node** list, InsertListFn insertFn);
