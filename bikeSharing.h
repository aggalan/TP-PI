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
    int trips_start_end;
    int trips_end_start;
} q2_struct;

typedef struct q3_struct {
    char * station_name;
    int months[MONTHS]; 
} q3_struct;

/*Frees up the resources used by the ADT*/
void freeBikeSharing(bikeSharingADT bs);

// Frees up the resources used by a q1_struct style array
void freeVec1(bikeSharingADT bs, q1_struct * vec1);

// Frees up the resources used by a q2_struct style array
void freeVec2(bikeSharingADT bs, q2_struct * vec2, int dim);

// Frees up the resources used by a q3_struct style array
void freeVec3(bikeSharingADT bs, q3_struct * vec3);

/*Returns a new bikeSharing, which is empty at first*/
bikeSharingADT newBikeSharing(int start_year, int end_year);

/*Stores a new station in alphabetical order in the TAD. Returns a boolean character to check for memory allocation errors.*/
int addStation(bikeSharingADT bikeSharing, char * station, int id);

/*
Reserves space for the matrix and sets the indexes.
It must be called before starting to load the trips.
Returns a boolean character to check for memory allocation errors.
*/
int setMatrix(bikeSharingADT bs, int * cant);

/*Stores a new trip. Addtrip should load the source trip and the destination trip to the matrix*/ 
void addTrip(bikeSharingADT bikeSharing, int isMember, int startId, int endId, int year, int month);




/* 
Creates a new array that is the same as Idarray and sorts it in descending order (without id).
Returns a vector of structs of the style of q1_struct:
*/
q1_struct * q1(bikeSharingADT bikeSharing, int query); 

/* 
Acceses the matrix and gets the ij and ji values. Ignores i = j. Calls getdata and goes to the requested indexes.
Returns a struct of the style q2_struct.
*/
q2_struct * q2(bikeSharingADT bikeSharing, int * dim); 
 
 /* 
Gets the information out of the monthly travel array, by first looking up the id in the idArray
Returns and array of structs of the style q3_struct.
 */
q3_struct * q3(bikeSharingADT bikeSharing);





#endif
