#ifndef ARCHIVO_H
#define ARCHIVO_H

#include "definiciones.hpp"
#include "estructuras_comunes.hpp"
#include "version.hpp"

struct nodo_archivo {
    char* nombre;
    version_struct* primeraVersion;
};

typedef struct nodo_archivo* Archivo;

// Operaciones básicas del archivo
Archivo CrearArchivo(char* nombre);
TipoRet BorrarArchivo(Archivo& a);

// operaciones de versionado
TipoRet CrearVersion(Archivo& a, char* version, char* error);
TipoRet BorrarVersion(Archivo& a, char* version);
TipoRet MostrarVersiones(Archivo a);

// operaciones de texto
TipoRet InsertarLinea(Archivo& a, char* version, char* linea, unsigned int nroLinea, char* error);
TipoRet BorrarLinea(Archivo& a, char* version, unsigned int nroLinea, char* error);
TipoRet MostrarTexto(Archivo a, char* version);

// operaciones no implementadas
TipoRet MostrarCambios(Archivo a, char* version);
TipoRet Iguales(Archivo a, char* version1, char* version2, bool& iguales);
TipoRet VersionIndependiente(Archivo& a, char* version);

#endif