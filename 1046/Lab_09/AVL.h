#pragma once

#include "VideoGame.h"

typedef struct AVLNode {
    VideoGame* data;
    struct AVLNode* left;
    struct AVLNode* right;
} AVLNode;

// Insertion & loading
void avlInsert(AVLNode** root, VideoGame* game);
int avlLoadGames(const char* filename, AVLNode** root);

// Traversals
void avlInorder(AVLNode* root);
void avlPreorder(AVLNode* root);
void avlPostorder(AVLNode* root);

// Search & info
AVLNode* avlSearch(AVLNode* root, unsigned int gameID);
AVLNode* avlFindMin(AVLNode* root);
AVLNode* avlFindMax(AVLNode* root);
int avlHeight(AVLNode* root);
int avlCountNodes(AVLNode* root);
int avlBalanceFactor(AVLNode* root);

// Rotations
void avlRotateLeft(AVLNode** root);
void avlRotateRight(AVLNode** root);

// Deletion
void avlDeleteNode(AVLNode** root, unsigned int gameID);

// Display & cleanup
void avlPrintTree(AVLNode* root, int space);
void avlFreeTree(AVLNode* root);
