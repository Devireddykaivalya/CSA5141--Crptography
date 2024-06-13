#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26
#define MAX_PLAINTEXTS 10

// Frequency of letters in English text (in percentages)
const double english_freq[ALPHABET_SIZE] = {
    8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094,
    6.966, 0.153, 0.772, 4.025, 2.406, 6.749, 7.507, 1.929,
    0.095, 5.987, 6.327, 9.056, 2.758, 0.978, 2.360, 0.150,
    1.974, 0.074
};

// Structure to hold candidate plaintexts and their scores
typedef struct {
    char plaintext[1024];
    double score;
} Candidate;

// Function to compute the frequency of letters in a given text
void compute_frequency(const char *text, double *freq) {
    int count[ALPHABET_SIZE] = {0};
    int total = 0;

    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            count[tolower(text[i]) - 'a']++;
            total++;
        }
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        freq[i] = ((double)count[i] / total) * 100;
    }
}

// Function to decrypt the ciphertext with a given key
void decrypt(const char *ciphertext, char *plaintext, int key) {
    for (int i = 0; ciphertext[i] != '\0'; i++) {
        if (isalpha(ciphertext[i])) {
            int offset = isupper(ciphertext[i]) ? 'A' : 'a';
            plaintext[i] = ((ciphertext[i] - offset - key + ALPHABET_SIZE) % ALPHABET_SIZE) + offset;
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[strlen(ciphertext)] = '\0';
}

// Function to score the plaintext based on letter frequencies
double score_plaintext(const char *plaintext) {
    double freq[ALPHABET_SIZE];
    compute_frequency(plaintext, freq);

    double score = 0.0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        score += (freq[i] - english_freq[i]) * (freq[i] - english_freq[i]);
    }
    return score;
}

// Function to perform letter frequency attack
void letter_frequency_attack(const char *ciphertext, int num_candidates) {
    Candidate candidates[ALPHABET_SIZE];

    for (int key = 0; key < ALPHABET_SIZE; key++) {
        decrypt(ciphertext, candidates[key].plaintext, key);
        candidates[key].score = score_plaintext(candidates[key].plaintext);
    }

    // Sort candidates based on score
    for (int i = 0; i < ALPHABET_SIZE - 1; i++) {
        for (int j = i + 1; j < ALPHABET_SIZE; j++) {
            if (candidates[i].score > candidates[j].score) {
                Candidate temp = candidates[i];
                candidates[i] = candidates[j];
                candidates[j] = temp;
            }
        }
    }

    // Print top candidate plaintexts
    printf("\nTop %d possible plaintexts:\n", num_candidates);
    for (int i = 0; i < num_candidates && i < ALPHABET_SIZE; i++) {
        printf("Candidate %d: %s\n", i + 1, candidates[i].plaintext);
    }
}

int main() {
    char ciphertext[1024];
    int num_candidates = MAX_PLAINTEXTS;

    printf("Enter the ciphertext: ");
    fgets(ciphertext, sizeof(ciphertext), stdin);
    ciphertext[strcspn(ciphertext, "\n")] = '\0';  // Remove newline character

    printf("Enter the number of top plaintexts to display: ");
    scanf("%d", &num_candidates);

    letter_frequency_attack(ciphertext, num_candidates);

    return 0;
}
