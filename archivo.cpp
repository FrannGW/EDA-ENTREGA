#include "archivo.hpp"
#include "CreacionVersiones.hpp"
#include "EliminacionVersiones.hpp"
#include "MostrarVersiones.hpp"
#include "arbol_versiones.hpp"
#include "version.hpp"
#include <iostream>
#include <string.h>

using namespace std;

Archivo CrearArchivo(char* nombre) {
    if (nombre == NULL) {
        return NULL;
    }

    Archivo a = new nodo_archivo;
    if (a == NULL) {
        return NULL;
    }

    // Asignar y copiar nombre
    a->nombre = new char[strlen(nombre) + 1];
    strcpy(a->nombre, nombre);
    
    // Inicializar sin versiones
    a->primeraVersion = NULL;

    return a;
}

TipoRet BorrarArchivo(Archivo& a) {
    if (a == NULL) {
        return ERROR;
    }

    // Liberar todas las versiones raíz recursivamente
    version_struct* versionActual = a->primeraVersion;
    while (versionActual != NULL) {
        version_struct* siguiente = versionActual->sigVersion;
        eliminarVersionRecursiva(versionActual);
        versionActual = siguiente;
    }

    // Liberar nombre y estructura principal
    delete[] a->nombre;
    delete a;
    a = NULL;

    return OK;
}

// AHORA SOLO INVOCAMOS LOS MÓDULOS:

TipoRet CrearVersion(Archivo& a, char* version, char* error) {
    return crearVersionModulo(a, version, error);
}

TipoRet BorrarVersion(Archivo& a, char* version) {
    char error[100];
    return borrarVersionModulo(a, version, error);
}

TipoRet MostrarVersiones(Archivo a) {
    return mostrarVersionesModulo(a);
}

// MANTENER EL RESTO DE FUNCIONES IGUAL:

// Función auxiliar para verificar si una versión puede ser editada
bool versionPuedeSerEditada(version_struct* version) {
    return version != NULL && version->primeraSubversion == NULL;
}

TipoRet InsertarLinea(Archivo& a, char* version, char* contenidoLinea, unsigned int nroLinea, char* error) {
    if (a == NULL) {
        strcpy(error, "Archivo no existe");
        return ERROR;
    }

    version_struct* versionEncontrada = buscarVersion(a->primeraVersion, version);
    if (versionEncontrada == NULL) {
        strcpy(error, "Version no existe");
        return ERROR;
    }

    if (versionEncontrada->primeraSubversion != NULL){
        strcpy(error, "No se puede editar version con subversiones");
        return ERROR;
    }
    
    if (contenidoLinea == NULL || strlen(contenidoLinea) == 0){
        strcpy(error, "Linea Vacia");
        return ERROR;
    }
    
    if (nroLinea < 1){
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

    linea* nuevaLinea = new linea;
    nuevaLinea->contenido = new char[strlen(contenidoLinea) + 1];
    strcpy(nuevaLinea->contenido, contenidoLinea);
    nuevaLinea->sig = NULL;

    if (nroLinea == 1) {
        // Insertar al principio
        nuevaLinea->sig = txt->primeralineas;
        txt->primeralineas = nuevaLinea;
    } else {
        // Insertar en medio o al final
        linea* actual = txt->primeralineas;
        for (unsigned int i = 1; i < nroLinea - 1; i++) {
            actual = actual->sig;
        }
        nuevaLinea->sig = actual->sig;
        actual->sig = nuevaLinea;
    }

    txt->cantidadLineas++;
    return OK;
}                               

TipoRet BorrarLinea(Archivo& a, char* version, unsigned int nroLinea, char* error) {
    // TODO: Implementar eliminación de línea
    (void)a; (void)version; (void)nroLinea; (void)error;
    return NO_IMPLEMENTADA;
}

TipoRet MostrarTexto(Archivo a, char* version) {
    if (a == NULL){
        return ERROR;
    }

    version_struct* versionEncontrada = buscarVersion(a->primeraVersion, version);
    if (versionEncontrada == NULL){
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

    while (actual != NULL){
        cout << numeroLinea << "\t" << actual->contenido << endl;
        actual = actual->sig;
        numeroLinea++;
    }
    
    return OK;
}

// Funciones no implementadas
TipoRet MostrarCambios(Archivo a, char* version) { 
    (void)a; (void)version;
    return NO_IMPLEMENTADA; 
}

TipoRet Iguales(Archivo a, char* version1, char* version2, bool& iguales) { 
    (void)a; (void)version1; (void)version2; (void)iguales;
    return NO_IMPLEMENTADA; 
}

TipoRet VersionIndependiente(Archivo& a, char* version) { 
    (void)a; (void)version;
    return NO_IMPLEMENTADA; 
}