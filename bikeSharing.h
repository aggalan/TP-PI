#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#ifndef __bikeSharing_h
#define __bikeSharing_h

typedef struct bikeSharingCDT * bikeSharingADT;


/*libera toda memoria reservada por el TAD*/
void freeBikeSharing(bikeSharingADT bikeSharing);

/*Retorna un nuevo bikesharing. Al principio esta vacio*/
bikeSharingADT newBikeSharing();

/*Almacena una nueva estacion en el TAD*/
void addStation(bikeSharingADT bikeSharing, char * station, int id);

/*Almacena un nuevo viaje*/
void addTrip(bikeSharingADT bikeSharing, char isMember, size_t startId, size_t endId, int year, int month);











#endif