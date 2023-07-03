#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main()
{
    FILE * fp;
    char str[60];
    const char s[2] = ";";
    char *token;
    int i = 0;


   
        fp = fopen("stationsMON.csv" , "r");

        if(fp == NULL) 
        {
            perror("Error opening file");
            return(-1);
        }

        while(fgets(str, sizeof(str), fp) != NULL)
        {
            
            token = strtok(str, s);
                while( token != NULL )
                {
                    

                    token = strtok(NULL, s);    
                }

        }
    
}
