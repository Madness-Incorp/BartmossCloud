//
// Created by Oisin Lynch on 18/10/2024.
//

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef struct fileNode{
    char* filename;
    struct fileNode * next;
}fnode_t;

void printLinkedList(const fnode_t *head);
const fnode_t* createLinkedList(char* listofFileNames[], int numberOfFiles);

#endif //LINKEDLIST_H


