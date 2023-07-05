#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "bikeSharing.h"
#define MAX_WORD_LENGTH 30
#define MAX_LINE_LENGTH 100


int main(int argc, char * argv[])
{
    FILE * fp_stations, * fp_trips;
    char str[MAX_LINE_LENGTH];               // max strg length
    const char s[2] = ";";    // sets the break parameter
    errno = 0;

    //Checks for errors int he amount of parameters

    if (argc > 5 || argc < 4) 
    {
        perror("Error in the amount of parameters");
        return 1;
    }
    
    //creates variables for stations id and name, coordinates not needed for our querys.

    bikeSharingADT bikeSharing = newBikeSharing();

    char * sName = malloc(MAX_WORD_LENGTH);
    
    if (errno == ENOMEM)
    {
         perror("Not enough memory");

        return 1;
    }

    size_t sId;

    char *token;

    int i = 0;



    // pass each station data to parameters and send it to backend


    fp_stations = fopen(argv[1] , "r");   // opens the stations csv file
    fp_trips = fopen(argv[2], "r");


    if(fp_stations == NULL) 
    {
            perror("Error opening file");
            return(-1);
    }

    while(fgets(str, sizeof(str), fp_stations) != NULL)
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
            i++;
        }

        addStation(bikeSharing, sName, sId);
    }
    
    // When finished loading the stations, we load the trips (reutilizing str)
    // start_date;emplacement_pk_start;end_date;emplacement_pk_end;is_member

    char * date_delim = '-';
    char * date_token;
    int start_year, start_month;
    size_t start_id, end_id;
    char is_member;

    int limit_start_year, limit_end_year; 

    if (argc < 3) {
        limit_start_year = -1;
        limit_end_year = 3000;
    }
    else if (argc == 3) {
        limit_start_year = argv[3];
        limit_end_year = 3000;
    }
    else if (argc == 4) {
        limit_start_year = argv[3];
        limit_end_year = argv[4];
    }

    // start_date;emplacement_pk_start;end_date;emplacement_pk_end;is_member

    i=0;
    while (fgets(str, sizeof(str), fp_trips) != NULL)
    {
        token = strtok(str, s);
        while (token != NULL) 
        {
            switch(i) {
                case 0:
                    int j;
                    for (int j=0; j<2; j++) { // Arreglar este magic number !
                        date_token = strtok(token, date_delim);
                        if (j == 0) {
                            start_year = atoi(date_token);
                        }
                        else{
                            start_month = atoi(date_token);
                        }
                    }
                    break;

                case 1:
                    start_id = atoi(token);
                    break;
                case 2:
                    break;
                case 3:
                    end_id = atoi(token);
                    break;
                case 4:
                    is_member = atoi(token);
                    break;
            }
            i++;
            

            addTrip(bikeSharing, is_member, start_id, end_id, start_year, start_month, limit_start_year, limit_end_year);
        }
    }
}
