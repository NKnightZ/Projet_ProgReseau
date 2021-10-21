#include <stdio.h>

struct var{
    double spent;
    char* name;
    char* date;
    char* utility; // for example hotel, market, travel
};

// faire de l'UDP, (et sinon mettre un compteur de paquet pour verifier qu'il n'y a pas de paquet perdu ça sera un bonus) 