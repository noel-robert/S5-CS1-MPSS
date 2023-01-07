// Assembler Pass 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



// between `m` and `n` (excluding `n`)
const char* extractFromBYTE(char src[25], int m, int n) {
    int len = n - m;
    char *dest = (char*)malloc(sizeof(char) * (len + 1));
 
    for (int i = m; i < n && (*(src + i) != '\0'); i++) {
        *dest = *(src + i);
        dest++;
    }
 
    // null-terminate the destination string
    *dest = '\0';
 
    return dest - len;
}

const char* appendZeroes(char operand[25]) {
    char *result = (char*)malloc(sizeof(char) * 5);
    strcpy(result, operand);

    char ch = '0';    
    strrev(result);
    while (strlen(result) < 4) strncat(result, &ch, 1);
    strrev(result);

    return result;
}

const char* searchSYMTAB(char searchValue[25]) {
	char label[25];
	char address[25];
    char *return_value = malloc(sizeof(char) * 8);
	
	FILE *symtab_file;
	symtab_file = fopen("symtab.txt", "r");
	while(!feof(symtab_file)) {
		fscanf(symtab_file, "%s %s", label, address);
		if (strcmp(label, searchValue) == 0) {
			fclose(symtab_file);
            strcpy(return_value, address);
			return return_value;
		}
	}

    // if not found, there is chance of BUFFER,X case
    char *str1 = malloc(sizeof(char) * 8);
    char *str2 = malloc(sizeof(char) * 8);
    int i;
    for (i=0; i<strlen(searchValue); i++) if (searchValue[i] == ',') break;
    str1 = extractFromBYTE(searchValue, 0, i);
    str2 = extractFromBYTE(searchValue, i+1, strlen(searchValue));

    // not actually like this, also need to add X to this
    if (strcmp(searchSYMTAB(str1), "false") != 0) return searchSYMTAB(str1);
    else if (strcmp(searchSYMTAB(str2), "false") != 0) return searchSYMTAB(str2);

	fclose(symtab_file);
	return "false";
}

const char* searchOPTAB(char searchValue[25]) {
	char opcode[25];
	char mnemonic_code[25];
    char *return_value = malloc(sizeof(char) * 8);
	
	FILE *optab_file;
	optab_file = fopen("optab.txt", "r");
	while(!feof(optab_file)) {
		fscanf(optab_file, "%s %s", opcode, mnemonic_code);
		if (strcmp(searchValue, opcode) == 0) {
			fclose(optab_file);
			strcpy(return_value, mnemonic_code);
			return return_value;
		}
	}
	fclose(optab_file);
	return "false";
}




/*
assembly listing - intermediate program with opcode
object program - ...^...^...^...
*/
int main() {
    FILE *intermediate_file;
    FILE *assembly_listing_file, *object_program;

    char label[25], opcode[25], operand[25], LOCCTR[25];
    char length_in_string[25], starting_adderss[25];
    char machineCode_generated[25];

    // clear object_program and listing file
    object_program = fopen("object_program.txt", "w");
	fprintf(object_program, "%s", "");
	fclose(object_program);
    assembly_listing_file = fopen("assembly_listing_file.txt", "w");
    fprintf(assembly_listing_file, "%s", "");
    fclose(assembly_listing_file);


    intermediate_file = fopen("intermediate_file.txt", "r");
    fscanf(intermediate_file, "%s%s%s", label, opcode, operand);

    if (strcmp(opcode, "START") == 0) {
        FILE *progLength;         
	    progLength = fopen("length.txt", "r");
	    fscanf(progLength, "%s", length_in_string);
	    fclose(progLength);

        strcpy(starting_adderss, operand);
        object_program = fopen("object_program.txt", "a");
        fprintf(object_program, "H^%s^%s^%s\n", label, starting_adderss, length_in_string);
        fprintf(object_program, "T^%s^%s", operand, length_in_string);
        fclose(object_program);

        assembly_listing_file = fopen("assembly_listing_file.txt", "a");
        fprintf(assembly_listing_file, "%s\t\t%s\t\t%s\t\t%s", starting_adderss, label, opcode, operand);
        fclose(assembly_listing_file);

        fscanf(intermediate_file, "%s%s%s%s", LOCCTR, label, opcode, operand);
    }


    int recordLength = 0;
    while(!feof(intermediate_file)) {
        strcpy(machineCode_generated, "");
        if (label[0] != '.' || LOCCTR[0] != '.') {
            if (strcmp(searchOPTAB(opcode), "false") != 0) {
                strcat(machineCode_generated, searchOPTAB(opcode)); // assemble object code instruction
                if (strcmp(operand, "**") != 0) { 
                    if (strcmp(searchSYMTAB(operand), "false") != 0) {
                        strcat(machineCode_generated, searchSYMTAB(operand));
                    } else {
                        // store  0 as operand address
                        // set error flag -> undefined symbol
                    }
                } else {
                    // store 0 as operand address
                }
                
            } else if (strcmp(opcode, "BYTE") == 0) {
                // convert constant to object code
                strcpy(machineCode_generated, extractFromBYTE(operand, 2, 4));
            } else if (strcmp(opcode, "WORD") == 0) {
                /* TODO: add excess zeroes to opcode */
                strcpy(machineCode_generated, appendZeroes(operand));
            } 


        }

        if (strcmp(opcode, "RESB") != 0 && strcmp(opcode, "RESW") != 0) {
            object_program = fopen("object_program.txt", "a");
            fprintf(object_program, "^%s", machineCode_generated);
            fclose(object_program);
        }

        assembly_listing_file = fopen("assembly_listing_file.txt", "a");
        fprintf(assembly_listing_file, "\n%s\t\t%s\t\t%s\t\t%s\t\t%s", LOCCTR, label, opcode, operand, machineCode_generated);
        fclose(assembly_listing_file);

        // for debugging
        // printf("\n{%s(%s) + %s(%s) -> %s},", opcode, searchOPTAB(opcode), operand, searchSYMTAB(operand), machineCode_generated);

        fscanf(intermediate_file, "%s%s%s%s", LOCCTR, label, opcode, operand);
    }
    // printf("%s\t%s\t%s\t%s", LOCCTR, label, opcode, operand);
    // printf("%s\t%s\t%s\t%s", LOCCTR, label, opcode, operand);



    /*TODO: last text record to object program*/

    object_program = fopen("object_program.txt", "a");
    fprintf(object_program, "\nE^%s", starting_adderss);
    fclose(object_program);

    // write last listing line 
    assembly_listing_file = fopen("assembly_listing_file.txt", "a");
    fprintf(assembly_listing_file, "\n%s\t\t%s\t\t%s\t\t%s", LOCCTR, label, opcode, operand);
    fclose(assembly_listing_file);

    fclose(intermediate_file);
    return 0;
}