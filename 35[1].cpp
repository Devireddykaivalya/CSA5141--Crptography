#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Function to generate a random key
void generateKey(int *key, int length) {
    for (int i = 0; i < length; i++) {
        key[i] = rand() % 26; // Random number between 0 and 25
    }
}

// Function to encrypt the plaintext
void encrypt(char *plaintext, int *key, char *ciphertext) {
    int length = strlen(plaintext);
    for (int i = 0; i < length; i++) {
        if (plaintext[i] >= 'A' && plaintext[i] <= 'Z') {
            ciphertext[i] = ((plaintext[i] - 'A' + key[i]) % 26) + 'A';
        } else if (plaintext[i] >= 'a' && plaintext[i] <= 'z') {
            ciphertext[i] = ((plaintext[i] - 'a' + key[i]) % 26) + 'a';
        } else {
            ciphertext[i] = plaintext[i]; // Non-alphabetic characters are not encrypted
        }
    }
    ciphertext[length] = '\0'; // Null-terminate the ciphertext
}

// Function to decrypt the ciphertext
void decrypt(char *ciphertext, int *key, char *plaintext) {
    int length = strlen(ciphertext);
    for (int i = 0; i < length; i++) {
        if (ciphertext[i] >= 'A' && ciphertext[i] <= 'Z') {
            plaintext[i] = ((ciphertext[i] - 'A' - key[i] + 26) % 26) + 'A';
        } else if (ciphertext[i] >= 'a' && ciphertext[i] <= 'z') {
            plaintext[i] = ((ciphertext[i] - 'a' - key[i] + 26) % 26) + 'a';
        } else {
            plaintext[i] = ciphertext[i]; // Non-alphabetic characters are not decrypted
        }
    }
    plaintext[length] = '\0'; // Null-terminate the plaintext
}

int main() {
    char plaintext[256];
    char ciphertext[256];
    int key[256];

    printf("Enter the plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);
    plaintext[strcspn(plaintext, "\n")] = '\0'; // Remove the newline character from the input

    int length = strlen(plaintext);

    // Initialize the random number generator
    srand(time(0));

    // Generate the random key
    generateKey(key, length);

    // Encrypt the plaintext
    encrypt(plaintext, key, ciphertext);

    printf("Ciphertext: %s\n", ciphertext);

    // Decrypt the ciphertext to verify
    char decrypted[256];
    decrypt(ciphertext, key, decrypted);

    printf("Decrypted text: %s\n", decrypted);

    return 0;
}
