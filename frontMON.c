#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "bikeSharing.h"
#include "htmlTable.h"
#define MAX_WORD_LENGTH 30
#define MAX_LINE_LENGTH 100

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

    // creates variables for stations id and name, coordinates not needed for our querys.

    bikeSharingADT bikeSharing = newBikeSharing();

    char *sName = malloc(MAX_LINE_LENGTH);

    if (errno == ENOMEM)
    {
        perror("Not enough memory");

        return 1;
    }

    int sId;

    char *token = malloc(MAX_LINE_LENGTH);

    int i = 0;

    // pass each station data to parameters and send it to backend

    fp_stations = fopen(argv[1], "r"); // opens the stations csv file
    fp_trips = fopen(argv[2], "r");

    if (fp_stations == NULL)
    {
        perror("Error opening file");
        return (-1);
    }


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

        addStation(bikeSharing, sName, sId);
    }

    
    int cantStations;
    
    setMatrix(bikeSharing, &cantStations);


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
        limit_end_year = 3000;                      /// guarda esto porque capaz te hace un archivo hasta el 3000 todo vacio
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

    q2_struct *vec2 = q2(bikeSharing);

    q3_struct *vec3 = q3(bikeSharing);

    q1_struct *vec4 = q1(bikeSharing, 4);

   
   
    // CREACION TABLAS PARA LOS 4 QUERYS

    htmlTable table1 = newTable("query1.html", 2, "Station", "StartedTrips");

    htmlTable table2 = newTable("query2.html", 4, "Station A", "Station B", "Trips A -> B", "Trips B -> A");

    htmlTable table3 = newTable("query3.html", 13, "J", "F", "M", "A", "M", "J", "J", "A", "S", "O", "N", "D", "Station"); 

    htmlTable table4 = newTable("query4.html", 2, "Station", "RoundingTrips");

    
    
    // APERTURA ARCHIVOS CSV Y PONEMOS LOS TITULOS DE LOS QUERYS

    //crear un solo string largo


    fp_q1 = fopen("query1.csv", "w");
    fprintf(fp_q1, "Station;");
    fprintf(fp_q1, "StartedTrips\n");


    fp_q2 = fopen("query2.csv", "w");
    fprintf(fp_q2, "StationA;");
    fprintf(fp_q2, "StationB;");
    fprintf(fp_q2, "Trips A -> B;");
    fprintf(fp_q2, "Trips B -> A\n");


    fp_q3 = fopen("query3.csv", "w"); 
    fprintf(fp_q3, "J;");
    fprintf(fp_q3, "F;");
    fprintf(fp_q3, "M;");
    fprintf(fp_q3, "A;");
    fprintf(fp_q3, "M;");
    fprintf(fp_q3, "J;");
    fprintf(fp_q3, "J;");
    fprintf(fp_q3, "A;");
    fprintf(fp_q3, "S;");
    fprintf(fp_q3, "O;");
    fprintf(fp_q3, "N;");
    fprintf(fp_q3, "D\n");


    fp_q4 = fopen("query4.csv", "w");
    fprintf(fp_q4, "Station;");
    fprintf(fp_q4, "RoundingTrips\n");



    //creo strings para pasar de int  (capaz podemos hacer uno solo largo)


     char pasajeString[15];
     char pasajeString2[15];

     char pasajeJ[10];  
     char pasajeF[10];
     char pasajeM[10];
     char pasajeA[10];
     char pasajeMy[10];  
     char pasajeJu[10];
     char pasajeJl[10];
     char pasajeAg[10];
     char pasajeS[10];  
     char pasajeO[10];
     char pasajeN[10];
     char pasajeD[10];


     // AGREGAMOS LA INFO A LOS QUERYS (PODRIAMOS HACER UN SOLO CICLO Y QUE LOS 1 3 4 SE CORTEN CUANDO I >= CANTSTATIONS CON UN IF)


    for (int i = 0; i < (cantStations * cantStations) - cantStations; i++)  // la longitud de este ciclo esta mal
    { 

        // QUERY 2 CSV

        sprintf(pasajeString, "%d",vec2[i].trips_end_start );
        sprintf(pasajeString2, "%d",vec2[i].trips_start_end );

        fprintf(fp_q2, "%s;", vec2[i].start_station);
        fprintf(fp_q2, "%s;", vec2[i].end_station);
        fprintf(fp_q2, "%d;", vec2[i].trips_start_end );
        fprintf(fp_q2, "%d\n", vec2[i].trips_end_start);

        // QUERY 2 HTML

        addHTMLRow(table2, vec2[i].start_station, vec2[i].end_station, pasajeString2, pasajeString);
    }

 
    for (int i = 0; i < cantStations; i++)
    { 
        // QUERY 1 CSV Y HTML

        fprintf(fp_q1, "%s;", vec1[i].station_name);
        fprintf(fp_q1, "%ld\n", vec1[i].trips);

        sprintf(pasajeString2, "%ld",vec1[i].trips );
        addHTMLRow(table1, vec1[i].station_name, pasajeString2);

        // QUERY 3 CSV Y HTML

         for (int j = 0; j < 12; j++)
        {
            fprintf(fp_q3, "%d;", vec3[i].months[j]);
        }
            fprintf(fp_q3, "%s\n", vec3[i].station_name);

            sprintf(pasajeJ, "%ld",vec1[i].trips );  // podriamos hacer que el q1 directamente haga sprintf al struct y no hacerlos en el front
            sprintf(pasajeF, "%ld",vec1[i].trips );
            sprintf(pasajeM, "%ld",vec1[i].trips );
            sprintf(pasajeA, "%ld",vec1[i].trips );
            sprintf(pasajeMy, "%ld",vec1[i].trips );
            sprintf(pasajeJu, "%ld",vec1[i].trips );
            sprintf(pasajeJl, "%ld",vec1[i].trips );
            sprintf(pasajeAg, "%ld",vec1[i].trips );
            sprintf(pasajeS, "%ld",vec1[i].trips );
            sprintf(pasajeO, "%ld",vec1[i].trips );
            sprintf(pasajeN, "%ld",vec1[i].trips );
            sprintf(pasajeD, "%ld",vec1[i].trips );

        addHTMLRow(table3, pasajeJ, pasajeF, pasajeM, pasajeA, pasajeMy, pasajeJu, pasajeJl, pasajeAg, pasajeS, pasajeO, pasajeN, pasajeD, vec3[i].station_name);

        //addHTMLRow(table3, vec3[i].months[0], vec3[i].months[1], vec3[i].months[2], vec3[i].months[3], vec3[i].months[4], vec3[i].months[5], vec3[i].months[6], vec3[i].months[7], vec3[i].months[8], vec3[i].months[9], vec3[i].months[10], vec3[i].months[11], vec3[i].station_name);


        // QUERY 4 CSV Y HTML

        fprintf(fp_q4, "%s;", vec4[i].station_name);
        fprintf(fp_q4, "%ld\n", vec4[i].trips);

        sprintf(pasajeString, "%ld",vec4[i].trips );
        addHTMLRow(table4, vec4[i].station_name, pasajeString);

    }

    closeHTMLTable(table1);
    closeHTMLTable(table2);
    closeHTMLTable(table3);
    closeHTMLTable(table4);

    fclose(fp_q1);
    fclose(fp_q2);
    fclose(fp_q3);
    fclose(fp_q4);
    fclose(fp_stations);
    fclose(fp_trips);


   
    freeBikeSharing(bikeSharing, vec1, vec4, vec2, vec3);
    free(token);
}
