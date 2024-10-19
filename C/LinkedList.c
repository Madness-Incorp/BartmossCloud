//
// Created by Oisin Lynch on 18/10/2024.
//

#include "LinkedList.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


void freeLinkedList(fnode_t *head) {
    while (head != NULL) {
        fnode_t *temp = head;
        head = head->next;
        free(temp->filename);
        free(temp);
    }
};

const fnode_t* createLinkedList(char* listofFileNames[], const int numberOfFiles) {

    if(listofFileNames == NULL || numberOfFiles < 1) {
        fprintf(stderr, "Cannot create a linked list from nothing\n");
        return NULL;
    }

    fnode_t *head = NULL;
    head = (fnode_t*)malloc(sizeof(fnode_t));
    if(head == NULL) {
        perror("Error creating a node");
        free(head);
        return NULL;
    }

    head->filename = listofFileNames[0];
    if(listofFileNames[0] == NULL) {
        perror("First element is NULL");
        free(head->filename);
        free(head);
        return NULL;
    }
    head->next = NULL;

    fnode_t *current = head;

    for(int i = 1; i < numberOfFiles; i++) {
        fnode_t *newNode = malloc(sizeof(fnode_t));
        if(newNode == NULL) {
            perror("Error creating a new Node");
            freeLinkedList(newNode);
            return NULL;
        }

        if(listofFileNames[i] == NULL) {
            fprintf(stderr, "File Name Null");
            free(newNode);
            freeLinkedList(head);
            return NULL;
        }

        newNode->filename = listofFileNames[i];
        newNode->next = NULL;
        current->next = newNode;
        current = newNode;
    }

    return head;

}

void printLinkedList(const fnode_t *head) {

    const fnode_t *current = head;
    const fnode_t *next = current->next;

    printf("Filename: %s\n", head->filename);

    current = next;
    next = current->next;

    while(1) {
        if(current->next == NULL) {
            printf("Filename: %s\n", current->filename);
            break;
        }
        printf("Filename: %s\n", current->filename);
        current = next;
        next = current->next;
    }

}