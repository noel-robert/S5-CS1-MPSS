// Experiment No. 4
// One Pass Assembler
// work on this file to dubug

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


struct node {
	char data[25];	// store opcode or address

	struct node link;
};

// between `m` and `n` (excluding `n`)
const char* extract(char src[25], int m, int n) {
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

const char* stringrev(char source[25]) {
    char *result = (char*)malloc(sizeof(char) * 5);
    strcpy(result, source);

    if (!result || ! *result) return result;

    int i = 0, j = strlen(result) - 1;
    char ch;

    while (i < j) {
        ch = result[i];
        result[i] = result[j];
        result[j] = ch;
        i++, j--;
    }

    return result;
}

const char* appendZeroes(char operand[25]) {
    char *result = (char*)malloc(sizeof(char) * 5);
    strcpy(result, operand);

    char ch = '0';    
    stringrev(result);
    while (strlen(result) < 6) strncat(result, &ch, 1);
    stringrev(result);

    return result;
}

const char* searchSYMTAB(char searchValue[25]) {
	char label[25];
	char address[25];
    char *return_value = malloc(sizeof(char) * 25);
	
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
    char *str1 = malloc(sizeof(char) * 25);
    char *str2 = malloc(sizeof(char) * 25);
    int i;
    for (i=0; i<strlen(searchValue); i++) {
		if (searchValue[i] == ',') {
			strcpy(str1, extract(searchValue, 0, i));
			strcpy(str2, extract(searchValue, i+1, strlen(searchValue)));
			// printf("%s, %s\n", str1, str2);


			// not actually like this, also need to add X to this
			if (strcmp(searchSYMTAB(str1), "false") != 0) return searchSYMTAB(str1);
			else if (strcmp(searchSYMTAB(str2), "false") != 0) return searchSYMTAB(str2);

			break;
		}
	}

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


int main() {
	FILE *input_file, *assembly_file, *symtab_file;

	char label[25], opcode[25], operand[25];
	char machineCode[25];
	int startingAddress, LOCCTR;

	// clears assembly_file file, symtab and length on starting the program
	assembly_file = fopen("assembly_file.txt", "w");
	fprintf(assembly_file, "%s", "");
	fclose(assembly_file);
	symtab_file = fopen("symtab.txt", "w");
	fprintf(symtab_file, "%s", "");
	fclose(symtab_file);

	
	input_file = fopen("input_file.txt", "r");
	fscanf(input_file, "%s\t%s\t%s", label, opcode, operand);
	
	if (strcmp(opcode, "START") == 0) {
		startingAddress = (int)strtol(operand, NULL, 16);
		LOCCTR = startingAddress;
		
		assembly_file = fopen("assembly_file.txt", "a");
		fprintf(assembly_file, "%s\t\t%s\t\t%s\n", label, opcode, operand);
		fclose(assembly_file);

		fscanf(input_file, "%s\t%s\t%s", label, opcode, operand);
	} else {
		LOCCTR = 0;
	}

	
	while (strcmp(opcode, "END") != 0) {
		// write line to assembly_file
		assembly_file = fopen("assembly_file.txt", "a");
		fprintf(assembly_file, "%x\t\t%s\t\t%s\t\t%s", LOCCTR, label, opcode, operand);
		fclose(assembly_file);
		
		// set machine code string to null
		strcpy(machineCode, "");

		// insert into symtab
		if (strcmp(label, "**") != 0) {	// if symbol in LABEL field
			if (strcmp(searchSYMTAB(label), "false") != 0) {	// search SYMTAB for label
				/*TODO: if label is in symtab, but address is ****, 
				then we need to update address with locctr we get here*/
				/* how??
				1. open as read and set something like offset to 0. offset counts offset value for fseek()
				2. when label occurred, add 1 (for space, check if length of lable needs to be added) to offset
				3. there, use fseek to insert
				*/

				/*if label is in symtab, and address is not ***** then set error flag {duplicate symbol}*/
			} else {
				// insert to SYMTAB, as value not present already
				symtab_file = fopen("symtab.txt", "a");
				fprintf(symtab_file, "%s\t%x\n", label, LOCCTR);
				fclose(symtab_file);
			}
		}

		// search optab for opcode
		if (strcmp(searchOPTAB(opcode), "false") != 0) { 
			strcat(machineCode, searchOPTAB(opcode));
			LOCCTR += 3;  
		} 
		else if (strcmp(opcode, "WORD") == 0) { 
			strcpy(machineCode, appendZeroes(operand));
			LOCCTR += 3; 
		}	
		else if (strcmp(opcode, "RESW") == 0) {	LOCCTR += 3*atoi(operand); }
		else if (strcmp(opcode, "RESB") == 0) {	LOCCTR += atoi(operand); }
		else if (strcmp(opcode, "BYTE") == 0) {	
			strcpy(machineCode, extract(operand, 2, 4));

			int length = strlen(operand);
			length -= 3;
			if (operand[0] == 'X') { length /= 2; } 
			else if (operand[0] == 'c') { /*NothingHappensToLength*/ }
			else { /*NothingHappensHere*/ }
			LOCCTR += length;
		} else { /*Set error flag - invalid error code*/ }


		if (strcmp(opcode, "RESB") != 0 && strcmp(opcode, "RESW") != 0) {
			// take from symtab
			if (strcmp(searchSYMTAB(operand), "false") != 0) { strcat(machineCode, searchSYMTAB(operand)); }
			else if (strcmp(opcode, "WORD") == 0 || strcmp(opcode, "BYTE") == 0) { /*alreadyDid*/ }
			else { 
				strcat(machineCode, "----");

				/* TODO: add to symtab operand, ----*/
				symtab_file = fopen("symtab.txt", "a");
				fprintf(symtab_file, "%s\t%s\n", operand, "----");
				fclose(symtab_file);
			}

			// write machine code to assembly_file
			assembly_file = fopen("assembly_file.txt", "a");
			fprintf(assembly_file, "\t\t%s\n", machineCode);
			fclose(assembly_file);
		} else {
			assembly_file = fopen("assembly_file.txt", "a");
			fprintf(assembly_file, "\n");
			fclose(assembly_file);
		}

		// read next input line
		fscanf(input_file, "%s\t%s\t%s", label, opcode, operand);
	}

	// LOCCTR = "**";
	// write last line to assembly_file file
	assembly_file = fopen("assembly_file.txt", "a");
	fprintf(assembly_file, "%s\t\t%s\t\t%s\t\t%s", "**", label, opcode, operand);
	fclose(assembly_file);

	// save locctr - starting address as program length
	FILE *progLength;
	progLength = fopen("length.txt", "w");
	fprintf(progLength, "%x", LOCCTR-startingAddress);
	fclose(progLength);
	
	fclose(input_file);
}