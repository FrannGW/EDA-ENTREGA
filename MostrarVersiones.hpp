#ifndef MOSTRAR_VERSIONES_H
#define MOSTRAR_VERSIONES_H

#include "estructuras_comunes.hpp"
#include "archivo.hpp"

// Mostrar versiones
void mostrarVersionesRecursivo(version_struct* version, int nivel);
TipoRet mostrarVersionesModulo(Archivo a);

#endif