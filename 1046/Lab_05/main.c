#include <stdio.h>
#include "Movie.h"
#include "DoublyLinkedList.h"

int main() {

    Node* list = NULL;
    int count = loadMovies("movies.csv", &list, NULL);

    if (count < 0) {
        printf("Failed to load movies.\n");
        return 1;
    }

    printf("Loaded %d movies.\n", count);

    return 0;
}
