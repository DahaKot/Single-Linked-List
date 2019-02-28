#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

typedef const char * data_t;

static const double ERR_PROBABILITY = 0.5;

enum ERRORS {
	ALL_RIGHT = 0,
	LIST_ERR = 1,
	CONSTRUCT_ERR = 2,
	ARG_ERR = 3
};

struct Node {
    data_t data;
    struct Node *next;
};

struct List {
    struct Node *head;
    struct Node *tail;
    size_t len;
};

struct Node *Node_Construct (data_t value);                         //creates new node and sets data = value #
struct List *List_Construct ();										//creates new empty list #

void Node_Destruct 	(struct Node **old_one);						//destructs node # (frees *old_one)
void List_Destruct 	(struct List **old_one);						//destructs list # (frees *old_one)
//const?
int List_Add_First 		(struct List *list, data_t value);			//add new node at the start of the list with data =#value
int List_Add_Last 		(struct List *list, data_t value);			//adds new node at the end of the list with data = value#
int List_Insert			(struct List *list, data_t value, size_t n);//inserts new node on n-th place #
int List_Remove_n_Node  (struct List *list, size_t n);				//remove n-th node from list &call Destruct for the node#
struct Node *List_Find 	(struct List *list, data_t value);          //finds the first element with data == value #
struct Node *List_n_Find(struct List *list, size_t n);				//returns pointer to n-th element from the list #
size_t List_Count_Len	(struct List *list);
						
int List_Ok 		(struct List *list);							//checks if list's structure has no errors (verificator)#

int List_Print 		(struct List *list);							//prints all elements of the list
int List_Dump		(struct List *list);							//draws list using dot #
int List_Iterate	(struct List *list, void * (*func) (struct Node *node, va_list), ...);	//iteartes through the list

void *my_calloc		(size_t nmemb, size_t size);
void *node_print	(struct Node *node, va_list args);

// va_args

// some ideas:
//void List_AddAfter (struct List *list, struct Node *node, data_t value); 	//add new node after already known node
//int Node_Set_Value (struct Node *node, data_t value);						//changes value of already exisiting node
//int List_Add_First(Last, etc) (struct List *list, struct  Node *node);	//adds certain node to the list
//int List_Remove_Node 	(struct List *list, struct Node **node);  			//removes node from the list (awful) #
// struct Node *List_Get_Head (struct List *list);						//returns pointer on list's head
// int List_Get_Len 	(struct List *list);							//returns lists's len

#endif // LIST_H
