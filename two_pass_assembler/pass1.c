// Experiment No. 3
// Assembler Pass 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


bool searchSYMTAB(char searchValue[25]) {
	char label[25];
	char address[25];
	
	FILE *symtab_file;
	symtab_file = fopen("symtab.txt", "r");
	while(!feof(symtab_file)) {
		fscanf(symtab_file, "%s %s", label, address);
		if (strcmp(searchValue, label) == 0) {
			fclose(symtab_file);
			return true;
		}
	}
	fclose(symtab_file);
	return false;
}

bool searchOPTAB(char searchValue[25]) {
	char opcode[25];
	char mnemonicCode[25];
	
	FILE *optab_file;
	optab_file = fopen("optab.txt", "r");
	while(!feof(optab_file)) {
		fscanf(optab_file, "%s %s", opcode, mnemonicCode);
		if (strcmp(searchValue, opcode) == 0) {
			fclose(optab_file);
			return true;
		}
	}
	fclose(optab_file);
	return false;
}


int main() {
	FILE *input_file, *intermediate_file, *symtab_file;

	char label[25], opcode[25], operand[25];
	int startingAddress, LOCCTR;

	// clears intermediate file and symtab on starting the program
	intermediate_file = fopen("intermediate_file.txt", "w");
	fprintf(intermediate_file, "%s", "");
	fclose(intermediate_file);
	symtab_file = fopen("symtab.txt", "w");
	fprintf(symtab_file, "%s", "");
	fclose(symtab_file);
	
	input_file = fopen("input_file.txt", "r");
	fscanf(input_file, "%s\t%s\t%s", label, opcode, operand);
	
	if (strcmp(opcode, "START") == 0) {
		startingAddress = (int)strtol(operand, NULL, 16);
		LOCCTR = startingAddress;
		
		intermediate_file = fopen("intermediate_file.txt", "a");
		fprintf(intermediate_file, "\t\t%s\t\t%s\t\t%s\n", label, opcode, operand);
		fclose(intermediate_file);

		fscanf(input_file, "%s\t%s\t%s", label, opcode, operand);
	} else {
		LOCCTR = 0;
	}

	
	while (strcmp(opcode, "END") != 0) {
		// write line to intermediate file
		intermediate_file = fopen("intermediate_file.txt", "a");
		fprintf(intermediate_file, "%x\t\t%s\t\t%s\t\t%s\n", LOCCTR, label, opcode, operand);
		fclose(intermediate_file);
		
		if (strcmp(label, "**") != 0) {	// if symbol in LABEL field
			if (searchSYMTAB(label)) {	// search SYMTAB for label
				// set error flag
			} else {
				// insert to SYMTAB
				symtab_file = fopen("symtab.txt", "a");
				fprintf(symtab_file, "%s\t%x\n", label, LOCCTR);
				fclose(symtab_file);
			}
		}

		// search optab for opcode
		if (searchOPTAB(opcode)) { LOCCTR += 3;	} 
		else if (strcmp(opcode, "WORD") == 0) { LOCCTR += 3; }	
		else if (strcmp(opcode, "RESW") == 0) {	LOCCTR += 3*atoi(operand); }
		else if (strcmp(opcode, "RESB") == 0) {	LOCCTR += atoi(operand); }
		else if (strcmp(opcode, "BYTE") == 0) {	
			int length = strlen(operand);
			length -= 3;
			if (operand[0] == 'X') { length /= 2; } 
			else if (operand[0] == 'c') { /*NothingHappensToLength*/ }
			else { /*NothingHappensHere*/ }
			LOCCTR += length;
		} else { /*Set error flag - invalid error code*/ }

		// write line to intermediate file
		// intermediate_file = fopen("intermediate_file.txt", "a");
		// fprintf(intermediate_file, "%x %s %s %s\n", LOCCTR, label, opcode, operand);
		// fclose(intermediate_file);

		// read next input line
		fscanf(input_file, "%s\t%s\t%s", label, opcode, operand);
	}

	// LOCCTR = "**";
	// write last line to intermediate file
	intermediate_file = fopen("intermediate_file.txt", "a");
	fprintf(intermediate_file, "%s\t\t%s\t\t%s\t\t%s", "**", label, opcode, operand);
	fclose(intermediate_file);

	// save locctr - starting address as program length
	FILE *progLength;
	progLength = fopen("length.txt", "w");
	fprintf(progLength, "%x", LOCCTR-startingAddress);
	fclose(progLength);
	
	fclose(input_file);
}