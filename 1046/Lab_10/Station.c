#include <stdio.h>
#include "Station.h"

void printStation(const Station* station) {
    printf("[%d] %s (Line %s)\n", station->id, station->name, station->line);
}
