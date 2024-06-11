#include <stdio.h>
#include <string.h>
int mod(int a, int m) {
    int r = a % m;
    return r < 0 ? r + m : r;
}
int determinant(int mat[2][2]) {
    return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
}

int modInverse(int num, int m) {
    num = num % m;
    int x;
    for (x = 1; x < m; x++) {
        if ((num * x) % m == 1) {
            return x;
        }
    }
    return -1;
}

void inverseKey(int key[2][2], int inv_key[2][2], int det_inverse) {
    inv_key[0][0] = key[1][1] * det_inverse % 26;
    inv_key[0][1] = (-key[0][1] * det_inverse + 26) % 26;
    inv_key[1][0] = (-key[1][0] * det_inverse + 26) % 26;
    inv_key[1][1] = key[0][0] * det_inverse % 26;
}

void matrixMultiply(int mat[2][2], int vec[], int result[]) {
    result[0] = (mat[0][0] * vec[0] + mat[0][1] * vec[1]) % 26;
    result[1] = (mat[1][0] * vec[0] + mat[1][1] * vec[1]) % 26;
}

int main() {
    int key[2][2];
    int inv_key[2][2];
    int plaintext[20];
    int ciphertext[20];
    int i, j;

    printf("Enter the key matrix (2x2):\n");
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            scanf("%d", &key[i][j]);
        }
    }

    int det = determinant(key);
    int det_inverse = modInverse(det, 26);
    if (det_inverse == -1) {
        printf("The key matrix is not invertible. Please enter a valid key.\n");
        return 1;
    }

    inverseKey(key, inv_key, det_inverse);
    printf("Inverse Key Matrix: \n");
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            printf("%d ", inv_key[i][j]);
        }
        printf("\n");
    }

    printf("Enter the plaintext: ");
    char msg[21];
    scanf(" %[^\n]s", msg);
    int len = strlen(msg);
    for (i = 0; i < len; i++) {
        plaintext[i] = msg[i] - 'A';
    }

    int encrypted_len = len;
    if (encrypted_len % 2 != 0) {
        plaintext[encrypted_len] = 'X' - 'A';
        encrypted_len++;
    }

    printf("\nEncrypted Cipher Text: ");
    for (i = 0; i < encrypted_len; i += 2) {
        int cipher_block[2];
        matrixMultiply(key, plaintext + i, cipher_block);
        for (j = 0; j < 2; j++) {
            ciphertext[i + j] = cipher_block[j];
            printf("%c", cipher_block[j] + 'A');
        }
    }

   printf("\nDecrypted Cipher Text: ");
    for (i = 0; i < encrypted_len; i += 2) {
        int plain_block[2];
        matrixMultiply(inv_key, ciphertext + i, plain_block);
        for (j = 0; j < 2; j++) {
            int decrypted_char = mod(plain_block[j], 26) + 'A';
            printf("%c", decrypted_char);
        }
    }
    printf("\n");
    return 0;
}
