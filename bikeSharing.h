#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#ifndef __bikeSharing_h
#define __bikeSharing_h

typedef struct bikeSharingCDT * bikeSharingADT;


typedef struct idArray
{

    size_t id;
    char * station_name;
    size_t member_trips;
    int months[12];

}idArray;


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

/*crea un vector que relaciona el id, el nombre, los viajes de los miembros, y un vector de 12 posiciones que contenga la cant de viajes x mes de las estaciones con una posicion de la matriz (chequear toArray)
 hacer la matriz al mismo tiempo 
 */

void makeIdArray(idArray * vec, TList list, size_t size);

/*recorre el vector y cuando matchea el id de la estacion devuelve la posicion en la matriz*/
int getIndex(int id, int * vec, size_t dim);

/*ordena de manera descendientemente por cantidad de viajes*/
void sort(size_t dim, int * vec);



#endif