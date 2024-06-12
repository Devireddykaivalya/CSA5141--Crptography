#include <stdio.h>
#include <stdint.h>

// S-DES key generation and functions
void generate_keys(uint16_t key, uint8_t *k1, uint8_t *k2);
uint8_t sdes_encrypt(uint8_t plaintext, uint8_t k1, uint8_t k2);
uint8_t sdes_decrypt(uint8_t ciphertext, uint8_t k1, uint8_t k2);

// Helper functions for S-DES
uint8_t initial_permutation(uint8_t input);
uint8_t inverse_initial_permutation(uint8_t input);
uint8_t f_function(uint8_t r, uint8_t k);
uint8_t s_box(uint8_t input, int box);
uint8_t permute4(uint8_t input);
uint8_t permute8(uint8_t input);
uint8_t permute10(uint16_t input);

// CBC mode encryption and decryption
void encrypt_cbc(uint8_t *plaintext, uint8_t *ciphertext, uint8_t iv, uint8_t k1, uint8_t k2, int length);
void decrypt_cbc(uint8_t *ciphertext, uint8_t *plaintext, uint8_t iv, uint8_t k1, uint8_t k2, int length);

int main() {
    uint8_t iv = 0xAA; // 1010 1010
    uint8_t key = 0x1FD; // 01111 11101

    uint8_t plaintext[2] = {0x01, 0x23}; // 0000 0001 0010 0011
    uint8_t ciphertext[2];
    uint8_t decrypted[2];

    uint8_t k1, k2;
    generate_keys(key, &k1, &k2);

    // Encrypt plaintext
    encrypt_cbc(plaintext, ciphertext, iv, k1, k2, 2);

    // Print encrypted ciphertext
    printf("Ciphertext: ");
    for (int i = 0; i < 2; ++i) {
        printf("%02X ", ciphertext[i]);
    }
    printf("\n");

    // Decrypt ciphertext
    decrypt_cbc(ciphertext, decrypted, iv, k1, k2, 2);

    // Print decrypted plaintext
    printf("Decrypted plaintext: ");
    for (int i = 0; i < 2; ++i) {
        printf("%02X ", decrypted[i]);
    }
    printf("\n");

    return 0;
}

// S-DES key generation
void generate_keys(uint16_t key, uint8_t *k1, uint8_t *k2) {
    key = permute10(key);
    uint8_t left = (key >> 5) & 0x1F;
    uint8_t right = key & 0x1F;
    left = ((left << 1) | (left >> 4)) & 0x1F;
    right = ((right << 1) | (right >> 4)) & 0x1F;
    *k1 = permute8((left << 5) | right);
    left = ((left << 2) | (left >> 3)) & 0x1F;
    right = ((right << 2) | (right >> 3)) & 0x1F;
    *k2 = permute8((left << 5) | right);
}

// Initial permutation for S-DES
uint8_t initial_permutation(uint8_t input) {
    return ((input & 0x80) >> 5) | ((input & 0x40) >> 7) |
           ((input & 0x20) >> 1) | ((input & 0x10) >> 1) |
           ((input & 0x08) << 3) | ((input & 0x04) << 3) |
           ((input & 0x02) << 7) | ((input & 0x01) << 1);
}

// Inverse initial permutation for S-DES
uint8_t inverse_initial_permutation(uint8_t input) {
    return ((input & 0x80) >> 1) | ((input & 0x40) << 7) |
           ((input & 0x20) << 1) | ((input & 0x10) >> 3) |
           ((input & 0x08) >> 3) | ((input & 0x04) << 1) |
           ((input & 0x02) >> 7) | ((input & 0x01) << 5);
}

// F function for S-DES
uint8_t f_function(uint8_t r, uint8_t k) {
    uint8_t ep = ((r & 0x8) << 1) | ((r & 0x4) << 3) |
                 ((r & 0x2) << 1) | ((r & 0x1) << 3) |
                 ((r & 0x8) >> 3) | ((r & 0x4) >> 1) |
                 ((r & 0x2) >> 3) | ((r & 0x1) >> 1);
    ep ^= k;
    uint8_t left = (ep >> 4) & 0xF;
    uint8_t right = ep & 0xF;
    left = s_box(left, 0);
    right = s_box(right, 1);
    return permute4((left << 2) | right);
}

// S-Box for S-DES
uint8_t s_box(uint8_t input, int box) {
    uint8_t s0[4][4] = {{1, 0, 3, 2},
                        {3, 2, 1, 0},
                        {0, 2, 1, 3},
                        {3, 1, 3, 2}};
    uint8_t s1[4][4] = {{0, 1, 2, 3},
                        {2, 0, 1, 3},
                        {3, 0, 1, 0},
                        {2, 1, 0, 3}};
    uint8_t row = ((input & 0x8) >> 2) | (input & 0x1);
    uint8_t col = (input >> 1) & 0x3;
    if (box == 0) {
        return s0[row][col];
    } else {
        return s1[row][col];
    }
}

// Permutation 4 for S-DES
uint8_t permute4(uint8_t input) {
    return ((input & 0x8) >> 3) | ((input & 0x4) >> 1) |
           ((input & 0x2) << 1) | ((input & 0x1) << 3);
}

// Permutation 8 for S-DES key generation
uint8_t permute8(uint8_t input) {
    return ((input & 0x80) >> 2) | ((input & 0x40) << 1) |
           ((input & 0x20) >> 1) | ((input & 0x10) << 2) |
           ((input & 0x08) >> 2) | ((input & 0x04) << 2) |
           ((input & 0x02) >> 1) | ((input & 0x01) << 2);
}

// Permutation 10 for S-DES key generation
uint8_t permute10(uint16_t input) {
    return ((input & 0x200) >> 6) | ((input & 0x100) >> 3) |
           ((input & 0x080) >> 2) | ((input & 0x040) >> 2) |
           ((input & 0x020) << 2) | ((input & 0x010) << 1) |
           ((input & 0x008) >> 1) | ((input & 0x004) << 3) |
           ((input & 0x002) << 2) | ((input & 0x001) << 1);
}

// S-DES encryption
uint8_t sdes_encrypt(uint8_t plaintext, uint8_t k1, uint8_t k2) {
    plaintext = initial_permutation(plaintext);
    uint8_t left = plaintext >> 4;
    uint8_t right = plaintext & 0xF;
    left ^= f_function(right, k1);
    uint8_t temp = left;
    left = right;
    right = temp;
    left ^= f_function(right, k2);
    uint8_t pre_output = (left << 4) | right;
    return inverse_initial_permutation(pre_output);
}

// S-DES decryption
uint8_t sdes_decrypt(uint8_t ciphertext, uint8_t k1, uint8_t k2) {
    ciphertext = initial_permutation(ciphertext);
    uint8_t left = ciphertext >> 4;
    uint8_t right = ciphertext & 0xF;
    left ^= f_function(right, k2);
    uint8_t temp = left;
    left = right;
    right = temp;
    left ^= f_function(right, k1);
    uint8_t pre_output = (left << 4) | right;
    return inverse_initial_permutation(pre_output);
}

// CBC mode encryption
void encrypt_cbc(uint8_t *plaintext, uint8_t *ciphertext, uint8_t iv, uint8_t k1, uint8_t k2, int length) {
    uint8_t previous = iv;
    for (int i = 0; i < length; ++i) {
        uint8_t input = plaintext[i] ^ previous;
        ciphertext[i] = sdes_encrypt(input, k1, k2);
        previous = ciphertext[i];
    }
}

// CBC mode decryption
void decrypt_cbc(uint8_t *ciphertext, uint8_t *plaintext, uint8_t iv, uint8_t k1, uint8_t k2, int length) {
    uint8_t previous = iv;
    for (int i = 0; i < length; ++i) {
        uint8_t decrypted = sdes_decrypt(ciphertext[i], k1, k2);
        plaintext[i] = decrypted ^ previous;
        previous = ciphertext[i];
    }
}
