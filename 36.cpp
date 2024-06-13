#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to compute gcd
int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// Function to find modular multiplicative inverse of a under modulo m
int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1;
}

// Function to encrypt the plaintext
void encrypt(char *plaintext, char *ciphertext, int a, int b) {
    int length = strlen(plaintext);
    for (int i = 0; i < length; i++) {
        if (plaintext[i] >= 'A' && plaintext[i] <= 'Z') {
            ciphertext[i] = ((a * (plaintext[i] - 'A') + b) % 26) + 'A';
        } else if (plaintext[i] >= 'a' && plaintext[i] <= 'z') {
            ciphertext[i] = ((a * (plaintext[i] - 'a') + b) % 26) + 'a';
        } else {
            ciphertext[i] = plaintext[i]; // Non-alphabetic characters are not encrypted
        }
    }
    ciphertext[length] = '\0'; // Null-terminate the ciphertext
}

// Function to decrypt the ciphertext
void decrypt(char *ciphertext, char *plaintext, int a, int b) {
    int length = strlen(ciphertext);
    int a_inv = modInverse(a, 26);
    if (a_inv == -1) {
        printf("Error: 'a' has no modular inverse, decryption not possible.\n");
        exit(1);
    }
    for (int i = 0; i < length; i++) {
        if (ciphertext[i] >= 'A' && ciphertext[i] <= 'Z') {
            plaintext[i] = ((a_inv * ((ciphertext[i] - 'A' - b + 26)) % 26) + 'A');
        } else if (ciphertext[i] >= 'a' && ciphertext[i] <= 'z') {
            plaintext[i] = ((a_inv * ((ciphertext[i] - 'a' - b + 26)) % 26) + 'a');
        } else {
            plaintext[i] = ciphertext[i]; // Non-alphabetic characters are not decrypted
        }
    }
    plaintext[length] = '\0'; // Null-terminate the plaintext
}

int main() {
    char plaintext[256];
    char ciphertext[256];
    int a, b;

    printf("Enter the plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    plaintext[strcspn(plaintext, "\n")] = '\0'; // Remove the newline character from the input

    printf("Enter the value of a (must be coprime with 26): ");
    scanf("%d", &a);
    printf("Enter the value of b: ");
    scanf("%d", &b);

    // Check if 'a' is coprime with 26
    if (gcd(a, 26) != 1) {
        printf("Error: 'a' is not coprime with 26. Please choose a different value for 'a'.\n");
        return 1;
    }

    // Encrypt the plaintext
    encrypt(plaintext, ciphertext, a, b);
    printf("Ciphertext: %s\n", ciphertext);

    // Decrypt the ciphertext to verify
    char decrypted[256];
    decrypt(ciphertext, decrypted, a, b);
    printf("Decrypted text: %s\n", decrypted);

    return 0;
}
