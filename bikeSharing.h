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

/*Almacena una nueva estacion en orden alfabetico el TAD*/
void addStation(bikeSharingADT bikeSharing, char * station, int id);

/*Almacena un nuevo viaje*/
void addTrip(bikeSharingADT bikeSharing, char isMember, size_t startId, size_t endId, int year, int month);

/*Se setea que vamos a empezar a recorrer desde el primer nodo*/
void toBegin(bikeSharingADT bikeSharing);

/*retorna true si hay algo pendiente por recorrer*/
size_t hasNext(const bikeSharingADT bikeSharing);

/*apunta a la siguiente posición de la lista, solo si hasNext devolvio ture*/
void next(bikeSharingADT bikeSharing); 


#endif