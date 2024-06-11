#include <stdio.h>
#include <string.h>
#include <ctype.h>

void monoalphabeticCipher(char* text, const char* key) {
    for (int i = 0; text[i] != '\0'; ++i) {
        if (isupper(text[i])) {
            text[i] = key[text[i] - 'A'];
        } else if (islower(text[i])) {
            text[i] = tolower(key[text[i] - 'a']);
        }
    }
}

int main() {
    char text[100];
    char key[27];

    printf("Enter a plaintext string: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = '\0';

    printf("Enter a 26-letter key: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = '\0';

    if (strlen(key) != 26) {
        printf("Key must be exactly 26 letters.\n");
        return 1;
    }

    monoalphabeticCipher(text, key);
    printf("Encrypted string: %s\n", text);

    return 0;
}

