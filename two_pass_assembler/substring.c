#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int substrcmp(char subString[25], char mainString[25]) {
    int subStringLength = strlen(subString);
    int mainStringLength = strlen(mainString);

    if (subStringLength > mainStringLength) return 1;

    int flag = 1;
    for (int i=0; i<=(mainStringLength-subStringLength); i++) {
        for (int j=i; j<i+subStringLength; j++) {
            flag = 0;
            if (mainString[j] != subString[j-i]) { flag = 1; break; }
        }

        if(flag == 0) break;
    }

    return flag;
} 

int main() {
    if (substrcmp("BUFFER", "BUFFER,X") == 0) printf("true"); else printf("false");

    return 0;
}