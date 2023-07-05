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


idArray * makeIdArray(size_t size)
{
    
}

void fillIdArray(idArray * vec, TList list)
{
    TList aux = list;
    int i = 0;

    while(aux != NULL)
    {
        strcpy(vec[i].station_name, aux->station_name);
        vec[i].id = aux->id;
        aux = aux->tail;
        i++;
    }

}



int getIndex(int id, int * vec, size_t dim){
    for(int i=0; i < dim; i++){
    if(id == vec[i])
        return i;
}
return -1;
}

void sort(size_t dim,  int * vec){
    for (int i = 0; i < dim - 1; i++) {
        for (int j = 0; j < dim - i - 1; j++) {
            if (vec[j] < vec[j + 1]) {
                // Intercambiar los elementos
                int aux = vec[j];
                vec[j] = vec[j + 1];
                vec[j + 1] = aux;
            }
        }
    }
}




