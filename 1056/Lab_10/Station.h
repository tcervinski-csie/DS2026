#pragma once

typedef struct {
    int id;
    char* name;
    char* line;
} Station;

void printStation(const Station* station);
