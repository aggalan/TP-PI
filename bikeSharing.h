#include <stdio.h>
#include <stdlib.h>
#ifndef __bikeSharing_h
#define __bikeSharing_h

typedef struct bikeSharingCDT * bikeSharingADT;


/*libera toda memoria reservada por el TAD*/
void freeBikeSharing(bikeSharingADT bikeSharing);

/*Retorna un nuevo bikesharing. Al principio esta vacio*/
bikeSharingADT newBikeSharing();












#endif