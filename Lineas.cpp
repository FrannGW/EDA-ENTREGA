#include "Lineas.hpp"
#include "NavegacionVersiones.hpp"
#include <string.h>
#include <iostream>

using namespace std;

TipoRet InsertarLineaModulo(Archivo& a, char* version, char* contenidoLinea, unsigned int nroLinea, char* error) {
    if (a == NULL) {
        strcpy(error, "Archivo no existe");
        return ERROR;
    }

    version_struct* versionEncontrada = buscarVersionRecursiva(a->primeraVersion, version);
    if (versionEncontrada == NULL) {
        strcpy(error, "Version no existe");
        return ERROR;
    }

    if (versionEncontrada->primeraSubversion != NULL) {
        strcpy(error, "No se puede editar version con subversiones");
        return ERROR;
    }

    if (contenidoLinea == NULL || strlen(contenidoLinea) == 0) {
        strcpy(error, "Linea Vacia");
        return ERROR;
    }

    if (nroLinea < 1) {
        strcpy(error, "Numero de linea invalido");
        return ERROR;
    }

    if (versionEncontrada->textoVersion == NULL) {
        versionEncontrada->textoVersion = new texto;
        versionEncontrada->textoVersion->primeralineas = NULL;
        versionEncontrada->textoVersion->cantidadLineas = 0;
    }

    texto* txt = versionEncontrada->textoVersion;

    if (nroLinea > txt->cantidadLineas + 1) {
        strcpy(error, "Número de línea fuera de rango");
        return ERROR;
    }

    // Crear nueva línea
    linea* nuevaLinea = new linea;
    nuevaLinea->contenido = new char[strlen(contenidoLinea) + 1];
    strcpy(nuevaLinea->contenido, contenidoLinea);
    nuevaLinea->sig = NULL;

    // Insertar en posición correcta
    if (nroLinea == 1) {
        nuevaLinea->sig = txt->primeralineas;
        txt->primeralineas = nuevaLinea;
    } else {
        linea* actual = txt->primeralineas;
        for (unsigned int i = 1; i < nroLinea - 1; i++) {
            actual = actual->sig;
        }
        nuevaLinea->sig = actual->sig;
        actual->sig = nuevaLinea;
    }

    txt->cantidadLineas++;

    // REGISTRAR CAMBIO - INSERCIÓN
    cambio* nuevoCambio = new cambio;
    nuevoCambio->tipo = 'I';
    nuevoCambio->nroLinea = nroLinea;
    nuevoCambio->contenido = new char[strlen(contenidoLinea) + 1];
    strcpy(nuevoCambio->contenido, contenidoLinea);
    nuevoCambio->sig = versionEncontrada->cambios;
    versionEncontrada->cambios = nuevoCambio;

    return OK;
}

TipoRet MostrarTextoModulo(Archivo a, char* version) {
    if (a == NULL) {
        return ERROR;
    }

    version_struct* versionEncontrada = buscarVersionRecursiva(a->primeraVersion, version);
    if (versionEncontrada == NULL) {
        return ERROR;
    }
    
    cout << a->nombre << " - " << version << endl;
    cout << endl;

    if (versionEncontrada->textoVersion == NULL || 
        versionEncontrada->textoVersion->primeralineas == NULL) {
        cout << "No contiene lineas" << endl;
        return OK;
    }

    linea* actual = versionEncontrada->textoVersion->primeralineas;
    unsigned int numeroLinea = 1;

    while (actual != NULL) {
        cout << numeroLinea << "\t" << actual->contenido << endl;
        actual = actual->sig;
        numeroLinea++;
    }
    
    return OK;
}

TipoRet BorrarLineaModulo(Archivo& a, char* version, unsigned int nroLinea, char* error) {
    if (a == NULL) {
        strcpy(error, "Archivo no existe");
        return ERROR;
    }

    version_struct* versionEncontrada = buscarVersionRecursiva(a->primeraVersion, version);
    if (versionEncontrada == NULL) {
        strcpy(error, "Version no existe");
        return ERROR;
    }

    if (versionEncontrada->primeraSubversion != NULL) {
        strcpy(error, "No se puede editar version con subversiones");
        return ERROR;
    }

    if (versionEncontrada->textoVersion == NULL || 
        versionEncontrada->textoVersion->primeralineas == NULL) {
        strcpy(error, "El texto de la version esta vacio");
        return ERROR;
    }

    texto* txt = versionEncontrada->textoVersion;

    if (nroLinea < 1 || nroLinea > txt->cantidadLineas) {
        strcpy(error, "Número de línea fuera de rango");
        return ERROR;
    }

    linea* actual = txt->primeralineas;
    linea* anterior = NULL;
    char* contenidoBorrado = NULL;

    if (nroLinea == 1) {
        contenidoBorrado = new char[strlen(actual->contenido) + 1];
        strcpy(contenidoBorrado, actual->contenido);
        txt->primeralineas = actual->sig;
        delete[] actual->contenido;
        delete actual;
    } else {
        for (unsigned int i = 1; i < nroLinea; i++) {
            anterior = actual;
            actual = actual->sig;
        }

        contenidoBorrado = new char[strlen(actual->contenido) + 1];
        strcpy(contenidoBorrado, actual->contenido);

        if (anterior != NULL) {
            anterior->sig = actual->sig;
        }

        delete[] actual->contenido;
        delete actual;
    }

    txt->cantidadLineas--;

    if (txt->cantidadLineas == 0) {
        txt->primeralineas = NULL;
    }

    // REGISTRAR CAMBIO - BORRADO
    cambio* nuevoCambio = new cambio;
    nuevoCambio->tipo = 'B';
    nuevoCambio->nroLinea = nroLinea;
    nuevoCambio->contenido = NULL; // Los borrados no muestran contenido
    nuevoCambio->sig = versionEncontrada->cambios;
    versionEncontrada->cambios = nuevoCambio;

    delete[] contenidoBorrado;

    return OK;
}