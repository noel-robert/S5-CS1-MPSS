// single pass assembler


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct table {
  char operandName[25];
  int operandUsedAddress;
  int labelDefinedAddress;
} tab[100];

const char* stringrev(char source[10]) {
	char *result = (char*) malloc(sizeof(char) * 7);
	strcpy(result, source);
	
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
  FILE *input_file, *record_file, *symtab_file, *optab_file, *output_file;
  FILE *new_file;

  input_file = fopen("input.txt", "r");
  record_file = fopen("record.txt", "w");
  output_file = fopen("output.txt", "w");

  symtab_file = fopen("symtab.txt", "w");
  fclose(symtab_file);

  new_file = fopen("new_file.txt", "w");
  fclose(new_file);

  char label[10], opcode[10], operand[10], progName[10];
  char optab_opcode[10], optab_mnemonic[10], symtab_label[10], symtab_addr[10];
  int record_file_offset, output_file_offset;
  int LOCCTR, startAddress, is_operand_in_symtab;
  int tableTraversalCounter = 0;


  fscanf(input_file, "%s %s %X", label, opcode, &startAddress);


  if (strcmp(opcode, "START") == 0) {
    LOCCTR = startAddress;

    fprintf(record_file, "H^%s^%X^****\n", label, startAddress);
    fprintf(record_file, "T^%X^****^", startAddress);
    fprintf(output_file, "%s\t\t%s\t\t%X\n", label, opcode, startAddress);
    strcpy(progName, label);
    // printf("%s conversion started", progName);

    fscanf(input_file, "%s %s %s", label, opcode, operand);
  } else {
    LOCCTR = 0;
  }


  while (strcmp(opcode, "END") != 0) {
    fprintf(output_file, "%X\t\t%s\t\t%s\t\t%s\t\t", LOCCTR, label, opcode, operand);

    if (strcmp(label, "**") != 0) {
      symtab_file = fopen("symtab.txt", "r+");

      while (!feof(symtab_file)) {
        fscanf(symtab_file, "%s %s \n", symtab_label, symtab_addr);
        if (strcmp(symtab_label, label) == 0) {
          // if address in symtab is null, edit it
          if (strcmp(symtab_addr, "----") == 0) {
            int retAddr = ftell(output_file);

            // editing output and record files
            fscanf(symtab_file, "%d %d", &record_file_offset, &output_file_offset);
            fseek(output_file, output_file_offset, SEEK_SET);
            fprintf(output_file, "%X", LOCCTR);
            // fseek(record_file, record_file_offset, SEEK_SET);
            // fprintf(record_file, "%X^", LOCCTR);

            fseek(output_file, retAddr, SEEK_SET);

            for(int i=0; i<tableTraversalCounter; i++) {
              if(strcmp(tab[i].operandName, label) == 0) {
                tab[i].labelDefinedAddress = LOCCTR;

                new_file = fopen("new_file.txt", "a");
                fprintf(new_file, "T^%X^02^%X\n", tab[i].operandUsedAddress, tab[i].labelDefinedAddress);
                fclose(new_file);

                break;
              }
            }
 
          } else { fprintf(symtab_file, "%s\t\t%X\n", label, LOCCTR); }
        }
      }

      fprintf(symtab_file, "%s\t\t%X\n", label, LOCCTR);
      fclose(symtab_file);
    }

    // search optab for opcode
    int is_opcode_in_optab = 0;
    strcpy(optab_mnemonic, searchOPTAB(opcode));
    if(strcmp(optab_mnemonic, "false") != 0) {
      is_opcode_in_optab = 1;
      fprintf(record_file, "%s", optab_mnemonic);
      fprintf(output_file, "%s", optab_mnemonic);
    }
    

    if (is_opcode_in_optab == 1) {
      is_operand_in_symtab = 0;
      symtab_file = fopen("symtab.txt", "r");
      
      strcpy(symtab_addr, searchSYMTAB(operand));
      if(strcmp(symtab_addr, "false") != 0) {
        is_operand_in_symtab = 1;
        fprintf(record_file, "%s^", symtab_addr);
        fprintf(output_file, "%s", symtab_addr);
      }

      if (is_operand_in_symtab == 0) {
        fprintf(output_file, "----");
        fprintf(record_file, "----^");
        symtab_file = fopen("symtab.txt", "a");
        fprintf(symtab_file, "%s\t\t----\t%ld %ld\n", operand, ftell(record_file) - 5, ftell(output_file) - 4); // setting record_file and output_file offset values
        fclose(symtab_file);

        strcpy(tab[tableTraversalCounter].operandName, operand);
        tab[tableTraversalCounter].operandUsedAddress = LOCCTR + 1;
        tableTraversalCounter++;
      }
    }

    if (is_opcode_in_optab == 1) { LOCCTR = LOCCTR + 3; }
    else if (strcmp(opcode, "WORD") == 0) {
      fprintf(record_file, "%s^", appendZero(operand));
      fprintf(output_file, "%s", appendZero(operand));
      LOCCTR = LOCCTR + 3;
    }
    else if (strcmp(opcode, "RESW") == 0) { LOCCTR = 3 * atoi(operand) + LOCCTR; }
    else if (strcmp(opcode, "RESB") == 0) { LOCCTR = atoi(operand) + LOCCTR; }
    else if (strcmp(opcode, "BYTE") == 0) {
      fprintf(record_file, "%s^", extract(operand, 2, 4));
      fprintf(output_file, "%s", extract(operand, 2, 4));

      int length = strlen(operand);
			length -= 3;
			
			if (operand[0] == 'X') { LOCCTR += length/2; }
			else if (operand[0] == 'C') { LOCCTR += length; }
			else { /*error*/ }
    }
    else { /*Invalid operation code - program should not reach this condition*/ }

    fprintf(output_file, "\n");
    fscanf(input_file, "%s %s %s", label, opcode, operand);
  } 
  fclose(input_file);


  new_file = fopen("new_file.txt", "a");
  fprintf(new_file, "%s", "END");
  fclose(new_file);

  // write modifications to record
  new_file = fopen("new_file.txt", "r");
  char modificationString[50];
  fscanf(new_file, "%s", modificationString);
  while(!feof(new_file)) {
    fprintf(record_file, "\n%s", modificationString);
    fscanf(new_file, "%s", modificationString);
  }
  fclose(new_file);
  
  fprintf(record_file, "\nE^%X", startAddress);
  fclose(record_file);


  fprintf(output_file, "**\t\t%s\t\t%s\t\t%s\t\t", label, opcode, operand);
  fclose(output_file);

  /*
  // check struct content - 
  printf("Struct values:\n");
  for(int i=0; i<tableTraversalCounter; i++) {
    printf("%s - %X - %X\n", tab[i].operandName, tab[i].labelDefinedAddress, tab[i].operandUsedAddress);
  }
  */
  


  // update length in header and text records
  int length = LOCCTR - startAddress;
  char progLength[10];
  itoa(length, progLength, 16);

  record_file = fopen("record.txt", "r+");
  char str[150];
  int p = 0;
 
  // while(fgets(str, 150, record_file) != NULL) {
  //   if (str[0] == 'H') {
  //     for(int i=0; str[i]!='\n'&&str[i]!='\0'; i++) {
  //       if (str[i] == '*') {
  //         int retPos = ftell(record_file);
  //         fseek(record_file, i, SEEK_SET);
  //         fprintf(record_file, "%s", progLength);
  //         fseek(record_file, retPos, SEEK_SET);

  //         p = strlen(str);
  //         break;
  //       }
  //     }
  //   } else if (str[0] == 'T') {
  //     for(int i=0; str[i]!='\n'&&str[i]!='\0'; i++) {
  //       if (str[i] == '*') {
  //         int retPos = ftell(record_file);
  //         fseek(record_file, i+p+1, SEEK_SET);
  //         fprintf(record_file, "%s", progLength);
  //         fseek(record_file, retPos, SEEK_SET);

  //         p += strlen(str);
  //         break;
  //       }
  //     }
  //   }  
  // }

  while(fgets(str, 150, record_file) != NULL) {
    if (str[0] == 'H' || str[0] == 'T') {
      for(int i=0; str[i]!='\n'&&str[i]!='\0'; i++) {
        if (str[i] == '*') {
          int retPos = ftell(record_file);
          fseek(record_file, i+p, SEEK_SET);
          fprintf(record_file, "%s", progLength);
          fseek(record_file, retPos, SEEK_SET);

          p += strlen(str) + 1;
          break;
        }
      }
    }
  }

  fclose(record_file);
}