#include "archivo.hpp"
#include "CreacionVersiones.hpp"
#include "EliminacionVersiones.hpp"
#include "MostrarVersiones.hpp"
#include "NavegacionVersiones.hpp"
#include "version.hpp"
#include <iostream>
#include <string.h>

using namespace std;

// nombre tiene que ser un string valido
//retornamos archivo con nombre especificado y sin vesriones
// si falla asignacion de memoria retorna NULL
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
    
    
    a->primeraVersion = NULL;   // Inicializar sin versiones

    return a;
}

//referencia a un puntero de archivo creado]
//cuando borro libero todo la memoria del archivo y sus versiones.
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

// a es un archivo valido, y version un string con el formato version
// crea la version del archivo, y retorna OK si se crea
TipoRet CrearVersion(Archivo& a, char* version, char* error) {
    return crearVersionModulo(a, version, error);
}

//elimina version y subversion y las renumera a las versiones hermanas
TipoRet BorrarVersion(Archivo& a, char* version) {
    char error[100];
    return borrarVersionModulo(a, version, error);
}

//muestra las versiones de forma jerarquica
TipoRet MostrarVersiones(Archivo a) {
    return mostrarVersionesModulo(a);
}

// AUN SIN IMPLEMENTAR

// Función auxiliar para verificar si una versión puede ser editada
bool versionPuedeSerEditada(version_struct* version) {
    return version != NULL && version->primeraSubversion == NULL;
}


//MENCIONAR AL PROFESOR QUE ESTO NO ESTA IMPLEMENTADO AUN, ES CODIGO QUE AUN NO FUNCIONA
TipoRet InsertarLinea(Archivo& a, char* version, char* contenidoLinea, unsigned int nroLinea, char* error) {
    return NO_IMPLEMENTADA;
}                               

TipoRet BorrarLinea(Archivo& a, char* version, unsigned int nroLinea, char* error) {
    // TODO: Implementar eliminación de línea
    (void)a; (void)version; (void)nroLinea; (void)error;
    return NO_IMPLEMENTADA;
}

TipoRet MostrarTexto(Archivo a, char* version) {
     return NO_IMPLEMENTADA;
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