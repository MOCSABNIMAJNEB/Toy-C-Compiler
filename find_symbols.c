
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse_function_header(char *header)
{
  if (header == NULL) return 0;

  // these are the 10 strings that you will populate
  char s0[10];
  char s1[10];
  char s2[10];
  char s3[10];
  char s4[10];
  char s5[10];
  char s6[10];
  char s7[10];
  char s8[10];
  char s9[10];
  char *strings[] = {s0, s1, s2, s3, s4, s5, s6, s7, s8, s9};
  
  int numWords = tokenize(header, strings);

  int j = 0;
  int param_offset = 4;
  for (j = 1; j < 9; j++) {

    if (!(strings[j]==NULL) && !(strcmp(strings[j], "(int")) || !(strcmp(strings[j], "int"))) {
      add_node(strings[j+1], param_offset);
      param_offset++;
    }
  }

  return 1;
}

int param_length(char *header)
{
  if (header == NULL) return 0;

  // these are the 10 strings that you will populate
  char s0[10];
  char s1[10];
  char s2[10];
  char s3[10];
  char s4[10];
  char s5[10];
  char s6[10];
  char s7[10];
  char s8[10];
  char s9[10];
  char *strings[] = {s0, s1, s2, s3, s4, s5, s6, s7, s8, s9};
  
  int numWords = tokenize(header, strings);

  int j = 0;
  int nparams = 0;
  for (j = 1; j < 9; j++) {

    if (!(strings[j]==NULL) && !(strcmp(strings[j], "(int")) || !(strcmp(strings[j], "int"))) {
      nparams++;
    }
  }

  return nparams;
}



int parse_line(char *line, int *offset_index, int line_num)
{
  if (line == NULL) return 0;

  // these are the 10 strings that you will populate
  char s0[10];
  char s1[10];
  char s2[10];
  char s3[10];
  char s4[10];
  char s5[10];
  char s6[10];
  char s7[10];
  char s8[10];
  char s9[10];
  char *strings[] = {s0, s1, s2, s3, s4, s5, s6, s7, s8, s9};
  
  //printf("Calling tokenize function with input \"%s\".\n", line);
  int numWords = tokenize(line, strings);

  if (!(strcmp(strings[0], "return"))) {
    return 1;
  }

  int j = 0;
  for (j = 1; j<10; j++) {
    if (!(strlen(strings[j]) == 0)) {
      if (!(strcmp(strings[j-1], "int")) || (strcmp(strings[j-1], "=") && (strcmp(strings[j-1], "+") && strcmp(strings[j-1], "-"))) && (strcmp(strings[j], "=")  && strcmp(strings[j], "+") && strcmp(strings[j], "-"))) {
	if ((contains(strings[j]) == 0)) {
	  add_node(strings[j], (*offset_index));
	  (*offset_index)--;
	} else {
	  printf("%s is declared for a second time at line %d\n", strings[j], line_num);
	  return -1;
	}
      } else {
	if (!(contains(strings[j])) &&  (is_letter(strings[j][0]) || strings[j][0] == 95) && !(strcmp(strings[j], "return")==0) )  {
	  printf("%s has not yet been declared on line %d\n", strings[j], line_num);
	    return -1;
	  }
      }
    }
  }

  return 1;
}


