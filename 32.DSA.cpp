#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void generate_dsa_signature(const char *message, char *signature) {
    // Simulate a random value k
    int k = rand() % 100 + 1;
    sprintf(signature, "DSA(%s,k=%d)", message, k);
}

void generate_rsa_signature(const char *message, char *signature) {
    sprintf(signature, "RSA(%s)", message);
}

int main() {
    const char *message = "Hello, world!";
    char dsa_sig1[256], dsa_sig2[256], rsa_sig[256];

    // Initialize random number generator
    srand(time(NULL));

    // Generate DSA signatures
    generate_dsa_signature(message, dsa_sig1);
    generate_dsa_signature(message, dsa_sig2);

    // Generate RSA signature
    generate_rsa_signature(message, rsa_sig);

    // Print signatures
    printf("DSA Signature 1: %s\n", dsa_sig1);
    printf("DSA Signature 2: %s\n", dsa_sig2);
    printf("RSA Signature: %s\n", rsa_sig);

    return 0;
}

