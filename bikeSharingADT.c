#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>
#include <errno.h>
#include "bikeSharing.h"

#define MAX_LETTERS 60
#define MONTHS 12 // fijarse

static enum months { JAN = 0,
                     FEB,
                     MAR,
                     APR,
                     JUN,
                     JUL,
                     AUG,
                     SEP,
                     OCT,
                     NOV,
                     DEC };

/*

typedef struct trips {
    char end_station_name[MAX_LETTERS];
    size_t trips;
} Ttrip;

*/

typedef struct idArray
{

    size_t id;
    char * station_name;
    size_t member_trips;
    int months[12];

}idArray;

typedef struct node
{

    size_t id;
    char station_name[MAX_LETTERS];
    struct node * tail;

} TNode;

typedef TNode * TList;

typedef struct bikeSharingCDT
{
    TList first; // puntero a la primera estacion
    size_t cant; // cantidad de estaciones
    idArray * vec;
    int ** matrix;

} bikeSharingCDT;

bikeSharingADT newBikeSharing()
{
    errno = 0; // creo que se inicializa en 0

    bikeSharingADT new = calloc(1, sizeof(bikeSharingCDT));

    if (errno == ENOMEM)
    {
        perror("Insufficient memory");
        exit(1);
    }

    return new;
}

static TList addStationRec(TList list, char *station_name, int id)
{
    int c;
    if (list == NULL || (c = strcmp(station_name, list->station_name)) < 0)
    {
        errno = 0; // errno ya empieza en 0?
        TList new = malloc(sizeof(TNode));

        if (errno == ENOMEM)
        {
            perror("Insufficient memory");
            exit(1); // no pude exitiar creo
        }

        new->id = id;
        strcpy(new->station_name, station_name); // chequear si se puede hacer esto
        new->tail = list;
        return new;
    }

    if (c > 0)
    {
        list->tail = addStationRec(list->tail, station_name, id);
        return list;
    }

    return list; // Ya existia esa estacion en la lista
}

void addStation(bikeSharingADT bikeSharing, char *station_name, int id)
{
    bikeSharing->first = addStationRec(bikeSharing->first, station_name, id);
    bikeSharing->cant++;
}


// validar los allocs

void prepare_data_for_trips(bikeSharingADT bs)
{
    int i;
    bs->vec = malloc(bs->cant * sizeof(idArray));
    bs->matrix = malloc(bs->cant * sizeof(int *));

    TList aux = bs->first;

    for(i = 0; i < bs->cant; i++)
    {
        bs->matrix[i]= calloc(bs->cant, sizeof(int));

        bs->vec[i].id = aux->id;
        strcpy(bs->vec[i].station_name, aux->station_name);
        bs->vec[i].member_trips = 0;

        for(int j = 0; j < 12; j++)
        bs->vec[i].months[j] = 0;

        aux = aux -> tail;
    }

}



int getIndex(int id, int * vec, size_t dim)
{

    for(int i=0; i < dim; i++)
    {
    
    if(id == vec[i])
        return i;
    }

return -1;

}

void sort(size_t dim,  int * vec)
{

    for (int i = 0; i < dim - 1; i++) 
    {
        for (int j = 0; j < dim - i - 1; j++) 
        {
            if (vec[j] < vec[j + 1]) 
            {
                // Intercambiar los elementos
                int aux = vec[j];
                vec[j] = vec[j + 1];
                vec[j + 1] = aux;
            }
        }
    }
}


void addTrip(bikeSharingADT bikeSharing, char isMember, size_t startId, size_t endId, int year, int month, int sYear, int eYear) 
{
    int idxStart, idxEnd;

    //Chequeamos que ambos IDs esten en la lista. Si alguno no está retornamos (no queremos agregarlo)
    if ((idxStart = getIndex(startId, bikeSharing->vec, bikeSharing->cant)) == -1 || (idxEnd = getIndex(end_id, bikeSharing->vec, bikeSharing->cant)) == -1) {
        return;
    }

    //Si es miembro agregamos el viaje al vector
    if (isMember) {
        bikeSharing->vec[idxStart].member_trips++;
    }

    //Agregamos viaje al vector por mes
    bikeSharing->vec[idxStart].months[month]++;

    //Si el lugar de comienzo y fin son distintos, lo agregamos directo a la matriz. Si el viaje es circular, si el año esté dentro de los parámetroslo agregamos a la matriz
    if (startId != endId || (startId == endId && year >= sYear && year <= eYear)) {
       bikeSharing->matrix[idxStart][idxEnd]++;
    }
    
}



/*crea un vector que relaciona el id, el nombre, los viajes de los miembros, y un vector de 12 posiciones que contenga la cant de viajes x mes de las estaciones con una posicion de la matriz (chequear toArray)
 hacer la matriz al mismo tiempo 
 */

/*recorre el vector y cuando matchea el id de la estacion devuelve la posicion en la matriz*/
int getIndex(int id, int * vec, size_t dim);


