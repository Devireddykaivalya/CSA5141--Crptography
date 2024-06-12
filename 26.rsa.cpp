#include <stdio.h>
#include <stdlib.h>

// Function to compute gcd
unsigned long long gcd(unsigned long long a, unsigned long long b) {
    while (b != 0) {
        unsigned long long t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// Function to compute modular inverse using Extended Euclidean Algorithm
unsigned long long mod_inverse(unsigned long long e, unsigned long long phi) {
    long long t = 0, new_t = 1, r = phi, new_r = e;
    while (new_r != 0) {
        long long quotient = r / new_r;
        long long temp = t;
        t = new_t;
        new_t = temp - quotient * new_t;
        temp = r;
        r = new_r;
        new_r = temp - quotient * new_r;
    }
    return t < 0 ? t + phi : t;
}

// Function to perform modular exponentiation
unsigned long long mod_exp(unsigned long long base, unsigned long long exp, unsigned long long mod) {
    unsigned long long result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

// Function to generate RSA keys
void generate_keys(unsigned long long *n, unsigned long long *e, unsigned long long *d) {
    unsigned long long p = 61, q = 53;
    *n = p * q;
    unsigned long long phi = (p - 1) * (q - 1);
    *e = 17;
    while (gcd(*e, phi) != 1) (*e)++;
    *d = mod_inverse(*e, phi);
}

// RSA encryption
unsigned long long rsa_encrypt(unsigned long long msg, unsigned long long e, unsigned long long n) {
    return mod_exp(msg, e, n);
}

// RSA decryption
unsigned long long rsa_decrypt(unsigned long long cipher, unsigned long long d, unsigned long long n) {
    return mod_exp(cipher, d, n);
}

int main() {
    unsigned long long n, e, d;
    generate_keys(&n, &e, &d);

    printf("Public Key: (e = %llu, n = %llu)\n", e, n);
    printf("Private Key: (d = %llu, n = %llu)\n", d, n);

    unsigned long long msg = 42;
    unsigned long long encrypted = rsa_encrypt(msg, e, n);
    unsigned long long decrypted = rsa_decrypt(encrypted, d, n);

    printf("Original: %llu\n", msg);
    printf("Encrypted: %llu\n", encrypted);
    printf("Decrypted: %llu\n", decrypted);

    // Key regeneration if private key is leaked
    generate_keys(&n, &e, &d);
    printf("\nNew Public Key: (e = %llu, n = %llu)\n", e, n);
    printf("New Private Key: (d = %llu, n = %llu)\n", d, n);

    encrypted = rsa_encrypt(msg, e, n);
    decrypted = rsa_decrypt(encrypted, d, n);

    printf("Original: %llu\n", msg);
    printf("Encrypted: %llu\n", encrypted);
    printf("Decrypted: %llu\n", decrypted);

    return 0;
}
