#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Function to compute (base^exp) % mod using modular exponentiation
unsigned long long mod_exp(unsigned long long base, unsigned long long exp, unsigned long long mod) {
    unsigned long long result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

// Function to encrypt a single character using RSA
unsigned long long rsa_encrypt(char ch, unsigned long long e, unsigned long long n) {
    unsigned long long m = (unsigned long long)(ch - 'A'); // Map A-Z to 0-25
    return mod_exp(m, e, n);
}

int main() {
    unsigned long long e = 65537; // Example public exponent
    unsigned long long n = 4294967311; // Example large modulus, should be a product of two large primes

    char message[] = "HELLO"; // Example message to encrypt
    unsigned long long encrypted[sizeof(message)];

    printf("Original message: %s\n", message);
    printf("Encrypted message: ");
    for (int i = 0; i < strlen(message); i++) {
        encrypted[i] = rsa_encrypt(message[i], e, n);
        printf("%llu ", encrypted[i]);
    }
    printf("\n");

    return 0;
}
