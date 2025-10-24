#ifndef VERSION_H
#define VERSION_H

#include "texto.hpp"

struct version_struct {
    char* numero;
    texto* textoVersion;
    version_struct* padre;
    version_struct* primeraSubversion;
    version_struct* sigVersion;
};

version_struct* crearVersionSimple(char* numero);
version_struct* buscarVersion(version_struct* raiz, char* numeroVersion);
version_struct* buscarUltimaVersion(version_struct* raiz);
bool validarVersion(char* version, char* error);

#endif