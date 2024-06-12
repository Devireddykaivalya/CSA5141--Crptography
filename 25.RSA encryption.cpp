#include <stdio.h>

// Function to compute the greatest common divisor (GCD) of two numbers
int gcd(int a, int b) {
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

// Function to simulate RSA encryption
int rsa_encrypt(int plaintext, int e, int n) {
    return (int)((long long)plaintext * plaintext) % n; // Simplified RSA encryption
}

int main() {
    int n = 323; // Modulus n = p * q, where p and q are prime
    int e = 7;   // Public key exponent
    int plaintext = 46; // Plain text block

    printf("Original plaintext: %d\n", plaintext);

    // Check if plaintext shares a common factor with n
    if (gcd(plaintext, n) > 1) {
        printf("Plaintext has a common factor with n.\n");
        printf("This information might be useful for factoring n.\n");
    } else {
        printf("Plaintext does not have a common factor with n.\n");
    }

    // Encrypt plaintext using RSA
    int ciphertext = rsa_encrypt(plaintext, e, n);
    printf("Encrypted ciphertext: %d\n", ciphertext);

    return 0;
}
