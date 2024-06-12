#include <stdio.h>
#include <stdint.h>
#include <string.h>

// Block size in bytes (8 bytes = 64 bits)
#define BLOCK_SIZE 8

// Simple XOR-based block cipher for demonstration (not secure)
void xor_cipher(uint8_t *block, uint8_t *key) {
    for (int i = 0; i < BLOCK_SIZE; ++i) {
        block[i] ^= key[i];
    }
}

// Padding function
void pad(uint8_t *input, int length, int padded_length) {
    input[length] = 0x80; // 1 bit followed by zeros
    for (int i = length + 1; i < padded_length; ++i) {
        input[i] = 0x00;
    }
}

// ECB mode encryption
void encrypt_ecb(uint8_t *plaintext, uint8_t *ciphertext, uint8_t *key, int length) {
    for (int i = 0; i < length; i += BLOCK_SIZE) {
        memcpy(ciphertext + i, plaintext + i, BLOCK_SIZE);
        xor_cipher(ciphertext + i, key);
    }
}

// ECB mode decryption
void decrypt_ecb(uint8_t *ciphertext, uint8_t *plaintext, uint8_t *key, int length) {
    for (int i = 0; i < length; i += BLOCK_SIZE) {
        memcpy(plaintext + i, ciphertext + i, BLOCK_SIZE);
        xor_cipher(plaintext + i, key);
    }
}

// CBC mode encryption
void encrypt_cbc(uint8_t *plaintext, uint8_t *ciphertext, uint8_t *key, uint8_t *iv, int length) {
    uint8_t block[BLOCK_SIZE];
    memcpy(block, iv, BLOCK_SIZE);
    for (int i = 0; i < length; i += BLOCK_SIZE) {
        for (int j = 0; j < BLOCK_SIZE; ++j) {
            block[j] ^= plaintext[i + j];
        }
        xor_cipher(block, key);
        memcpy(ciphertext + i, block, BLOCK_SIZE);
    }
}

// CBC mode decryption
void decrypt_cbc(uint8_t *ciphertext, uint8_t *plaintext, uint8_t *key, uint8_t *iv, int length) {
    uint8_t block[BLOCK_SIZE], prev_block[BLOCK_SIZE];
    memcpy(prev_block, iv, BLOCK_SIZE);
    for (int i = 0; i < length; i += BLOCK_SIZE) {
        memcpy(block, ciphertext + i, BLOCK_SIZE);
        xor_cipher(block, key);
        for (int j = 0; j < BLOCK_SIZE; ++j) {
            plaintext[i + j] = block[j] ^ prev_block[j];
        }
        memcpy(prev_block, ciphertext + i, BLOCK_SIZE);
    }
}

// CFB mode encryption
void encrypt_cfb(uint8_t *plaintext, uint8_t *ciphertext, uint8_t *key, uint8_t *iv, int length) {
    uint8_t block[BLOCK_SIZE];
    memcpy(block, iv, BLOCK_SIZE);
    for (int i = 0; i < length; i += BLOCK_SIZE) {
        xor_cipher(block, key);
        for (int j = 0; j < BLOCK_SIZE; ++j) {
            ciphertext[i + j] = plaintext[i + j] ^ block[j];
            block[j] = ciphertext[i + j];
        }
    }
}

// CFB mode decryption
void decrypt_cfb(uint8_t *ciphertext, uint8_t *plaintext, uint8_t *key, uint8_t *iv, int length) {
    uint8_t block[BLOCK_SIZE];
    memcpy(block, iv, BLOCK_SIZE);
    for (int i = 0; i < length; i += BLOCK_SIZE) {
        xor_cipher(block, key);
        for (int j = 0; j < BLOCK_SIZE; ++j) {
            plaintext[i + j] = ciphertext[i + j] ^ block[j];
            block[j] = ciphertext[i + j];
        }
    }
}

int main() {
    uint8_t key[BLOCK_SIZE] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    uint8_t iv[BLOCK_SIZE] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t plaintext[16] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10};
    uint8_t padded_plaintext[24];
    uint8_t ciphertext[24];
    uint8_t decrypted[24];

    // Padding
    memcpy(padded_plaintext, plaintext, 16);
    pad(padded_plaintext, 16, 24);

    printf("Original plaintext: ");
    for (int i = 0; i < 24; ++i) {
        printf("%02X ", padded_plaintext[i]);
    }
    printf("\n");

    // Encrypt using ECB mode
    encrypt_ecb(padded_plaintext, ciphertext, key, 24);
    printf("ECB ciphertext: ");
    for (int i = 0; i < 24; ++i) {
        printf("%02X ", ciphertext[i]);
    }
    printf("\n");

    // Decrypt using ECB mode
    decrypt_ecb(ciphertext, decrypted, key, 24);
    printf("ECB decrypted: ");
    for (int i = 0; i < 24; ++i) {
        printf("%02X ", decrypted[i]);
    }
    printf("\n");

    // Encrypt using CBC mode
    encrypt_cbc(padded_plaintext, ciphertext, key, iv, 24);
    printf("CBC ciphertext: ");
    for (int i = 0; i < 24; ++i) {
        printf("%02X ", ciphertext[i]);
    }
    printf("\n");

    // Decrypt using CBC mode
    decrypt_cbc(ciphertext, decrypted, key, iv, 24);
    printf("CBC decrypted: ");
    for (int i = 0; i < 24; ++i) {
        printf("%02X ", decrypted[i]);
    }
    printf("\n");

    // Encrypt using CFB mode
    encrypt_cfb(padded_plaintext, ciphertext, key, iv, 24);
    printf("CFB ciphertext: ");
    for (int i = 0; i < 24; ++i) {
        printf("%02X ", ciphertext[i]);
    }
    printf("\n");

    // Decrypt using CFB mode
    decrypt_cfb(ciphertext, decrypted, key, iv, 24);
    printf("CFB decrypted: ");
    for (int i = 0; i < 24; ++i) {
        printf("%02X ", decrypted[i]);
    }
    printf("\n");

    return 0;
}
