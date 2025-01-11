//
// Created by Oisin Lynch on 04/01/2025.
//

#ifndef SHA256_H
#define SHA256_H

int *convertCharToBinary(char letter);
int **StringToBinary(const char *string);
uint32_t rightRotate(uint32_t value, int count);
char* hash(const char* password);

#endif //SHA256_H
