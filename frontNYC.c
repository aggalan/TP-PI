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
                            return 1;\
                          }\

enum months {JAN=0, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC};

int main(int argc, char *argv[])
{
    FILE *fp_stations, *fp_trips, *fp_q1, *fp_q2, *fp_q3, *fp_q4;


    char str[MAX_LINE_LENGTH]; // For storing one .csv line at a time
    const char s[2] = ";";     // ";" is the separating character in both files

    errno = 0;

    // Checks for errors in of parameters (amount and order)

    if (argc > 5 || argc < 3)
    {
        perror("Error in the amount of parameters");
        return 1;
    }

    fp_stations = fopen(argv[1], "r"); 
    fp_trips = fopen(argv[2], "r");

    MEMORY_CHECK(fp_stations) //If the first parameters were years (wrong order), this will abort
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

        if (limit_end_year < limit_start_year) {
            perror("Invalid range of parameters");
            return 1;
        }
    }

    // Loading process starts

    bikeSharingADT bikeSharing = newBikeSharing(limit_start_year, limit_end_year);

    MEMORY_CHECK(bikeSharing);

    //Declares variables for file reading

    char *sName = malloc(MAX_LINE_LENGTH), *token = malloc(MAX_LINE_LENGTH), *tokenAux = token;
    int sId, cantStations;
    int i = 0;

    MEMORY_CHECK(sName)
    MEMORY_CHECK(token)


    //Passes each station data to parameters and sends it to the backend

    fgets(str, sizeof(str), fp_stations); // First line of stations.csv is non important
   

    while (fgets(str, sizeof(str), fp_stations) != NULL)
    {

        i = 0;

        token = strtok(str, s);
        while (token != NULL)
        {
            if (i == 3)
            {
                sId = atoi(token);
            }
            if (i == 0)
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

    
    int memFlag = setMatrix(bikeSharing, &cantStations);
    if(memFlag)
    {
        perror("Error allocating memory");
        return 1;
    }



    // Passes each trip data to parameters and sends it to the backend (reutilizing str)


    fgets(str, sizeof(str), fp_trips); // First line of bikes.csv is also non important

    while (fgets(str, sizeof(str), fp_trips) != NULL)
    {

        int i = 0;
        char member_casual[7] = "casual";

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
            if (i == 5)
            {
                is_member = strcmp(member_casual,token);
            }
            token = strtok(NULL, s);

        }

         addTrip(bikeSharing, is_member, start_id, end_id, start_year, start_month);
    }

    free(tokenAux);
    free(sName);

    // Creates vectors with the necessary information for each query

     q1_struct *vec1 = q1(bikeSharing, 1);

    MEMORY_CHECK(vec1)

    q2_struct *vec2 = q2(bikeSharing);

    MEMORY_CHECK(vec2)

    q3_struct *vec3 = q3(bikeSharing);

    MEMORY_CHECK(vec3)

    q1_struct *vec4 = q1(bikeSharing, 4);

    MEMORY_CHECK(vec4)


   
   
    // Creates tables for the html files

    htmlTable table1 = newTable("query1.html", 2, "Station", "StartedTrips");

    htmlTable table2 = newTable("query2.html", 4, "Station A", "Station B", "Trips A -> B", "Trips B -> A");

    htmlTable table3 = newTable("query3.html", 13, "J", "F", "M", "A", "M", "J", "J", "A", "S", "O", "N", "D", "Station"); 

    htmlTable table4 = newTable("query4.html", 2, "Station", "RoundingTrips");

    
    
    // APERTURA ARCHIVOS CSV Y PONEMOS LOS TITULOS DE LOS QUERYS

    //crear un solo string largo

    char pasajeString[MAX_NUMBER_LENGTH];
    char pasajeString2[MAX_NUMBER_LENGTH];

    char pasajesMonths[MONTHS][MAX_NUMBER_LENGTH]; 


    fp_q1 = fopen("query1.csv", "w");
    fprintf(fp_q1, "Station;StartedTrips\n");

    for (int i = 0; i < cantStations; i++)
    { 
        // QUERY 1 CSV Y HTML

        fprintf(fp_q1, "%s;%ld\n", vec1[i].station_name, vec1[i].trips);

        sprintf(pasajeString2, "%ld",vec1[i].trips );
        addHTMLRow(table1, vec1[i].station_name, pasajeString2);

    }
    fclose(fp_q1);
    closeHTMLTable(table1);
    freeVec1(bikeSharing, vec1);


    fp_q2 = fopen("query2.csv", "w");
    fprintf(fp_q2, "StationA;StationB;Trips A -> B;Trips B -> A\n");

    for (int i = 0; i < (cantStations * cantStations) - cantStations; i++)  // la longitud de este ciclo esta mal
    { 

        // QUERY 2 CSV

        fprintf(fp_q2, "%s;%s;%d;%d\n", vec2[i].start_station, vec2[i].end_station, vec2[i].trips_start_end, vec2[i].trips_end_start);

        // QUERY 2 HTML

        
        sprintf(pasajeString, "%d",vec2[i].trips_end_start );
        sprintf(pasajeString2, "%d",vec2[i].trips_start_end );

        addHTMLRow(table2, vec2[i].start_station, vec2[i].end_station, pasajeString2, pasajeString);
    }
    fclose(fp_q2);
    closeHTMLTable(table2);
    freeVec2(bikeSharing, vec2);


    fp_q3 = fopen("query3.csv", "w"); 
    fprintf(fp_q3, "J;F;M;A;M;J;J;A;S;O;N;D\n");


    for (int i = 0; i < cantStations; i++)
    { 

        // QUERY 3 CSV Y HTML

         for (int m = JAN; m <= DEC; m++)
        {
            sprintf(pasajesMonths[m], "%d", vec3[i].months[m]);
            fprintf(fp_q3, "%d;", vec3[i].months[m]);
        }
        fprintf(fp_q3, "%s\n", vec3[i].station_name);

        addHTMLRow(table3, pasajesMonths[JAN], pasajesMonths[FEB], pasajesMonths[MAR], pasajesMonths[APR], pasajesMonths[MAY], pasajesMonths[JUN], pasajesMonths[JUL], pasajesMonths[AUG], pasajesMonths[SEP], pasajesMonths[OCT], pasajesMonths[NOV], pasajesMonths[DEC], vec3[i].station_name);

    }    
    fclose(fp_q3);
    closeHTMLTable(table3);
    freeVec3(bikeSharing, vec3);


    fp_q4 = fopen("query4.csv", "w");
    fprintf(fp_q4, "Station;RoundingTrips\n");

    for (int i = 0; i < cantStations; i++)
    { 

        // QUERY 4 CSV Y HTML

        fprintf(fp_q4, "%s;%ld\n", vec4[i].station_name, vec4[i].trips);

        sprintf(pasajeString, "%ld",vec4[i].trips );
        addHTMLRow(table4, vec4[i].station_name, pasajeString);

    }
    fclose(fp_q4);
    closeHTMLTable(table4);
    freeVec1(bikeSharing, vec4);

    fclose(fp_stations);
    fclose(fp_trips);

    freeBikeSharing(bikeSharing); 
}
