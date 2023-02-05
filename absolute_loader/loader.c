#include <stdio.h>
#include <stdlib.h>

#define MAX 150

int main() {
    char str[MAX];
    FILE *input_file, *output_file;
    
    input_file = fopen("input_file.txt", "r");
    output_file = fopen("output_file.txt", "w");

    int programAddress = 0;
    fprintf(output_file, "Program loaded into memory with addresses:");

    while (fgets(str, MAX, input_file) != NULL) {
        if (str[0] == 'H') {
            sscanf(str+9, "%x", &programAddress);
            continue;
        }

        if (str[0] == 'E') break;

        int recordAddress;
        sscanf(str, "T %x", &recordAddress);
        while(programAddress < recordAddress) {
            fprintf(output_file, "\n%X\txx", programAddress++);
        }

        char *ptr = str + 12;
        for (int i=0; ptr[i]!='\0'&&ptr[i]!='\n'; i+=2) {
            if (ptr[i] == ' ') i++;
            fprintf(output_file, "\n%X\t%c%c", programAddress++, ptr[i], ptr[i++]);
        }

    }
}