#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#ifndef __bikeSharing_h
#define __bikeSharing_h

typedef struct bikeSharingCDT * bikeSharingADT;



//struct * makeIdArray()  crea un vector que relaciona el id, el nombre, los viajes de los miembros, y un vector de 12 posiciones que contenga la cant de viajes x mes de las estaciones con una posicion de la matriz (chequear toArray)

// hacer la matriz al mismo tiempo 

// int getIndex()  recorre el vector y cuando matchea el id de la estacion devuelve la posicion en la matriz

// addtrip deberia cargar el viaje de destino y origen a la matriz

// q1() crea un vector nuevo igual al Idarray y lo ordena descendentemente (sin id)

// q2() va a la matriz y obtiene los valores ij y ji. ignora i = j. llama a getdata y va a los indices solicitados.

// q3() recorre el IdArray, va al vector de los viajes de los meses y saca la info ( ya esta ordenado alfabeticamente).

// q4() va a la matriz, agarra la diagonal, lo pasa a un vector, y llama a la funcion de orden para ordenar ese vector. (descendente)

// order() ...


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