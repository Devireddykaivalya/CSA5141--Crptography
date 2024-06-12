#include <stdio.h>
#include <string.h>

#define BLOCK_SIZE 16

void xor_blocks(unsigned char *out, const unsigned char *in1, const unsigned char *in2) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        out[i] = in1[i] ^ in2[i];
    }
}

void pseudo_encrypt(unsigned char *out, const unsigned char *in, const unsigned char *key) {
    // Simple XOR-based pseudo encryption for demonstration
    xor_blocks(out, in, key);
}

void cbc_mac(unsigned char *out, const unsigned char *key, const unsigned char *message, int length) {
    unsigned char block[BLOCK_SIZE] = {0};
    unsigned char iv[BLOCK_SIZE] = {0}; // Initialization Vector (IV) set to zero

    for (int i = 0; i < length / BLOCK_SIZE; i++) {
        xor_blocks(block, iv, &message[i * BLOCK_SIZE]);
        pseudo_encrypt(block, block, key);
        iv[BLOCK_SIZE - 1] = block[BLOCK_SIZE - 1]; // For simplicity, copying only the last byte
    }

    memcpy(out, block, BLOCK_SIZE);
}

void print_block(const unsigned char *block) {
    for (int i = 0; i < BLOCK_SIZE; i++) {
        printf("%02x", block[i]);
    }
    printf("\n");
}

int main() {
    unsigned char key[BLOCK_SIZE] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                     0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    unsigned char message[BLOCK_SIZE] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                                         0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
    unsigned char mac[BLOCK_SIZE];
    unsigned char extended_message[2 * BLOCK_SIZE];
    unsigned char new_mac[BLOCK_SIZE];

    // Compute CBC-MAC for the original one-block message
    cbc_mac(mac, key, message, BLOCK_SIZE);
    printf("CBC-MAC of the one-block message: ");
    print_block(mac);

    // Create the extended message: X || (XT)
    memcpy(extended_message, message, BLOCK_SIZE);
    memcpy(extended_message + BLOCK_SIZE, mac, BLOCK_SIZE);

    // Compute CBC-MAC for the extended message
    cbc_mac(new_mac, key, extended_message, 2 * BLOCK_SIZE);
    printf("CBC-MAC of the extended message (X || (XT)): ");
    print_block(new_mac);

    return 0;
}

