#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define SIZE 5

void generateKeyTable(char key[], char keyTable[SIZE][SIZE]) {
    int used[26] = {0}, i, j, index = 0;
    for (i = 0; i < strlen(key); i++) {
        if (key[i] != 'J' && !used[key[i] - 'A']) {
            keyTable[index / SIZE][index % SIZE] = key[i];
            used[key[i] - 'A'] = 1;
            index++;
        }
    }
    for (i = 0; i < 26; i++) {
        if ((i + 'A') != 'J' && !used[i]) {
            keyTable[index / SIZE][index % SIZE] = i + 'A';
            index++;
        }
    }
}

void findPosition(char keyTable[SIZE][SIZE], char c, int *row, int *col) {
    int i, j;
    if (c == 'J') c = 'I';
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            if (keyTable[i][j] == c) {
                *row = i;
                *col = j;
                return;
            }
        }
    }
}

void decryptPair(char keyTable[SIZE][SIZE], char in1, char in2, char *out1, char *out2) {
    int row1, col1, row2, col2;
    findPosition(keyTable, in1, &row1, &col1);
    findPosition(keyTable, in2, &row2, &col2);
    if (row1 == row2) {
        *out1 = keyTable[row1][(col1 + SIZE - 1) % SIZE];
        *out2 = keyTable[row2][(col2 + SIZE - 1) % SIZE];
    } else if (col1 == col2) {
        *out1 = keyTable[(row1 + SIZE - 1) % SIZE][col1];
        *out2 = keyTable[(row2 + SIZE - 1) % SIZE][col2];
    } else {
        *out1 = keyTable[row1][col2];
        *out2 = keyTable[row2][col1];
    }
}

void decryptMessage(char keyTable[SIZE][SIZE], char *ciphertext, char *plaintext) {
    int len = strlen(ciphertext);
    for (int i = 0; i < len; i += 2) {
        decryptPair(keyTable, ciphertext[i], ciphertext[i + 1], &plaintext[i], &plaintext[i + 1]);
    }
    plaintext[len] = '\0';
}

int main() {
    char key[] = "MONARCHY";
    char keyTable[SIZE][SIZE];
    char ciphertext[] = "KXJEYUREBEZWEHEWRYTUHEYFSKREHEGOYFIWTTTUOLKSYCAJPO???????NTXBYBNTGONEYCUZWRGDSONSXBOUYWRHEBAAHYUSEDQ";
    char plaintext[sizeof(ciphertext)];

    generateKeyTable(key, keyTable);
    decryptMessage(keyTable, ciphertext, plaintext);

    printf("Ciphertext: %s\n", ciphertext);
    printf("Plaintext: %s\n", plaintext);

    return 0;
}
