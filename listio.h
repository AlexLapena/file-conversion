/*
 * Alexander Lapena 
 * 0844071
 * CIS* 2750 - A4
 *
 * listio.h
 * 
 * 17 November 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <mysql/mysql.h>

#define _GNU_SOURCE 

#define MAX_QUERY 1024
#define HOSTNAME  "dursley.socs.uoguelph.ca"

#define USERNAME  "alapena"
#define PASSWORD  "0844071"
#define DATABASE  "alapena"
 
#define FAILURE 0;
#define SUCCESS 1;
 
struct dataHeader {
    char *name;
    int length;
    struct dataString *next;
};

struct dataString {
    char *string;
    struct dataString *next;
};

struct returnStruct {
    int value;
    struct dataHeader *header;
};

/*
 * Returns a pointer to a struct of type dataHeader. It must dynamically allocate
 * the structure and set the initial values of the variables.
 */
struct returnStruct * buildHeader();

/*
 * Allocates and sets the name string in a dataHeader structure.
 */
int setName(struct dataHeader *header, char *name);

/*
 * Returns a pointer to the name string in the dataHeader structure.
 */
char * getName(struct dataHeader *header);

/*
 * Returns the integer value stored in the length variable in the dataHeader
 * structure.
 */
int getLength(struct dataHeader *header);

/*
 * Strings are stored in the order which they are added to the list. The first
 * string should be stored in the first structure linked to the dataHeader.
 * Each later string is stored in a dataString structure which is added to
 * the end of the list.
 */
int addString(struct dataHeader *header, char *str);

/*
 * This function prints out all of the strings stored in the list.
 */
int printString(struct dataHeader *header);

/*
 * This function modifies the characters space, tab, newline, carriage
 * return contained within the strings. You should probably look at the
 * ASCII tables to learn how to identify these characters
 */
int processStrings(struct dataHeader *header);

/*
 * Associated with process strings -- Processes extra tabs and spaces
 */
void spaceAdjust(char * string, int index, struct dataHeader *header, int count);

/*
 * Associated with process strings -- Processes newlines and carriage returns
 */
void newLineAdjust(char * string, int index, struct dataHeader *header, int count);

/*
 * Removes extra HTML tags if there are excess white spaces int a string
 */
void removeExtraTags(struct dataString *dS) ;

/*
 * Removes extra HTML tags and white spaces between strings
 */
void cleanString(struct dataString *current);

/* 
 * Free() all of the dataString structures and the dataHeader structure.
 */
int freeStructure(struct dataHeader *header);

/* 
 * Reverses the list from tail to head.
 */
struct dataString * reverseList(struct dataString *dsHead);

/*
 * Writes all of the contents of the dataHeader and linked structure to a
 * binary file.
 */
int writeStrings(char *filename, struct dataHeader *header);

/*
 * This function reads the linked structure from a file created using the 
 * writeStrings() function and rebuilds the linked structure that is stored
 * in the file.
 */
struct returnStruct *readStrings(char *filename);

void clrstr(char *buf);
