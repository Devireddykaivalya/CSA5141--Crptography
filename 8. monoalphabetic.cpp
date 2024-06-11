#include<stdio.h>
char monocipher_encr(char);
char alpha[27][3] = { { 'a', 't' }, { 'b', 'h' }, { 'c', 'e' }, { 'd', 'q' }, {
        'e', 'u' }, { 'f', 'i' }, { 'g', 'c' }, { 'h', 'k' }, { 'i', 'b' }, {
        'j', 'r' }, { 'k', 'o' }, { 'l', 'w' }, { 'm', 'n' }, { 'n', 'f' }, {
        'o', 'x' }, { 'p', 'j' }, { 'q', 'm' }, { 'r', 'p' }, { 's', 's' }, {
        't', 'v' }, { 'u', 'l' }, { 'v', 'a' }, { 'w', 'z' }, { 'x', 'y' }, {
        'y', 'd' }, { 'z', 'g' } };
char str[200];
int main() {
    char str[200], str2[200];
    int i;
    printf("\n enter string = ");
    gets(str);
    for (i = 0; str[i]; i++) {
        str2[i] = monocipher_encr(str[i]);
    }
    str2[i] = '\0';
    printf("\n before decryption = %s", str);
    printf("\n after decryption = %s\n", str2);
}
char monocipher_encr(char a) {
    int i;
    for (i = 0; i < 27; i++) {
        if (a == alpha[i][0])
            break;
    }
    return alpha[i][1];
}
