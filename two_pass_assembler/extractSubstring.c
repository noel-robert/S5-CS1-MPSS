#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* substr(char str[50], int limiter) {
    char *substring = (char*)malloc(sizeof(char)*50);
    
    int i;
    for (i=0; i<limiter; i++)   substring[i] = str[i];
    str[i] = '\0';
    
    return substring;
}

int main()
{
    char str[50] = "Hello, I am Noel John Robert";
    char substring[50];
    
    strcpy(substring, substr(str, 2));
    printf("%s", substring);
    
    return 0;
}