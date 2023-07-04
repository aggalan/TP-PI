#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stddef.h>
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
    double latitude;
    double longitude;
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