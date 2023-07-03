#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char * argv[])
{
    FILE * fp;
    char str[60];
    const char s[2] = ";";
    char *token;
    int i = 0;

    if (argc > 2 || argc < 2) {
        perror("Error in the amount of parameters");
        return 1;
    }
   
        fp = fopen(argv[1] , "r");

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
