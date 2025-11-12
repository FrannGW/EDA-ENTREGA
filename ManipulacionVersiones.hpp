ManipulacionVersiones.hpp:
#ifndef MANIPULACION_VERSIONES_H
#define MANIPULACION_VERSIONES_H

#include "estructuras_comunes.hpp"


// Manipulación de versiones
void renumerarVersionCompleta(version_struct* version, char* nuevoNumeroPadre);
void correrVersionesHermanas(version_struct* padre, int numeroDesde);

#endif