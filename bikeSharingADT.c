#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>
#include <errno.h>
#include "bikeSharing.h"

#define MONTHS 12 // fijarse

typedef struct node
{
    int id;
    int months[MONTHS];
    char *station_name;
    size_t member_trips;
    size_t circular_trips;
    struct node *tail;

} TNode;

typedef TNode *TList;

typedef struct bikeSharingCDT
{
    TList first; // puntero a la primera estacion
    TList sIter;
    TList eIter;
    size_t cant; // cantidad de estaciones
    int **matrix;
    char matrix_exists;

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

void freeBikeSharing(bikeSharingADT bs, q1_struct *vec1, q1_struct *vec2, q2_struct *vec3, q3_struct *vec4) // hacer
{
    TList curr = bs->first, aux;

    while (curr != NULL)
    {
        aux = curr->tail;
        free(curr->station_name);
        free(curr);
        curr = aux;
    }

    if (bs->matrix_exists)
    {
        for (int i = 0; i < bs->cant; i++)
        {
            free(bs->matrix[i]);
        }
    }

    free(bs->matrix);

    for (int i = 0; i < bs->cant; i++)
    { // ver hasta donde se hace el ciclo
        free(vec2[i].station_name);
        free(vec1[i].station_name);
        free(vec4[i].station_name);
    }
    for (int i = 0; i < (bs->cant * bs->cant) - bs->cant; i++)
    { // ver hasta donde se hace el ciclos
        free(vec3[i].start_station);
        free(vec3[i].end_station);
    }

    free(vec1);
    free(vec2);
    free(vec3);
    free(vec4);

    free(bs);
}

static TList addStationRec(TList list, char *station_name, int id, int *flag)
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

        new->station_name = malloc(strlen(station_name) + 1);

        strcpy(new->station_name, station_name); // chequear si se puede hacer esto

        new->member_trips = 0; // inicializo los viajes de miembros en 0

        new->circular_trips = 0; // inicializo los viajes circulares

        for (int i = 0; i < MONTHS; i++)
        {
            new->months[i] = 0; // inicializo los viajes del mes en 0
        }

        new->tail = list;

        (*flag)++;
        return new;
    }

    if (c > 0)
    {
        list->tail = addStationRec(list->tail, station_name, id, flag);
        return list;
    }

    return list; // Ya existia esa estacion en la lista
}

void addStation(bikeSharingADT bikeSharing, char *station_name, int id)
{
    int flag = 0;
    bikeSharing->first = addStationRec(bikeSharing->first, station_name, id, &flag);
    bikeSharing->cant += flag; // fijarse si no esta agregando de mas
}

// validar los allocs

void setMatrix(bikeSharingADT bs, int *cant)
{
    bs->matrix_exists = 1; // Avisamos al free que debe liberar la matriz tambien
    // una vez que cargue todas las estaciones ya puedo reservar memoria para la matriz y asignarle un index a cada nodo
    int i;
    errno = 0;

    bs->matrix = malloc(bs->cant * sizeof(int *)); // reservo memoria filas

    if (errno == ENOMEM)
    {
        return;
    }

    for (i = 0; i < bs->cant; i++) // ver si hay que ir hasta cant o cant-1
    {
        bs->matrix[i] = calloc(bs->cant, sizeof(int)); // reservo fila columnas

        if (errno == ENOMEM)
        {
            return;
        }
    }

    *cant = bs->cant;
}

/* Retorna el nodo de la estacion de salida. Deja en start_index y end_index los indices, o no los toca si los id's no estaban. flag debe ser = 0 al pasarlo a la funcion!*/
static TList getIndex(TList first, int start_id, int end_id, int *start_index, int *end_index, int *flag)
{

    TList aux = first;
    TList ans = NULL;

    int index = 0;

    while (aux != NULL && *flag < 2)
    {
        if (aux->id == start_id)
        {
            *start_index = index;
            ans = aux;
            (*flag)++;
        }

        if (aux->id == end_id)
        {
            *end_index = index;
            (*flag)++;
        }

        index++;

        aux = aux->tail;
    }

    return ans;
}

// nose si va a hacer falta sort

void addTrip(bikeSharingADT bikeSharing, int isMember, int startId, int endId, int year, int month, int sYear, int eYear)
{
    int idxStart, idxEnd, flag = 0;
    TList sAux;

    // Chequeamos que ambos IDs esten en la lista. Si alguno no está retornamos (no queremos agregarlo)
    sAux = getIndex(bikeSharing->first, startId, endId, &idxStart, &idxEnd, &flag);

    if (flag < 2)
    {
        return;
    }

    // Si es miembro agregamos el viaje a la estacion
    if (isMember)
    {
        sAux->member_trips++;
    }

    // Agregamos viaje a la estacion por mes
    sAux->months[month - 1]++;

    // Agregamos los viajes circulares
    if (startId == endId && (year >= sYear && year <= eYear))
    {
        sAux->circular_trips++;
    }

    // Si el lugar de comienzo y fin son distintos, lo agregamos directo a la matriz. Si el viaje es circular, si el año esté dentro de los parámetroslo agregamos a la matriz
    else
    {
        bikeSharing->matrix[idxStart][idxEnd]++;
    }
}

// para los iteradores poner 1 si es de salida o 0 sino asi modifica ese iterador

static void toBegin(bikeSharingADT bikeSharing, char start)
{

    if (start)
        bikeSharing->sIter = bikeSharing->first;

    bikeSharing->eIter = bikeSharing->first;
}

static int hasNext(const bikeSharingADT bikeSharing, char start)
{

    if (start)
        return bikeSharing->sIter != NULL;

    return bikeSharing->eIter != NULL;
}

static TList next(bikeSharingADT bikeSharing, char start)
{

    if (!hasNext(bikeSharing, start))
    {
        return NULL;
    }

    if (start)
    {
        TList aux = bikeSharing->sIter;
        bikeSharing->sIter = bikeSharing->sIter->tail;
        return aux;
    }

    TList aux = bikeSharing->eIter;
    bikeSharing->eIter = bikeSharing->eIter->tail;
    return aux;
}

static int q1_cmp(const void *e1, const void *e2)
{
    const q1_struct *ptr1 = (const q1_struct *)e1;
    const q1_struct *ptr2 = (const q1_struct *)e2;
    return (ptr2->trips - ptr1->trips);
}

q1_struct *q1(bikeSharingADT bikeSharing, int query)
{
    TList aux = bikeSharing->first;
    errno = 0;

    q1_struct *vec1 = malloc(bikeSharing->cant * sizeof(q1_struct));

    if (errno == ENOMEM)
    {
        return NULL; // preguntar
    }

    for (int i = 0; i < bikeSharing->cant; i++) // capaz podemos hacer un ciclo while hasta aux == null porque aca se pasa por alguna razon
    {
        switch (query)
        {
        case 1:
            vec1[i].trips = aux->member_trips; // seg fault aca
            break;
        case 4:
            vec1[i].trips = aux->circular_trips;
            break;
        }

        int len = strlen(aux->station_name);
        vec1[i].station_name = malloc(len + 1);
        if (errno == ENOMEM)
        {
            return NULL;
        }
        strcpy(vec1[i].station_name, aux->station_name);
        aux = aux->tail;
    }

    qsort(vec1, bikeSharing->cant, sizeof(q1_struct), q1_cmp);

    return vec1;
}

q2_struct *q2(bikeSharingADT bikeSharing)
{
    errno = 0;

    q2_struct *vec2 = malloc(((bikeSharing->cant * bikeSharing->cant) - (bikeSharing->cant)) * sizeof(q2_struct)); // puede llegar a reservar de mas
    if (errno == ENOMEM)
    {
        return NULL;
    }

    int k = 0;

    TList sAux, eAux;
    toBegin(bikeSharing, 1);

    for (int i = 0; i < bikeSharing->cant ; i++) // revisar este ciclo, aux == null -1 PORQUE NO INCLUYE DIAGONAL
    {
        toBegin(bikeSharing, 0);
        sAux = next(bikeSharing, 1);

        for (int j = 0; j < bikeSharing->cant ; j++) // revisar este ciclo, aux == null  -1 PORQUE NO INCLUYE DIAGONAL
        {
            eAux = next(bikeSharing, 0);
            if (i == j)
            {
                continue;
            }

            vec2[k].start_station = malloc(strlen(sAux->station_name) + 1);

            if (errno == ENOMEM)
            {
                return NULL;
            }

            vec2[k].end_station = malloc(strlen(eAux->station_name) + 1);

            if (errno == ENOMEM)
            {
                return NULL;
            }

            strcpy(vec2[k].start_station, sAux->station_name);
            strcpy(vec2[k].end_station, eAux->station_name);
        

            vec2[k].trips_start_end = bikeSharing->matrix[i][j];
            vec2[k].trips_end_start = bikeSharing->matrix[j][i];

            k++;
        }
    }
    return vec2;
}

q3_struct *q3(bikeSharingADT bikeSharing)
{
    TList aux = bikeSharing->first;
    errno = 0;
    q3_struct *vec3 = malloc(bikeSharing->cant * sizeof(q3_struct));
    if (errno == ENOMEM)
    {
        return NULL; // preguntar
    }

    for (int i = 0; i < bikeSharing->cant; i++) // POSIBLE -1
    {
        vec3[i].station_name = malloc(strlen(aux->station_name) + 1);
        if (errno == ENOMEM)
        {
            return NULL; // preguntar
        }
        strcpy(vec3[i].station_name, aux->station_name);
        for (int j = 0; j < MONTHS; j++)
        {
            vec3[i].months[j] = aux->months[j];
        }
        aux = aux->tail;
    }

    return vec3;
}
