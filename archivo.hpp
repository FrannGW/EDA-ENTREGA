#ifndef ARCHIVO_H
#define ARCHIVO_H

#include "definiciones.hpp"
#include "estructuras_comunes.hpp"
#include "version.hpp"


// struct principal del archivo con sus versiones
struct nodo_archivo {
    char* nombre;                   // nombre del archivo
    version_struct* primeraVersion; // raíz del árbol de versiones
};

typedef struct nodo_archivo* Archivo;

// Operaciones básicas del archivo
Archivo CrearArchivo(char* nombre);
TipoRet BorrarArchivo(Archivo& a);

// operaciones de versionado
TipoRet CrearVersion(Archivo& a, char* version, char* error);
TipoRet BorrarVersion(Archivo& a, char* version);
TipoRet MostrarVersiones(Archivo a);

// operaciones de texto (parcialmente implementadas)
TipoRet InsertarLinea(Archivo& a, char* version, char* linea, unsigned int nroLinea, char* error);
TipoRet BorrarLinea(Archivo& a, char* version, unsigned int nroLinea, char* error);
TipoRet MostrarTexto(Archivo a, char* version);

// operaciones no implementadas (para futuro)
TipoRet MostrarCambios(Archivo a, char* version);
TipoRet Iguales(Archivo a, char* version1, char* version2, bool& iguales);
TipoRet VersionIndependiente(Archivo& a, char* version);

#endif