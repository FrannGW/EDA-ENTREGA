#include "arbol_versiones.hpp"
#include "version.hpp"
#include <string.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

// Función para buscar una versión en el árbol
version_struct* buscarVersion(version_struct* raiz, char* numeroVersion) {
    version_struct* actual = raiz;
    while (actual != NULL) {
        if (strcmp(actual->numero, numeroVersion) == 0) {
            return actual;
        }
        actual = actual->sigVersion;
    }
    return NULL;
}

// Función para encontrar la última versión en una lista
version_struct* buscarUltimaVersion(version_struct* raiz) {
    if (raiz == NULL) return NULL;
    version_struct* actual = raiz;
    while (actual->sigVersion != NULL) {
        actual = actual->sigVersion;
    }
    return actual;
}

// Función para obtener el padre de una versión
char* obtenerPadre(char* version) {
    char* ultimoPunto = strrchr(version, '.');
    if (ultimoPunto == NULL) return NULL;
    
    char* padre = new char[ultimoPunto - version + 1];
    strncpy(padre, version, ultimoPunto - version);
    padre[ultimoPunto - version] = '\0';
    return padre;
}

// Función para obtener el número de subversión
int obtenerNumeroSubversion(char* version) {
    char* ultimoPunto = strrchr(version, '.');
    if (ultimoPunto == NULL) return 0; // Versión raíz
    
    return atoi(ultimoPunto + 1);
}

// Función para crear una versión con nuevo número
version_struct* crearVersionConNumero(char* versionBase, int nuevoNumero) {
    char* padre = obtenerPadre(versionBase);
    char nuevoNumeroStr[100];
    
    if (padre != NULL) {
        sprintf(nuevoNumeroStr, "%s.%d", padre, nuevoNumero);
        delete[] padre;
    } else {
        sprintf(nuevoNumeroStr, "%d", nuevoNumero);
    }
    
    return crearVersionSimple(nuevoNumeroStr);
}

// Función para renumerar una versión y todas sus subversiones
void renumerarVersionYSubversiones(version_struct* version, char* nuevoNumero) {
    if (version == NULL) return;
    
    // Guardar el número antiguo para usar en subversiones
    char* numeroAntiguo = version->numero;
    
    // Cambiar el número de esta versión
    delete[] version->numero;
    version->numero = new char[strlen(nuevoNumero) + 1];
    strcpy(version->numero, nuevoNumero);
    
    // Renumerar todas las subversiones recursivamente
    version_struct* subversion = version->primeraSubversion;
    while (subversion != NULL) {
        // Extraer la parte específica de la subversión
        char* puntoAntiguo = strrchr(numeroAntiguo, '.');
        //char* puntoNuevo = strrchr(nuevoNumero, '.');
        
        char parteSubversion[100];
        if (puntoAntiguo != NULL) {
            // Si el padre tenía puntos, tomar solo la última parte
            strcpy(parteSubversion, puntoAntiguo + 1);
        } else {
            // Si el padre era raíz, tomar todo el número de la subversión
            strcpy(parteSubversion, subversion->numero);
        }
        
        // Construir nuevo número para la subversión
        char nuevoNumeroSubversion[100];
        sprintf(nuevoNumeroSubversion, "%s.%s", nuevoNumero, parteSubversion);
        
        // Llamar recursivamente para renumerar la subversión
        renumerarVersionYSubversiones(subversion, nuevoNumeroSubversion);
        
        subversion = subversion->sigVersion;
    }
    
    delete[] numeroAntiguo;
}

// Función recursiva para mostrar versiones de forma jerárquica
void mostrarVersionesRecursivo(version_struct* version, int nivel) {
    if (version == NULL) return;
    
    // Imprimir indentación según el nivel
    for (int i = 0; i < nivel; i++) {
        cout << "\t";  // Un tabulador por nivel
    }
    
    // Imprimir el número de versión
    cout << version->numero << endl;
    
    // Mostrar subversiones recursivamente (aumentando el nivel)
    mostrarVersionesRecursivo(version->primeraSubversion, nivel + 1);
    
    // Mostrar versiones hermanas (mismo nivel)
    mostrarVersionesRecursivo(version->sigVersion, nivel);
}