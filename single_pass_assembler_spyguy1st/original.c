// single pass assembler - working code, original


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char addr[10];

int searchStrSym(char operand[10]) {
  FILE *symtab_file;
  char label[10];
  symtab_file = fopen("symtab.txt", "r");
  while (!feof(symtab_file)) {
    fscanf(symtab_file, "%s %s", label, addr);
    if (strstr(operand, label) == 0) {
      fclose(symtab_file);
      return 0;
    }
  }
  fclose(symtab_file);
  return 1;
}



int main() {
  FILE *input_file, *record_file, *symtab_file, *optab_file, *intermediate_file;

  input_file = fopen("input.txt", "r");
  record_file = fopen("record.txt", "w");
  intermediate_file = fopen("intermediate.txt", "w");

  symtab_file = fopen("symtab.txt", "w");
  fclose(symtab_file);

  char label[10], opcode[10], operand[10];
  char optab_opcode[10], optab_mnemonic[10], symtab_label[10], symtab_addr[10];
  int rec_file_offset, intermediate_file_offset;
  int LOCCTR, startAddress, dup = 0, invalid = 0, flag3, c = -1;


  fscanf(input_file, "%s %s %X", label, opcode, &startAddress);


  if (strcmp(opcode, "START") == 0) {
    LOCCTR = startAddress;

    fprintf(record_file, "H^%s^%X^****^\n", label, startAddress);
    fprintf(record_file, "T^%X^****^", startAddress);
    fprintf(intermediate_file, "%20s%11s%11X\n", label, opcode, startAddress);

    fscanf(input_file, "%s %s %s", label, opcode, operand);
  } else {
    LOCCTR = 0;
  }


  while (strcmp(opcode, "END") != 0) {
    fprintf(intermediate_file, "%X\t\t%s\t\t%s\t\t%s\t\t", LOCCTR, label, opcode, operand);

    if (strcmp(label, "**") != 0) {
      int flag1 = 0;
      symtab_file = fopen("symtab.txt", "r+");

      while (!feof(symtab_file)) {
        fscanf(symtab_file, "%s %s \n", symtab_label, symtab_addr);
        if (strcmp(symtab_label, label) == 0) {
          // if address in symtab is null, edit it
          if (strcmp(symtab_addr, "----") == 0) {
            int retAddr = ftell(intermediate_file);

            fscanf(symtab_file, "%d %d", &rec_file_offset, &intermediate_file_offset);
            fseek(record_file, rec_file_offset, SEEK_SET);
            fseek(intermediate_file, intermediate_file_offset, SEEK_SET);
            fprintf(record_file, "%X^", LOCCTR);
            fprintf(intermediate_file, "%X", LOCCTR);
            // printf(" %d", LOCCTR);

            fseek(intermediate_file, retAddr, SEEK_SET);
            // flag1 = 1;
            // break;
          }
          // dup = 1;
          // flag1 = 1;
          // break;
        }
      }

      if (flag1 == 0) {
        // printf("%s\t\t%X\n", label, LOCCTR);
        fprintf(symtab_file, "%s\t\t%X\n", label, LOCCTR);
      }
      fclose(symtab_file);
    }

    
    int flag2 = 0;
    optab_file = fopen("optab.txt", "r");
    while (!feof(optab_file))
    {
      fscanf(optab_file, "%s %s", optab_opcode, optab_mnemonic);
      if (strcmp(optab_opcode, opcode) == 0) {
        flag2 = 1;
        fprintf(record_file, "%s", optab_mnemonic);
        fprintf(intermediate_file, "%s", optab_mnemonic);
      }
    }
    fclose(optab_file);

    if (flag2 == 1)
    {
      flag3 = 0;
      symtab_file = fopen("symtab.txt", "r");
      while (!feof(symtab_file))
      {
        // printf("hello");
        fscanf(symtab_file, "%s %s \n", symtab_label, addr);
        if (strcmp(symtab_label, operand) == 0)
        {
          flag3 = 1;
          fprintf(record_file, "%s^", addr);
          fprintf(intermediate_file, "%s", addr);
        }
        // if (strstr(operand, symtab_label) == 0) { // strstr is not working
        //   fprintf(record_file, "%s^", addr);
        // }
        else
        {
          char cop[10];
          for (int i = 0; i < strlen(operand); i++)
            if (operand[i] == 44)
            { //, ascii value is 44
              // if (flag3 == 0) {
              flag3 = 1;
              int j = 0;
              int k = 0;
              while (j < i)
                cop[k++] = operand[j++];
              // printf("hello");
              // strncpy(cop, operand, i);     // strncpy copies n characters
              // printf("%s ", symtab_label);
              if (strcmp(symtab_label, cop) == 0)
              {
                printf("yo");
                fprintf(record_file, "%s^", addr);
                fprintf(intermediate_file, "%s", addr);
              }
              break;
              // }
            }
        }
      }
      fclose(symtab_file);
      if (flag3 == 0)
      {
        fprintf(intermediate_file, "----");
        fprintf(record_file, "----^");
        symtab_file = fopen("symtab.txt", "a");
        fprintf(symtab_file, "%s\t\t----\t%ld %ld\n", operand, ftell(record_file) - 5, ftell(intermediate_file) - 4);
        fclose(symtab_file);
      }
    }

    if (flag2 == 1)
      LOCCTR = LOCCTR + 3;
    else if (strcmp(opcode, "WORD") == 0)
    {
      int a = atoi(operand);
      if (strlen(operand) == 1)
      {
        fprintf(record_file, "00000%d^", a);
        fprintf(intermediate_file, "00000%d", a);
      }

      if (strlen(operand) == 2)
      {
        fprintf(record_file, "0000%d^", a);
        fprintf(intermediate_file, "0000%d", a);
      }
      if (strlen(operand) == 3)
      {
        fprintf(record_file, "000%d^", a);
        fprintf(intermediate_file, "000%d", a);
      }
      if (strlen(operand) == 4)
      {
        fprintf(record_file, "00%d^", a);
        fprintf(intermediate_file, "00%d", a);
      }
      if (strlen(operand) == 5)
      {
        fprintf(record_file, "0%d^", a);
        fprintf(intermediate_file, "0%d", a);
      }
      if (strlen(operand) == 6)
      {
        fprintf(record_file, "%d^", a);
        fprintf(intermediate_file, "%d", a);
      }
      LOCCTR = LOCCTR + 3;
    }
    else if (strcmp(opcode, "RESW") == 0)
      LOCCTR = 3 * atoi(operand) + LOCCTR;
    else if (strcmp(opcode, "RESB") == 0)
      LOCCTR = atoi(operand) + LOCCTR;
    else if (strcmp(opcode, "BYTE") == 0)
    {
      // int space = 6 - (strlen(operand) - 3);
      // printf("%d ", space);
      // while (space > 0) {
      //   fprintf(intermediate_file, " ");
      //   space--;
      // }
      for (int i = 0; i < strlen(operand); i++)
        if (operand[i] == 39)
        { //' ascii value is 39
          i++;
          while (operand[i] != 39)
          {
            fprintf(record_file, "%c", operand[i]);
            fprintf(intermediate_file, "%c", operand[i]);
            i++;
          }
        }
      fprintf(record_file, "^");
      LOCCTR++;
    }
    else
    {
      invalid = 1;
    }
    fprintf(intermediate_file, "\n");
    fscanf(input_file, "%s %s %s", label, opcode, operand);
  } 

  fprintf(record_file, "\nE^%X", startAddress);
  fclose(record_file);

  int length = LOCCTR - startAddress;
  printf("\nProgram Length = %X\n", length);

  record_file = fopen("record.txt", "r+");
  char rep;
  int l = -1;
  while (1)
  {
    rep = fgetc(record_file);
    l++;
    if (feof(record_file))
      break;
    // printf("%c ", rep);
    if (rep == '*')
    {
      fseek(record_file, l, SEEK_SET);
      printf("replace ");
      fprintf(record_file, "%X", length);
      fseek(record_file, l + 1, SEEK_SET);
    }
  }

  fclose(input_file);
  fclose(record_file);
  // fclose(symtab_file);
  printf("\n");
  // fclose(intermediate_file);
}
