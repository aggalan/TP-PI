#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "bikeSharing.h"
#include "htmlTable.h"
#define MAX_WORD_LENGTH 30 
#define MAX_LINE_LENGTH 100 //Para leer una linea de cualquier archivo
#define MAX_NUMBER_LENGTH 15 //Cantidad de memoria para pasar un int a un char (para los archivos html)
#define MEMORY_CHECK(ptr) if (ptr == NULL){\
                            perror("Error, insufficient memory");\
                            return 1;\
                          }\

enum months {JAN=0, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC};

int main(int argc, char *argv[])
{
    FILE *fp_stations, *fp_trips, *fp_q1, *fp_q2, *fp_q3, *fp_q4;


    char str[MAX_LINE_LENGTH]; // max strg length
    const char s[2] = ";";     // sets the break parameter

    errno = 0;

    // Checks for errors int he amount of parameters

    if (argc > 5 || argc < 3)
    {
        perror("Error in the amount of parameters");
        return 1;
    }

    fp_stations = fopen(argv[1], "r"); // opens the stations csv file
    fp_trips = fopen(argv[2], "r");

    MEMORY_CHECK(fp_stations)
    MEMORY_CHECK(fp_trips)

    
    int start_year, start_month, start_id, end_id, is_member, limit_start_year, limit_end_year;


    if (argc < 4)
    {
        limit_start_year = -1;
        limit_end_year = 3000;
    }
    else if (argc == 4)
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
            return(1);
        }
    }

    // creates variables for stations id and name, coordinates not needed for our querys.

    bikeSharingADT bikeSharing = newBikeSharing(limit_start_year, limit_end_year);

    MEMORY_CHECK(bikeSharing);

    //Declares variables for file reading

    char *sName = malloc(MAX_LINE_LENGTH), *token = malloc(MAX_LINE_LENGTH), *tokenAux = token;
    int sId, cantStations, dim;

    MEMORY_CHECK(sName)
    MEMORY_CHECK(token)

    // Passes each station's data to parameters and send it to backend

    int i = 0;

    fgets(str, sizeof(str), fp_stations); // descarto primera linea;
   

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
            return 1;
        }
    }
    
    if(setMatrix(bikeSharing, &cantStations))
    {
        perror("Error allocating memory");
        return 1;
    }


    // Loads trips data (reutilizing str)


    fgets(str, sizeof(str), fp_trips); // descarto la primera linea

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

    


    //We need to pass the numbers to a string format

    char trips_per_Months[MONTHS][MAX_NUMBER_LENGTH]; 
    char tripsString2[MAX_NUMBER_LENGTH];
    char tripsString[MAX_NUMBER_LENGTH];
    

    // QUERY 1

    q1_struct *vec1 = q1(bikeSharing, 1);
    MEMORY_CHECK(vec1)


    htmlTable table1 = newTable("query1.html", 2, "Station", "StartedTrips");
    fp_q1 = fopen("query1.csv", "w");
    fprintf(fp_q1, "Station;StartedTrips\n");


    for (int i = 0; i < cantStations; i++)
    { 

        fprintf(fp_q1, "%s;%ld\n", vec1[i].station_name, vec1[i].trips);

        sprintf(tripsString, "%ld",vec1[i].trips );
        addHTMLRow(table1, vec1[i].station_name, tripsString);

    }

    fclose(fp_q1);
    closeHTMLTable(table1);
    freeVec1(bikeSharing, vec1);


    //QUERY 2

    q2_struct *vec2 = q2(bikeSharing, &dim);
    MEMORY_CHECK(vec2)


    htmlTable table2 = newTable("query2.html", 4, "Station A", "Station B", "Trips A -> B", "Trips B -> A");
    fp_q2 = fopen("query2.csv", "w");
    fprintf(fp_q2, "StationA;StationB;Trips A -> B;Trips B -> A\n");


    for (int i = 0; i < dim; i++)
    { 

        fprintf(fp_q2, "%s;%s;%d;%d\n", vec2[i].start_station, vec2[i].end_station, vec2[i].trips_start_end, vec2[i].trips_end_start);
        
        sprintf(tripsString, "%d",vec2[i].trips_start_end );
        sprintf(tripsString2, "%d",vec2[i].trips_end_start );

        addHTMLRow(table2, vec2[i].start_station, vec2[i].end_station, tripsString, tripsString2);
    }


    fclose(fp_q2);
    closeHTMLTable(table2);
    freeVec2(bikeSharing, vec2, dim);


    //QUERY 3

    q3_struct *vec3 = q3(bikeSharing);
    MEMORY_CHECK(vec3)

    htmlTable table3 = newTable("query3.html", 13, "J", "F", "M", "A", "M", "J", "J", "A", "S", "O", "N", "D", "Station"); 
    fp_q3 = fopen("query3.csv", "w"); 
    fprintf(fp_q3, "J;F;M;A;M;J;J;A;S;O;N;D\n");


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


    fclose(fp_q3);
    closeHTMLTable(table3);
    freeVec3(bikeSharing, vec3);


    //QUERY 4

    q1_struct *vec4 = q1(bikeSharing, 4);
    MEMORY_CHECK(vec4)

    htmlTable table4 = newTable("query4.html", 2, "Station", "RoundingTrips");
    fp_q4 = fopen("query4.csv", "w");
    fprintf(fp_q4, "Station;RoundingTrips\n");

    for (int i = 0; i < cantStations; i++)
    { 

        fprintf(fp_q4, "%s;%ld\n", vec4[i].station_name, vec4[i].trips);

        sprintf(tripsString, "%ld",vec4[i].trips );
        addHTMLRow(table4, vec4[i].station_name, tripsString);

    }


    fclose(fp_q4);
    closeHTMLTable(table4);
    freeVec1(bikeSharing, vec4);


    fclose(fp_stations);
    fclose(fp_trips);

    free(tokenAux);
    free(sName);
    freeBikeSharing(bikeSharing);

}

