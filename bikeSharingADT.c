#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef struct node {
    int id;
    double latitude;
    double longitude;
    char station_name[60];
    struct node  * tail;
}TNode;

typedef TNode * TList;
#include "bikeSharing.h"

typedef struct bikeSharingCDT {
    

}bikeSharingCDT;
