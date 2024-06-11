#include <stdio.h>
#include <stdlib.h>

#define N 3 // Matrix size (for 3x3 Hill cipher)

// Function to find the modular inverse of a number
int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) return x;
    }
    return -1;
}

// Function to calculate the determinant of a 3x3 matrix
int determinant(int matrix[N][N], int n) {
    int det = 0;
    if (n == 1) return matrix[0][0];
    if (n == 2) return (matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]);

    int temp[N][N]; // To store cofactors
    int sign = 1; // To store sign multiplier

    for (int f = 0; f < n; f++) {
        // Getting Cofactor of matrix[0][f]
        int subi = 0;
        for (int i = 1; i < n; i++) {
            int subj = 0;
            for (int j = 0; j < n; j++) {
                if (j == f) continue;
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

// Function to find the adjoint of a 3x3 matrix
void adjoint(int A[N][N], int adj[N][N]) {
    if (N == 1) {
        adj[0][0] = 1;
        return;
    }

    int sign = 1, temp[N][N];

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int subi = 0;
            for (int k = 0; k < N; k++) {
                if (k == i) continue;
                int subj = 0;
                for (int l = 0; l < N; l++) {
                    if (l == j) continue;
                    temp[subi][subj] = A[k][l];
                    subj++;
                }
                subi++;
            }
            sign = ((i + j) % 2 == 0) ? 1 : -1;
            adj[j][i] = (sign) * (determinant(temp, N - 1));
        }
    }
}

// Function to find the inverse of a matrix modulo m
int inverseMatrix(int A[N][N], int inverse[N][N], int m) {
    int det = determinant(A, N);
    int invDet = modInverse(det, m);
    if (invDet == -1) return 0; // No inverse exists

    int adj[N][N];
    adjoint(A, adj);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            inverse[i][j] = (adj[i][j] * invDet) % m;
            if (inverse[i][j] < 0) inverse[i][j] += m;
        }
    }
    return 1;
}

// Function to perform matrix multiplication
void multiplyMatrices(int a[N][N], int b[N], int res[N], int m) {
    for (int i = 0; i < N; i++) {
        res[i] = 0;
        for (int j = 0; j < N; j++) {
            res[i] += a[i][j] * b[j];
        }
        res[i] %= m;
    }
}

// Function to decrypt ciphertext using Hill cipher
void decrypt(int ciphertext[N], int key[N][N], int plaintext[N], int m) {
    int inverseKey[N][N];
    if (!inverseMatrix(key, inverseKey, m)) {
        printf("Key matrix is not invertible\n");
        return;
    }
    multiplyMatrices(inverseKey, ciphertext, plaintext, m);
}

int main() {
    int key[N][N] = {
        {6, 24, 1},
        {13, 16, 10},
        {20, 17, 15}
    };

    int plaintext[N][N] = {
        {19, 7, 17},  // 's', 'h', 'r'
        {4, 3, 19},   // 'e', 'd', 's'
        {13, 4, 19}   // 'n', 'e', 't'
    };

    int ciphertext[N][N] = {
        {0, 9, 4},    // Example ciphertext for known plaintext
        {9, 5, 19},
        {4, 4, 5}
    };

    // Decrypt each block of ciphertext
    for (int i = 0; i < N; i++) {
        int decrypted[N];
        decrypt(ciphertext[i], key, decrypted, 26);

        printf("Decrypted block %d: ", i + 1);
        for (int j = 0; j < N; j++) {
            printf("%c ", decrypted[j] + 'A');
        }
        printf("\n");
    }

    return 0;
}
