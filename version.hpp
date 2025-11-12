#ifndef VERSION_H
#define VERSION_H

#include "estructuras_comunes.hpp"

// crea una nueva versión con el número especificado
version_struct* crearVersionSimple(char* numero);

// valida que el formato del número de versión sea correcto
bool validarVersion(char* version, char* error);

#endif