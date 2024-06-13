#include <stdio.h>
#include <stdint.h>

// Initial and final permutation tables (IP and FP)
const int initial_permutation[64] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
};

const int final_permutation[64] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25
};

// Example key and plaintext (64 bits each)
uint64_t key = 0x133457799BBCDFF1;
uint64_t plaintext = 0x0123456789ABCDEF;

// Perform initial permutation (IP)
uint64_t permute(uint64_t input, const int* perm, int size) {
    uint64_t output = 0;
    for (int i = 0; i < size; ++i) {
        output <<= 1;
        output |= (input >> (64 - perm[i])) & 1;
    }
    return output;
}

// DES encryption function
uint64_t des_encrypt(uint64_t plaintext, uint64_t key) {
    // Perform initial permutation (IP) on plaintext
    uint64_t permuted_plaintext = permute(plaintext, initial_permutation, 64);

    // Perform DES rounds (for simplicity, not implemented here)

    // Perform final permutation (FP) on ciphertext
    uint64_t ciphertext = permute(permuted_plaintext, final_permutation, 64);

    return ciphertext;
}

int main() {
    uint64_t ciphertext = des_encrypt(plaintext, key);

    printf("Plaintext: 0x%llx\n", plaintext);
    printf("Ciphertext: 0x%llx\n", ciphertext);

    return 0;
}

