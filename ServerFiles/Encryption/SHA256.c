#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

uint32_t rightRotate(const uint32_t value, const int count) {
    return (value >> count) | (value << (32 - count));
}

uint32_t binaryArrayToUint32(const int *binaryArray) {
    uint32_t value = 0;
    for (int i = 0; i < 32; i++) {
        value = (value << 1) | binaryArray[i];
    }
    return value;
}

void padMessage(const char *input, uint8_t **paddedMessage, size_t *paddedLength) {
    const size_t inputLength = strlen(input);
    const size_t bitLength = inputLength * 8;

    // Calculate the padded message length (in bytes)
    const size_t paddingLength = 64 - ((inputLength + 9) % 64);
    *paddedLength = inputLength + paddingLength + 9; // Original + Padding + Length field

    *paddedMessage = calloc(*paddedLength, sizeof(uint8_t));
    if (*paddedMessage == NULL) {
        perror("Failed to allocate padded message");
        exit(EXIT_FAILURE);
    }

    // Copy the original message
    memcpy(*paddedMessage, input, inputLength);

    // Append the 1-bit
    (*paddedMessage)[inputLength] = 0x80;

    // Append the original message length in bits (big-endian)
    for (int i = 0; i < 8; i++) {
        (*paddedMessage)[*paddedLength - 1 - i] = (bitLength >> (i * 8)) & 0xFF;
    }
}

void processMessage(const uint8_t *message, const size_t length, uint32_t *hash) {
    const uint32_t k[64] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
    };

    // Initial hash values
    uint32_t h[8] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };

    const size_t numBlocks = length / 64;

    for (size_t i = 0; i < numBlocks; i++) {
        const uint8_t *block = message + (i * 64);

        uint32_t w[64];
        memset(w, 0, sizeof(w));

        for (int j = 0; j < 16; j++) {
            w[j] = (block[j * 4] << 24) | (block[j * 4 + 1] << 16) |
                   (block[j * 4 + 2] << 8) | block[j * 4 + 3];
        }

        for (int j = 16; j < 64; j++) {
            uint32_t s0 = rightRotate(w[j - 15], 7) ^ rightRotate(w[j - 15], 18) ^ (w[j - 15] >> 3);
            uint32_t s1 = rightRotate(w[j - 2], 17) ^ rightRotate(w[j - 2], 19) ^ (w[j - 2] >> 10);
            w[j] = w[j - 16] + s0 + w[j - 7] + s1;
        }

        uint32_t a = h[0];
        uint32_t b = h[1];
        uint32_t c = h[2];
        uint32_t d = h[3];
        uint32_t e = h[4];
        uint32_t f = h[5];
        uint32_t g = h[6];
        uint32_t t = h[7];

        for (int j = 0; j < 64; j++) {
            uint32_t S1 = rightRotate(e, 6) ^ rightRotate(e, 11) ^ rightRotate(e, 25);
            uint32_t ch = (e & f) ^ ((~e) & g);
            uint32_t temp1 = t + S1 + ch + k[j] + w[j];
            uint32_t S0 = rightRotate(a, 2) ^ rightRotate(a, 13) ^ rightRotate(a, 22);
            uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
            uint32_t temp2 = S0 + maj;

            t = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;
        }

        h[0] += a;
        h[1] += b;
        h[2] += c;
        h[3] += d;
        h[4] += e;
        h[5] += f;
        h[6] += g;
        h[7] += t;
    }

    memcpy(hash, h, 8 * sizeof(uint32_t));
}

char *hash(const char *input) {
    uint8_t *paddedMessage;
    size_t paddedLength;
    padMessage(input, &paddedMessage, &paddedLength);

    uint32_t hash[8];
    processMessage(paddedMessage, paddedLength, hash);

    free(paddedMessage);

    char *hashString = malloc(65);
    if (hashString == NULL) {
        perror("Failed to allocate hash string");
        exit(EXIT_FAILURE);
    }

    snprintf(hashString, 65, "%08x%08x%08x%08x%08x%08x%08x%08x",
             hash[0], hash[1], hash[2], hash[3], hash[4], hash[5], hash[6], hash[7]);

    return hashString;
}
