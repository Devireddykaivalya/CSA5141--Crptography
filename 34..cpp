#include <stdio.h>
#include <string.h>

#define BLOCK_SIZE 16

void simple_block_cipher(unsigned char *input, unsigned char *output, unsigned char *key) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        output[i] = input[i] ^ key[i];
    }
}

void print_hex(unsigned char *data, int len) {
    for (int i = 0; i < len; i++) {
        printf("%02x", data[i]);
    }
    printf("\n");
}

void ecb_mode(unsigned char *plaintext, int len, unsigned char *key) {
    unsigned char output[BLOCK_SIZE];
    for (int i = 0; i < len; i += BLOCK_SIZE) {
        simple_block_cipher(plaintext + i, output, key);
        printf("ECB Encrypted block: ");
        print_hex(output, BLOCK_SIZE);
        simple_block_cipher(output, output, key);
        printf("ECB Decrypted block: ");
        print_hex(output, BLOCK_SIZE);
    }
}

void cbc_mode(unsigned char *plaintext, int len, unsigned char *key, unsigned char *iv) {
    unsigned char output[BLOCK_SIZE], xor_block[BLOCK_SIZE];
    memcpy(xor_block, iv, BLOCK_SIZE);
    for (int i = 0; i < len; i += BLOCK_SIZE) {
        for (int j = 0; j < BLOCK_SIZE; j++) xor_block[j] ^= plaintext[i + j];
        simple_block_cipher(xor_block, output, key);
        memcpy(xor_block, output, BLOCK_SIZE);
        printf("CBC Encrypted block: ");
        print_hex(output, BLOCK_SIZE);
        simple_block_cipher(output, output, key);
        for (int j = 0; j < BLOCK_SIZE; j++) output[j] ^= iv[j];
        memcpy(iv, xor_block, BLOCK_SIZE);
        printf("CBC Decrypted block: ");
        print_hex(output, BLOCK_SIZE);
    }
}

void cfb_mode(unsigned char *plaintext, int len, unsigned char *key, unsigned char *iv) {
    unsigned char output[BLOCK_SIZE], feedback[BLOCK_SIZE];
    memcpy(feedback, iv, BLOCK_SIZE);
    for (int i = 0; i < len; i += BLOCK_SIZE) {
        simple_block_cipher(feedback, output, key);
        for (int j = 0; j < BLOCK_SIZE; j++) output[j] ^= plaintext[i + j];
        memcpy(feedback, output, BLOCK_SIZE);
        printf("CFB Encrypted block: ");
        print_hex(output, BLOCK_SIZE);
        simple_block_cipher(feedback, feedback, key);
        for (int j = 0; j < BLOCK_SIZE; j++) feedback[j] ^= iv[j];
        memcpy(iv, output, BLOCK_SIZE);
        printf("CFB Decrypted block: ");
        print_hex(feedback, BLOCK_SIZE);
    }
}

int main() {
    unsigned char key[BLOCK_SIZE] = "0123456789abcdef";
    unsigned char iv[BLOCK_SIZE] = "abcdef9876543210";
    unsigned char plaintext[BLOCK_SIZE * 2] = "This is a test msg for ECB, CBC, and CFB modes.";

    printf("ECB Mode:\n");
    ecb_mode(plaintext, sizeof(plaintext), key);
    
    printf("\nCBC Mode:\n");
    unsigned char iv_cbc[BLOCK_SIZE];
    memcpy(iv_cbc, iv, BLOCK_SIZE);
    cbc_mode(plaintext, sizeof(plaintext), key, iv_cbc);

    printf("\nCFB Mode:\n");
    unsigned char iv_cfb[BLOCK_SIZE];
    memcpy(iv_cfb, iv, BLOCK_SIZE);
    cfb_mode(plaintext, sizeof(plaintext), key, iv_cfb);

    return 0;
}

