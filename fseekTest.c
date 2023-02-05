// testing fseek function
// https://man7.org/linux/man-pages/man3/fseek.3.html

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

int main() {
    FILE *fseekTestDocument;
    int returnPos;

    fseekTestDocument = fopen("fseekTestDocument.txt", "r");

    fseek(fseekTestDocument, 0, SEEK_END);
    returnPos = ftell(fseekTestDocument);
    fclose(fseekTestDocument);


    fseekTestDocument = fopen("fseekTestDocument.txt", "r+");
    fseek(fseekTestDocument, 11, SEEK_SET);
    fputs("trebor nhoj leon", fseekTestDocument);

    fseek(fseekTestDocument, returnPos, SEEK_SET);

    fprintf(fseekTestDocument, "\nWait, who did that??");
    

    fclose(fseekTestDocument);
    return 0;
}