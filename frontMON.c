#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define MAX_LENGTH 30


int main(int argc, char * argv[])
{
    FILE * fp;
    char str[60];               // max strg length
    const char s[2] = ";";    // sets the break parameter

    errno = 0;

    
    //creates variables for stations id and name, coordinates not needed for our querys.

    char * sName = malloc(MAX_LENGTH); 
    
    if (errno == ENOMEM)
    {
         perror("Not enough memory");

        return 1;
    }

    size_t sId;

    char *token;

    int i = 0;

        // pass each station data to parameters and send it to backend


    if (argc > 3 || argc < 3) 
    {
        // if command line arguments dont match the requirements, send message

        perror("Error in the amount of parameters");

        return 1;
    }


    fp = fopen(argv[1] , "r");   // opens the stations csv file

    if(fp == NULL) 
    {
            perror("Error opening file");
            return(-1);
    }

        while(fgets(str, sizeof(str), fp) != NULL)
        {

            i = 0;
            
                token = strtok(str, s);
                while( token != NULL )
                {
                    if(i == 0)
                    {
                        sId = atoi(token);
                    }
                    else
                    {
                        strcpy(sName, token);
                    }

                    token = strtok(NULL, s);    
                }

            // call the add station in the backend

        }
    
}