#include <stdio.h>
#include "VideoGame.h"
#include "AVL.h"

int main() {

    AVLNode* root = NULL;
    int count = avlLoadGames("games.csv", &root);

    if (count < 0) {
        printf("Failed to load games.\n");
        return 1;
    }

    printf("Loaded %d games.\n\n", count);

    printf("--- AVL tree visual (balance factors shown) ---\n");
    avlPrintTree(root, 0);

    printf("\n\n--- Inorder traversal (sorted by ID) ---\n");
    avlInorder(root);

    printf("\n--- Tree info ---\n");
    printf("Height: %d\n", avlHeight(root));
    printf("Node count: %d\n", avlCountNodes(root));
    printf("Root balance factor: %d\n", avlBalanceFactor(root));

    AVLNode* minNode = avlFindMin(root);
    AVLNode* maxNode = avlFindMax(root);
    printf("Min ID: ");
    printGame(minNode->data);
    printf("Max ID: ");
    printGame(maxNode->data);

    printf("\n--- Search ---\n");
    unsigned int searchID = 105;
    AVLNode* result = avlSearch(root, searchID);
    if (result) {
        printf("Found: ");
        printGame(result->data);
    } else {
        printf("Game with ID %u not found.\n", searchID);
    }

    printf("\n--- Delete game with ID 108 ---\n");
    avlDeleteNode(&root, 108);
    printf("After deletion:\n");
    avlPrintTree(root, 0);
    printf("\n\n");

    avlFreeTree(root);

    return 0;
}
