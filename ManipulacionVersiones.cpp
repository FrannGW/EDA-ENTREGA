#include "ManipulacionVersiones.hpp"
#include "NavegacionVersiones.hpp"
#include <string.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

//version es una version valida, nuevoNumeroPadre es el prefijo para la numeracion
//Renumera recusrivamente la version y todas sus subversiones con el nuevo prefijo
void renumerarVersionCompleta(version_struct* version, char* nuevoNumeroPadre) {
    if (version == NULL) return;
    
    char* numeroAntiguo = version->numero;
    char* ultimoPunto = strrchr(numeroAntiguo, '.');
    char* parteFinal = ultimoPunto + 1;
    
    char nuevoNumero[100];
    if (strcmp(nuevoNumeroPadre, "") == 0) {
        sprintf(nuevoNumero, "%s", parteFinal);
    } else {
        sprintf(nuevoNumero, "%s.%s", nuevoNumeroPadre, parteFinal);
    }
    
    delete[] version->numero;
    version->numero = new char[strlen(nuevoNumero) + 1];
    strcpy(version->numero, nuevoNumero);
    
    version_struct* subversion = version->primeraSubversion;
    while (subversion != NULL) {
        renumerarVersionCompleta(subversion, nuevoNumero);
        subversion = subversion->sigVersion;
    }
}

//incrementa 1 versiones hermanas a partir de numeroDesde Y renumeramos subversiones
void correrVersionesHermanas(version_struct* padre, int numeroDesde) {
    if (padre == NULL) return;
    
// primero, contamos cuántas versiones hermanas hay
    int totalHermanas = 0;
    version_struct* actual = padre->primeraSubversion;
    while (actual != NULL) {
        totalHermanas++;
        actual = actual->sigVersion;
    }
    
    // si no hay hermanas, no hay nada que hacer
    if (totalHermanas == 0) return;
    
    // Creamos un array temporal para almacenar las versiones
    version_struct** hermanas = new version_struct*[totalHermanas];
    
    // Llenamos el array con las versiones hermanas
    actual = padre->primeraSubversion;
    for (int i = 0; i < totalHermanas && actual != NULL; i++) {
        hermanas[i] = actual;
        actual = actual->sigVersion;
    }
    
    // procesamos de mayor a menor número de subversión
    for (int i = totalHermanas - 1; i >= 0; i--) {
        version_struct* hermana = hermanas[i];
        int numHermana = obtenerNumeroSubversion(hermana->numero);
        
        if (numHermana >= numeroDesde) {
            char nuevoNumeroPadre[100];
            if (padre->numero != NULL) {
                strcpy(nuevoNumeroPadre, padre->numero);
            } else {
                strcpy(nuevoNumeroPadre, "");
            }
            
            char nuevoNumero[100];
            sprintf(nuevoNumero, "%s.%d", nuevoNumeroPadre, numHermana + 1);
            renumerarVersionCompleta(hermana, nuevoNumeroPadre);
            
            delete[] hermana->numero;
            hermana->numero = new char[strlen(nuevoNumero) + 1];
            strcpy(hermana->numero, nuevoNumero);
        }
    }
    
    // Liberamos el array temporal
    delete[] hermanas;
}