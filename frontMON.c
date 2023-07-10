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

    // creates variables for stations id and name, coordinates not needed for our querys.

    bikeSharingADT bikeSharing = newBikeSharing();

    MEMORY_CHECK(bikeSharing);

    //Declares variables for file reading

    char *sName = malloc(MAX_LINE_LENGTH), *token = malloc(MAX_LINE_LENGTH), *tokenAux = token;
    int sId, cantStations;

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
    
    int memFlag = setMatrix(bikeSharing, &cantStations);
    if(memFlag)
    {
        perror("Error allocating memory");
        return 1;
    }


    // When finished loading the stations, we load the trips (reutilizing str)
    // start_date;emplacement_pk_start;end_date;emplacement_pk_end;is_member

    int start_year, start_month;
    int start_id, end_id;
    int is_member;

    int limit_start_year, limit_end_year;

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
    }


    fgets(str, sizeof(str), fp_trips); // descarto la primera linea

    // start_date;emplacement_pk_start;end_date;emplacement_pk_end;is_member

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

            addTrip(bikeSharing, is_member, start_id, end_id, start_year, start_month, limit_start_year, limit_end_year);
        }
    }

    


    // CREO LOS VECTORES CON LA INFORMACION PARA LAS 4 QUERYS

    q1_struct *vec1 = q1(bikeSharing, 1);

    if(vec1 == NULL)
    {
        perror("Error allocating memory");
        return 1;
    }

    q2_struct *vec2 = q2(bikeSharing);

    if(vec2 == NULL)
    {
        perror("Error allocating memory");
        return 1;
    }

    q3_struct *vec3 = q3(bikeSharing);

    if(vec3 == NULL)
    {
        perror("Error allocating memory");
        return 1;
    }

    q1_struct *vec4 = q1(bikeSharing, 4);

    if(vec4 == NULL)
    {
        perror("Error allocating memory");
        return 1;
    }

   
   
    // CREACION TABLAS PARA LOS 4 QUERYS

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


    fclose(fp_stations);
    fclose(fp_trips);


    free(tokenAux);
    free(sName);
    freeBikeSharing(bikeSharing, vec1, vec4, vec2, vec3);
    
}
