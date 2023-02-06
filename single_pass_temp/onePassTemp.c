#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
  int LOCCTR, startingAddress, i = 0, j = 0, m[10], programLength, len, k, len1, l = 0;
  char name[10], operand[10], label[10], opcode[10], s1[10], mne1[10], opnd1[10];
  char lcs[10], ms[10];
  char sym[10], symaddr[10], obj1[10], obj2[10], s2[10], q[10], s3[10];

  FILE *input_file, *optab_file, *symtab_file, *symtab1_file, *output_file;
  input_file = fopen("input.txt", "r");
  optab_file = fopen("optab.txt", "r");
  symtab_file = fopen("symtab.txt", "w+");
  symtab1_file = fopen("symtab1.txt", "w+");
  output_file = fopen("output.txt", "w+");

  fscanf(input_file, "%s%s%s", label, opcode, operand);
  if (strcmp(opcode, "START") == 0)
  {
    startingAddress = atoi(operand);
    strcpy(name, label);
    LOCCTR = startingAddress;
  }

  strcpy(s1, "*");
  fscanf(input_file, "%s%s%s", label, opcode, operand);
  while (strcmp(opcode, "END") != 0)
  {
    if (strcmp(label, "-") == 0)
    {
      fscanf(optab_file, "%s%s", mne1, opnd1);
      while (!feof(optab_file))
      {
        if (strcmp(mne1, opcode) == 0)
        {
          m[i] = LOCCTR + 1;
          fprintf(symtab_file, "%s\t%s\n", operand, s1);
          fprintf(output_file, "%s\t0000\n", opnd1);
          LOCCTR = LOCCTR + 3;
          i = i + 1;
          break;
        }
        else
          fscanf(optab_file, "%s%s", mne1, opnd1);
      }
    }
    else
    {
      fseek(symtab_file, SEEK_SET, 0);
      fscanf(symtab_file, "%s%s", sym, symaddr);
      while (!feof(symtab_file))
      {
        if (strcmp(sym, label) == 0)
        {
          sprintf(lcs, "%d", LOCCTR);
          fprintf(symtab1_file, "%s\t%s\n", label, lcs);
          sprintf(ms, "%d", m[j]);
          j = j + 1;
          fprintf(output_file, "%s\t%s\n", ms, lcs);
          i = i + 1;
          break;
        }
        else
          fscanf(symtab_file, "%s%s", sym, symaddr);
      } // symtab_file
      if (strcmp(opcode, "RESW") == 0)
        LOCCTR = LOCCTR + 3 * atoi(operand);
      else if (strcmp(opcode, "BYTE") == 0)
      {
        strcpy(s2, "-");
        len = strlen(operand);
        printf("%d\n", len);
        LOCCTR = LOCCTR + len - 2;
        for (k = 1; k < len - 1; k++)
        {
          q[l] = operand[k];
          l = l + 1;
        }
        fprintf(output_file, "%s\t%s\n", q, s2);
        break;
      }
      else if (strcmp(opcode, "RESB") == 0)
        LOCCTR = LOCCTR + atoi(operand);
      else if (strcmp(opcode, "WORD") == 0)
      {
        strcpy(s3, "#");
        LOCCTR = LOCCTR + 3;
        fprintf(output_file, "%s\t%s\n", operand, s3);
        break;
      }
    } // else label=-
    fseek(optab_file, SEEK_SET, 0);
    fscanf(input_file, "%s%s%s", label, opcode, operand);
  }
  fseek(output_file, SEEK_SET, 0);
  programLength = LOCCTR - startingAddress;
  printf("H^%s^%d^0%x\n", name, startingAddress, programLength);
  printf("T^");
  printf("00%d^0%x", startingAddress, programLength);
  fscanf(output_file, "%s%s", obj1, obj2);
  while (!feof(output_file))
  {
    if (strcmp(obj2, "0000") == 0)
      printf("^%s%s", obj1, obj2);
    else if (strcmp(obj2, "-") == 0)
    {
      printf("^");
      len1 = strlen(obj1);
      printf("%s", obj1);
      // for(k=0;k<len1;k++)
      // printf("%d",obj1[k]);
    }
    else if (strcmp(obj2, "#") == 0)
    {
      printf("^");
      printf("%s", obj1);
    }
    fscanf(output_file, "%s%s", obj1, obj2);
  }
  fseek(output_file, SEEK_SET, 0);
  fscanf(output_file, "%s%s", obj1, obj2);
  while (!feof(output_file))
  {
    if (strcmp(obj2, "0000") != 0)
    {
      if (strcmp(obj2, "-") != 0)
      {
        if (strcmp(obj2, "#") != 0)
        {
          printf("\n");
          printf("T^%s^02^%s", obj1, obj2);
        }
      }
    }
    fscanf(output_file, "%s%s", obj1, obj2);
  }
  printf("\nE^00%d", startingAddress);
}