#include "arbol_versiones.hpp"
#include "version.hpp"
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Función que busca recursivamente en todo el árbol
version_struct* buscarVersionRecursiva(version_struct* raiz, char* numeroVersion) {
    if (raiz == NULL) return NULL;
    
    if (strcmp(raiz->numero, numeroVersion) == 0) {
        return raiz;
    }
    
    version_struct* encontrada = buscarVersionRecursiva(raiz->primeraSubversion, numeroVersion);
    if (encontrada != NULL) return encontrada;
    
    return buscarVersionRecursiva(raiz->sigVersion, numeroVersion);
}

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

version_struct* buscarUltimaVersion(version_struct* raiz) {
    if (raiz == NULL) return NULL;
    version_struct* actual = raiz;
    while (actual->sigVersion != NULL) {
        actual = actual->sigVersion;
    }
    return actual;
}

char* obtenerPadre(char* version) {
    char* ultimoPunto = strrchr(version, '.');
    if (ultimoPunto == NULL) return NULL;
    
    char* padre = new char[ultimoPunto - version + 1];
    strncpy(padre, version, ultimoPunto - version);
    padre[ultimoPunto - version] = '\0';
    return padre;
}

int obtenerNumeroSubversion(char* version) {
    char* ultimoPunto = strrchr(version, '.');
    if (ultimoPunto == NULL) return atoi(version);
    
    return atoi(ultimoPunto + 1);
}

// Función auxiliar para obtener todas las versiones hermanas de un padre
void obtenerTodasHermanas(version_struct* padre, vector<version_struct*>& hermanas) {
    if (padre == NULL) return;
    
    version_struct* actual = padre->primeraSubversion;
    while (actual != NULL) {
        hermanas.push_back(actual);
        actual = actual->sigVersion;
    }
}

// Función para renumerar una versión y TODAS sus subversiones recursivamente
void renumerarVersionCompleta(version_struct* version, char* nuevoNumeroPadre) {
    if (version == NULL) return;
    
    // Guardar el número antiguo
    char* numeroAntiguo = version->numero;
    
    // Calcular nuevo número para esta versión
    char* ultimoPunto = strrchr(numeroAntiguo, '.');
    char* parteFinal = ultimoPunto + 1;
    
    char nuevoNumero[100];
    if (strcmp(nuevoNumeroPadre, "") == 0) {
        sprintf(nuevoNumero, "%s", parteFinal);
    } else {
        sprintf(nuevoNumero, "%s.%s", nuevoNumeroPadre, parteFinal);
    }
    
    // Actualizar el número de esta versión
    delete[] version->numero;
    version->numero = new char[strlen(nuevoNumero) + 1];
    strcpy(version->numero, nuevoNumero);
    
    // Renumerar recursivamente todas las subversiones
    version_struct* subversion = version->primeraSubversion;
    while (subversion != NULL) {
        renumerarVersionCompleta(subversion, nuevoNumero);
        subversion = subversion->sigVersion;
    }
}

// Función corregida para correr versiones hermanas
void correrVersionesHermanas(version_struct* padre, int numeroDesde) {
    if (padre == NULL) return;
    
    // Obtener todas las hermanas
    vector<version_struct*> hermanas;
    obtenerTodasHermanas(padre, hermanas);
    
    // Ordenar las hermanas por número (de mayor a menor para evitar conflictos)
    for (int i = hermanas.size() - 1; i >= 0; i--) {
        version_struct* hermana = hermanas[i];
        int numHermana = obtenerNumeroSubversion(hermana->numero);
        
        if (numHermana >= numeroDesde) {
            // Calcular nuevo número
            char nuevoNumeroPadre[100];
            if (padre->numero != NULL) {
                strcpy(nuevoNumeroPadre, padre->numero);
            } else {
                strcpy(nuevoNumeroPadre, "");
            }
            
            // Renumerar esta hermana y todas sus subversiones
            char nuevoNumero[100];
            sprintf(nuevoNumero, "%s.%d", nuevoNumeroPadre, numHermana + 1);
            renumerarVersionCompleta(hermana, nuevoNumeroPadre);
            
            // Actualizar el número de la versión principal
            delete[] hermana->numero;
            hermana->numero = new char[strlen(nuevoNumero) + 1];
            strcpy(hermana->numero, nuevoNumero);
        }
    }
}

void mostrarVersionesRecursivo(version_struct* version, int nivel) {
    if (version == NULL) return;
    
    for (int i = 0; i < nivel; i++) {
        cout << "\t";
    }
    
    cout << version->numero << endl;
    
    mostrarVersionesRecursivo(version->primeraSubversion, nivel + 1);
    mostrarVersionesRecursivo(version->sigVersion, nivel);
}

/**
 * @brief Elimina recursivamente una versión y todas sus subversiones
 * @param version Versión a eliminar
 */
void eliminarVersionRecursiva(version_struct* version) {
    if (version == NULL) return;
    
    // Primero eliminar todas las subversiones recursivamente
    version_struct* subversion = version->primeraSubversion;
    while (subversion != NULL) {
        version_struct* siguiente = subversion->sigVersion;
        eliminarVersionRecursiva(subversion);
        subversion = siguiente;
    }
    
    // Luego eliminar el texto si existe
    if (version->textoVersion != NULL) {
        // Liberar todas las líneas de texto
        linea* actualLinea = version->textoVersion->primeralineas;
        while (actualLinea != NULL) {
            linea* siguienteLinea = actualLinea->sig;
            delete[] actualLinea->contenido;
            delete actualLinea;
            actualLinea = siguienteLinea;
        }
        delete version->textoVersion;
    }
    
    // Finalmente liberar la versión misma
    delete[] version->numero;
    delete version;
}