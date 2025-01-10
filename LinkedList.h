#ifndef LINKEDLIST_H
#define LINKEDLIST_H

struct fileNode {
    char* filename;
    fileNode* next;

    ~fileNode() {
        delete[] filename;
    }
};

class LinkedList {
    fileNode* head;

public:

    LinkedList();

    ~LinkedList();

    void freeLinkedList();

    fileNode* createLinkedList(char* listOfFileNames[], int numberOfNames);

    void printLinkedList() const;

    fileNode* getHead() const;
};

#endif // LINKEDLIST_H

