#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
int char_to_num(char c) {
    return toupper(c) - 'A';
}

char num_to_char(int n) {
    return n + 'A';
}

void encrypt(char* plaintext, int* key, int len, char* ciphertext) {
    for (int i = 0; i < len; i++) {
        if (isalpha(plaintext[i])) {
            int p = char_to_num(plaintext[i]);
            int c = (p + key[i]) % 26;
            ciphertext[i] = num_to_char(c);
        } else {
            ciphertext[i] = plaintext[i]; 
        }
    }
    ciphertext[len] = '\0';
}

void find_key(char* ciphertext, char* desired_plaintext, int* key, int len) {
    for (int i = 0; i < len; i++) {
        if (isalpha(ciphertext[i]) && isalpha(desired_plaintext[i])) {
            int c = char_to_num(ciphertext[i]);
            int p = char_to_num(desired_plaintext[i]);
            key[i] = (c - p + 26) % 26;
        } else {
            key[i] = 0; 
        }
    }
}

int main() {

    char plaintext[] = "send more money";
    int key[] = {9, 0, 17, 23, 15, 21, 14, 11, 11, 2, 8, 9};
    int len = strlen(plaintext);

    for (int i = 0; i < len; i++) {
        if (isalpha(plaintext[i])) {
            plaintext[i] = toupper(plaintext[i]);
        }
    }

    char ciphertext[len + 1];
    encrypt(plaintext, key, len, ciphertext);
    printf("Ciphertext: %s\n", ciphertext);

    char desired_plaintext[] = "cash not needed";
    int key_for_decryption[len];

    find_key(ciphertext, desired_plaintext, key_for_decryption, len);

    printf("Key for decryption: ");
    for (int i = 0; i < len; i++) {
        if (isalpha(desired_plaintext[i])) {
            printf("%d ", key_for_decryption[i]);
        }
    }
    printf("\n");

    return 0;
}

