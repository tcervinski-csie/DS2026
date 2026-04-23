#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BST.h"

void insert(TreeNode** root, VideoGame* game) {

}

TreeNode* search(TreeNode* root, unsigned int gameID) {
    return NULL;
}

TreeNode* findMin(TreeNode* root) {
    return NULL;
}

TreeNode* findMax(TreeNode* root) {
    return NULL;
}

int height(TreeNode* root) {
    return 0;
}

int countNodes(TreeNode* root) {
    return 0;
}

void deleteNode(TreeNode** root, unsigned int gameID) {

}

void inorder(TreeNode* root) {

}

void preorder(TreeNode* root) {

}

void postorder(TreeNode* root) {

}

void printTree(TreeNode* root, int space) {

}

void freeTree(TreeNode* root) {

}

int loadGames(const char* filename, TreeNode** root) {
    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        printf("Error: could not open file '%s'\n", filename);
        return -1;
    }

    char line[256];
    int count = 0;

    while (fgets(line, sizeof(line), f) != NULL) {
        VideoGame* game = malloc(sizeof(VideoGame));

        char* token = strtok(line, ",");
        game->gameID = (unsigned int)atoi(token);

        token = strtok(NULL, ",");
        game->title = malloc(strlen(token) + 1);
        strcpy(game->title, token);

        token = strtok(NULL, ",");
        game->studio = malloc(strlen(token) + 1);
        strcpy(game->studio, token);

        token = strtok(NULL, ",");
        token[strcspn(token, "\n")] = '\0';
        game->releaseYear = atoi(token);

        insert(root, game);
        count++;
    }

    fclose(f);
    return count;
}
