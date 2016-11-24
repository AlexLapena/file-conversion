/*
 * Alexander Lapena 
 * 0844071
 * CIS* 2750 - A4
 *
 * main.c
 * 
 * 17 November 2016
 */

#include "listio.h"

int main(int argc, char **argv)
{
    struct returnStruct *rS; 
    FILE *fp;
    char *fileName;
    char *htmlFile; 
    int errCheck = 1, length, i, j = 0, x;
    char *string;
    int dbStore, check = 0, qCheck = 0, hSize = 0;
    MYSQL mysql;
    MYSQL_RES *res;
    MYSQL_ROW row;
    MYSQL_FIELD *field;
    char query[MAX_QUERY];
    int d = 0;
    //char record[5000]; //Fix me... make dynamic?
    char * record;
    char * fn = NULL;
    char str[60000];
    
    if (argc != 3) {
        printf("Error! Filename not entered properly.\n");
        return 0;
    }    
    
    /* Retrieves file name from command line */
    length = strlen(argv[1]);
    fileName = calloc(length+1, sizeof(char));
    strcpy(fileName, argv[1]);

    htmlFile = malloc(sizeof(char) * (strlen(fileName) + 6));
    strcpy(htmlFile, fileName);
    strcat(htmlFile, ".html");

    fp = fopen(htmlFile, "r");
    // Prints out HTML file 
    if (fp != NULL) {
        fseek(fp, 0, SEEK_END);
        length = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        string = malloc(sizeof(char) * length+1);
        while (fgets(string, length, fp) != NULL) {
            printf("%s", string);          
        }
        fclose (fp);
        free(string);
        free(fileName);
        free(htmlFile);
        return 0;
    }

    fp = fopen(fileName, "r");
    // If HTML file does not exist 
    if (fp != NULL) {
        rS = buildHeader();
        errCheck = setName(rS->header, fileName);
        // Reads in text file 
        if (fp) {
            fseek(fp, 0, SEEK_END);
            length = ftell(fp) + 1;
            fseek(fp, 0, SEEK_SET);
            string = calloc(length+1, sizeof(char));
            fread(string, 1, length, fp);
            addString(rS->header, string);
        }
        free(string);
        fclose (fp);
        errCheck = processStrings(rS->header);
        errCheck = freeStructure(rS->header);
        errCheck = freeReturnStruct(rS);
        // Read strings from .html doc created 
        fp = fopen(htmlFile, "r");
        if (fp != NULL) {
            fseek(fp, 0, SEEK_END);
            length = ftell(fp);
            hSize = length;
            fseek(fp, 0, SEEK_SET);
            string = calloc(length+1, sizeof(char));
            fread(string, 1, length, fp);
            printf("%s\n", string); 
        }
    }
    
    // Database Storage
    if (strcmp(argv[2],"1")==0) {
        printf("Connecting to database...\n");
    
        mysql_init(&mysql);
        mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
        if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD,
            DATABASE, 0, NULL, 0)) {
           error("Could not connect to host.",&mysql);
        }   
        printf("Found database.\n");
        
        // Checks if table is made or not.
        clrstr(query);
        printf("create\n");
        clrstr(query);  
        strcat(query, "create table if not exists hfile (");
        strcat(query, "html text(65000),");
        strcat(query, "length integer,");
        strcat(query, "name char(30),");
        strcat(query, "date_time datetime,");
        strcat(query, "primary key(name) )");
        printf("Creating hfile table.\n");      
        // Create Table
        if(mysql_query(&mysql, query)){
          error("Could not create table!",&mysql);
        }
        clrstr(query);
        
        // Insert values
        printf("Inserting hfile.\n");
        fread(string, 1, length, fp);
        // Remove .txt and directory from fileName  
        length = strlen(fileName);  
        for (i = 0; i < length; i++) {
            if(fileName[i] == '.'){
                check = i;
            }
        }
        // if there is a .txt 
        if (check != 0) {
            fn = calloc(length-9, sizeof(char));
            for (i = 6; i<length; i++) {
                if(fileName[i] == '.'){
                    break;
                }
                fn[j] = fileName[i];
                j++;
            }
        }
        // if there is no .txt
        else{
            length = strlen(fileName);
            fn = calloc(length-5, sizeof(char));
            for (i = 6; i<length; i++) {
                fn[j] = fileName[i];
                j++;
            }
        }
        // Length of string in bytes
        length = strlen(string);    
        mysql_real_escape_string(&mysql, str, string, length);
        asprintf(&record,"insert into hfile values ('%s', %d, '%s',NOW())", str, hSize, fn);
        
        if(mysql_query(&mysql, record)){
            printf("Failure to insert: %s\n",record);
            error("Could not insert record",&mysql);
        }
        
        // Close Connection to server
        remove(htmlFile);       
        mysql_server_end();
        mysql_close(&mysql);
        free(fn);
        free(record);
        printf("Finished.\n");
        
    }
    
    fclose(fp); 
    free(string);
    free(fileName);
    free(htmlFile);
    
    return 0;
}
