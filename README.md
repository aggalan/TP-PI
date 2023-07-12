# TPE Programación Imperativa

El trabajo consiste en un programa que lee y procesa viajes entre distintas estaciones de alquiler de bicicletas de una ciudad en especifico.

EL programa genera 4 queries distintas, en base a la información extraida de los archivos .csv provistos por el usuario:

 - Query 1: Total de viajes iniciados por miembros por estación, donde la información se guarda ordenada de forma descendente por cantidad total de viajes, y a igualdad de viajes desempata alfabéticamente por nombre de la estación.

 - Query 2: Total de viajes entre dos estaciones (A y B), donde la información se guarda ordenada alfabéticamente, y en caso de igualdad de viajes, desempata a partir del orden alfabética de la estacion B.

 - Query 3: Total de viajes por mes por estacion, donde la información se guarda en orden alfabética segun el nombre de la estación.

 - Query 4: Total de viajes circulares por estación, donde viajes circulares se refiere a viajes que arrancan y terminan en la misma estación. La información se guarda ordenada de forma descecndente por cantidad total de viajes, y a igualdad de viajes desempata alfábeticamente por nombre de la estación.

 Aclaración: Para el query 4 se pueden introducir años limites, tanto inicial como final. 

 Aclaración: Todos los archivos .csv deben estar en el mismo directorio, en caso contrario se detallara que hacer en cada caso.

 La información de cada query se guarda en dos archivos, uno .csv y otro .html.



## Instalación: 

Clone el repositorio:

- HTTPS:

```sh
git clone https://github.com/aggalan/TP-PI.git
```

- SSH:

```sh
git clone git@github.com:aggalan/TP-PI.git
```

### Instrucciones: 

Instrucciones para generar los ejecutables y como correrlos. Se toma como ejemplo la ciudad de Montreal, Canadá.

1. Preparar los archivos con la información en dos archivos .csv, en los cuales se tendrá la información de los viajes y la información de las estaciones, cada uno en su archivo correspondiente.

2. Generar el ejecutable con el comando 'make':
```sh
make bikeSharingMON
```

3. Ejecutar el programa:
```sh
./bikeSharingMON bikesMON.csv stationsMON.csv año_comienzo (opcional) año_fin (opcional)
```
Aclaración:
En el caso que los archivos provistos no esten en mismo directorio que los ejecutables, se debera ejecutar el programa de la siguiente forma:
```sh
./bikeSharingMON path/bikesMON.csv path/stationsMON.csv año_comienzo (opcional) año_fin (opcional)
```


### Integrantes: 

Agustín Galan (64098) - aggalan@itba.edu.ar

José Benegas Lynch (64242) - jmblynch@itba.edu.ar

Leo Weitz (64365) - lweitz@itba.edu.ar

Santiago Maffeo (64245) - smaffeo@itba.edu.ar
