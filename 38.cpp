#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MATRIX_SIZE 3 
#define ALPHABET_SIZE 26
int determinant(int matrix[MATRIX_SIZE][MATRIX_SIZE], int n) {
    int det = 0;
    if (n == 1)
        return matrix[0][0];

    int temp[MATRIX_SIZE][MATRIX_SIZE];
    int sign = 1;
    for (int f = 0; f < n; f++) {
        int subi = 0;
        for (int i = 1; i < n; i++) {
            int subj = 0;
            for (int j = 0; j < n; j++) {
                if (j == f)
                    continue;
                temp[subi][subj] = matrix[i][j];
                subj++;
            }
            subi++;
        }
        det += sign * matrix[0][f] * determinant(temp, n - 1);
        sign = -sign;
    }
    return det;
}
int mod_inverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1)
            return x;
    }
    return -1;
}
void adjoint(int matrix[MATRIX_SIZE][MATRIX_SIZE], int adj[MATRIX_SIZE][MATRIX_SIZE]) {
    if (MATRIX_SIZE == 1) {
        adj[0][0] = 1;
        return;
    }
    int sign = 1, temp[MATRIX_SIZE][MATRIX_SIZE];
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            int subi = 0;
            for (int ii = 0; ii < MATRIX_SIZE; ii++) {
                if (ii == i)
                    continue;
                int subj = 0;
                for (int jj = 0; jj < MATRIX_SIZE; jj++) {
                    if (jj == j)
                        continue;
                    temp[subi][subj] = matrix[ii][jj];
                    subj++;
                }
                subi++;
            }
            sign = ((i + j) % 2 == 0) ? 1 : -1;
            adj[j][i] = (sign * determinant(temp, MATRIX_SIZE - 1)) % ALPHABET_SIZE;
        }
    }
}
int inverse(int matrix[MATRIX_SIZE][MATRIX_SIZE], int inv[MATRIX_SIZE][MATRIX_SIZE]) {
    int det = determinant(matrix, MATRIX_SIZE);
    if (det == 0) {
        printf("Matrix is singular and cannot have an inverse\n");
        return 0;
    }
    int det_inv = mod_inverse(det, ALPHABET_SIZE);
    if (det_inv == -1) {
        printf("Modular inverse of determinant doesn't exist\n");
        return 0;
    }
    int adj[MATRIX_SIZE][MATRIX_SIZE];
    adjoint(matrix, adj);
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            inv[i][j] = (adj[i][j] * det_inv) % ALPHABET_SIZE;
            if (inv[i][j] < 0)
                inv[i][j] += ALPHABET_SIZE;
        }
    }
    return 1;
}
void matrix_multiply(int a[MATRIX_SIZE][MATRIX_SIZE], int b[MATRIX_SIZE], int res[MATRIX_SIZE]) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        res[i] = 0;
        for (int j = 0; j < MATRIX_SIZE; j++) {
            res[i] += a[i][j] * b[j];
        }
        res[i] %= ALPHABET_SIZE;
    }
}
void encrypt_block(int plaintext[MATRIX_SIZE], int key[MATRIX_SIZE][MATRIX_SIZE], int ciphertext[MATRIX_SIZE]) {
    matrix_multiply(key, plaintext, ciphertext);
}
void decrypt_block(int ciphertext[MATRIX_SIZE], int key_inv[MATRIX_SIZE][MATRIX_SIZE], int plaintext[MATRIX_SIZE]) {
    matrix_multiply(key_inv, ciphertext, plaintext);
}
void known_plaintext_attack(int plaintext[MATRIX_SIZE][MATRIX_SIZE], int ciphertext[MATRIX_SIZE][MATRIX_SIZE], int key[MATRIX_SIZE][MATRIX_SIZE]) {
    int plaintext_inv[MATRIX_SIZE][MATRIX_SIZE];
    if (inverse(plaintext, plaintext_inv)) {
        matrix_multiply(plaintext_inv, (int *)ciphertext, (int *)key);
        printf("Recovered key matrix:\n");
        for (int i = 0; i < MATRIX_SIZE; i++) {
            for (int j = 0; j < MATRIX_SIZE; j++) {
                printf("%d ", key[i][j]);
            }
            printf("\n");
        }
    }
}
int main() {
    int key[MATRIX_SIZE][MATRIX_SIZE] = {
        {6, 24, 1},
        {13, 16, 10},
        {20, 17, 15}
    };
    int plaintext[MATRIX_SIZE] = {7, 4, 19}; 
    int ciphertext[MATRIX_SIZE];
    printf("Original plaintext: ");
    for (int i = 0; i < MATRIX_SIZE; i++) {
        printf("%d ", plaintext[i]);
    }
    printf("\n");
    encrypt_block(plaintext, key, ciphertext);
    printf("Ciphertext: ");
    for (int i = 0; i < MATRIX_SIZE; i++) {
        printf("%d ", ciphertext[i]);
    }
    printf("\n");
    int key_inv[MATRIX_SIZE][MATRIX_SIZE];
    if (inverse(key, key_inv)) {
        int decryptedtext[MATRIX_SIZE];
        decrypt_block(ciphertext, key_inv, decryptedtext);
        printf("Decrypted text: ");
        for (int i = 0; i < MATRIX_SIZE; i++) {
            printf("%d ", decryptedtext[i]);
        }
        printf("\n");
    }
    int known_plaintext[MATRIX_SIZE][MATRIX_SIZE] = {
        {7, 4, 19},
        {0, 13, 19},
        {8, 19, 0}
    };
    int known_ciphertext[MATRIX_SIZE][MATRIX_SIZE] = {
        {11, 22, 0},
        {6, 24, 1},
        {4, 19, 10}
    };
    known_plaintext_attack(known_plaintext, known_ciphertext, key);
    return 0;
}
