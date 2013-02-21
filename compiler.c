#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// so you can call this function from here
extern char* trim(char*);
extern int length();
extern int param_length(char *a);
extern FILE *open_file(char *name);
char *format(char *a, char *b, FILE *c);
void decrement_R0(char *a, char *b, FILE *c);
void increment_R0(char *a, char *b, FILE *c);
void write_addr(char *a, char*b, FILE *c);

/*
 * Generate the prologue for the function and write it
 * to the output file.
 * Use the function header to determine the name of the
 * function, and use it as the label of the first
 * instruction of the prologue.
 */
int generate_prologue(int local_var_length, FILE *output){
  int i;
  fprintf(output, ";;Generate Prologue\n");
  fprintf(output, "ADD SP, SP, #-1; Create space for return value\n"); 
  fprintf(output, "ADD SP, SP, #-1; Create space for return address\n");
  fprintf(output, "STR R7, SP, #0; Store return address on stack\n");
  fprintf(output, "ADD SP, SP, #-1; Move up Stack\n");
  fprintf(output, "STR FP, SP, #0; Store FP on stack\n"); 

  if (local_var_length > 0)
    fprintf(output, "ADD SP, SP, #-1; Make space for %s\n", get_name(local_var_length-1)); 
  else
    fprintf(output, "ADD SP, SP, #-1; Move up to store FP bc no local variables\n");

  fprintf(output, "STR FP, SP, #0; set FP to this address\n");


  //start at index of of 2nd local variable since we push room for the first local variable above when we're setting the frame pointer
  if (local_var_length > 1) {
    for (i = local_var_length-2; i >= 0; i--)
      fprintf(output, "ADD SP, SP, #-1; Made Space for local variable %s\n", get_name(i));
  }
  fprintf(output, "AND R0, R0, #0; Clear R0\n");
  fprintf(output, ";; content goes here\n");

  return 1;
}


/*
 * Generate the epilogue for this function.
 */
int generate_epilogue(int local_var_length, FILE *output) {
  int i;
  fprintf(output, ";; Generate epilogue\n");
  for (i = 0; i < local_var_length; i++)
    fprintf(output, "ADD SP, SP, #1; pop local variable %s\n", get_name(i));
  if (local_var_length == 0) 
    fprintf(output, "ADD SP, SP, #1; move down since we moved up, even though no local vars\n");

  fprintf(output, "LDR FP, SP, #0; restore old FP\n");
  fprintf(output, "ADD SP, SP, #1; pop old FP\n");
  fprintf(output, "LDR R7, SP, #0; restore return address\n");
  fprintf(output, "ADD SP, SP, #1; pop return address\n");
  fprintf(output, "ADD SP, SP, #1; pop return value\n");

  for (i = length(); i > local_var_length; i--)
    fprintf(output, "ADD SP, SP, #1; pop parameter %s\n", get_name(i-1));
  fprintf(output, "RET\n");
  return 1;

}


/*
 * Convert this line of code to LC-3 and write to
 * the output file.
 */

int compile_line(char *line, FILE *output) {

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
  char *tokenp;
  char format_str[10];
  char *format_strp = format_str;

  int numWords = tokenize(line, strings);

  int j;
  //fprintf(output, "AND R0, R0, #0; Clear R0 since we're starting a new line\n");

  for (j = 9; j>=0; j--) {

    if (!(strlen(strings[j]) == 0)) {

      if ((j != 0) && (!(strcmp(strings[j-1], "int")) || (strcmp(strings[j-1], "=") && strcmp(strings[j-1], "+") && strcmp(strings[j-1], "-")) && (strcmp(strings[j], "=") && strcmp(strings[j], "+") && strcmp(strings[j], "-")) )) { //ignore
	continue;
	
      } else if (!(strcmp(strings[j], "="))) {

	remove_semicolon(strings[j+1]);
	tokenp = strings[j+1];
	increment_R0(format(tokenp, format_strp, output), strings[j+1], output);

	remove_semicolon(strings[j-1]);
	tokenp = strings[j-1];

	write_addr(format(tokenp, format_strp, output), strings[j-1], output);
	fprintf(output, "AND R0, R0, #0; Clear R0 since we just assigned something\n");
      } else if (!(strcmp(strings[j], "+"))) {

	remove_semicolon(strings[j+1]);

	tokenp = strings[j+1];
	increment_R0(format(tokenp, format_strp, output), strings[j+1], output);
	

      } else if (!(strcmp(strings[j], "-"))) {

	remove_semicolon(strings[j+1]);

	tokenp = strings[j+1];
	decrement_R0(format(tokenp, format_strp, output), strings[j+1], output);
	
      } else if (!(strcmp(strings[j], "return"))) {


	remove_semicolon(strings[j+1]);
	tokenp = strings[j+1];

	increment_R0(format(tokenp, format_strp, output), strings[j+1], output);
	
	
	fprintf(output, "STR R0, FP, #3; Store the return value\n");
      }
    }
  }

  return 1;
}

/*
 * Tries to format the value stored in a token 
 */
char *format(char *input, char *out, FILE *output) {
  int offset = 0;

  if (get_offset(input, &offset)) {
    sprintf(out, "FP, #%d", offset);
  } else {
    int i = atoi(input);

    fprintf(output, "ADD R0, R0, #%d; adds modulo int to R0\n", (i%15));
    i -= i%15;

    while (i > 0) {
      fprintf(output, "ADD R0, R0, #15; adds 15 to R0 until i equals 0\n");
      i -= 15;
    }
    out = "R0, #0";
  }
  

  return out;
}


/*
 * Generate addition of two values
 */

void increment_R0(char *one, char *varname, FILE *output) {
  if (contains(varname)) {
    fprintf(output, "LDR R1, %s; puts %s in R1\n", one, varname);
    fprintf(output, "ADD R0, R0, R1; increments R0 by R1\n");
  }
}

/*
 * Generate subtraction of two values
 */

void decrement_R0(char *one, char *varname, FILE *output) {
  if (contains(varname)) {
    fprintf(output, "LDR R1, %s; puts %s in R1\n", one, varname);
    fprintf(output, "NOT R1, R1; inverts value in R1\n");
    fprintf(output, "ADD R0, R0, R1; increments R0 by R1\n");
  } else {
    fprintf(output, "NOT R0, R0; invert R0, making it negative\n");
  }
}

/*
 * Store a value in R0 at addr
 */

void write_addr(char *addr, char *varname, FILE*output) {  
  fprintf(output, "STR R0, %s; storing R0 in %s\n", addr, varname);
}

/*
 * Read Globals
 */



main(int argc, char *argv[]) {

  // make sure the filename is specified
  if (argc < 2) {
    printf("Please specify the name of the file to compile\n");
    return;
  }
  char *name  = argv[1];
  
  FILE *infile = open_file(name);
  if (infile == NULL) {
    return 0;
  }

  // create the symbol table
  if (create_symbol_table(name) == 0) {
    exit(0);
  }
 
  //This is the most ridiculous thing ever
  char *trim_name = malloc(strlen(name));
  strcpy(trim_name, name);
  remove_extension(trim_name);

  char out[15];
  sprintf(out, "%s.LC3", trim_name);
  char *outfile_name = out;

  FILE *output = fopen(outfile_name, "w");
  FILE *infile2 = open_file(name);
  char line[110];

  fgets(line, 110, infile2);
  int local_var_length = length() - param_length(line);
  
  generate_prologue(local_var_length, output);


  while (fgets(line, 110, infile2) != NULL) {
    char *token;
    token = trim(line);
    compile_line(token, output);
  }

  generate_epilogue(local_var_length, output);
  fclose(output);
  fclose(infile2);
  
}


