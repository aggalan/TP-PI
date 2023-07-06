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
    size_t trips_start_end;
    size_t trips_end_start;
} q2_struct;

typedef struct q3_struct {
    char * station_name;
    size_t months[MONTHS];
} q3_struct;

/*libera toda memoria reservada por el TAD*/
void freeBikeSharing(bikeSharingADT bikeSharing);

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
void addTrip(bikeSharingADT bikeSharing, char isMember, size_t startId, size_t endId, int year, int month, int sYear, int eYear);


void toBegin(bikeSharingADT bikeSharing, char start);


int hasNext(const bikeSharingADT bikeSharing, char start);


TList next(bikeSharingADT bikeSharing, char start);


//chequear que devuelven las funciones de query(tiene sentido que devuelva un nuevo vector para podedr imprimir lindo)

/* 
   Crea un vector nuevo igual al Idarray y lo ordena descendentemente (sin id).
   Retorna un vector de structs del estilo: 
   struct q1_struct {
    char * station_name;
    size_t trips;
   }
*/
q1_struct * q1(bikeSharingADT bikeSharing); 

/* 
    Va a la matriz y obtiene los valores ij y ji. ignora i = j. llama a getdata y va a los indices solicitados. 
    Retorna un struct del estilo q2_struct.
*/
q2_struct * q2(); 
 
 /* 
    Recorre el IdArray, va al vector de los viajes de los meses y saca la info ( ya esta ordenado alfabeticamente).
    Retorna un vector de structs del estilo q3_struct;
 */
q3_struct * q3();

/*
    Va a la matriz, agarra la diagonal, lo pasa a un vector, y llama a la funcion de orden para ordenar ese vector. (descendente) 
    Retorna un vector de struct del estilo q1_struct.    
*/
q1_struct * q4();





#endif