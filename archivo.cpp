#include "archivo.hpp"
#include "CreacionVersiones.hpp"
#include "EliminacionVersiones.hpp"
#include "MostrarVersiones.hpp"
#include "version.hpp"
#include "Lineas.hpp"
#include "MostrarCambios.hpp"
#include <iostream>
#include <string.h>
#include "MostrarVersiones.hpp"
#include "NavegacionVersiones.hpp"

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
    return InsertarLineaModulo(a, version, contenidoLinea, nroLinea, error);
}                               

TipoRet BorrarLinea(Archivo& a, char* version, unsigned int nroLinea, char* error) {
    return BorrarLineaModulo(a, version, nroLinea, error);
}

TipoRet MostrarTexto(Archivo a, char* version) {
     return MostrarTextoModulo(a, version);
}

// Funciones no implementadas
TipoRet MostrarCambios(Archivo a, char* version) { 
    return MostrarCambiosModulo(a, version);
}


TipoRet Iguales(Archivo a, char* version1, char* version2, bool& iguales) {
    if (a == NULL) {
        return ERROR;
    }

    // Usar la función existente buscarVersionRecursiva
    version_struct* v1 = buscarVersionRecursiva(a->primeraVersion, version1);
    version_struct* v2 = buscarVersionRecursiva(a->primeraVersion, version2);
    
    if (v1 == NULL || v2 == NULL) {
        return ERROR;
    }

    // Obtener los textos de ambas versiones
    texto* texto1 = v1->textoVersion;
    texto* texto2 = v2->textoVersion;

    // Caso 1: Ambos textos son NULL o vacíos
    if ((texto1 == NULL || texto1->primeralineas == NULL) && 
        (texto2 == NULL || texto2->primeralineas == NULL)) {
        iguales = true;
        return OK;
    }

    // Caso 2: Solo uno de los textos es NULL o vacío
    if ((texto1 == NULL || texto1->primeralineas == NULL) || 
        (texto2 == NULL || texto2->primeralineas == NULL)) {
        iguales = false;
        return OK;
    }

    // Caso 3: Diferente cantidad de líneas
    if (texto1->cantidadLineas != texto2->cantidadLineas) {
        iguales = false;
        return OK;
    }

    // Caso 4: Comparar línea por línea
    linea* linea1 = texto1->primeralineas;
    linea* linea2 = texto2->primeralineas;
    
    iguales = true; // Asumimos que son iguales hasta encontrar una diferencia
    
    while (linea1 != NULL && linea2 != NULL) {
        if (strcmp(linea1->contenido, linea2->contenido) != 0) {
            iguales = false;
            break;
        }
        linea1 = linea1->sig;
        linea2 = linea2->sig;
    }

    return OK;
}

TipoRet VersionIndependiente(Archivo& a, char* version) { 
    (void)a; (void)version;
    return NO_IMPLEMENTADA; 
}