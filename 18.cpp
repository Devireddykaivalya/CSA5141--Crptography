#include <stdio.h>
#include <stdint.h>


int pc1[] = {57, 49, 41, 33, 25, 17, 9,
             1, 58, 50, 42, 34, 26, 18,
             10, 2, 59, 51, 43, 35, 27,
             19, 11, 3, 60, 52, 44, 36,
             63, 55, 47, 39, 31, 23, 15,
             7, 62, 54, 46, 38, 30, 22,
             14, 6, 61, 53, 45, 37, 29,
             21, 13, 5, 28, 20, 12, 4};


int shift_schedule[] = {1, 1, 2, 2, 2, 2, 2, 2,
                        1, 2, 2, 2, 2, 2, 2, 1};
                        
int pc2[] = {14, 17, 11, 24, 1, 5,
             3, 28, 15, 6, 21, 10,
             23, 19, 12, 4, 26, 8,
             16, 7, 27, 20, 13, 2,
             41, 52, 31, 37, 47, 55,
             30, 40, 51, 45, 33, 48,
             44, 49, 39, 56, 34, 53,
             46, 42, 50, 36, 29, 32};

void generate_keys(uint64_t key, uint64_t *keys);

int main() {
    
    uint64_t key = 0x133457799BBCDFF1;
    uint64_t keys[16];

    generate_keys(key, keys);

    printf("Keys:\n");
    for (int i = 0; i < 16; i++) {
        printf("%d: %llx\n", i+1, keys[i]);
    }

    return 0;
}

void generate_keys(uint64_t key, uint64_t *keys) {
    
    uint64_t permuted_key = 0;
    for (int i = 0; i < 56; i++) {
        permuted_key |= ((key >> (64 - pc1[i])) & 0x1) << (55 - i);
    }


    uint32_t left_part = (permuted_key >> 28) & 0x0FFFFFFF;
    uint32_t right_part = permuted_key & 0x0FFFFFFF;

    for (int round = 0; round < 16; round++) {
        
        for (int i = 0; i < shift_schedule[round]; i++) {
            left_part = ((left_part << 1) & 0x0FFFFFFF) | ((left_part >> 27) & 0x1);
            right_part = ((right_part << 1) & 0x0FFFFFFF) | ((right_part >> 27) & 0x1);
        }

        uint64_t combined_key = (((uint64_t)left_part) << 28) | right_part;

        uint64_t subkey = 0;
        for (int i = 0; i < 48; i++) {
            subkey |= ((combined_key >> (56 - pc2[i])) & 0x1) << (47 - i);
        }

    
        keys[round] = subkey;
    }
}
