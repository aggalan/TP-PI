#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#define MONTHS 12
#ifndef __bikeSharing_h
#define __bikeSharing_h

typedef struct bikeSharingCDT * bikeSharingADT;

typedef struct q1_struct {
    char * station_name;
    size_t trips;
} q1_struct;

typedef struct q2_struct {
    char * start_station;
    char * end_station;
    int trips_start_end; // es una matriz de ints, recibe ints
    int trips_end_start;
} q2_struct;

typedef struct q3_struct {
    char * station_name;
    int months[MONTHS];  // esta en ints en el back
} q3_struct;

/*libera toda memoria reservada por el TAD*/
void freeBikeSharing(bikeSharingADT bs, q1_struct *vec1, q1_struct *vec2, q2_struct *vec3, q3_struct *vec4);

/*Retorna un nuevo bikesharing. Al principio esta vacio*/
bikeSharingADT newBikeSharing();

/*Almacena una nueva estacion en orden alfabetico el TAD*/
void addStation(bikeSharingADT bikeSharing, char * station, int id);

/*
Reserva espacio para la matriz y pone los indices.
Se debe llamar antes de empezar a cargar los viajes.
*/
void setMatrix(bikeSharingADT bs);

/*Almacena un nuevo viaje. Addtrip deberia cargar el viaje de destino y origen a la matriz*/ 
void addTrip(bikeSharingADT bikeSharing, int isMember, int startId, int endId, int year, int month, int sYear, int eYear);


//chequear que devuelven las funciones de query(tiene sentido que devuelva un nuevo vector para podedr imprimir lindo)

/* 
   Crea un vector nuevo igual al Idarray y lo ordena descendentemente (sin id).
   Retorna un vector de structs del estilo: 
   struct q1_struct {
    char * station_name;
    size_t trips;
   }
*/
q1_struct * q1(bikeSharingADT bikeSharing, int query); 

/* 
    Va a la matriz y obtiene los valores ij y ji. ignora i = j. llama a getdata y va a los indices solicitados. 
    Retorna un struct del estilo q2_struct.
*/
q2_struct * q2(bikeSharingADT bikeSharing); 
 
 /* 
    Recorre el IdArray, va al vector de los viajes de los meses y saca la info ( ya esta ordenado alfabeticamente).
    Retorna un vector de structs del estilo q3_struct;
 */
q3_struct * q3(bikeSharingADT bikeSharing);





#endif