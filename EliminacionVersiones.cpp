#include "EliminacionVersiones.hpp"
#include "arbol_versiones.hpp"
#include <string.h>

using namespace std;

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

    // Reorganizar punteros del padre/hermanos
    if (versionEliminar->padre == NULL) {
        // Es versión raíz
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
        // Es subversión
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

    // Eliminar recursivamente
    eliminarVersionRecursiva(versionEliminar);
    return OK;
}