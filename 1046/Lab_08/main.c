#include <stdio.h>
#include "VideoGame.h"
#include "BST.h"

int main() {

    TreeNode* root = NULL;
    int count = loadGames("games.csv", &root);

    if (count < 0) {
        printf("Failed to load games.\n");
        return 1;
    }

    printf("Loaded %d games.\n\n", count);

    printf("--- BST visual ---\n");
    printTree(root, 0);

    printf("\n\n--- Inorder traversal (sorted by ID) ---\n");
    inorder(root);

    printf("\n--- Preorder traversal ---\n");
    preorder(root);

    printf("\n--- Postorder traversal ---\n");
    postorder(root);

    printf("\n--- Tree info ---\n");
    printf("Height: %d\n", height(root));
    printf("Node count: %d\n", countNodes(root));

    TreeNode* minNode = findMin(root);
    TreeNode* maxNode = findMax(root);
    printf("Min ID: ");
    printGame(minNode->data);
    printf("Max ID: ");
    printGame(maxNode->data);

    printf("\n--- Search ---\n");
    unsigned int searchID = 105;
    TreeNode* result = search(root, searchID);
    if (result) {
        printf("Found: ");
        printGame(result->data);
    }
    else {
        printf("Game with ID %u not found.\n", searchID);
    }

    printf("\n--- Delete game with ID 108 (root) ---\n");
    deleteNode(&root, 108);
    printf("After deletion:\n");
    printTree(root, 0);
    printf("\n");
    inorder(root);

    freeTree(root);

    return 0;
}
