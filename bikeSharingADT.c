#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>
#include <errno.h>
#include "bikeSharing.h"

#define MONTHS 12 // fijarse
#define MEMORY_CHECK_NULL(ptr) if (ptr == NULL) {\
                                      return NULL;\
                                  }\

#define MEMORY_CHECK_1(ptr) if (ptr == NULL) {\
                                      return 1;\
                                  }\


typedef struct node
{
    int id;
    int index;
    int months[MONTHS];
    char *station_name;
    size_t member_trips;
    size_t circular_trips;
    struct node *tail;

} TNode;

typedef TNode *TList;

typedef struct idSort   // CAMBIO
{
    int id;
    TList station;
    
}idSort;

typedef struct bikeSharingCDT
{
    TList first; // puntero a la primera estacion
    TList sIter;
    TList eIter;
    size_t cant; // cantidad de estaciones
    int **matrix;
    idSort * arr;
    int limit_start_year;
    int limit_end_year;
    int l;
    int r;

} bikeSharingCDT;

bikeSharingADT newBikeSharing(int start_year, int end_year)
{
    errno = 0;

    bikeSharingADT new = calloc(1, sizeof(bikeSharingCDT));
    MEMORY_CHECK_NULL(new)

    new->limit_start_year = start_year;
    new->limit_end_year = end_year;

    return new;
}

void freeBikeSharing(bikeSharingADT bs) // hacer
{
    TList curr = bs->first, aux;

    while (curr != NULL)
    {
        aux = curr->tail;
        free(curr->station_name);
        free(curr);
        curr = aux;
    }

    if (bs->matrix != NULL) //si se reservo memoria
    {
        for (int i = 0; i < bs->cant; i++)
        {
            free(bs->matrix[i]);
        }
    }

    free(bs->matrix);

    free(bs->arr);

    free(bs);
}

void freeVec1(bikeSharingADT bs, q1_struct * vec1) 
{
    for (int i = 0; i < bs->cant; i++)
    { 
        free(vec1[i].station_name);
    }
    free(vec1);  
}

void freeVec2(bikeSharingADT bs, q2_struct * vec2, int dim)
{
    for (int i = 0; i < dim; i++)
    { 
        free(vec2[i].start_station);
        free(vec2[i].end_station);
    }
    free(vec2);
}

void freeVec3(bikeSharingADT bs, q3_struct * vec3) 
{
    for (int i = 0; i < bs->cant; i++)
    { 
        free(vec3[i].station_name);
    }  
    free(vec3);
}


static TList addStationRec(TList list, char *station_name, int id, int *flag, int * memFlag)
{
    int c;
    if (list == NULL || (c = strcmp(station_name, list->station_name)) < 0)
    {
        errno = 0;
        TList new = malloc(sizeof(TNode));

        if (errno == ENOMEM || new == NULL)
        {
            *memFlag = 1;
            return NULL;
        }

        new->id = id;
        new->station_name = malloc(strlen(station_name) + 1);
        strcpy(new->station_name, station_name); 
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
        list->tail = addStationRec(list->tail, station_name, id, flag, memFlag);
        return list;
    }

    return list; // Ya existia esa estacion en la lista
}

int addStation(bikeSharingADT bikeSharing, char *station_name, int id)
{
    int flag = 0, memFlag = 0;
    bikeSharing->first = addStationRec(bikeSharing->first, station_name, id, &flag, &memFlag);
    bikeSharing->cant += flag; 
    return memFlag;
}


static int idCmp(const void *e1, const void *e2)
{
    const idSort *ptr1 = (const idSort *)e1;
    const idSort *ptr2 = (const idSort *)e2;
    return (ptr1->id - ptr2->id);
}

int setArr(bikeSharingADT bs) // CAMBIO
{
    errno = 0;
    bs->arr = malloc(bs->cant * sizeof(idSort));
    MEMORY_CHECK_1(bs->arr)

    TList aux = bs->first;

    for(int i = 0; i < bs->cant; i++)
    {   
        aux->index = i;
        bs->arr[i].id = aux->id;
        bs->arr[i].station = aux;

        aux = aux->tail;
    }

    qsort(bs->arr, bs->cant, sizeof(idSort), idCmp);

    bs->l=0;
    bs->r=bs->cant-1;

    return 0;

}

int setMatrix(bikeSharingADT bs, int *cant)
{
    int i;
    errno = 0;

    if(setArr(bs))
        return 1;
        
    bs->matrix = malloc(bs->cant * sizeof(int *)); // reservo memoria filas
    MEMORY_CHECK_1(bs->matrix)

    for (i = 0; i < bs->cant; i++) 
    {
        bs->matrix[i] = calloc(bs->cant, sizeof(int)); // reservo fila columnas
        MEMORY_CHECK_1(bs->matrix[i])
    }

    *cant = bs->cant;
    return 0;
}

// An iterative binary search function.
TList binarySearch(idSort * arr, int l, int r, int id)
{
    while (l <= r) {
        
        int m = l + (r - l) / 2;
 
        // Check if x is present at mid
        if (arr[m].id == id)
            return arr[m].station;
 
        // If x greater, ignore left half
        if (arr[m].id < id)
            l = m + 1;
 
        // If x is smaller, ignore right half
        else
            r = m - 1;
    }
 
    // If we reach here, then element was not present
    return NULL;
}
 


void addTrip(bikeSharingADT bikeSharing, int isMember, int startId, int endId, int year, int month)
{
    TList sAux, eAux;

    // Chequeamos que ambos IDs esten en la lista. Si alguno no está retornamos (no queremos agregarlo)
     // sAux = getIndex(bikeSharing->first, startId, endId, &idxStart, &idxEnd, &flag);

    sAux = binarySearch(bikeSharing->arr, bikeSharing->l, bikeSharing->r, startId);

    eAux = binarySearch(bikeSharing->arr, bikeSharing->l, bikeSharing->r, endId);

    if (sAux == NULL || eAux == NULL)
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
    if (startId == endId && (year >= bikeSharing->limit_start_year && year <= bikeSharing->limit_end_year))
    {
        sAux->circular_trips++;
    }

    // Si el lugar de comienzo y fin son distintos, lo agregamos directo a la matriz. Si el viaje es circular, si el año esté dentro de los parámetroslo agregamos a la matriz
    else
    {
        bikeSharing->matrix[sAux->index][eAux->index]++;
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
    MEMORY_CHECK_NULL(vec1)


    for (int i = 0; i < bikeSharing->cant; i++) 
    {
        switch (query)
        {
        case 1:
            vec1[i].trips = aux->member_trips; 
            break;
        case 4:
            vec1[i].trips = aux->circular_trips;
            break;
        }

        vec1[i].station_name = malloc(strlen(aux->station_name) + 1);
        MEMORY_CHECK_NULL(vec1[i].station_name)
        strcpy(vec1[i].station_name, aux->station_name);
        aux = aux->tail;
    }

    qsort(vec1, bikeSharing->cant, sizeof(q1_struct), q1_cmp);

    return vec1;
}

q2_struct *q2(bikeSharingADT bikeSharing, int * dim)
{
    errno = 0;

    q2_struct *vec2 = malloc(((bikeSharing->cant * bikeSharing->cant) - (bikeSharing->cant)) * sizeof(q2_struct)); // puede llegar a reservar de mas
    MEMORY_CHECK_NULL(vec2)

    int k = 0;

    TList sAux, eAux;
    toBegin(bikeSharing, 1);

    for (int i = 0; i < bikeSharing->cant ; i++) 
    {
        toBegin(bikeSharing, 0);
        sAux = next(bikeSharing, 1);

        for (int j = 0; j < bikeSharing->cant ; j++) 
        {
            eAux = next(bikeSharing, 0);

            if ((i == j) || (bikeSharing->matrix[i][j] == 0 && bikeSharing->matrix[j][i] == 0))
            {
                continue;
            }

            vec2[k].start_station = malloc(strlen(sAux->station_name) + 1);
            MEMORY_CHECK_NULL(vec2[k].start_station)

            vec2[k].end_station = malloc(strlen(eAux->station_name) + 1);
            MEMORY_CHECK_NULL(vec2[k].end_station)

            strcpy(vec2[k].start_station, sAux->station_name);
            strcpy(vec2[k].end_station, eAux->station_name);
        

            vec2[k].trips_start_end = bikeSharing->matrix[i][j];
            vec2[k].trips_end_start = bikeSharing->matrix[j][i];

            k++;
        }
    }

    *dim = k;
    vec2 = realloc(vec2, k * sizeof(q2_struct));
    MEMORY_CHECK_NULL(vec2) //Hace falta? En todo caso estamos reduciendo el bloque de memoria utilizada

    return vec2;
}

q3_struct *q3(bikeSharingADT bikeSharing)
{
    TList aux = bikeSharing->first;
    errno = 0;
    q3_struct *vec3 = malloc(bikeSharing->cant * sizeof(q3_struct));
    MEMORY_CHECK_NULL(vec3)

    for (int i = 0; i < bikeSharing->cant; i++) 
    {
        vec3[i].station_name = malloc(strlen(aux->station_name) + 1);
        MEMORY_CHECK_NULL(vec3[i].station_name)
        strcpy(vec3[i].station_name, aux->station_name);
        for (int j = 0; j < MONTHS; j++)
        {
            vec3[i].months[j] = aux->months[j];
        }
        
        aux = aux->tail;
    }

    return vec3;
}
