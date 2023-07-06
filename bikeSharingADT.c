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

typedef struct node
{
    size_t id;
    int index;
    int months[MONTHS];
    char station_name[MAX_LETTERS];
    size_t member_trips;
    struct node *tail;

} TNode;

typedef TNode * TList;

typedef struct bikeSharingCDT
{
    TList first; // puntero a la primera estacion
    size_t cant; // cantidad de estaciones
    int ** matrix;

} bikeSharingCDT;

bikeSharingADT newBikeSharing()
{
    errno = 0;

    bikeSharingADT new = calloc(1, sizeof(bikeSharingCDT));

    if (errno == ENOMEM)
    {
        return NULL;
    }

    return new;
}

void freeBikeSharing(bikeSharingADT bs)
{
    TList curr = bs->first, aux;

    while (curr != NULL)
    {
        aux = curr->tail;
        free(curr);
        curr = aux;
    }

    /*

    for(int i = 0; i < bs->cant; i++)
    {
        free(bs->matrix[i]);
    }

    */

    free(bs->matrix);
    free(bs);
}

static TList addStationRec(TList list, char *station_name, int id)
{
    int c;
    if (list == NULL || (c = strcmp(station_name, list->station_name)) < 0)
    {
        errno = 0;
        TList new = malloc(sizeof(TNode));

        if (errno == ENOMEM)
        {
            return NULL;
        }

        new->id = id;

        strcpy(new->station_name, station_name); // chequear si se puede hacer esto

        new->member_trips = 0; // inicializo los viajes de miembros en 0

        for (int i = 0; i < MONTHS; i++)
        {
            new->months[i] = 0; // inicializo los viajes del mes en 0
        }

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

void setMatrix(bikeSharingADT bs)
{
    // una vez que cargue todas las estaciones ya puedo reservar memoria para la matriz y asignarle un index a cada nodo

    int i;
    errno = 0;

    bs->matrix = malloc(bs->cant * sizeof(int *)); // reservo memoria filas

    if (errno == ENOMEM)
    {
        return NULL;
    }

    TList aux = bs->first;

    for (i = 0; i < bs->cant; i++) // como ya esta ordenado alfabeticamente directamente pongo el index
    {
        bs->matrix[i] = calloc(bs->cant, sizeof(int)); // reservo fila columnas

        if (errno == ENOMEM)
        {
            return NULL;
        }

        aux->index = i;

        aux = aux->tail;
    }
}

TList getIndex(int id, TList first, int *index) // devuelve el nodo de la estacion solicitada y el index en un parametro de salida
{

    TList aux = first;

    while (aux != NULL)
    {
        if (id == aux->id)

            *index = aux->index;

        return aux; // si matchea el id devuelvo la direccion del nodo y el index en la matriz

        aux = aux->tail;
    }

    return NULL;
}

// nose si va a hacer falta sort

void addTrip(bikeSharingADT bikeSharing, char isMember, size_t startId, size_t endId, int year, int month, int sYear, int eYear)
{
    int idxStart, idxEnd;
    TList sAux, eAux;

    // Chequeamos que ambos IDs esten en la lista. Si alguno no está retornamos (no queremos agregarlo)
    if ((sAux = getIndex(startId, bikeSharing->first, &idxStart)) == NULL || (eAux = getIndex(endId, bikeSharing->first, &eAux)) == NULL)
    {
        return;
    }

    // Si es miembro agregamos el viaje a la estacion
    if (isMember)
    {
        sAux->member_trips++;
    }

    // Agregamos viaje a la estacion por mes
    sAux->months[month]++;

    // Si el lugar de comienzo y fin son distintos, lo agregamos directo a la matriz. Si el viaje es circular, si el año esté dentro de los parámetroslo agregamos a la matriz
    if (startId != endId || (startId == endId && year >= sYear && year <= eYear))
    {
        bikeSharing->matrix[idxStart][idxEnd]++;
    }
}


static int q1_cmp (q1_struct e1, q1_struct e2) {
    return e1.trips - e2.trips;
}

struct q1_struct *q1(bikeSharingADT bikeSharing)  //falta actualizar esto
{
    TList aux = bikeSharing->first;
    errno = 0;
    struct q1_struct *vec1 = malloc(bikeSharing->cant * sizeof(struct q1_struct));
    if (errno == ENOMEM)
    {
        return NULL; // preguntar
    }
    for (int i = 0; i < bikeSharing->cant; i++) //copio todos los station name y los member trips al vector
    {
        vec1[i].trips = aux->member_trips;
        strcpy(vec1[i].station_name, aux->station_name);
        aux=aux->tail;
    }

    qsort(vec1, bikeSharing->cant, sizeof(q1_struct), q1_cmp);

    // sort(bikeSharing->cant, vec1); //ordeno por cantidad de viajes (de mayor a menor)
    // We use qsort, which is better for our purpouses

    return vec1;
}



