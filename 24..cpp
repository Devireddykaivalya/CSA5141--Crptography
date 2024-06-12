#include <stdio.h>

// Function to compute gcd using the Extended Euclidean Algorithm
int extended_gcd(int a, int b, int *x, int *y) {
    if (a == 0) {
        *x = 0;
        *y = 1;
        return b;
    }

    int x1, y1;
    int gcd = extended_gcd(b % a, a, &x1, &y1);

    *x = y1 - (b / a) * x1;
    *y = x1;

    return gcd;
}

// Function to find the multiplicative inverse of e modulo phi(n)
int mod_inverse(int e, int phi) {
    int x, y;
    int gcd = extended_gcd(e, phi, &x, &y);

    if (gcd != 1) {
        // Multiplicative inverse doesn't exist if gcd is not 1
        return -1;
    } else {
        // Make x positive
        return (x % phi + phi) % phi;
    }
}

int main() {
    // Given public key
    int e = 31;
    int n = 3599;

    // Factorize n to find p and q
    int p = 59;
    int q = 61;

    // Calculate phi(n)
    int phi = (p - 1) * (q - 1); // phi = (59 - 1) * (61 - 1) = 58 * 60 = 3480

    // Calculate the private key d
    int d = mod_inverse(e, phi);

    if (d == -1) {
        printf("Multiplicative inverse does not exist.\n");
    } else {
        printf("Public Key (e, n): (%d, %d)\n", e, n);
        printf("Private Key (d, n): (%d, %d)\n", d, n);
    }

    return 0;
}
