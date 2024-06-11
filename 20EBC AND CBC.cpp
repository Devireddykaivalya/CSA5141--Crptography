#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
void DES_encrypt_block(const unsigned char *plaintext, unsigned char *ciphertext, const unsigned char *key);
void DES_decrypt_block(const unsigned char *ciphertext, unsigned char *plaintext, const unsigned char *key);

void xor_blocks(unsigned char *a, const unsigned char *b, size_t size) {
    for (size_t i = 0; i < size; i++) {
        a[i] ^= b[i];
    }
}

void ECB_encrypt(const unsigned char *plaintext, unsigned char *ciphertext, size_t size, const unsigned char *key) {
    for (size_t i = 0; i < size; i += 8) {
        DES_encrypt_block(plaintext + i, ciphertext + i, key);
    }
}

void ECB_decrypt(const unsigned char *ciphertext, unsigned char *plaintext, size_t size, const unsigned char *key) {
    for (size_t i = 0; i < size; i += 8) {
        DES_decrypt_block(ciphertext + i, plaintext + i, key);
    }
}

void CBC_encrypt(const unsigned char *plaintext, unsigned char *ciphertext, size_t size, const unsigned char *key, unsigned char *iv) {
    unsigned char block[8];
    memcpy(block, iv, 8);
    
    for (size_t i = 0; i < size; i += 8) {
        xor_blocks(block, plaintext + i, 8);
        DES_encrypt_block(block, ciphertext + i, key);
        memcpy(block, ciphertext + i, 8);
    }
}

// CBC mode decryption
void CBC_decrypt(const unsigned char *ciphertext, unsigned char *plaintext, size_t size, const unsigned char *key, unsigned char *iv) {
    unsigned char block[8];
    unsigned char prev_block[8];
    memcpy(prev_block, iv, 8);
    
    for (size_t i = 0; i < size; i += 8) {
        DES_decrypt_block(ciphertext + i, block, key);
        xor_blocks(block, prev_block, 8);
        memcpy(plaintext + i, block, 8);
        memcpy(prev_block, ciphertext + i, 8);
    }
}

void simulate_error_propagation(const unsigned char *plaintext, size_t size, const unsigned char *key, unsigned char *iv) {
    unsigned char *ciphertext_ecb = (unsigned char *)malloc(size);
    unsigned char *decrypted_ecb = (unsigned char *)malloc(size);
    unsigned char *ciphertext_cbc = (unsigned char *)malloc(size);
    unsigned char *decrypted_cbc = (unsigned char *)malloc(size);
    unsigned char *iv_copy = (unsigned char *)malloc(8);

    ECB_encrypt(plaintext, ciphertext_ecb, size, key);

    ciphertext_ecb[8] ^= 0xFF; 
    ECB_decrypt(ciphertext_ecb, decrypted_ecb, size, key);

    memcpy(iv_copy, iv, 8);
    CBC_encrypt(plaintext, ciphertext_cbc, size, key, iv_copy);

    ciphertext_cbc[8] ^= 0xFF; 
    memcpy(iv_copy, iv, 8);
    CBC_decrypt(ciphertext_cbc, decrypted_cbc, size, key, iv_copy);

    printf("Original Plaintext:\n");
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", plaintext[i]);
    }
    printf("\n");

    printf("Decrypted Plaintext after ECB with error in second block:\n");
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", decrypted_ecb[i]);
    }
    printf("\n");

    printf("Decrypted Plaintext after CBC with error in second block:\n");
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", decrypted_cbc[i]);
    }
    printf("\n");

    free(ciphertext_ecb);
    free(decrypted_ecb);
    free(ciphertext_cbc);
    free(decrypted_cbc);
    free(iv_copy);
}

int main() {
    unsigned char plaintext[] = {
        0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
        0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
        0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF
    };
    unsigned char key[8] = { 0x13, 0x34, 0x57, 0x79, 0x9B, 0xBC, 0xDF, 0xF1 };
    unsigned char iv[8] = { 0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF };
    size_t size = sizeof(plaintext);

    simulate_error_propagation(plaintext, size, key, iv);

    return 0;
}

void DES_encrypt_block(const unsigned char *plaintext, unsigned char *ciphertext, const unsigned char *key) {

    for (int i = 0; i < 8; i++) {
        ciphertext[i] = plaintext[i] ^ key[i];
    }
}

void DES_decrypt_block(const unsigned char *ciphertext, unsigned char *plaintext, const unsigned char *key) {

    for (int i = 0; i < 8; i++) {
        plaintext[i] = ciphertext[i] ^ key[i];
    }
}
