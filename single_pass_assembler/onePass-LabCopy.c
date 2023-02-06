// One Pass Assembler
// Noel John Robert
// B20CS1147

/*
TODO: to linked list, add operand and offset value
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct node {
	char data[25];
	struct node *link;
};

const char* extract (char src[10], int m, int n) {
	int len = n - m;
	char *dest = (char*) malloc(sizeof(char) * (len+1));
	
	for (int i=m; i<n && (*(src+i) != '\0'); i++) {
		*dest = *(src+i);
		dest++;
	}
	*dest = '\0';
	
	return dest-len;
}

const char* stringrev(char source[10]) {
	char *result = (char*) malloc(sizeof(char) * 7);
	strcpy(result, source);
	
	if (!result || ! *result) return result;
	
	int i = 0, j = strlen(result)-1;
	char ch;
	
	while (i<j) {
		ch = result[i];
		result[i] = result[j];
		result[j] = ch;
		i++, j--;
	}
	
	return result;
}

const char* appendZero(char operand[10]) {
	char *result = (char*) malloc(sizeof(char) * 7);
	strcpy(result, operand);
	
	char ch = '0';
	strcpy(result, stringrev(result));
	while (strlen(result) < 6) strncat(result, &ch, 1);
	strcpy(result, stringrev(result));
	
	return result;
}

const char* searchSYMTAB (char searchValue[10]) {
	FILE *symtab;
	symtab = fopen("symtab.txt", "r");
	char label[10], locctr[10];
	char *returnValue = malloc(sizeof(char) * 8);

	while(!feof(symtab)) {
		fscanf(symtab, "%s %s", label, locctr);
		if(strcmp(label, searchValue) == 0) { fclose(symtab); strcpy(returnValue, locctr); return returnValue; }
	}
	
	char *str1 = malloc(sizeof(char) * 8);
	char *str2 = malloc(sizeof(char) * 8);
	int i;
	for (i=0; i<strlen(searchValue); i++) {
		if(searchValue[i] == ',') {
			strcpy(str1, extract(searchValue, 0, i));
			strcpy(str2, extract(searchValue, i+1, strlen(searchValue)));
	
			if (strcmp(searchSYMTAB(str1), "false") != 0) return searchSYMTAB(str1);
			else if (strcmp(searchSYMTAB(str2), "false") != 0) return searchSYMTAB(str2);
			
			break;
		}
		
	}
	
	fclose(symtab); 
	return "false";
}

void updateSYMTAB(char parameter_label, char parmeter_locctr) {
	FILE *symtab;
	symtab = fopen("symtab.txt", "r+");
	char label[10], locctr[10];

	while (!feof(symtab)) {
		fscanf(symtab, "%s %s", label, locctr);
		if(strcmp(label, parameter_label) == 0) {
			fseek(symtab, -strlen(locctr), SEEK_CUR);
			fputs(parmeter_locctr, symtab);
			break;
		}
	}
}

const char* searchOPTAB(char searchValue[10]) {
	FILE *optab;
	optab = fopen("optab.txt", "r");
	char opcode[10], mnemonic_code[10];
	char *returnValue = malloc(sizeof(char) * 8);
	
	while(!feof(optab)) {
		fscanf(optab, "%s %s", opcode, mnemonic_code);
		if(strcmp(searchValue, opcode) == 0) { fclose(optab); strcpy(returnValue, mnemonic_code); return returnValue; }
	}
	
	fclose(optab); 
	return "false";
}



int main() {
	// open files necessary for execution
	FILE *input_file, *symtab_file, *assembly_file;
	
	// clear assembly file, symtab and length on starting program
	assembly_file = fopen("assembly_file.txt", "w");
	fprintf(assembly_file, "%s", "");
	fclose(assembly_file);
	symtab_file = fopen("symtab.txt", "w");
	fprintf(symtab_file, "%s", "");
	fclose(symtab_file);
	
	// begin
	char label[10], opcode[10], operand[10];
	char machineCode[10];
	int startingAddress, LOCCTR;
	input_file = fopen("input_file.txt", "r");
	int offset = 0;
	struct node *head = malloc(sizeof(struct node));
	head->link = NULL;
	
	
	// read first input line
	fscanf(input_file, "%s\t%s\t%s", label, opcode, operand);

	if(strcmp(opcode, "START") == 0) {
		startingAddress = (int)strtol(operand, NULL, 16);
		LOCCTR = startingAddress;
		
		assembly_file = fopen("assembly_file.txt", "a");
		fprintf(assembly_file, "%s\t%s\t%s\n", label, opcode, operand);
		offset += strlen(label) + 1 + strlen(opcode) + 1 + strlen(operand);
		fclose(assembly_file);
		
		// read next input line
		fscanf(input_file, "%s %s %s", label, opcode, operand);
	} else {
		LOCCTR = 0;
	}
	
	char fileName[10];
	strcpy(fileName, label);
	
	while(strcmp(opcode, "END") != 0) {
		// write line to assembly file
		assembly_file = fopen("assembly_file.txt", "a");
		fprintf(assembly_file, "%x\t%s\t%s\t%s", LOCCTR, label, opcode, operand);
		offset += 4 + 1 + strlen(label) + 1 + strlen(opcode) + 1 + strlen(operand);
		fclose(assembly_file);
		

		strcpy(machineCode, "");	// set machine code to null
	
		if(strcmp(label, "**") != 0) {	
			if(strcmp(searchSYMTAB(label), "false") != 0) {
				if(strcmp(searchSYMTAB(label), "null") == 0) {
					updateSYMTAB(label, LOCCTR);
					/*TODO: updateOutputFile(label); */
				}
				/*TODO-DONE: if label in symtab but address is ----,
				then we need to update address with locctr we have here. steps - 
				1. open as read and set offset for seek to 0
				2. when label found add 1 to offset
				3. there, use fseek to insert
				*/
			} else {
				// insert into symtab
				symtab_file = fopen("symtab.txt", "a");
				fprintf(symtab_file, "%s\t%x\n", label, LOCCTR);
				fclose(symtab_file);
			}
		}
	
		// search optab for opcode
		if (strcmp(searchOPTAB(opcode), "false") != 0) {
			strcat(machineCode, searchOPTAB(opcode));
			
			// search symtab for operand address
			if (strcmp(searchSYMTAB(operand), "false") != 0) {
				if (strcmp(searchSYMTAB(operand), "null") != 0) { strcat(machineCode, searchSYMTAB(operand)); }
				else { /*insert to ll offset*/ }
			} else {
				symtab_file = fopen("symtab.txt", "a");
				fprintf(symtab_file, "%s\t%s\n", operand, "null");
				fclose(symtab_file);
			}
			
			LOCCTR += 3;
		} 
		else if (strcmp(opcode, "WORD") == 0) {
			strcat(machineCode, appendZero(operand));
			LOCCTR += 3;
		}
		else if (strcmp(opcode, "RESW") == 0) { LOCCTR += 3*atoi(operand); }
		else if (strcmp(opcode, "RESB") == 0) { LOCCTR += atoi(operand); }
		else if (strcmp(opcode, "BYTE") == 0) {
			strcpy(machineCode, extract(operand, 2, 4));
			
			int length = strlen(operand);
			length -= 3;
			
			if (operand[0] == 'X') { LOCCTR += length/2; }
			else if (operand[0] == 'C') { LOCCTR += length; }
			else { /*error*/ }
		}
		else { /*Invalid operation code*/ }
	
		if (strcmp(opcode, "RESB") != 0 && strcmp(opcode, "RESW") != 0) {
			// take from symtab
			if (strcmp(searchSYMTAB(operand), "false") != 0) { strcat(machineCode, searchSYMTAB(operand)); }
			else if (strcmp(opcode, "WORD") == 0 || strcmp(opcode, "BYTE") == 0) {}
			else { strcat(machineCode, "----"); }
			
			// write machine code to assembly file
			assembly_file = fopen("assembly_file.txt", "a");
			fprintf(assembly_file, "\t%s\n", machineCode);
			fclose(assembly_file);	
		} else {
			assembly_file = fopen("assembly_file.txt", "a");
			fprintf(assembly_file, "\n");
			fclose(assembly_file);
		}
		
		// read next input line
		fscanf(input_file, "%s %s %s", label, opcode, operand);
	}
	
	// write last line to intermediate file
	assembly_file = fopen("assembly_file.txt", "a");
	fprintf(assembly_file, "%s\t%s\t%s\t%s\n", "**", label, opcode, operand);
	fclose(assembly_file);
	
	// save locctr-startingAddress as program length
	/*
	FILE *progLength;
	progLength = fopen("length.txt", "w");
	fprintf(progLength, "%x", (LOCCTR-startingAddress));
	fclose(progLength);
	*/
	
	printf("%d\n", offset);
	fclose(input_file);
	
	return 0;
}
