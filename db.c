/*
 * Alexander Lapena 
 * 0844071
 * CIS* 2750 - A4
 *
 * db.c
 * 
 * 17 November 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

#define MAX_QUERY 512
#define HOSTNAME  "dursley.socs.uoguelph.ca"
#define USERNAME  "alapena"
#define PASSWORD  "0844071"
#define DATABASE  "alapena"

void clrstr(char *buf);
void error(char *msg, MYSQL *mysql);

int main(int argc, char *argv[]) {
    MYSQL mysql;
    MYSQL_RES *res;
    MYSQL_ROW row;
    MYSQL_FIELD *field;
    char query[MAX_QUERY];
    int x;
    int i = 0;
    char records[1024]; 

    mysql_init(&mysql);
    mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "mydb");
    if (!mysql_real_connect(&mysql, HOSTNAME, USERNAME, PASSWORD,
        DATABASE, 0, NULL, 0)) {
       error("Could not connect to host.",&mysql);
    }
    
    if (strcmp(argv[1],"-clear") == 0) {
        clrstr(query);
        strcpy(query, "truncate table hfile");
        
        if(mysql_query(&mysql,query)){
            error("fail truncate 1",&mysql);
        }
        printf("Table rows cleared!\n");
    }
    else if (strcmp(argv[1],"-reset")== 0) {
        clrstr(query);
        strcpy(query, "drop table hfile");
    
        if(mysql_query(&mysql,query)){
            error("fail drop 1",&mysql);
        }
        printf("Table dropped\n");
    }
    else if (strcmp(argv[1],"-show") == 0) {
        clrstr(query);
        strcpy(query, "select name, length, date_time from hfile order by name");
        
        if(mysql_query(&mysql, query)){
            error("failed select 1",&mysql);
        }
        
        // Store results from query into res structure.
        if (!(res = mysql_store_result(&mysql))){
            error("failed store 1",&mysql);
        }
        printf("\n");
        mysql_free_result(res);
        
        // second query
        strcpy(query, "select name, length, date_time from hfile order by name");
        
        if(mysql_query(&mysql, query)){
            error("fail select 2",&mysql);
        }

        //  Store results from query into res structure.
        if (!(res = mysql_store_result(&mysql))){
            error("fail store 2",&mysql);
        }

        // print all results
        printf("DB Store:\n");
        while ((row = mysql_fetch_row(res))) {
            for (i=0; i < mysql_num_fields(res); i++){
                printf("%s ", row[i]);
            }
            printf("\n");
        }
        printf("\n");   
        mysql_free_result(res);
    }
    //Calls from php
    else if (strcmp(argv[1],"-db") == 0) {
        clrstr(query);
        strcpy(query, "select name from hfile order by name");
        
        if(mysql_query(&mysql, query)){
            return 0;
        }
        
        if (!(res = mysql_store_result(&mysql))){
            error("failed store 1",&mysql);
        }
        printf("\n");
        
        // print all results
        while ((row = mysql_fetch_row(res))) {
            printf("%s.html ", row[0]);
        }
        mysql_free_result(res);
    }
    else if (strcmp(argv[1],"-disp") == 0) {
        char filename[80];
        
        strcpy(filename,argv[2]);
        clrstr(query);
        sprintf(query, "select html from hfile where name = '%s'", filename);
    
        if(mysql_query(&mysql, query)){
            printf("fail\n");
            return 0;
        }   
        if (!(res = mysql_store_result(&mysql))){
            error("failed store 1",&mysql);
        }
        // print all results
        while ((row = mysql_fetch_row(res))) {
            printf("%s", row[0]);
        }
        mysql_free_result(res);
    }
    else if (strcmp(argv[1],"-help") == 0) {
        printf("**************\n");
        printf("DATABASE HELP:\n");
        printf("**************\n");
        printf("-show: reveals information in the data table\n");
        printf("-clear: removes rows from the tables in the database\n");
        printf("-reset: deletes the tables from the database\n\n");
    }

    /*Close connection to server*/
    mysql_server_end();
    mysql_close(&mysql);

    return 0;
}

/* Standard error clause*/
void error(char *msg, MYSQL *mysql){
    printf("%s\n%s\n",msg,mysql_error(mysql));
    exit(1);
}

/* Clear char array*/
void clrstr(char *buf){
    buf[0] = '\0';
}

