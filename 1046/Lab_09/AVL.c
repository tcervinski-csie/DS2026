#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AVL.h"

/*
 * AVL TREE — SELF-BALANCING BINARY SEARCH TREE
 *
 * An AVL tree is a BST with one extra guarantee: for every node,
 * the heights of its left and right subtrees differ by at most 1.
 * This is called the "balance condition", and it ensures that the
 * tree never degenerates into a linked list (worst case of a plain BST).
 *
 * As a result, search, insert, and delete are all guaranteed O(log n).
 *
 * The balance factor of a node = height(right subtree) - height(left subtree)
 * Valid values: -1, 0, +1
 * If a node reaches -2 or +2 after an insert/delete, we rebalance it
 * using one or two rotations.
 */


/* avlHeight
 * ---------
 * Returns the height of the subtree rooted at `root`.
 * Height = number of edges on the longest path from root to a leaf.
 * An empty tree has height 0.
 *
 * This is computed recursively: the height of a node is 1 plus
 * the maximum of its two children's heights.
 */
int avlHeight(AVLNode* root) {
    if (root == NULL) return 0;
    int leftHeight  = avlHeight(root->left);
    int rightHeight = avlHeight(root->right);
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}


/* avlBalanceFactor
 * ----------------
 * Returns the balance factor of `root`:
 *   balance = height(right) - height(left)
 *
 *  0  → perfectly balanced at this node
 * +1  → right subtree one level taller  (still valid)
 * -1  → left subtree one level taller   (still valid)
 * +2  → right-heavy, needs left rotation
 * -2  → left-heavy, needs right rotation
 */
int avlBalanceFactor(AVLNode* root) {
    if (root == NULL) return 0;
    return avlHeight(root->right) - avlHeight(root->left);
}


/* avlRotateRight
 * --------------
 * Performs a RIGHT rotation around `*root`.
 * Used when the left subtree is too tall (balance factor = -2).
 *
 * Before rotation:          After rotation:
 *
 *      (*root)                   aux
 *      /     \                  /   \
 *    aux       C     -->       A   (*root)
 *   /   \                          /    \
 *  A     B                        B      C
 *
 * The key moves:
 *   1. aux = left child of root
 *   2. root's left child becomes aux's right child (B moves up)
 *   3. aux's right child becomes root (root moves down)
 *   4. The pointer that was pointing at root now points at aux
 */
void avlRotateRight(AVLNode** root) {
    AVLNode* aux = (*root)->left;   // aux is the new root after rotation
    (*root)->left = aux->right;     // B moves: becomes left child of old root
    aux->right = *root;             // old root moves down to become right child of aux
    *root = aux;                    // update the pointer so the parent sees the new root
}


/* avlRotateLeft
 * -------------
 * Performs a LEFT rotation around `*root`.
 * Used when the right subtree is too tall (balance factor = +2).
 *
 * Before rotation:          After rotation:
 *
 *   (*root)                       aux
 *   /     \                      /   \
 *  A       aux       -->    (*root)   C
 *          / \               /   \
 *         B   C             A     B
 *
 * Mirror image of avlRotateRight.
 */
void avlRotateLeft(AVLNode** root) {
    AVLNode* aux = (*root)->right;  // aux is the new root after rotation
    (*root)->right = aux->left;     // B moves: becomes right child of old root
    aux->left = *root;              // old root moves down to become left child of aux
    *root = aux;                    // update the pointer so the parent sees the new root
}


/* rebalance  (static — internal use only)
 * -----------------------------------------
 * Checks the balance factor of `*root` and applies the appropriate
 * rotation(s) if the tree is out of balance.
 *
 * There are four cases:
 *
 * Case 1 — Right-Right (balance = +2, right child balance >= 0):
 *   Single LEFT rotation fixes it.
 *
 * Case 2 — Right-Left (balance = +2, right child balance < 0):
 *   The right child is left-heavy. A single rotation won't work.
 *   First rotate RIGHT on the right child, then LEFT on root.
 *   (Double rotation)
 *
 * Case 3 — Left-Left (balance = -2, left child balance <= 0):
 *   Single RIGHT rotation fixes it.
 *
 * Case 4 — Left-Right (balance = -2, left child balance > 0):
 *   The left child is right-heavy. Double rotation:
 *   First rotate LEFT on the left child, then RIGHT on root.
 */
static void rebalance(AVLNode** root) {
    int balance = avlBalanceFactor(*root);

    if (balance == 2) {
        // Right subtree is too tall
        if (avlBalanceFactor((*root)->right) >= 0) {
            // Right-Right case: single left rotation
            avlRotateLeft(root);
        } else {
            // Right-Left case: right rotate the right child first, then left rotate root
            avlRotateRight(&(*root)->right);
            avlRotateLeft(root);
        }
    }

    if (balance == -2) {
        // Left subtree is too tall
        if (avlBalanceFactor((*root)->left) <= 0) {
            // Left-Left case: single right rotation
            avlRotateRight(root);
        } else {
            // Left-Right case: left rotate the left child first, then right rotate root
            avlRotateLeft(&(*root)->left);
            avlRotateRight(root);
        }
    }
}


/* avlInsert
 * ---------
 * Inserts `game` into the AVL tree rooted at `*root`, maintaining
 * the BST ordering (by gameID) and the AVL balance condition.
 *
 * Strategy:
 *   1. Recurse down exactly like a plain BST insert.
 *   2. On the way BACK UP (after each recursive call returns),
 *      call rebalance() at every ancestor.
 *      This fixes any imbalance introduced by the new node.
 *
 * Duplicates are ignored (equal gameID → do nothing).
 */
void avlInsert(AVLNode** root, VideoGame* game) {
    if (*root == NULL) {
        // Found the correct empty spot — create the node here
        AVLNode* newNode = malloc(sizeof(AVLNode));
        newNode->data  = game;
        newNode->left  = NULL;
        newNode->right = NULL;
        *root = newNode;
    } else if (game->gameID < (*root)->data->gameID) {
        avlInsert(&(*root)->left, game);
        rebalance(root);   // check balance on the way back up
    } else if (game->gameID > (*root)->data->gameID) {
        avlInsert(&(*root)->right, game);
        rebalance(root);   // check balance on the way back up
    }
    // equal gameID: ignore duplicate
}


/* avlSearch
 * ---------
 * Searches for a node with the given gameID.
 * Identical to BST search — AVL's balance guarantee just makes it faster
 * in the worst case (O(log n) always, vs O(n) for a degenerate BST).
 *
 * Returns a pointer to the matching AVLNode, or NULL if not found.
 */
AVLNode* avlSearch(AVLNode* root, unsigned int gameID) {
    if (root == NULL)                    return NULL;   // not found
    if (gameID == root->data->gameID)    return root;   // found
    if (gameID  < root->data->gameID)    return avlSearch(root->left,  gameID);
    return                                      avlSearch(root->right, gameID);
}


/* avlFindMin
 * ----------
 * Returns the node with the smallest gameID (leftmost node in the tree).
 * Used internally by avlDeleteNode to find the inorder successor.
 */
AVLNode* avlFindMin(AVLNode* root) {
    if (root == NULL)        return NULL;
    if (root->left == NULL)  return root;   // no smaller node exists
    return avlFindMin(root->left);
}


/* avlFindMax
 * ----------
 * Returns the node with the largest gameID (rightmost node in the tree).
 */
AVLNode* avlFindMax(AVLNode* root) {
    if (root == NULL)        return NULL;
    if (root->right == NULL) return root;   // no larger node exists
    return avlFindMax(root->right);
}


/* avlCountNodes
 * -------------
 * Counts all nodes in the tree using a simple postorder traversal.
 * Each call returns 1 (itself) + count of left subtree + count of right subtree.
 */
int avlCountNodes(AVLNode* root) {
    if (root == NULL) return 0;
    return 1 + avlCountNodes(root->left) + avlCountNodes(root->right);
}


/* avlDeleteNode
 * -------------
 * Deletes the node with `gameID` from the tree, then rebalances.
 *
 * Three structural cases after finding the node:
 *
 *   Case 1 — Leaf (no children):
 *     Free the node and set the pointer to NULL.
 *
 *   Case 2 — One child:
 *     Bypass the node: set the parent's pointer directly to the single child.
 *     Then free the node.
 *
 *   Case 3 — Two children:
 *     We can't just remove the node because it has two subtrees.
 *     Solution: find the INORDER SUCCESSOR (smallest node in right subtree),
 *     copy its data into the current node, then delete the successor
 *     (which has at most one child, so it's a simpler case).
 *
 * After any deletion, rebalance() is called on the way back up,
 * just like avlInsert.
 */
void avlDeleteNode(AVLNode** root, unsigned int gameID) {
    if (*root == NULL) {
        printf("Game with ID %u not found.\n", gameID);
        return;
    }

    if (gameID < (*root)->data->gameID) {
        avlDeleteNode(&(*root)->left, gameID);
    } else if (gameID > (*root)->data->gameID) {
        avlDeleteNode(&(*root)->right, gameID);
    } else {
        // Found the node to delete
        if ((*root)->left == NULL && (*root)->right == NULL) {
            // Case 1: leaf node — simply free it
            free((*root)->data->title);
            free((*root)->data->studio);
            free((*root)->data);
            free(*root);
            *root = NULL;
            return;
        } else if ((*root)->left == NULL) {
            // Case 2a: only right child — bypass this node
            AVLNode* toDelete = *root;
            *root = (*root)->right;
            free(toDelete->data->title);
            free(toDelete->data->studio);
            free(toDelete->data);
            free(toDelete);
        } else if ((*root)->right == NULL) {
            // Case 2b: only left child — bypass this node
            AVLNode* toDelete = *root;
            *root = (*root)->left;
            free(toDelete->data->title);
            free(toDelete->data->studio);
            free(toDelete->data);
            free(toDelete);
        } else {
            // Case 3: two children — replace with inorder successor
            // The successor is the smallest value in the right subtree.
            // It will always have at most one child (no left child),
            // so deleting it is straightforward.
            AVLNode* successor = avlFindMin((*root)->right);

            // Copy successor's data into the current node
            free((*root)->data->title);
            free((*root)->data->studio);
            (*root)->data->gameID     = successor->data->gameID;
            (*root)->data->releaseYear = successor->data->releaseYear;
            (*root)->data->title  = malloc(strlen(successor->data->title)  + 1);
            (*root)->data->studio = malloc(strlen(successor->data->studio) + 1);
            strcpy((*root)->data->title,  successor->data->title);
            strcpy((*root)->data->studio, successor->data->studio);

            // Now delete the successor from the right subtree
            avlDeleteNode(&(*root)->right, successor->data->gameID);
        }
    }

    // Rebalance on the way back up (only if the node wasn't set to NULL above)
    if (*root) rebalance(root);
}


/* avlInorder
 * ----------
 * Visits nodes in LEFT → ROOT → RIGHT order.
 * Because of the BST property, this produces nodes sorted by gameID ascending.
 */
void avlInorder(AVLNode* root) {
    if (root) {
        avlInorder(root->left);
        printGame(root->data);
        avlInorder(root->right);
    }
}


/* avlPreorder
 * -----------
 * Visits nodes in ROOT → LEFT → RIGHT order.
 * Useful for copying or serializing the tree structure.
 */
void avlPreorder(AVLNode* root) {
    if (root) {
        printGame(root->data);
        avlPreorder(root->left);
        avlPreorder(root->right);
    }
}


/* avlPostorder
 * ------------
 * Visits nodes in LEFT → RIGHT → ROOT order.
 * Useful for deletion (children freed before parent).
 */
void avlPostorder(AVLNode* root) {
    if (root) {
        avlPostorder(root->left);
        avlPostorder(root->right);
        printGame(root->data);
    }
}


/* avlPrintTree
 * ------------
 * Prints a sideways visual of the tree (right subtree at top, left at bottom).
 * Each node shows its gameID and balance factor in brackets.
 * Indentation represents depth — deeper nodes are indented more.
 *
 * Example output (read sideways, right=top):
 *
 *             [112] (bf:0)
 *       [108] (bf:1)
 *             [106] (bf:0)
 * [105] (bf:0)
 *       [102] (bf:0)
 */
void avlPrintTree(AVLNode* root, int space) {
    if (root) {
        space += 6;
        avlPrintTree(root->right, space);   // right subtree printed first (appears at top)
        printf("\n");
        for (int i = 6; i < space; i++) printf(" ");
        printf("[%u] (bf:%d)", root->data->gameID, avlBalanceFactor(root));
        avlPrintTree(root->left, space);    // left subtree printed after (appears at bottom)
    }
}


/* avlFreeTree
 * -----------
 * Frees all memory in the tree using postorder traversal.
 * Postorder is necessary: children must be freed before the parent,
 * otherwise we'd lose the pointers to the children.
 */
void avlFreeTree(AVLNode* root) {
    if (root) {
        avlFreeTree(root->left);
        avlFreeTree(root->right);
        free(root->data->title);
        free(root->data->studio);
        free(root->data);
        free(root);
    }
}


/* avlLoadGames
 * ------------
 * Reads games from a CSV file (one per line: id,title,studio,year)
 * and inserts each one into the AVL tree.
 * Returns the number of games loaded, or -1 on file error.
 */
int avlLoadGames(const char* filename, AVLNode** root) {
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

        avlInsert(root, game);
        count++;
    }

    fclose(f);
    return count;
}
