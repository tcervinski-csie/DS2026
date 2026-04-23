#pragma once

#include "FootballTeam.h"

typedef struct Node {
    FootballTeam* data;
    struct Node* next;
} Node;

typedef struct {
    int size;
    Node** buckets;
} HashTable;

int hash(int size, const char* key);
HashTable initHashTable(int size);
void insert(HashTable* ht, FootballTeam* team);
FootballTeam* search(HashTable* ht, const char* country);
void deleteByCountry(HashTable* ht, const char* country);
void printHashTable(HashTable* ht);
void freeHashTable(HashTable* ht);
int loadTeams(const char* filename, HashTable* ht);
