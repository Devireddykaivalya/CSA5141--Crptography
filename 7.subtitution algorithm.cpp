#include <stdio.h>
#include <string.h>

#define MAX_TEXT_LENGTH 256


void countFrequencies(char *text, int *freq) {
    for (int i = 0; i < strlen(text); i++) {
        freq[(unsigned char)text[i]]++;
    }
}


int findMaxFrequency(int *freq, int exclude) {
    int maxFreq = 0;
    int index = -1;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > maxFreq && i != exclude) {
            maxFreq = freq[i];
            index = i;
        }
    }
    return index;
}

int main() {
    char ciphertext[MAX_TEXT_LENGTH] = "53‡‡†305))6*;4826)4‡.)4‡);806*;48+860))85;;]8*;:$*8+83"
                                       "(88)5*†;46(;88*96*7;8)‡(;485);5+2:(;4956*2(5-4)88*"
                                       ";4069285);)6+8)4‡‡;1($9;48081;8:81;48+85;4)485+528806*81"
                                       "($9;48;(88;4($234;48)4‡;161;:188;‡?";
    
    int freq[256] = {0}; 

    
    countFrequencies(ciphertext, freq);

    
    int maxFreqIndex1 = findMaxFrequency(freq, -1);  
    int maxFreqIndex2 = findMaxFrequency(freq, maxFreqIndex1);  
    int maxFreqIndex3 = findMaxFrequency(freq, maxFreqIndex2);  

   
    char mapping[256] = {0};
    mapping[maxFreqIndex1] = 'e';
    mapping[maxFreqIndex2] = 't';
    mapping[maxFreqIndex3] = 'h';

    
    
    printf("Ciphertext:\n%s\n", ciphertext);
    printf("Deciphered text (initial guess):\n");

    for (int i = 0; i < strlen(ciphertext); i++) {
        char ch = ciphertext[i];
        if (mapping[(unsigned char)ch]) {
            printf("%c", mapping[(unsigned char)ch]);
        } else {
            printf("%c", ch); 
        }
    }
    
    printf("\n");

    

    return 0;
}
