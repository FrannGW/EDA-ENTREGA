#ifndef ELIMINACION_VERSIONES_H
#define ELIMINACION_VERSIONES_H

#include "estructuras_comunes.hpp"
#include "archivo.hpp"

// Eliminación de versiones
void eliminarVersionRecursiva(version_struct* version);
TipoRet borrarVersionModulo(Archivo& a, char* version, char* error);

#endif