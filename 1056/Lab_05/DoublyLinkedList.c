#include <stdio.h>
#include <stdlib.h>
#include "DoublyLinkedList.h"

void insertBeginning(List* list, Movie* movie) {
    Node* newNode = malloc(sizeof(Node));
    newNode->data = movie;
    newNode->prev = NULL;
    newNode->next = list->head;

    if (list->head) {
        list->head->prev = newNode;
    }
    else {
        list->tail = newNode;
    }

    list->head = newNode;
}

void insertEnd(List* list, Movie* movie) {
    Node* newNode = malloc(sizeof(Node));
    newNode->data = movie;
    newNode->next = NULL;
    newNode->prev = list->tail;

    if (list->tail) {
        list->tail->next = newNode;
    }
    else {
        list->head = newNode;
    }

    list->tail = newNode;
}

void deleteBeginning(List* list) {
    if (list->head) {
        Node* toDelete = list->head;
        list->head = list->head->next;

        if (list->head) {
            list->head->prev = NULL;
        }
        else {
            list->tail = NULL;
        }

        free(toDelete->data->title);
        free(toDelete->data);
        free(toDelete);
    }
    else {
        printf("List is empty\n");
    }
}

void deleteEnd(List* list) {
    if (list->tail) {
        Node* toDelete = list->tail;
        list->tail = list->tail->prev;

        if (list->tail) {
            list->tail->next = NULL;
        }
        else {
            list->head = NULL;
        }

        free(toDelete->data->title);
        free(toDelete->data);
        free(toDelete);
    }
    else {
        printf("List is empty\n");
    }
}

void freeNode(Node* node) {

    free(node->data->title);
    free(node->data);
    free(node);

}


void parseList(List* list) {
    if (list->head) {
        Node* current = list->head;
        while (current) {
            printMovie(current->data);
            current = current->next;
        }
    }
    else {
        printf("List is empty\n");
    }
}

void freeList(List* list) {
    Node* current = list->head;
    while (current) {
        Node* next = current->next;
        free(current->data->title);
        free(current->data);
        free(current);
        current = next;
    }
    list->head = NULL;
    list->tail = NULL;
}

void deleteByCondition(List* list, int (*conditionFn)(Movie*)) {

    if (list->head) {
       
        Node* current = list->head;

        while (current) {

            if (conditionFn(current->data)) {

                Node* toDelete = current;
                current = current->next;

                if (toDelete->prev && toDelete->next) {

                    toDelete->next->prev = toDelete->prev;
                    toDelete->prev->next = toDelete->next;

                    freeNode(toDelete);

                }
                else if (toDelete->prev) {

                    list->tail = toDelete->prev;
                    list->tail->next = NULL;
                    
                    freeNode(toDelete);

                }
                else if (toDelete->next) {

                    list->head = toDelete->next;
                    list->head->prev = NULL;

                    freeNode(toDelete);

                }
                else {

                    list->head = NULL;
                    list->tail = NULL;

                    freeNode(toDelete);

                }
            }
            else {
                current = current->next;
            }
        }

    }
    else {
        printf("List is empty\n");
    }
}
