#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26
#define MAX_TEXT_SIZE 10000

const double english_freq[ALPHABET_SIZE] = {
    8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966, 0.153,
    0.772, 4.025, 2.406, 6.749, 7.507, 1.929, 0.095, 5.987, 6.327, 9.056,
    2.758, 0.978, 2.361, 0.150, 1.974, 0.074
};

void analyze_frequency(const char *ciphertext, double *freq);
void generate_initial_mapping(const double *cipher_freq, int *mapping);
void decrypt_with_mapping(const char *ciphertext, const int *mapping, char *plaintext);

int main() {
    char ciphertext[MAX_TEXT_SIZE];
    printf("Enter the ciphertext: ");
    fgets(ciphertext, MAX_TEXT_SIZE, stdin);
    ciphertext[strcspn(ciphertext, "\n")] = '\0';

    double cipher_freq[ALPHABET_SIZE] = {0};
    int initial_mapping[ALPHABET_SIZE];
    char plaintext[MAX_TEXT_SIZE];

    analyze_frequency(ciphertext, cipher_freq);
    generate_initial_mapping(cipher_freq, initial_mapping);
    decrypt_with_mapping(ciphertext, initial_mapping, plaintext);

    printf("Possible plaintext: %s\n", plaintext);

    return 0;
}

void analyze_frequency(const char *ciphertext, double *freq) {
    int count[ALPHABET_SIZE] = {0};
    int length = strlen(ciphertext), total_letters = 0;

    for (int i = 0; i < length; i++) {
        if (ciphertext[i] >= 'a' && ciphertext[i] <= 'z') {
            count[ciphertext[i] - 'a']++;
            total_letters++;
        } else if (ciphertext[i] >= 'A' && ciphertext[i] <= 'Z') {
            count[ciphertext[i] - 'A']++;
            total_letters++;
        }
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        freq[i] = (double)count[i] / total_letters * 100;
    }
}

void generate_initial_mapping(const double *cipher_freq, int *mapping) {
    int sorted_indices[ALPHABET_SIZE], english_sorted_indices[ALPHABET_SIZE];
    for (int i = 0; i < ALPHABET_SIZE; i++) sorted_indices[i] = english_sorted_indices[i] = i;

    for (int i = 0; i < ALPHABET_SIZE - 1; i++) {
        for (int j = i + 1; j < ALPHABET_SIZE; j++) {
            if (cipher_freq[sorted_indices[i]] < cipher_freq[sorted_indices[j]]) {
                int temp = sorted_indices[i];
                sorted_indices[i] = sorted_indices[j];
                sorted_indices[j] = temp;
            }
            if (english_freq[english_sorted_indices[i]] < english_freq[english_sorted_indices[j]]) {
                int temp = english_sorted_indices[i];
                english_sorted_indices[i] = english_sorted_indices[j];
                english_sorted_indices[j] = temp;
            }
        }
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        mapping[sorted_indices[i]] = english_sorted_indices[i];
    }
}

void decrypt_with_mapping(const char *ciphertext, const int *mapping, char *plaintext) {
    int length = strlen(ciphertext);

    for (int i = 0; i < length; i++) {
        if (ciphertext[i] >= 'a' && ciphertext[i] <= 'z') {
            plaintext[i] = 'a' + mapping[ciphertext[i] - 'a'];
        } else if (ciphertext[i] >= 'A' && ciphertext[i] <= 'Z') {
            plaintext[i] = 'A' + mapping[ciphertext[i] - 'A'];
        } else {
            plaintext[i] = ciphertext[i];
        }
    }

    plaintext[length] = '\0';
}

