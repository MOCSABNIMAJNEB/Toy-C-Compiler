#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Function declarations */
char *trim(char *str);
FILE *open_file(char *name);


/*
 * This function takes in the name of a file as its argument
 * and then reads the file one line at a time.
 * The first line should be passed to parse_function_header.
 * The rest should be passed to parse_line.
 */ 
int create_symbol_table(char *name) {

  // open the file
  FILE *infile = open_file(name);
  if (infile == NULL) {
    fclose(infile);
    return 0;
  }

  // 100-element character array to hold the line we're reading
  char line[110];

  // read the first line
  fgets(line, 110, infile);

  parse_function_header(line);
  
  int offset_index = 0;
  int line_num = 2;
  int *index_pointer = &offset_index;
  while (fgets(line, 110, infile) != NULL) {
    char *token;
    token = trim(line);

    //sketchy style because we do the assignment in our guard :P
    if (parse_line(token, index_pointer, line_num) == -1) {
      fclose(infile);
      return 0;
    }

    line_num++;
  }

  fclose(infile);
  return 1;
 
}

/* This is a function to open a file. 
 * DO NOT CHANGE THIS CODE!!
 */
FILE *open_file(char *name) {
  
  if (name == NULL) {
    printf("Error! File name is null!\n");
    return NULL;
  }

  // open the file for reading
  FILE *infile = fopen(name, "r");

  if (infile == NULL) {
    printf("Error! Could not open file \"%s\" for reading\n", name);
    return NULL;
  }

  return infile;
}



/*
 * This function takes in a char pointer (presumably referring to a mutable
 * string) and removes any leading or trailing whitespace.
 * http://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way
 *
 * DO NOT CHANGE THIS CODE!!
 */
char *trim(char *str)
{
  char *end;

  // Trim leading space
  while(isspace(*str)) str++;

  if(*str == 0)  
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace(*end)) end--;

  // Write new null terminator
  *(end+1) = 0;

  return str;
}

char *remove_semicolon(char *str)
{
  char *end;

  if(*str == 0)  
    return str;

  // Trim trailing semicolon
  end = str + strlen(str) - 1;
  while (end > str && (*end == ';' || *end == ',')) end--;


  // Write new null terminator
  *(end+1) = 0;

  return str;
}

char *remove_extension(char *str)
{
  char *end;

  if(*str == 0)  
    return str;

  // Trim all chars up to '.'
  end = str + strlen(str) - 1;
  while(end > str && *end != '.') end--;
  end--;

  // Write new null terminator
  *(end+1) = 0;

  return str;
}

