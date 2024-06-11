#include <stdio.h>
#include <string.h>

void rail_fence_cipher(char* input, char* output, int length);
void rail_fence_decipher(char* input, char* output, int length);

int main() {
    int length;
    char input[20], cipher[20], decipher[20];

    printf("\n\t\t RAIL FENCE TECHNIQUE");
    printf("\n\nEnter the input string: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';  // Remove trailing newline

    length = strlen(input);

    // Ciphering
    rail_fence_cipher(input, cipher, length);
    printf("\nCipher text after applying rail fence: %s", cipher);

    // Deciphering
    rail_fence_decipher(cipher, decipher, length);
    printf("\nText after decryption: %s\n", decipher);

    return 0;
}

void rail_fence_cipher(char* input, char* output, int length) {
    int i, j = 0;

    for (i = 0; i < length; i++) {
        if (i % 2 == 0) {
            output[j++] = input[i];
        }
    }
    for (i = 0; i < length; i++) {
        if (i % 2 == 1) {
            output[j++] = input[i];
        }
    }
    output[j] = '\0';
}

void rail_fence_decipher(char* input, char* output, int length) {
    int i, j, k;

    if (length % 2 == 0) {
        k = length / 2;
    } else {
        k = (length / 2) + 1;
    }

    for (i = 0, j = 0; i < k; i++) {
        output[j] = input[i];
        j += 2;
    }
    for (i = k, j = 1; i < length; i++) {
        output[j] = input[i];
        j += 2;
    }
    output[length] = '\0';
}
