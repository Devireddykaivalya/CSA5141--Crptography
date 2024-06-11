#include <stdio.h>
#include <math.h>

int main() {
    // Calculate the factorial of 25
    double factorial = 1;
    for (int i = 1; i <= 25; i++) {
        factorial *= i;
    }

    // Calculate log2(25!)
    double log2_factorial = log2(factorial);

    // Print the result
    printf("The number of possible keys for the Playfair cipher is approximately 2^%.2f\n", log2_factorial);

    return 0;
}

