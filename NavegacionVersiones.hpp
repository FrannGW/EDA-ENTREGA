#ifndef NAVEGACION_VERSIONES_H
#define NAVEGACION_VERSIONES_H

#include "estructuras_comunes.hpp"

// Búsqueda y navegación
version_struct* buscarVersionRecursiva(version_struct* raiz, char* numeroVersion);
version_struct* buscarVersion(version_struct* raiz, char* numeroVersion);
version_struct* buscarUltimaVersion(version_struct* raiz);
char* obtenerPadre(char* version);
int obtenerNumeroSubversion(char* version);

#endif