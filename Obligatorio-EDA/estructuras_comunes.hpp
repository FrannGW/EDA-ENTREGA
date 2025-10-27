#ifndef ESTRUCTURAS_COMUNES_H
#define ESTRUCTURAS_COMUNES_H

#include "texto.hpp"

// Definir la estructura aquí para que todos la vean
struct version_struct {
    char* numero;
    texto* textoVersion;
    version_struct* padre;
    version_struct* primeraSubversion;
    version_struct* sigVersion;
};

#endif