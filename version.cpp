#include "version.hpp"
#include <string.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

version_struct* crearVersionSimple(char* numero) {
    version_struct* nueva = new version_struct;
    nueva->numero = new char[strlen(numero) + 1];
    strcpy(nueva->numero, numero);
    nueva->textoVersion = NULL;
    nueva->padre = NULL;
    nueva->primeraSubversion = NULL;
    nueva->sigVersion = NULL;
    return nueva;
}

// Versión SIMPLIFICADA de buscarVersion - solo busca en lista plana
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

// Versión SIMPLIFICADA de validarVersion - solo verifica formato básico
bool validarVersion(char* version, char* error) {
    // Verificar que no esté vacío
    if (version == NULL || strlen(version) == 0) {
        strcpy(error, "Version vacia");
        return false;
    }
    
    // Verificar que solo contenga números y puntos
    for (unsigned int i = 0; i < strlen(version); i++) {
        if (!(version[i] >= '0' && version[i] <= '9') && version[i] != '.') {
            strcpy(error, "Version contiene caracteres invalidos");
            return false;
        }
    }
    
    // Verificar que no empiece ni termine con punto
    if (version[0] == '.' || version[strlen(version) - 1] == '.') {
        strcpy(error, "Version no puede empezar o terminar con punto");
        return false;
    }
    
    // Verificar que no tenga puntos consecutivos
    for (unsigned int i = 0; i < strlen(version) - 1; i++) {
        if (version[i] == '.' && version[i + 1] == '.') {
            strcpy(error, "Version no puede tener puntos consecutivos");
            return false;
        }
    }
    
    return true;
}

version_struct* buscarUltimaVersion(version_struct* raiz) {
    if (raiz == NULL) return NULL;
    version_struct* actual = raiz;
    while (actual->sigVersion != NULL) {
        actual = actual->sigVersion;
    }
    return actual;
}