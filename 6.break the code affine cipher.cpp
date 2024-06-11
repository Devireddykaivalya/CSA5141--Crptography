#include <stdio.h>

int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 0) {
            return x;
        }
    }
    return 1;
}

void affineDecrypt(char* ciphertext, int a, int b) {
    int a_inv = modInverse(a, 26);
    for (int i = 0; ciphertext[i] != '\0'; i++) {
        if (ciphertext[i] >= 'A' && ciphertext[i] <= 'Z') {
            ciphertext[i] = (a_inv * (ciphertext[i] - 'A' - b + 26) % 26) + 'A';
        } else if (ciphertext[i] >= 'a' && ciphertext[i] <= 'z') {
            ciphertext[i] = (a_inv * (ciphertext[i] - 'a' - b + 26) % 26) + 'a';
        }
    }
}

int main() {
    char ciphertext[256];
    int a, b;

    printf("Enter the ciphertext: ");
    fgets(ciphertext, sizeof(ciphertext), stdin);

    // The equations derived from B and U being the most frequent letters
    int x1 = 4, y1 = 1; // E -> B
    int x2 = 19, y2 = 20; // T -> U

    // Calculate a and b
    a = ((y1 - y2) * modInverse((x1 - x2 + 26) % 26, 26)) % 26;
    if (a < 0) a += 26;
    b = (y1 - a * x1) % 26;
    if (b < 0) b += 26;

    printf("Calculated keys: a = %d, b = %d\n", a, b);

    affineDecrypt(ciphertext, a, b);
    printf("Decrypted text: %s\n", ciphertext);

    return 0;
}

