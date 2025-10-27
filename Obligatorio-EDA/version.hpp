#ifndef VERSION_H
#define VERSION_H

#include "estructuras_comunes.hpp"  // ← Esta línea es IMPORTANTE

// Solo declaraciones de funciones - NO la estructura version_struct
version_struct* crearVersionSimple(char* numero);
bool validarVersion(char* version, char* error);

#endif