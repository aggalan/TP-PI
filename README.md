Instructivo de como generar los ejecutables y como correrlos:

ACLARACION IMPORTANTE: Todos los archivos provistos por el grupo deben estar en el mismo directorio, en caso contrario se detallara que hacer en cada caso.

En una primera instancia se debe correr en la terminal el comando 'make' como se muestra a continuacion:

~$ make

Una vez que se haya ejecutado el comando, se van a generar los ejecutables bikeSharingMON y bikeSharingNYC en el directorio donde se esta trabajando.
Lo que realiza este este comando es compilar el programa con los respectivos archivos .c, .h y flags indicados por el archivo "Makefile".

Ahora lo que se debe hacer para correr los ejecutables correr en la terminal los siguientes comandos:

~$ ./bikeSharingMON stationsMON.csv bikesMON.csv año_comienzo (opcional) año_fin (opcional)

~$ ./bikeSharingNYC stationsNYC.csv bikesNYC.csv año_comienzo (opcional) año_fin (opcional)

En el caso de que los archivos .csv no se encuentren en el mismo directorio que los ejecutables, los ejecutables se deben correr de la siguiente manera:

~$ ./bikeSharingMON path/stationsMON.csv path/bikesMON.csv año_comienzo (opcional) año_fin (opcional)

~$ ./bikeSharingNYC path/stationsNYC.csv path/bikesNYC.csv año_comienzo (opcional) año_fin (opcional)
