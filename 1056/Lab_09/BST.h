#pragma once

#include "VideoGame.h"

typedef struct TreeNode {
    VideoGame* data;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

// Insertion & loading
void insert(TreeNode** root, VideoGame* game);
int loadGames(const char* filename, TreeNode** root);

// Traversals
void inorder(TreeNode* root);
void preorder(TreeNode* root);
void postorder(TreeNode* root);

// Search & info
TreeNode* search(TreeNode* root, unsigned int gameID);
TreeNode* findMin(TreeNode* root);
TreeNode* findMax(TreeNode* root);
int height(TreeNode* root);
int countNodes(TreeNode* root);

// Deletion
void deleteNode(TreeNode** root, unsigned int gameID);

// Display & cleanup
void printTree(TreeNode* root, int space);
void freeTree(TreeNode* root);
