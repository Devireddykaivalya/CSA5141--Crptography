#include <stdio.h>
#include <stdint.h>

int initial_permutation[] = {58, 50, 42, 34, 26, 18, 10, 2,
                             60, 52, 44, 36, 28, 20, 12, 4,
                             62, 54, 46, 38, 30, 22, 14, 6,
                             64, 56, 48, 40, 32, 24, 16, 8,
                             57, 49, 41, 33, 25, 17, 9, 1,
                             59, 51, 43, 35, 27, 19, 11, 3,
                             61, 53, 45, 37, 29, 21, 13, 5,
                             63, 55, 47, 39, 31, 23, 15, 7};

int final_permutation[] = {40, 8, 48, 16, 56, 24, 64, 32,
                           39, 7, 47, 15, 55, 23, 63, 31,
                           38, 6, 46, 14, 54, 22, 62, 30,
                           37, 5, 45, 13, 53, 21, 61, 29,
                           36, 4, 44, 12, 52, 20, 60, 28,
                           35, 3, 43, 11, 51, 19, 59, 27,
                           34, 2, 42, 10, 50, 18, 58, 26,
                           33, 1, 41, 9, 49, 17, 57, 25};


void des_decrypt(uint64_t *input, uint64_t *output, uint64_t *keys);
void generate_keys(uint64_t key, uint64_t *keys);

int main() {
    
    uint64_t key = 0x133457799BBCDFF1;
    uint64_t plaintext = 0x0123456789ABCDEF;

    uint64_t keys[16];
    uint64_t ciphertext, decrypted_text;

    generate_keys(key, keys);
    des_decrypt(&plaintext, &ciphertext, keys);

    printf("Ciphertext: %llx\n", ciphertext);

    
    des_decrypt(&ciphertext, &decrypted_text, keys);

    printf("Decrypted text: %llx\n", decrypted_text);

    return 0;
}


void des_decrypt(uint64_t *input, uint64_t *output, uint64_t *keys) {
    uint64_t permuted_input = 0;
    uint64_t permuted_output = 0;
    int i, j;

    
    for (i = 0; i < 64; i++) {
        permuted_input |= ((*input >> (64 - initial_permutation[i])) & 0x1) << (63 - i);
    }

    
    for (i = 15; i >= 0; i--) {
        uint64_t round_key = keys[i];

        
        uint64_t expanded_input = 0;
        for (j = 0; j < 48; j++) {
            expanded_input |= ((permuted_input >> (32 - j)) & 0x1) << (47 - j);
        }

        
        expanded_input ^= round_key;

        
    }

    
    for (i = 0; i < 64; i++) {
        permuted_output |= ((permuted_input >> (64 - final_permutation[i])) & 0x1) << (63 - i);
    }

    *output = permuted_output;
}


void generate_keys(uint64_t key, uint64_t *keys) {
	
}


