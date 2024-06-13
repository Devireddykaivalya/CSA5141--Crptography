#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26

// English letter frequencies (from https://en.wikipedia.org/wiki/Letter_frequency)
const float english_frequencies[ALPHABET_SIZE] = {
    0.0817, 0.0149, 0.0278, 0.0425, 0.1270, 0.0223, 0.0202, 0.0609,
    0.0697, 0.0015, 0.0077, 0.0403, 0.0241, 0.0675, 0.0751, 0.0193,
    0.0010, 0.0599, 0.0633, 0.0906, 0.0276, 0.0098, 0.0236, 0.0015,
    0.0197, 0.0007
};

// Function to calculate chi-square value
float calculate_chi_square(const int *observed_counts) {
    float chi_square = 0.0;
    int total = 0;

    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        total += observed_counts[i];
    }

    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        float expected = total * english_frequencies[i];
        float difference = observed_counts[i] - expected;
        chi_square += difference * difference / expected;
    }

    return chi_square;
}

// Function to perform letter frequency analysis
void frequency_attack(const char *ciphertext, int top_results) {
    int observed_counts[ALPHABET_SIZE] = {0};

    // Count frequencies of each letter in the ciphertext
    int length = strlen(ciphertext);
    for (int i = 0; i < length; ++i) {
        char c = tolower(ciphertext[i]);
        if (isalpha(c)) {
            observed_counts[c - 'a']++;
        }
    }

    // Calculate chi-square values for each possible shift
    // (assuming each shift is a potential decryption key)
    float chi_square_values[ALPHABET_SIZE];
    for (int shift = 0; shift < ALPHABET_SIZE; ++shift) {
        int shifted_counts[ALPHABET_SIZE] = {0};

        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            int original_index = (i + shift) % ALPHABET_SIZE;
            shifted_counts[i] = observed_counts[original_index];
        }

        chi_square_values[shift] = calculate_chi_square(shifted_counts);
    }

    // Find top results based on chi-square values
    for (int result_num = 0; result_num < top_results; ++result_num) {
        float min_chi_square = chi_square_values[0];
        int min_index = 0;

        for (int i = 1; i < ALPHABET_SIZE; ++i) {
            if (chi_square_values[i] < min_chi_square) {
                min_chi_square = chi_square_values[i];
                min_index = i;
            }
        }

        // Output the decrypted plaintext assuming the min_index shift
        printf("Possible plaintext (Chi-square %.2f): ", min_chi_square);
        for (int i = 0; i < length; ++i) {
            char c = ciphertext[i];
            if (isalpha(c)) {
                char decrypted_char = 'a' + (tolower(c) - 'a' - min_index + ALPHABET_SIZE) % ALPHABET_SIZE;
                if (isupper(c)) {
                    decrypted_char = toupper(decrypted_char);
                }
                printf("%c", decrypted_char);
            } else {
                printf("%c", c);
            }
        }
        printf("\n");

        // Mark this index as used by setting chi-square to a large value
        chi_square_values[min_index] = FLT_MAX;
    }
}

int main() {
    char ciphertext[] = "L fdph, L vdz, L frqtxhuhg.";
    int top_results = 10;

    printf("Performing frequency analysis on ciphertext:\n%s\n\n", ciphertext);
    printf("Top %d possible plaintexts:\n", top_results);
    frequency_attack(ciphertext, top_results);

    return 0;
}

