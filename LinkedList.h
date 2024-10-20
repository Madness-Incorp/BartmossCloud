//
// Created by Oisin Lynch on 18/10/2024.
//

#ifndef LINKEDLIST_H
#define LINKEDLIST_H



// Define the fileNode struct
struct fileNode {
    char* filename;
    fileNode* next;

    // Destructor to free dynamically allocated memory
    ~fileNode() {
        delete[] filename;
    }
};

// Define the LinkedList class
class LinkedList {

    fileNode* head;  // Pointer to the first node in the list

public:
    // Constructor to initialize the list
    LinkedList();

    // Destructor to clean up the list
    ~LinkedList();

    // Function to free the entire linked list
    void freeLinkedList();

    // Function to create a linked list from an array of filenames
    fileNode* createLinkedList(char* listOfFileNames[], int numberOfNames);

    // Function to print the linked list
    void printLinkedList() const;
};

#endif // LINKEDLIST_H

