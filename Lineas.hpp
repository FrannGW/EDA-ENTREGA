#ifndef LINEAS_H
#define LINEAS_H

#include "definiciones.hpp"
#include "estructuras_comunes.hpp"
#include "archivo.hpp"

// Operaciones sobre líneas de texto
TipoRet InsertarLineaModulo(Archivo& a, char* version, char* contenidoLinea, unsigned int nroLinea, char* error);
TipoRet BorrarLineaModulo(Archivo& a, char* version, unsigned int nroLinea, char* error);
TipoRet MostrarTextoModulo(Archivo a, char* version);

#endif