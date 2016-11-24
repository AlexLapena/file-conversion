/*
 * Alexander Lapena 
 * 0844071
 * CIS* 2750 - A4
 *
 * listio.c
 * 
 * 17 November 2016
 */

#include "listio.h"

struct returnStruct * buildHeader() 
{
    struct returnStruct *rS = (struct returnStruct *) malloc(sizeof(struct returnStruct));
    if (rS == NULL) {
        return (rS);
    }
    struct dataHeader *head = (struct dataHeader *) malloc(sizeof(struct dataHeader));
    if (head == NULL) {
        rS->value = FAILURE;
        rS->header = NULL;
        return (rS);
    }

    rS->header = head;

    /* Creates the memory for the header node*/
    if (head != NULL) {
        head->name = NULL;
        head->length = 0;
        head->next = NULL;
        rS->value = 1;
    }
    /* Returns NULL on failure */
    else {
        rS->value = 0;
        rS->header = NULL;
    }

    return(rS);
}

int setName(struct dataHeader *header, char *name)
{
    int length;
    if (header == NULL) {
        return FAILURE;
    }

    header->name = (char *) malloc(sizeof(char) * strlen(name)+1);
    if (header->name == NULL) {
        return FAILURE;
    }
    strcpy(header->name, name);
    length = strlen(header->name);
    //length = strlen(name);
    /* Fails if the name is NULL or empty */
    if (header->name == NULL ||  0 == strcmp(header->name, "")) {
        return FAILURE;
    }

    return SUCCESS;
}

char * getName(struct dataHeader *header)
{
    /* Returns a NULL if the header is not initialized */
    if (header == NULL) {
        return NULL;
    }

    /* Returns a NULL if the name is NULL or empty*/
    if (header->name == NULL || 0 == strcmp(header->name, "")) {
        return NULL;
    }

    return(header->name);
}

int getLength(struct dataHeader *header)
{
    /* Returns a fail if there is no header or the length is 0 */
    if (header == NULL || header->length == 0) {
        return FAILURE;
    }

    return(header->length);
}

int addString(struct dataHeader *header, char *str)
{
    struct dataString *current = header->next;
    struct dataString *dS = (struct dataString *) malloc(sizeof(struct dataString));
    if (dS == NULL) {
        return FAILURE;
    }
    
    /* Creates a new node to be placed into the list */
    dS->string = (char *) malloc(sizeof(char) * strlen(str)+1);
    if (dS->string == NULL) {
        return FAILURE;
    }
    else{
        strcpy(dS->string, str);
    }

    if (header == NULL || dS->string == NULL) {
        return FAILURE;
    }

    /* First string in the list */
    if (header->next == NULL) {
        header->next = dS;
        dS->next = NULL;
        return SUCCESS;
    }
        
    /* Used once the list has been created */
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = dS;
    dS->next = NULL;

    return SUCCESS;
}

int printString(struct dataHeader *header)
{
    struct dataString *current = header->next;
    
    if (header == NULL || header->next == NULL) {
        printf("ERROR: NULL DATA ENTERED.\n");
        return FAILURE;
    }

    printf("%s\n", getName(header));

    while (current != NULL) {
        current = current->next;
    }

    return SUCCESS;
}

int processStrings(struct dataHeader *header)
{
    struct dataString *dS = header->next;
    char * newString;
    int i, length, count = 0;
    FILE * fp, *pfp, *fp2;
    char *string;
    char * fName;

    if (header == NULL || header->next == NULL) {
        return FAILURE;
    }
    while (dS != NULL) {
        length = strlen(dS->string);
        for (i = 0; i < length; i++) {
            // Removes extra spaces and tabs 
            if ((dS->string[i] == ' ' && dS->string[i+1] == ' ') || (dS->string[i] == '\t'
                    && dS->string[i+1] == '\t')) {
                spaceAdjust(dS->string, i, header, count);
                //Resets string until fully processed 
                length = strlen(dS->string);
                i = 0;
            }
            // Removes extra newline and carriage returns. Replaces with HTML tags 
            else if (dS->string[i] == '\n' || dS->string[i] == '\r') {
                newLineAdjust(dS->string, i, header, count);     
                // Resets string until fully processed 
                length = strlen(dS->string);
                i = 0;
            }
        }

        length = strlen(header->name);
        fName = calloc(length + 7, sizeof(char));
        strcpy(fName, header->name);
        strcat(fName, ".html");

        // Python Script Call 
        pfp = popen("python a4.py", "w");  
        
        // Sends strings to python     
        fp = fopen("./files/q1", "w");
        fprintf(fp, "%s ", header->name);
        fprintf(fp, "%s", dS->string);
        fclose(fp);
        
        // Updated strings
        mkfifo("./files/q2", 0777);
        fp = fopen("./files/q2", "r");
        fp2 = fopen(fName, "w");

        string = malloc(sizeof(char) * 5000);
        while(fgets(string, 32432, fp)) {
            fprintf(fp2, "%s", string);
        }
        
        fclose(fp);
        remove("./files/q2");
        pclose(pfp);                      
        fclose(fp2);        
        free(fName);
        free(string);

        /* Calculates the length */
        count++;
        header->length = length + header->length;
        dS = dS->next;
    }
    
    return SUCCESS;
}

void spaceAdjust(char * string, int index, struct dataHeader *header, int count)
{
    struct dataString *dS = header->next;
    int i, j, length;
    char * newString = calloc(strlen(string)+1,sizeof(char));
    if (newString == NULL) {
        return;
    }
    length = strlen(string);

    for (i = 0, j = 0; i < length; i++) {
        if (i == index) {
            /* Skips of extra tab or space */
            continue;
        }
        else {
            /* Copies string into a new string without extra spaces or tabs */
            newString[j] = string[i];
            j++;
        }
    }
    
    for(i = 0; i <= count; i++) {
        if (i == count) {
            free(dS->string);
            dS->string = NULL;
            //dS->string = malloc(sizeof(char) * strlen(newString)+1);
            dS->string = calloc(strlen(newString)+1, sizeof(char));
            strcpy(dS->string, newString);
            free(newString);
            return;
        }
        dS = dS->next;
    }
}

void newLineAdjust(char * string, int index, struct dataHeader *header, int count) 
{
    struct dataString *dS = header->next;
    int i, j, length;
    char * newString;

    length = strlen(string);

    // For single newlines and carriage returns, replace with the <BR> tag 
    if (string[index+1] != '\n' && string[index+1] != '\r') {

        newString = malloc(sizeof(newString) * strlen(string) + 3);
        if (newString == NULL) {
            return;
        }

        for (i = 0, j = 0; i < length; i++) {
            if (i == index) {
                // Replacing characters 
                newString[j] = '<';
                newString[j+1] = 'B';
                newString[j+2] = 'R';
                newString[j+3] = '>';
                j = j + 4; 
            }
            else {
                newString[j] = string[i];
                j++;
            }
        }
        newString[j] = '\0';
    }

    // For multiple newlines and carriage returns, replace with the <P> tag 
    else if (string[index+1] == '\n' || string[index+1] == '\r') {
        // Removes excess newlines or Carriage returns 
        if (string[index+2] == '\n' || string[index+2] == '\r') {
            newString =  malloc(sizeof(newString) * strlen(string) -1);
            if (newString == NULL) {
                return;
            }

            for (i = 0, j = 0; i < length; i++) {
                if (i == index) {
                    // Skips extra newlines and carriage returns 
                    continue;
                }
                else {
                    newString[j] = string[i];
                    j++;
                }
            }
            newString[j] = '\0';
        }
        // If there are two newlines or carriage returns, replace with <P> tags 
        else {
            newString = malloc(sizeof(newString) * strlen(string) + 2);
            if (newString == NULL) {
                return;
            }

            for (i = 0, j = 0; i < length; i++) {
                if (i == index) {
                    // Replacing characters 
                    newString[j] = '<';
                    newString[j+1] = 'P';
                    newString[j+2] = '>';
                    j = j + 3;
                    i = i + 1; 
                }
                else {
                    newString[j] = string[i];
                    j++;
                }
            }
            newString[j] = '\0';
        }
    }

    for(i = 0; i <= count; i++) {
        if (i == count) {
            free(dS->string);
            dS->string = malloc(sizeof(char) * strlen(newString)+1);
            strcpy(dS->string, newString);
            free(newString);
            return;
        }
        dS = dS->next;
    }
    
}

void removeExtraTags(struct dataString *dS) 
{
    int length, i, j, k, start, end, diff;
    int checked = 0;
    char * newString;

    length = strlen(dS->string);
    for (i = 0; i < length; i++) {
        /* Checks for a tag */
        if (dS->string[i] == '<' && dS->string[i+1] == 'P' && dS->string[i+2] == '>' && checked == 0) {
            start = i;
            i = i + 2;
            checked = 1;
        }
        else if (dS->string[i] == '<' && dS->string[i+1] == 'P' && dS->string[i+2] == '>' && checked == 1) {
            end = i - 1;     
            diff = end - start;     

            /* Replaces string with white spaces removed */
            newString = malloc(sizeof(char) * (length - diff));
            if (newString == NULL) {
                return;
            }
            for (j = 0, k = 0; j < length; j++) {
                if (j == start) {
                    j = end + 1;
                }
                newString[k] = dS->string[j];
                k++;
            }
            dS->string = malloc(sizeof(char) * (length - diff));
            if (dS->string == NULL) {
                return;
            }
            strcpy(dS->string, newString);
            free(newString);
            i = i + 2;
            start = 0;
            end = 0;
            checked = 0;
        }
        /* Resets, no white spaces between P tags */
        else if (dS->string[i] != ' ' && dS->string[i] != '\t' && checked == 1) {
            start = 0;
            end = 0;
            checked = 0;
        }
    }
}

void cleanString(struct dataString *current) 
{
    struct dataString *next = current->next;
    int i, j, length, currIndex, nextIndex, nextLength;
    char * newString, * newNext; 

    /* There is no paragraph tag at start of next string */
    if (next->string[0] != ' ' && next->string[0] != '\t' && next->string[0] != '\n' && 
            next->string[0] != '\r') {
        return;
    }

    /* Finds out if paragraph tag at end of current */
    length = strlen(current->string);
    if (current->string[length-1] == '>' && current->string[length-2] == 'P') {
        currIndex = 3;
    }
    else if (current->string[length-1] == ' ' || current->string[length-1] == '\t') {
        for (i = 1; i < length; i++) {
            if (current->string[length-i] != ' ' && current->string[length-i] != '\t'
                    && (current->string[length-i] != '>' && current->string[length-i-1] != 'P')) {
                return;
            }
            else if (current->string[length-i] == '>' && current->string[length-i-1] == 'P') {
                currIndex = i + 2;
                break;
            }
        }
    }
    else {
        return;
    }

    /* Process next string */
    length = strlen(next->string);
    for (i = 0; i < length; i++) {
        /* Removes extra spaces and tabs */
        if ((next->string[i] == ' ' && next->string[i+1] == ' ') || (next->string[i] == '\t'
                && next->string[i+1] == '\t')) {
            //spaceAdjust(next->string, i, header);
            next->string = (char *) malloc(sizeof(char) * strlen(newString));
            if (next->string == NULL) {
                return;
            }
            strcpy(next->string, newString);
            free(newString);
            /* Resets string until fully processed */
            length = strlen(next->string);
            i = 0;
        }
        /* Removes extra newline and carriage returns. Replaces with HTML tags */
        else if (next->string[i] == '\n' || next->string[i] == '\r') {
           // newString = newLineAdjust(next->string, i);
            next->string = (char *) malloc(sizeof(char) * strlen(newString));
            if (next->string == NULL) {
                return;
            }
            strcpy(next->string, newString);
            free(newString);
            /* Resets string until fully processed */
            length = strlen(next->string);
            i = 0;
        }
    }

    /* Checks next string for paragraph tag */
    for (i = 0; i < length; i++) {   
        if (next->string[0] == '<') {
            if (next->string[1] == 'P') {
                nextIndex = 0;
                break;
            }
            else {
                return;
            }
        } 

        if (next->string[i] == ' ' || next->string[i] == '\t') {
            //find index of <P>
            if (next->string[i+1] == '<') {
                if (next->string[i+2] == 'P') {
                    nextIndex = i + 1;
                    break;
                }
                else {
                    return;
                }
            }
        }
        else {
            return;
        }
    }

    /* Remove tag and white spaces for current */
    length = strlen(current->string);
    newString = malloc(sizeof(char) * (length-currIndex));
    if (newString == NULL) {
        return;
    }
    for (i = 0; i < (length-currIndex); i++) {
        newString[i] = current->string[i];
    }
    current->string = malloc(sizeof(char) * (length-currIndex));
    if (current->string == NULL) {
        return;
    }
    strcpy(current->string, newString);
    free(newString);
    length = strlen(next->string);
    newString = malloc(sizeof(char) * (length-nextIndex));
    if (newString == NULL) {
        return;
    }
    for (i = nextIndex, j = 0; i < length; i++) {
        newString[j] = next->string[i];
        j++;
    }
    next->string = malloc(sizeof(char) * (length-nextIndex));
    if (next->string == NULL) {
        return;
    }
    strcpy(next->string, newString); 
    free(newString);
}

int freeStructure(struct dataHeader *header)
{
    struct dataString *dsHead = header->next;
    struct dataString *current, *next;

    /* If there is no data, returns from function */
    if (header == NULL) {
        return FAILURE;
    }

    /* Reverses the list to be freed backwards */
    current = reverseList(dsHead);
    //header->next = NULL;
    

    /* Free from tail to head */
    while (current != NULL) {
        next = current->next;
        free(current->string);
        free(current);
        current = next;
    }
    
    /* Frees header from list */
    free(header->name);
    header->length = 0;
    free(header);
    header = NULL;

    return SUCCESS;
}

int freeReturnStruct(struct returnStruct *rS)
{
    rS->value = 0;
    free(rS);
    
    return SUCCESS;
}

struct dataString * reverseList(struct dataString *dsHead)
{
    struct dataString *current, *prev, *next;
    current = dsHead;
    prev = NULL;

    /* Reverses the list */
    while(current != NULL){
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    dsHead = prev;

    /* Returns reversed list */
    return (dsHead);
}

int writeStrings(char *filename, struct dataHeader *header)
{
    struct dataString * dS = header->next;
    FILE *fp;
    int nameLength, strLength, totalLength;

    if (header == NULL) {
        return FAILURE;
    }

    /* Calculate the length of all strings */
    while (dS != NULL) {
        totalLength = (strlen(dS->string) + 1) + totalLength;
        dS = dS->next;
    }
    header->length = totalLength;

    fp = fopen(filename, "wb");

    /* Checks if file is missing */
    if (fp == NULL) {
        return FAILURE;
    }

    /* Writes the header to the binary file */
    nameLength = strlen(header->name) + 1;
    fwrite(&nameLength, sizeof(int), 1, fp);
    fwrite(header->name, sizeof(char), nameLength, fp);
    fwrite(&header->length, sizeof(int), 1, fp);
    dS = header->next;

    /* Writes the rest of the nodes to the binary file */
    while (dS != NULL) {
        strLength = strlen(dS->string) + 1;
        fwrite(&strLength, sizeof(int), 1, fp);
        fwrite(dS->string, sizeof(char), strLength, fp);
        dS = dS->next;
    }
    fclose(fp);

    return SUCCESS;
}

struct returnStruct *readStrings(char *filename)
{   
    // Might need another return struct if fails
    struct returnStruct *rS = buildHeader();
    struct dataHeader *header = rS->header;
    struct dataString *dS;
    FILE * fp;
    int tempBuff, skip = 0;
    char * buffer;

    fp = fopen(filename, "rb");

    /* Checks if file is missing */
    if (fp == NULL) {
        rS->value = 0;
        rS->header = NULL;
        return rS;
    }

    /* Occupies the header information */
    fread(&tempBuff, sizeof(int), 1, fp);
    buffer = malloc(sizeof(char) * tempBuff);
    if (buffer == NULL) {
        rS->value = FAILURE;
        rS->header = NULL;
        return (rS);
    }
    fread(buffer, sizeof(char), tempBuff, fp);
    setName(header, buffer);
    fread(&header->length, sizeof(int), 1, fp);

    /* Occupies the dataString information in until EOF */
    while (!feof(fp)) {
        if (skip != 0) {
            buffer = malloc(sizeof(char) * tempBuff);
            if (buffer == NULL) {
                rS->value = FAILURE;
                rS->header = NULL;
                return (rS);
            }
            fread(buffer, sizeof(char), tempBuff, fp);
            addString(header, buffer);
            skip--;
        }
        else{
            fread(&tempBuff, sizeof(int), 1, fp);
            skip++;
        }
    }

    fclose(fp);

    /* Returns an occupied list */
    free(buffer);
    rS->header = header;
    rS->value = SUCCESS;
    return(rS);
}

void clrstr(char *buf){
    buf[0] = '\0';
}
