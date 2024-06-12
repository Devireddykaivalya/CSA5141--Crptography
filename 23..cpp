#include <stdio.h>
#include <stdint.h>

#define KEY_LENGTH 10
#define BLOCK_SIZE 8

// Function prototypes
uint8_t sdes_encrypt(uint8_t block, uint16_t key);
uint8_t sdes_decrypt(uint8_t block, uint16_t key);
uint16_t generate_subkey(uint16_t key, int round);
uint8_t feistel(uint8_t r, uint8_t sk);
uint8_t sbox(uint8_t input, const uint8_t sbox_table[4][4]);
void increment_counter(uint8_t *counter);

const uint8_t SBOX1[4][4] = {
    {1, 0, 3, 2},
    {3, 2, 1, 0},
    {0, 2, 1, 3},
    {3, 1, 3, 2}
};

const uint8_t SBOX2[4][4] = {
    {0, 1, 2, 3},
    {2, 0, 1, 3},
    {3, 0, 1, 0},
    {2, 1, 0, 3}
};

int main() {
    uint8_t plaintext[] = {0x01, 0x02, 0x04};
    uint16_t key = 0x1FD; // Binary: 01111 11101
    uint8_t counter = 0x00;
    uint8_t ciphertext[3];
    uint8_t decrypted[3];
    
    // Encrypt the plaintext
    printf("Encryption:\n");
    for (int i = 0; i < 3; i++) {
        uint8_t encrypted_counter = sdes_encrypt(counter, key);
        ciphertext[i] = plaintext[i] ^ encrypted_counter;
        printf("Ciphertext[%d]: %02X\n", i, ciphertext[i]);
        increment_counter(&counter);
    }

    // Decrypt the ciphertext
    counter = 0x00; // Reset counter
    printf("\nDecryption:\n");
    for (int i = 0; i < 3; i++) {
        uint8_t encrypted_counter = sdes_encrypt(counter, key);
        decrypted[i] = ciphertext[i] ^ encrypted_counter;
        printf("Decrypted[%d]: %02X\n", i, decrypted[i]);
        increment_counter(&counter);
    }

    return 0;
}

uint8_t sdes_encrypt(uint8_t block, uint16_t key) {
    uint8_t left = block >> 4;
    uint8_t right = block & 0x0F;
    uint8_t subkey1 = generate_subkey(key, 1);
    uint8_t subkey2 = generate_subkey(key, 2);

    // Round 1
    uint8_t temp = right;
    right = left ^ feistel(right, subkey1);
    left = temp;

    // Round 2
    temp = right;
    right = left ^ feistel(right, subkey2);
    left = temp;

    return (right << 4) | left;
}

uint8_t sdes_decrypt(uint8_t block, uint16_t key) {
    uint8_t left = block >> 4;
    uint8_t right = block & 0x0F;
    uint8_t subkey1 = generate_subkey(key, 1);
    uint8_t subkey2 = generate_subkey(key, 2);

    // Round 2
    uint8_t temp = right;
    right = left ^ feistel(right, subkey2);
    left = temp;

    // Round 1
    temp = right;
    right = left ^ feistel(right, subkey1);
    left = temp;

    return (right << 4) | left;
}

uint16_t generate_subkey(uint16_t key, int round) {
    // Simplified key generation for S-DES, should be implemented as per S-DES specifications
    return (key >> (round - 1)) & 0xFF; // Example, modify as per actual S-DES key schedule
}

uint8_t feistel(uint8_t r, uint8_t sk) {
    uint8_t expanded = ((r & 0x8) << 1) | ((r & 0x4) << 1) | ((r & 0x2) >> 1) | ((r & 0x1) >> 1); // Example expansion
    uint8_t xor_res = expanded ^ sk;
    uint8_t left = (xor_res & 0xC) >> 2;
    uint8_t right = xor_res & 0x3;
    return (sbox(left, SBOX1) << 2) | sbox(right, SBOX2);
}

uint8_t sbox(uint8_t input, const uint8_t sbox_table[4][4]) {
    int row = (input & 0x2) >> 1;
    int col = input & 0x1;
    return sbox_table[row][col];
}

void increment_counter(uint8_t *counter) {
    (*counter)++;
}
