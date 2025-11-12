#include "EliminacionVersiones.hpp"
#include "NavegacionVersiones.hpp"
#include <string.h>
#include <iostream>

using namespace std;

//version es un puntero a version_struct
//liberamos de forma recusriva la version y todas sus subversiones
void eliminarVersionRecursiva(version_struct* version) {
    if (version == NULL) return;
    
    version_struct* subversion = version->primeraSubversion;
    while (subversion != NULL) {
        version_struct* siguiente = subversion->sigVersion;
        eliminarVersionRecursiva(subversion);
        subversion = siguiente;
    }
    
    // LIBERAR TEXTO - IMPLEMENTACIÓN COMPLETA
    if (version->textoVersion != NULL) {
        linea* actualLinea = version->textoVersion->primeralineas;
        while (actualLinea != NULL) {
            linea* siguienteLinea = actualLinea->sig;
            delete[] actualLinea->contenido;
            delete actualLinea;
            actualLinea = siguienteLinea;
        }
        delete version->textoVersion;
    }

    cambio* actualCambio = version->cambios;
    while (actualCambio != NULL) {
        cambio* siguienteCambio = actualCambio->sig;
        if (actualCambio->contenido != NULL) {
            delete[] actualCambio->contenido;
        }
        delete actualCambio;
        actualCambio = siguienteCambio;
    }
    
    delete[] version->numero;
    delete version;
}

//elimina version y sus subversiones, y renumera las versiones hermanas
TipoRet borrarVersionModulo(Archivo& a, char* version, char* error) {
    if (a == NULL) {
        strcpy(error, "Archivo no existe");
        return ERROR;
    }

    version_struct* versionEliminar = buscarVersionRecursiva(a->primeraVersion, version);
    if (versionEliminar == NULL) {
        strcpy(error, "Version no existe");
        return ERROR;
    }

    // reorganizacion de punteros del padre/hermanos
    if (versionEliminar->padre == NULL) {
        version_struct* actual = a->primeraVersion;
        version_struct* anterior = NULL;
        
        while (actual != NULL && actual != versionEliminar) {
            anterior = actual;
            actual = actual->sigVersion;
        }
        
        if (anterior == NULL) {
            a->primeraVersion = versionEliminar->sigVersion;
        } else {
            anterior->sigVersion = versionEliminar->sigVersion;
        }
    } else {
        version_struct* padre = versionEliminar->padre;
        version_struct* actual = padre->primeraSubversion;
        version_struct* anterior = NULL;
        
        while (actual != NULL && actual != versionEliminar) {
            anterior = actual;
            actual = actual->sigVersion;
        }
        
        if (anterior == NULL) {
            padre->primeraSubversion = versionEliminar->sigVersion;
        } else {
            anterior->sigVersion = versionEliminar->sigVersion;
        }
    }

    eliminarVersionRecursiva(versionEliminar);
    return OK;
}