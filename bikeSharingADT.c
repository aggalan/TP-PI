#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>
#include <errno.h>
#include "bikeSharing.h"

#define MAX_LETTERS 60
#define MONTHS 12

static enum months {JAN = 0, FEB, MAR, APR, JUN, JUL, AUG, SEP, OCT, NOV, DEC};

typedef struct trips {
    char end_station_name[MAX_LETTERS];
    size_t trips;
} Ttrip;

typedef struct node {
    int id;
    char station_name[MAX_LETTERS];
    size_t total_trips;
    size_t circular_trips;
    size_t trips_per_month[MONTHS];
    Ttrip * first;
    struct node  * tail;
}TNode;

typedef TNode * TList;

typedef struct bikeSharingCDT {
    TList first;
}bikeSharingCDT;

bikeSharingADT newBikeSharing() {
    calloc(1, sizeof(bikeSharingCDT));
}


static TList addStationRec (TList list, char * station_name, int id) {
    int c;
    if (list == NULL || (c = strcmp(station_name, list->station_name)) < 0) {
        errno = 0;
        TList new = calloc(1, sizeof(TNode));
        if (errno == ENOMEM) {
            perror("Insufficient memory");
            exit(1);
        }
        new->id = id;
        strcpy(new->station_name, station_name);
        new->tail = list;
        return new;
    }
    if (c > 0) {
        list->tail = addStationRec(list->tail, station_name, id);
        return list;
    }
    return list; //Ya existia esa estacion en la lista
}

void addStation (bikeSharingADT bikeSharing, char * station_name, int id) {
    bikeSharing->first = addStationRec(bikeSharing->first, station_name, id);
}