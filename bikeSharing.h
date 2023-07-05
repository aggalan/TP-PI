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

/*Almacena un nuevo viaje. Addtrip deberia cargar el viaje de destino y origen a la matriz*/ 
void addTrip(bikeSharingADT bikeSharing, char isMember, size_t startId, size_t endId, int year, int month);


//chequear que devuelven las funciones de query(tiene sentido que devuelva un nuevo vector para podedr imprimir lindo)

/* crea un vector nuevo igual al Idarray y lo ordena descendentemente (sin id)*/
int * q1(); 

/* va a la matriz y obtiene los valores ij y ji. ignora i = j. llama a getdata y va a los indices solicitados. */
int * q2(); 
 
 /* recorre el IdArray, va al vector de los viajes de los meses y saca la info ( ya esta ordenado alfabeticamente).*/
int * q3();

/*va a la matriz, agarra la diagonal, lo pasa a un vector, y llama a la funcion de orden para ordenar ese vector. (descendente) */
int * q4();

/*asigna la memoria necesaria al vector y la matriz (bikeSharing->cant) con calloc, y haria un recorrido de la lista llenando los campos de id y de station_name*/

void prepare_data_for_trips(bikeSharingADT bs);

/*ordena de manera descendientemente por cantidad de viajes*/
void sort(size_t dim, int * vec);



#endif