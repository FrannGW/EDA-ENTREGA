#ifndef NAVEGACION_VERSIONES_H
#define NAVEGACION_VERSIONES_H

#include "estructuras_comunes.hpp"
#include "archivo.hpp"

// Declaración forward para Archivo
struct nodo_archivo;
typedef struct nodo_archivo* Archivo;

// Búsqueda y navegación
version_struct* buscarVersionRecursiva(version_struct* raiz, char* numeroVersion);
version_struct* buscarVersion(version_struct* raiz, char* numeroVersion);
version_struct* buscarUltimaVersion(version_struct* raiz);
char* obtenerPadre(char* version);
int obtenerNumeroSubversion(char* version);

// Comparación de versiones
TipoRet IgualesModulo(Archivo a, char* version1, char* version2, bool& iguales);

TipoRet mostrarVersionesModulo(Archivo a);

#endif