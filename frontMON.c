#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "bikeSharing.h"
#include "htmlTable.h"  
#define MAX_WORD_LENGTH 30
#define MAX_LINE_LENGTH 100


int main(int argc, char * argv[])
{
    FILE * fp_stations, * fp_trips, * fp_q1, * fp_q2, * fp_q3, * fp_q4;
    char str[MAX_LINE_LENGTH];               // max strg length
    const char s[2] = ";";    // sets the break parameter
    size_t cantStations = 0; // counter for the number of stations
    errno = 0;

    //Checks for errors int he amount of parameters


    if (argc > 5 || argc < 3) 
    {
        perror("Error in the amount of parameters");
        return 1;
    }
    
    //creates variables for stations id and name, coordinates not needed for our querys.

    bikeSharingADT bikeSharing = newBikeSharing();

    char * sName = malloc(MAX_LINE_LENGTH);
    
    if (errno == ENOMEM)
    {
         perror("Not enough memory");

        return 1;
    }

    size_t sId;

    char *token = malloc(MAX_LINE_LENGTH);

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
        while( token != NULL && i < 2 )
        {
            if(i == 0)
            {
                sId = atoi(token);
            }
            if(i == 1)
            {
                strcpy(sName, token);
            }
            token = strtok(NULL, s); 
            i++;
        }
        cantStations++;
        addStation(bikeSharing, sName, sId);
    }
    
    // When finished loading the stations, we load the trips (reutilizing str)
    // start_date;emplacement_pk_start;end_date;emplacement_pk_end;is_member

    char * date_delim = "-";
    char * date_token;
    int start_year, start_month;
    size_t start_id, end_id;
    char is_member;

    int limit_start_year, limit_end_year; 

    if (argc < 4) {
        limit_start_year = -1;
        limit_end_year = 3000;
    }
    else if (argc == 4) {
        limit_start_year = atoi(argv[3]);
        limit_end_year = 3000;
    }
    else if (argc == 5) {
        limit_start_year = atoi(argv[3]);
        limit_end_year = atoi(argv[4]);
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

    q1_struct * vec1 = q1( bikeSharing, 1);


    q2_struct * vec2 = q2( bikeSharing );


    q3_struct * vec3 = q3( bikeSharing );


    q1_struct * vec4 = q1( bikeSharing, 4);



    // QUERY 1 HTML CREATION

    htmlTable table1 = newTable( "query1.html",2,"Station", "StartedTrips" );


    // QUERY 2 HTML

    htmlTable table2 = newTable( "query2.html",4,"Station A","Station B","Trips A -> B","Trips B -> A" );
    

    // QUERY 3 HTML CREATION

    htmlTable table3 = newTable( "query3.html",13,"J","F","M","A","M","J","J","A","S","O","N","D","Station" );


    // QUERY 1, 3, 4 HTML

    htmlTable table4 = newTable( "query4.html",2,"Station", "RoundingTrips" );

    

    //QUERY 1 CSV TITLES

    fp_q1 = fopen("query1.csv", "w");
    fprintf(fp_q1, "Station;");
    fprintf(fp_q1, "StartedTrips\n");
    

    //QUERY 2 CSV Y HTML

    fp_q2 = fopen("query2.csv", "w");
    fprintf(fp_q2, "StationA;");
    fprintf(fp_q2, "StationB;");
    fprintf(fp_q2, "Trips A -> B;");
    fprintf(fp_q2, "Trips B -> A\n");
    for ( int i = 0; i < (cantStations*cantStations) - cantStations; i++ ) { // VER HASTA DONDE VA I
        
        //QUERY 2 CSV
        
        fprintf(fp_q2, "%s;", vec2[i].start_station);
        fprintf(fp_q2,"%ld;" ,vec1[i].trips);
        fprintf(fp_q2, "%ld;", vec2[i].trips_start_end);
        fprintf(fp_q2, "%ld\n", vec2[i].trips_end_start);

        //QUERY 2 HTML

        addHTMLRow( table2, vec2[i].start_station, vec2[i].end_station, vec2[i].trips_start_end, vec2[i].trips_end_start ); 
    }

    //QUERY 3 CSV TITLES

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
    

    //QUERY 1, 3, 4 CSV AND HTML INFO

    fp_q4 = fopen("query4.csv", "w");
    fprintf(fp_q4, "Station;");
    fprintf(fp_q4, "RoundingTrips\n");
    for ( int i = 0; i < cantStations; i++ ) { // VER HASTA DONDE VA I

        //QUERY 4

        fprintf( fp_q4, "%s;", vec4[i].station_name );
        fprintf( fp_q4, "%ld\n", vec4[i].trips ); 

        //QUERY 3

        for ( int j = 0; j<12 ;j++)  {
            fprintf(fp_q3, "%ld;",vec3[i].months[j]);
        }
        fprintf(fp_q3, "%s\n",vec3[i].station_name);

        //QUERY 1

        fprintf(fp_q1, "%s;", vec1[i].station_name);
        fprintf(fp_q1, "%ld\n", vec1[i].trips);


        //QUERY 4 
        
        addHTMLRow( table4, vec4[i].station_name, vec4[i].trips );    

        //QUERY 3

        addHTMLRow( table3,vec3[i].months[0] ,vec3[i].months[1] ,vec3[i].months[2] ,vec3[i].months[3] ,vec3[i].months[4] ,vec3[i].months[5] ,vec3[i].months[6] ,vec3[i].months[7] ,vec3[i].months[8] ,vec3[i].months[9] ,vec3[i].months[10] ,vec3[i].months[11] ,vec3[i].station_name );    

        //QUERY 1

        addHTMLRow( table1, vec1[i].station_name, vec1[i].trips ); 


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

    for (int i = 0; i < cantStations ; i++) { //ver hasta donde se hace el ciclo
    free(vec3[i].station_name);
    free(vec1[i].station_name);
    free(vec4[i].station_name);
    }
    for (int i = 0; i < (cantStations*cantStations) - cantStations; i++) { //ver hasta donde se hace el ciclos
    free(vec2[i].start_station);
    free(vec2[i].end_station);
    }

     freeBikeSharing(bikeSharing,vec1,vec4,vec2,vec3); 
}
