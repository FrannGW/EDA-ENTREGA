#include "version.hpp"
#include <string.h>
#include <stdlib.h>
#include <iostream>

using namespace std;


//Crea una nueva versión básica sin texto ni relaciones
//dentificador de la versión (ej: "1", "2.1")
//Puntero a la versión creada
version_struct* crearVersionSimple(char* numero) {
    version_struct* nueva = new version_struct;
    nueva->numero = new char[strlen(numero) + 1];
    strcpy(nueva->numero, numero);
    nueva->textoVersion = NULL;     // sin texto 
    nueva->padre = NULL;            // sin padre 
    nueva->primeraSubversion = NULL; // sin subversiones 
    nueva->sigVersion = NULL;       // sin hermanas 
    return nueva;
}

/*
 * @brief Valida el formato de un número de versión
 * @param version String a validar
 * @param error Buffer para mensaje de error
 * @return true si es válido, false si no
 */


//establecemos las reglas de validacion que especifica la letra
 bool validarVersion(char* version, char* error) {
    if (version == NULL || strlen(version) == 0) {
        strcpy(error, "Version vacia");
        return false;
    }
    
    // Solo permite dígitos y puntos
    for (unsigned int i = 0; i < strlen(version); i++) {
        if (!(version[i] >= '0' && version[i] <= '9') && version[i] != '.') {
            strcpy(error, "Version contiene caracteres invalidos");
            return false;
        }
    }
    
    // no puede empezar o terminar con punto
    if (version[0] == '.' || version[strlen(version) - 1] == '.') {
        strcpy(error, "Version no puede empezar o terminar con punto");
        return false;
    }
    
    // no puede tener puntos consecutivos
    for (unsigned int i = 0; i < strlen(version) - 1; i++) {
        if (version[i] == '.' && version[i + 1] == '.') {
            strcpy(error, "Version no puede tener puntos consecutivos");
            return false;
        }
    }
    
    return true;
}