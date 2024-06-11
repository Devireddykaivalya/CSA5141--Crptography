#include <stdio.h>
#include <string.h>
#include <ctype.h>

void vigenere(char *input, char *key, char *output, int encrypt) {
    int text_len = strlen(input), key_len = strlen(key);
    for (int i = 0, j = 0; i < text_len; i++) {
        if (isalpha(input[i])) {
            char offset = isupper(input[i]) ? 'A' : 'a';
            int key_shift = tolower(key[j % key_len]) - 'a';
            key_shift = encrypt ? key_shift : -key_shift;
            output[i] = ((input[i] - offset + key_shift + 26) % 26) + offset;
            j++;
        } else {
            output[i] = input[i];
        }
    }
    output[text_len] = '\0';
}

int main() {
    char text[256], key[256], output[256];

    printf("Enter the text: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = '\0'; // Remove newline character

    printf("Enter the key: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = '\0'; // Remove newline character

    vigenere(text, key, output, 1);
    printf("Encrypted text: %s\n", output);

    vigenere(output, key, text, 0);
    printf("Decrypted text: %s\n", text);

    return 0;
}

