#include <stdio.h>
#include "VideoGame.h"

void printGame(const VideoGame* game) {
    printf("[%u] %s - %s (%d)\n", game->gameID, game->title, game->studio, game->releaseYear);
}
