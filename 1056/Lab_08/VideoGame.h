#pragma once

typedef struct {
    unsigned int gameID;
    char* title;
    char* studio;
    int releaseYear;
} VideoGame;

void printGame(const VideoGame* game);
