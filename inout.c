#include <stdio.h>
int main() {
    static char s[255];
    scanf("%s", s);
    for(char *c = s; *(c) != '\0'; c++) printf("%d ", *c);
    return 0;
}