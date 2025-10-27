#ifndef ESTRUCTURAS_COMUNES_H
#define ESTRUCTURAS_COMUNES_H

#include "definiciones.hpp"

// estructura para una línea de texto
struct linea {
    char* contenido;
    linea* sig;
};

// estructura para el texto de una versión
struct texto {
    linea* primeralineas;
    unsigned int cantidadLineas;
};

// Estructura para una versión
struct version_struct {
    char* numero;
    texto* textoVersion;
    version_struct* padre;
    version_struct* primeraSubversion;
    version_struct* sigVersion;
};

#endif