//
// Created by Oisin Lynch on 19/10/2024.
//

#include "LinkedList.h"  // Include the correct header file

#include <iostream>

// Constructor: Initializes the head of the linked list
LinkedList::LinkedList() : head(nullptr) {}

// Destructor: Calls freeLinkedList to clean up all dynamically allocated nodes
LinkedList::~LinkedList() {
    freeLinkedList();  // Automatically free the linked list when the object is destroyed
}

// Function to free the entire linked list
void LinkedList::freeLinkedList() {
    while (head != nullptr) {
        const fileNode* temp = head;
        head = head->next;
        delete temp;  // Automatically calls the fileNode destructor to free filename
    }
}

// Function to create a linked list from an array of file names
fileNode* LinkedList::createLinkedList(char* listOfFileNames[], const int numberOfNames) {
    if (listOfFileNames == nullptr || numberOfNames == 0) {
        std::cerr << "Cannot create LinkedList from an empty string array." << std::endl;
        return nullptr;
    }

    head = new fileNode;

    // Allocating memory for the first filename
    head->filename = new char[strlen(listOfFileNames[0]) + 1];
    strcpy(head->filename, listOfFileNames[0]);  // Copying the string
    head->next = nullptr;

    fileNode* current = head;

    for (int i = 1; i < numberOfNames; i++) {
        auto* newNode = new fileNode;

        newNode->filename = new char[strlen(listOfFileNames[i]) + 1];
        strcpy(newNode->filename, listOfFileNames[i]);
        newNode->next = nullptr;
        current->next = newNode;
        current = newNode;
    }
    return head;
}

// Function to print the linked list
void LinkedList::printLinkedList() const {
    const fileNode* current = head;
    while (current != nullptr) {
        std::cout << "Filename: " << current->filename << std::endl;
        current = current->next;
    }
}

fileNode* LinkedList::getHead() const {
    return head;
}
