#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SIZE 5

char keyTable[SIZE][SIZE] = {
    {'M', 'F', 'H', 'I', 'J'},
    {'K', 'U', 'N', 'O', 'P'},
    {'Q', 'Z', 'V', 'W', 'X'},
    {'Y', 'E', 'L', 'A', 'R'},
    {'G', 'D', 'S', 'T', 'B'}
};

void findPosition(char keyTable[SIZE][SIZE], char c, int *row, int *col) {
    if (c == 'J') c = 'I'; 
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (keyTable[i][j] == c) {
                *row = i;
                *col = j;
                return;
            }
        }
    }
}

void encryptPair(char keyTable[SIZE][SIZE], char in1, char in2, char *out1, char *out2) {
    int row1, col1, row2, col2;
    findPosition(keyTable, in1, &row1, &col1);
    findPosition(keyTable, in2, &row2, &col2);
    if (row1 == row2) {
        *out1 = keyTable[row1][(col1 + 1) % SIZE];
        *out2 = keyTable[row2][(col2 + 1) % SIZE];
    } else if (col1 == col2) {
        *out1 = keyTable[(row1 + 1) % SIZE][col1];
        *out2 = keyTable[(row2 + 1) % SIZE][col2];
    } else {
        *out1 = keyTable[row1][col2];
        *out2 = keyTable[row2][col1];
    }
}

void encryptMessage(char *plaintext, char *ciphertext) {
    int len = strlen(plaintext);
    for (int i = 0; i < len; i += 2) {
        char a = toupper(plaintext[i]);
        char b = (i + 1 < len) ? toupper(plaintext[i + 1]) : 'X';
        if (a == b) b = 'X';
        encryptPair(keyTable, a, b, &ciphertext[i], &ciphertext[i + 1]);
    }
    ciphertext[len] = '\0';
}

int main() {
    char plaintext[] = "Must see you over Cadogan West. Coming at once.";
    char preparedText[256], ciphertext[256];

    
    
    int j = 0;
    for (int i = 0; plaintext[i] != '\0'; i++) {
        if (isalpha(plaintext[i])) {
            preparedText[j++] = toupper(plaintext[i]);
        }
    }
    if (j % 2 != 0) {
        preparedText[j++] = 'X';
    }
    preparedText[j] = '\0';

    encryptMessage(preparedText, ciphertext);

    printf("Plaintext: %s\n", plaintext);
    printf("Ciphertext: %s\n", ciphertext);

    return 0;
}
