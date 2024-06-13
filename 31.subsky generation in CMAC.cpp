#include <stdio.h>
#include <stdint.h>
#include <string.h>

// AES block size constants
#define AES_BLOCK_SIZE_128 16
#define AES_BLOCK_SIZE_64  8

// Constants for 64-bit and 128-bit block sizes
const uint8_t R_64  = 0x1B;
const uint8_t R_128 = 0x87;

// Function to print a block (for debugging)
void print_block(uint8_t *block, int size) {
    for (int i = 0; i < size; i++) {
        printf("%02x ", block[i]);
    }
    printf("\n");
}

// AES encryption stub function (replace with actual AES encryption)
void AES_encrypt(uint8_t *plaintext, uint8_t *key, uint8_t *ciphertext) {
    // This is a stub. In a real scenario, this function would call a library's AES encrypt function.
    // For example, using OpenSSL:
    // AES_encrypt(plaintext, ciphertext, &aes_key);
    // Here we just copy the plaintext to ciphertext for demonstration.
    memcpy(ciphertext, plaintext, AES_BLOCK_SIZE_128);
}

// Function to generate subkeys K1 and K2
void generate_subkeys(uint8_t *key, uint8_t *K1, uint8_t *K2, int block_size) {
    uint8_t L[AES_BLOCK_SIZE_128] = {0};
    uint8_t constant;

    // Determine the constant based on block size
    if (block_size == AES_BLOCK_SIZE_64) {
        constant = R_64;
    } else if (block_size == AES_BLOCK_SIZE_128) {
        constant = R_128;
    } else {
        printf("Unsupported block size.\n");
        return;
    }

    // Encrypt the zero block with the given key
    uint8_t zero_block[AES_BLOCK_SIZE_128] = {0};
    AES_encrypt(zero_block, key, L);

    // Print L for debugging
    printf("L: ");
    print_block(L, block_size);

    // Generate K1
    int msb = (L[0] & 0x80) != 0;
    for (int i = 0; i < block_size; i++) {
        K1[i] = (L[i] << 1) | (i < block_size - 1 ? (L[i + 1] >> 7) : 0);
    }
    if (msb) {
        K1[block_size - 1] ^= constant;
    }

    // Print K1 for debugging
    printf("K1: ");
    print_block(K1, block_size);

    // Generate K2
    msb = (K1[0] & 0x80) != 0;
    for (int i = 0; i < block_size; i++) {
        K2[i] = (K1[i] << 1) | (i < block_size - 1 ? (K1[i + 1] >> 7) : 0);
    }
    if (msb) {
        K2[block_size - 1] ^= constant;
    }

    // Print K2 for debugging
    printf("K2: ");
    print_block(K2, block_size);
}

int main() {
    uint8_t key[AES_BLOCK_SIZE_128] = {0}; // Example key (all zeros)
    uint8_t K1[AES_BLOCK_SIZE_128] = {0};
    uint8_t K2[AES_BLOCK_SIZE_128] = {0};

    // Generate subkeys for 128-bit block size
    printf("Generating subkeys for 128-bit block size:\n");
    generate_subkeys(key, K1, K2, AES_BLOCK_SIZE_128);

    return 0;
}
