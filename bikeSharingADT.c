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

typedef struct idSort   
{
    int id;
    TList station;
    
}idSort;

typedef struct bikeSharingCDT
{
    TList first; 
    TList sIter;
    TList eIter;
    size_t cant; 
    int **matrix;
    idSort * arr;
    int limit_start_year;
    int limit_end_year;
    int l;
    int r;
    size_t last_index;

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

void freeBikeSharing(bikeSharingADT bs) 
{
    TList curr = bs->first, aux;

    while (curr != NULL)
    {
        aux = curr->tail;
        free(curr->station_name);
        free(curr);
        curr = aux;
    }

    if (bs->matrix != NULL) 
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


static TList addStationRec(TList list, char *station_name, int id, int *flag, int * memFlag, int * last_index)
{
    int c;
    if (list == NULL || (c = strcmp(station_name, list->station_name)) < 0)
    {
        errno = 0;
        TList new = malloc(sizeof(TNode));

        if (errno == ENOMEM)
        {
            *memFlag = 1;
            return NULL;
        }

        new->id = id;

        new->station_name = malloc(strlen(station_name) + 1);

        strcpy(new->station_name, station_name); 

        new->member_trips = 0; 

        new->circular_trips = 0; 

        for (int i = 0; i < MONTHS; i++)
        {
            new->months[i] = 0; 
        }

        new->tail = list;

        (*flag)++;
        return new;
    }

    if (c > 0)
    {
        (*last_index)++;
        list->tail = addStationRec(list->tail, station_name, id, flag, memFlag, last_index);
        return list;
    }

    return list;
}

int addStation(bikeSharingADT bikeSharing, char *station_name, int id)
{
    int flag = 0, memFlag = 0, last_index=0;
    bikeSharing->first = addStationRec(bikeSharing->first, station_name, id, &flag, &memFlag, &last_index);
    bikeSharing->cant += flag; 
    bikeSharing->last_index = last_index;
    return memFlag;
}


static int idCmp(const void *e1, const void *e2)
{
    const idSort *ptr1 = (const idSort *)e1;
    const idSort *ptr2 = (const idSort *)e2;
    return (ptr1->id - ptr2->id);
}

int setArr(bikeSharingADT bs, int index) 
{
    if (index < 0) {
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
    }
    else {
        int id;
        TList aux = bs->first;
        for (int i=0; i<index; i++) {
            aux = aux->tail;
        }
        id = aux->id;

        bs->arr = realloc(bs->arr, bs->cant * sizeof(idSort));
        MEMORY_CHECK_1(bs->arr)

        bs->arr[bs->cant-1].id = id;
        bs->arr[bs->cant-1].station = aux;

        qsort(bs->arr, bs->cant, sizeof(idSort), idCmp);

        bs->l=0;
        bs->r=bs->cant-1;

        aux->index = bs->last_index;
        aux=aux->tail;

        for (int i=index+1; i<bs->cant; i++) {
            aux->index = i; 
            aux=aux->tail;
        }
    }

    return 0;

}

int setMatrix(bikeSharingADT bs, int *cant)
{
    if (bs->matrix == NULL) {
        int i;
        errno = 0;

        if(setArr(bs, -1))
            return 1;
        
        bs->matrix = malloc(bs->cant * sizeof(int *)); // reservo memoria filas

        if (errno == ENOMEM)
        {
            return 1;
        }

        for (i = 0; i < bs->cant; i++) 
        {
            bs->matrix[i] = calloc(bs->cant, sizeof(int)); // reservo fila columnas

            if (errno == ENOMEM)
            {
                return 1;
            }
        }
    }
    else {
       
        int i, j;
        int index = bs->last_index;

        setArr(bs, index);


        bs->matrix = realloc(bs->matrix, bs->cant * sizeof(int *));
        MEMORY_CHECK_1(bs->matrix)

        for (i = bs->cant - 1; i > index; i--) {
            bs->matrix[i] = bs->matrix[i - 1];
        } 
        bs->matrix[index] = calloc(bs->cant, sizeof(int));
        MEMORY_CHECK_1(bs->matrix[index])



        for (i = 0; i < bs->cant; i++) {
            bs->matrix[i] = realloc(bs->matrix[i], bs->cant * sizeof(int));
            MEMORY_CHECK_1(bs->matrix[i])

            for (j = bs->cant - 1; j > index; j--) {
                bs->matrix[i][j] = bs->matrix[i][j - 1];
            }
            bs->matrix[i][index] = 0;
        }
    }

    *cant = bs->cant;

    return 0;
}


TList binarySearch(idSort * arr, int l, int r, int id)
{
    while (l <= r) {
        
        int m = l + (r - l) / 2;
 
      
        if (arr[m].id == id)
            return arr[m].station;
 
       
        if (arr[m].id < id)
            l = m + 1;
 
        
        else
            r = m - 1;
    }
 
   
    return NULL;
}
 


void addTrip(bikeSharingADT bikeSharing, int isMember, int startId, int endId, int year, int month)
{
    TList sAux, eAux;

    sAux = binarySearch(bikeSharing->arr, bikeSharing->l, bikeSharing->r, startId);

    eAux = binarySearch(bikeSharing->arr, bikeSharing->l, bikeSharing->r, endId);

    if (sAux == NULL || eAux == NULL)
    {
        return;
    }

    if (isMember)
    {
        sAux->member_trips++;
    }

    sAux->months[month - 1]++;

    if (startId == endId && (year >= bikeSharing->limit_start_year && year <= bikeSharing->limit_end_year))
    {
        sAux->circular_trips++;
    }

    else
    {
        bikeSharing->matrix[sAux->index][eAux->index]++;
    }
}

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

    q2_struct *vec2 = malloc(((bikeSharing->cant * bikeSharing->cant) - (bikeSharing->cant)) * sizeof(q2_struct)); 
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
    MEMORY_CHECK_NULL(vec2) 

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
