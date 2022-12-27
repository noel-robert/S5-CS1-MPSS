// Assembler Pass 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


const char* searchSYMTAB(char searchValue[25]) {
	char label[25];
	char address[25];
	
	FILE *symtab_file;
	symtab_file = fopen("symtab.txt", "r");
	while(!feof(symtab_file)) {
		fscanf(symtab_file, "%s %s", label, address);
		if (strcmp(searchValue, label) == 0) {
			fclose(symtab_file);
			return "true";
		}
	}
	fclose(symtab_file);
	return "false";
}

const char* searchOPTAB(char searchValue[25]) {
	char opcode[25];
	char mnemonicCode[25];
	
	FILE *optab_file;
	optab_file = fopen("optab.txt", "r");
	while(!feof(optab_file)) {
		fscanf(optab_file, "%s %s", opcode, mnemonicCode);
		if (strcmp(searchValue, opcode) == 0) {
			fclose(optab_file);
			return "true";
		}
	}
	fclose(optab_file);
	return "false";
}


// assembly listing - intermediate program with opcode
// object program - ...^...^...^...
int main() {
    FILE *intermediate_file, *symtab_file, *object_program;

    char label[25], opcode[25], operand[25], LOCCTR[25];
    char length_in_string[25], starting_adderss[25];

    // clear object_program file
    object_program = fopen("object_program.txt", "w");
	fprintf(object_program, "%s", "");
	fclose(object_program);

    intermediate_file = fopen("intermediate_file.txt", "r");
    fscanf(intermediate_file, "%s%s%s", label, opcode, operand);

    if (strcmp(opcode, "START") == 0) {
        /*TODO - write listing line*/

        FILE *progLength;         
	    progLength = fopen("length.txt", "r");
	    fscanf(progLength, "%s", length_in_string);
        // int length_in_hex;
        // length_in_hex = (int)strtol(length_in_string, NULL, 16);
        // printf("%x", length_in_hex);
	    fclose(progLength);

        strcpy(starting_adderss, operand);
        object_program = fopen("object_program.txt", "a");
        fprintf(object_program, "H^%s^%s^%s\n", label, starting_adderss, length_in_string);
        fprintf(object_program, "T^%s^%s^", operand, length_in_string);
        // printf("H^%s^%s^%s\n", label, operand, length_in_string);
        fclose(object_program);

        fscanf(intermediate_file, "%s%s%s%s", LOCCTR, label, opcode, operand);
    }

    // object_program = fopen("object_program.txt", "a");
    // fprintf(object_program, "T^%s^%s^", operand, length_in_string);
    // fclose(object_program);

    while (!feof(intermediate_file)) {
        // if not comment line
        if (label[0] != '.') {
            if (strcmp(searchOPTAB(opcode), "false") != 0) {
                if (strcmp(operand, "**") != 0) { /*TODO: if there is a symbol in operand field*/
                    if (strcmp(searchSYMTAB(operand), "false") != 0) {
                        // store symbol value of operand address
                    } else {
                        // store  0 as operand address
                        // set error flag -> undefined symbol
                    }
                } else {
                    // store 0 as operand address
                }
                // assemble object code instruction
            } 
            else if (strcmp(opcode, "BYTE") == 0 || strcmp(opcode, "WORD") == 0) {
                // convert constant to object code
            }
            
            /*object code does not fit into current text record -> 
                write Text record to object program
                initialize new Text record
            */

            /*add obj code to text record*/
        }

        /*TODO: write listing line*/

        fscanf(intermediate_file, "%s%s%s%s", LOCCTR, label, opcode, operand);
    }

    /*TODO: last text record to object program*/

    object_program = fopen("object_program.txt", "a");
    fprintf(object_program, "\nE^%s", starting_adderss);
    fclose(object_program);

    /* write last listing line */

    fclose(intermediate_file);
    return 0;
}