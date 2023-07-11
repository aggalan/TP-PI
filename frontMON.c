#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "bikeSharing.h"
#include "htmlTable.h"
#define MAX_WORD_LENGTH 30 
#define MAX_LINE_LENGTH 100 
#define MAX_NUMBER_LENGTH 15 
#define MEMORY_CHECK(ptr) if (ptr == NULL){\
                            perror("Error, insufficient memory");\
                            return NO_MEMORY;\
                          }\

enum months {JAN=0, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC};
enum status {OK=0,FILE_NOT_FOUND, NO_MEMORY, INCORRECT_AMOUNT_OF_PARAMETERS, INVALID_RANGE_YEARS, INCORRECT_FILE};

int main(int argc, char *argv[])
{
    FILE *fp_stations, *fp_trips, *fp_q1, *fp_q2, *fp_q3, *fp_q4;


    char str[MAX_LINE_LENGTH]; 
    const char s[2] = ";";     // ";" is the separating character in both files

    errno = 0;


    if (argc > 5 || argc < 3) // if true, either the user dindn´t run the executable with both .csv files or exceeded the maximum amount of parameters
    {
        perror("Error in the amount of parameters");
        return INCORRECT_AMOUNT_OF_PARAMETERS;
    }

    fp_trips = fopen(argv[1], "r");
    fp_stations = fopen(argv[2], "r"); 
    

    if(fp_stations == NULL || fp_trips == NULL) // if true, one or both files can´t be read
    {
        perror("Error opening file");
        return FILE_NOT_FOUND;
    }

    
    int start_year, start_month, start_id, end_id, is_member, limit_start_year, limit_end_year;


    if (argc < 4) //if true, no year restriction is required so limits are set so that every year is valid
    {
        limit_start_year = -1;
        limit_end_year = 3000;
    }
    else if (argc == 4) //if true, only start year is required
    {
        limit_start_year = atoi(argv[3]);
        limit_end_year = 3000;                      
    }
    else if (argc == 5)
    {
        limit_start_year = atoi(argv[3]);
        limit_end_year = atoi(argv[4]);

        if(limit_end_year < limit_start_year)
        {
            perror("Invalid range of years");
            return INVALID_RANGE_YEARS;
        }
    }
    
    fgets(str, sizeof(str), fp_stations);  // Check if the order of files is correct or is inverted.
    if(str[0] != 's')
    {
        perror("Error in order of .csv files");
        return INCORRECT_FILE;
    }

      // Loading process starts

    bikeSharingADT bikeSharing = newBikeSharing(limit_start_year, limit_end_year);

    MEMORY_CHECK(bikeSharing);

    char *sName = malloc(MAX_LINE_LENGTH), *token = malloc(MAX_LINE_LENGTH), *tokenAux = token;
    int sId, cantStations, dim;

    MEMORY_CHECK(sName)
    MEMORY_CHECK(token)

    // Every line of stationsMON.csv is broken-down to variables and the stations information is sent to the backend.


    int i = 0;   

    while (fgets(str, sizeof(str), fp_stations) != NULL)
    {

        i = 0;

        token = strtok(str, s);
        while (token != NULL && i < 2)
        {
            if (i == 0)
            {
                sId = atoi(token);
            }
            if (i == 1)
            {
                strcpy(sName, token);
            }
            token = strtok(NULL, s);
            i++;
        }

        if(addStation(bikeSharing, sName, sId))
        {
            perror("Error adding station");
            return NO_MEMORY;
        }
    }
    
    // once finished the upload of ALL stations it is compulsory to call setMatrix, so it prepares the data structure to store trips and saves the amount of stations.

    if(setMatrix(bikeSharing, &cantStations))
    {
        perror("Error allocating memory");
        return NO_MEMORY;
    }

    fgets(str, sizeof(str), fp_trips); // discards the first line of the file as it only contains titles.

    // Every line of bikesMON.csv is broken-down to variables and the trips information is sent to the backend.

    while (fgets(str, sizeof(str), fp_trips) != NULL)
    {

        int i = 0;

        token = strtok(str, s);
        for (i = 0; token != NULL; i++)
        {

            if (i == 0)
            {
                sscanf(token, "%d-%d", &start_year, &start_month);
            }

            if (i == 1)
            {
                start_id = atoi(token);
            }
            if (i == 3)
            {
                end_id = atoi(token);
            }
            if (i == 4)
            {
                is_member = atoi(token);
            }
            token = strtok(NULL, s);
        }

        addTrip(bikeSharing, is_member, start_id, end_id, start_year, start_month); 
    }

     //Frees the resources used to break-down the information from the .csv files

    free(tokenAux);
    free(sName);

    //As the html functions only accept string format tu upload a row, some variables are needed.

    char trips_per_Months[MONTHS][MAX_NUMBER_LENGTH]; 
    char tripsString2[MAX_NUMBER_LENGTH];
    char tripsString[MAX_NUMBER_LENGTH];
    

    // QUERY 1: .csv and .html files are completed

    // First we fill an array with structs with the info needed for the query
    q1_struct *vec1 = q1(bikeSharing, 1);

    MEMORY_CHECK(vec1)

    // Starts a new HTML table with the titles

    htmlTable table1 = newTable("query1.html", 2, "Station", "StartedTrips");

    // Opens the .csv so that it can be filled


    fp_q1 = fopen("query1.csv", "w");

    // Completes the first line with titles

    fprintf(fp_q1, "Station;StartedTrips\n");

    // Adds all the rows

    for (int i = 0; i < cantStations; i++)
    { 

        fprintf(fp_q1, "%s;%ld\n", vec1[i].station_name, vec1[i].trips);

        sprintf(tripsString, "%ld",vec1[i].trips );
        addHTMLRow(table1, vec1[i].station_name, tripsString);

    }


    //Closes the files and frees the resources used in the first query

    fclose(fp_q1);
    closeHTMLTable(table1);
    freeVec1(bikeSharing, vec1);



    // QUERY 2: .csv and .html files are completed

    // First we fill an array with structs with the info needed for the query (also array dimension is saved)
    q2_struct *vec2 = q2(bikeSharing, &dim);
    MEMORY_CHECK(vec2)

    // Starts a new HTML table with the titles

    htmlTable table2 = newTable("query2.html", 4, "Station A", "Station B", "Trips A -> B", "Trips B -> A");
    
    // Opens the .csv so that it can be filled
    
    fp_q2 = fopen("query2.csv", "w");

    // Completes the first line with titles

    fprintf(fp_q2, "StationA;StationB;Trips A -> B;Trips B -> A\n");

    // Adds all the rows

    for (int i = 0; i < dim; i++)
    { 

        fprintf(fp_q2, "%s;%s;%d;%d\n", vec2[i].start_station, vec2[i].end_station, vec2[i].trips_start_end, vec2[i].trips_end_start);
        
        sprintf(tripsString, "%d",vec2[i].trips_start_end );
        sprintf(tripsString2, "%d",vec2[i].trips_end_start );

        addHTMLRow(table2, vec2[i].start_station, vec2[i].end_station, tripsString, tripsString2);
    }

    //Closes the files and frees the resources used in the second query

    fclose(fp_q2);
    closeHTMLTable(table2);
    freeVec2(bikeSharing, vec2, dim);


    // QUERY 3: .csv and .html files are completed


    // First we fill an array with structs with the info needed for the query 

    q3_struct *vec3 = q3(bikeSharing);
    MEMORY_CHECK(vec3)

    // Starts a new HTML table with the titles

    htmlTable table3 = newTable("query3.html", 13, "J", "F", "M", "A", "M", "J", "J", "A", "S", "O", "N", "D", "Station"); 
    
    // Opens the .csv so that it can be filled
    
    fp_q3 = fopen("query3.csv", "w"); 
    
    // Completes the first line with titles
    
    fprintf(fp_q3, "J;F;M;A;M;J;J;A;S;O;N;D\n");

    // Adds all the rows

    for (int i = 0; i < cantStations; i++)
    { 

         for (int m = JAN; m <= DEC; m++)
        {

            sprintf(trips_per_Months[m], "%d", vec3[i].months[m]);
            fprintf(fp_q3, "%d;", vec3[i].months[m]);

        }
        fprintf(fp_q3, "%s\n", vec3[i].station_name);

        addHTMLRow(table3, trips_per_Months[JAN], trips_per_Months[FEB], trips_per_Months[MAR], trips_per_Months[APR], trips_per_Months[MAY], trips_per_Months[JUN], trips_per_Months[JUL], trips_per_Months[AUG], trips_per_Months[SEP], trips_per_Months[OCT], trips_per_Months[NOV], trips_per_Months[DEC], vec3[i].station_name);

    }   

    //Closes the files and frees the resources used in the third query

    fclose(fp_q3);
    closeHTMLTable(table3);
    freeVec3(bikeSharing, vec3);


    // QUERY 4: .csv and .html files are completed



    // First we fill an array with structs with the info needede for the query
    
    q1_struct *vec4 = q1(bikeSharing, 4);
    MEMORY_CHECK(vec4)

    // Starts a new HTML table with the titles
    
    htmlTable table4 = newTable("query4.html", 2, "Station", "RoundingTrips");

    // Opens the .csv so that it can be filled

    fp_q4 = fopen("query4.csv", "w");

    // Completes the first line with titles

    fprintf(fp_q4, "Station;RoundingTrips\n");

    // Adds all the rows

    for (int i = 0; i < cantStations; i++)
    { 

        fprintf(fp_q4, "%s;%ld\n", vec4[i].station_name, vec4[i].trips);

        sprintf(tripsString, "%ld",vec4[i].trips );
        addHTMLRow(table4, vec4[i].station_name, tripsString);

    }

    //Closes the files and frees the resources used in the fourth query

    fclose(fp_q4);
    closeHTMLTable(table4);
    freeVec1(bikeSharing, vec4);


    //Closes the .csv files

    fclose(fp_stations);
    fclose(fp_trips);

    //frees the resources used in the backend

    freeBikeSharing(bikeSharing);

    return OK;

}

