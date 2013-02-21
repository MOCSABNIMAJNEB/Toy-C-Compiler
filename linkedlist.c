#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*int remove_from_front() {
  int return value = head -> data; 
  node *free_me = head;
  head = head -> next;
  free(free_me);
  return return_value;
 */

/*
 * DO NOT CHANGE THIS DEFINITION!
 */
typedef struct Node node;

struct Node {
  char name[10];
  int offset;
  node *next;
};

// global pointer to first Node of Linked List
struct Node *head;


/*
  Adds a new node to the linked list, with name set to parameter "s" and offset set to parameter "v".
  Returns 1 if successful, 0 if an error occurred.
*/
int add_node(char *s, int v) {
  node *new = malloc(sizeof(node));
  
  if (new == NULL) {
    //Failed to allocate space
    return 0;
  }

  strcpy(new -> name, s);
  new -> offset = v;
  new -> next = head;
  head = new;

  return 1;

}

/*
  Finds the offset of the node whose name is equal to the first argument.
  If the node is found, its offset value is put into the "v" parameter, and the function returns 1.
  If an error occurs (e.g. there is no node with that name), the function returns 0.
*/
int get_offset(char *s, int *v) {  
  node *current = head;
  int i;

  while (current != NULL) {
    if (!(strcmp(current -> name, s))) {
      *v = current -> offset;
      return 1;
    }

    current = current -> next;

  }
  return 0;

}

/*
 Print out (to the screen) the name and offset value for each node
 in the linked list.
*/
void iterate() {
  node *current = head;

  while (current != NULL) {
    printf("%s: %d\n", current -> name, current -> offset) ;
    current = current -> next;
  }
  
}

/*
 * Return a pointer to the name of the node at position [i]
 */

char* get_name(int index) {
  node *current = head;
  int i = 0;

  while (i < index) {
    current = current -> next;
    i++;
  }

  return current -> name;
}


/*
 * function returning the number of times an node of name s occurs on the list
 */
int contains(char *s) {
  node *current = head;
  int matches = 0;

  while (current != NULL) {
    if (!(strcmp(current -> name, s))) {
      matches++;
    }
    current = current -> next;
  }
  return matches;
}

/*
 * function returning the length of the linked list
 */
int length() {
  node *current = head;
  int len = 0;

  while (current != NULL) {
    len++;
    current = current -> next;
  }

  return len;
}

/*
 * function returning 1 if all keys in linked list are unique else 0
 */
int is_unique() {
  node *current = head;

  while (current != NULL) {
    if (contains(current -> name) > 1) {
      return 0;
    }
    current = current -> next;
  }
  return 1;

}


