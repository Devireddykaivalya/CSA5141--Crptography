#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const double english_frequencies[26] = {
    8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094,
    6.966, 0.153, 0.772, 4.025, 2.406, 6.749, 7.507, 1.929,
    0.095, 5.987, 6.327, 9.056, 2.758, 0.978, 2.360, 0.150,
    1.974, 0.074
};

void calculate_frequencies(const char *text, double *frequencies) {
    int total_letters = 0, letter_counts[26] = {0};
    for (int i = 0; text[i] != '\0'; i++) {
        if (isalpha(text[i])) {
            letter_counts[toupper(text[i]) - 'A']++;
            total_letters++;
        }
    }
    for (int i = 0; i < 26; i++) frequencies[i] = (double)letter_counts[i] / total_letters * 100;
}

void decrypt(const char *ciphertext, int key, char *plaintext) {
    for (int i = 0; ciphertext[i] != '\0'; i++) {
        if (isalpha(ciphertext[i])) {
            char offset = isupper(ciphertext[i]) ? 'A' : 'a';
            plaintext[i] = (ciphertext[i] - offset - key + 26) % 26 + offset;
        } else {
            plaintext[i] = ciphertext[i];
        }
    }
    plaintext[strlen(ciphertext)] = '\0';
}

double score_plaintext(const char *plaintext) {
    double frequencies[26];
    calculate_frequencies(plaintext, frequencies);
    double score = 0.0;
    for (int i = 0; i < 26; i++) score += frequencies[i] * english_frequencies[i];
    return score;
}

typedef struct {
    char plaintext[1024];
    double score;
} ScoredPlaintext;

int compare_scored_plaintexts(const void *a, const void *b) {
    return ((ScoredPlaintext*)b)->score - ((ScoredPlaintext*)a)->score;
}

int main() {
    char ciphertext[1024], plaintext[1024];
    printf("Enter the ciphertext: ");
    fgets(ciphertext, sizeof(ciphertext), stdin);
    ciphertext[strcspn(ciphertext, "\n")] = '\0';

    int top_n;
    printf("Enter the number of top possible plaintexts to display: ");
    scanf("%d", &top_n);

    ScoredPlaintext scored_plaintexts[26];
    for (int key = 0; key < 26; key++) {
        decrypt(ciphertext, key, plaintext);
        scored_plaintexts[key].score = score_plaintext(plaintext);
        strcpy(scored_plaintexts[key].plaintext, plaintext);
    }

    qsort(scored_plaintexts, 26, sizeof(ScoredPlaintext), compare_scored_plaintexts);

    printf("Top %d possible plaintexts:\n", top_n);
    for (int i = 0; i < top_n && i < 26; i++) {
        printf("%2d. Score: %.2f, Plaintext: %s\n", i + 1, scored_plaintexts[i].score, scored_plaintexts[i].plaintext);
    }

    return 0;
}

