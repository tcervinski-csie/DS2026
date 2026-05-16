#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BST.h"

void insert(TreeNode** root, VideoGame* game) {
    if (*root == NULL) {
        TreeNode* newNode = malloc(sizeof(TreeNode));
        newNode->data = game;
        newNode->left = NULL;
        newNode->right = NULL;
        *root = newNode;
    }
    else if (game->gameID < (*root)->data->gameID) {
        insert(&(*root)->left, game);
    }
    else {
        insert(&(*root)->right, game);
    }
}

TreeNode* search(TreeNode* root, unsigned int gameID) {
    if (root == NULL) {
        return NULL;
    }

    if (gameID == root->data->gameID) {
        return root;
    }
    else if (gameID < root->data->gameID) {
        return search(root->left, gameID);
    }
    else {
        return search(root->right, gameID);
    }
}

TreeNode* findMin(TreeNode* root) {
    if (root == NULL) return NULL;
    if (root->left == NULL) return root;
    return findMin(root->left);
}

TreeNode* findMax(TreeNode* root) {
    if (root == NULL) return NULL;
    if (root->right == NULL) return root;
    return findMax(root->right);
}

int height(TreeNode* root) {
    if (root == NULL) return 0;

    int leftHeight = height(root->left);
    int rightHeight = height(root->right);

    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

int countNodes(TreeNode* root) {
    if (root == NULL) return 0;
    return 1 + countNodes(root->left) + countNodes(root->right);
}

void deleteNode(TreeNode** root, unsigned int gameID) {
    if (*root == NULL) {
        printf("Game with ID %u not found.\n", gameID);
        return;
    }

    if (gameID < (*root)->data->gameID) {
        deleteNode(&(*root)->left, gameID);
    }
    else if (gameID > (*root)->data->gameID) {
        deleteNode(&(*root)->right, gameID);
    }
    else {
        // Case 1: leaf node
        if ((*root)->left == NULL && (*root)->right == NULL) {
            free((*root)->data->title);
            free((*root)->data->studio);
            free((*root)->data);
            free(*root);
            *root = NULL;
        }
        // Case 2a: only right child
        else if ((*root)->left == NULL) {
            TreeNode* toDelete = *root;
            *root = (*root)->right;
            free(toDelete->data->title);
            free(toDelete->data->studio);
            free(toDelete->data);
            free(toDelete);
        }
        // Case 2b: only left child
        else if ((*root)->right == NULL) {
            TreeNode* toDelete = *root;
            *root = (*root)->left;
            free(toDelete->data->title);
            free(toDelete->data->studio);
            free(toDelete->data);
            free(toDelete);
        }
        // Case 3: two children — replace with inorder successor (min of right subtree)
        else {
            TreeNode* successor = findMin((*root)->right);

            // Deep copy successor data into current node
            free((*root)->data->title);
            free((*root)->data->studio);
            (*root)->data->gameID = successor->data->gameID;
            (*root)->data->releaseYear = successor->data->releaseYear;
            (*root)->data->title = malloc(strlen(successor->data->title) + 1);
            strcpy((*root)->data->title, successor->data->title);
            (*root)->data->studio = malloc(strlen(successor->data->studio) + 1);
            strcpy((*root)->data->studio, successor->data->studio);

            // Delete the successor node — it still has its own data, deleteNode will free it
            deleteNode(&(*root)->right, successor->data->gameID);
        }
    }
}

void inorder(TreeNode* root) {
    if (root) {
        inorder(root->left);
        printGame(root->data);
        inorder(root->right);
    }
}

void preorder(TreeNode* root) {
    if (root) {
        printGame(root->data);
        preorder(root->left);
        preorder(root->right);
    }
}

void postorder(TreeNode* root) {
    if (root) {
        postorder(root->left);
        postorder(root->right);
        printGame(root->data);
    }
}

void printTree(TreeNode* root, int space) {
    if (root) {
        space += 6;

        printTree(root->right, space);

        printf("\n");
        for (int i = 6; i < space; i++) {
            printf(" ");
        }
        printf("[%u]", root->data->gameID);

        printTree(root->left, space);
    }
}

void freeTree(TreeNode* root) {
    if (root) {
        freeTree(root->left);
        freeTree(root->right);
        free(root->data->title);
        free(root->data->studio);
        free(root->data);
        free(root);
    }
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
