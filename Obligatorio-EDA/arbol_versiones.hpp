#ifndef ARBOL_VERSIONES_H
#define ARBOL_VERSIONES_H

#include "version.hpp"
#include <vector>

using namespace std;

// Declaraciones de funciones
version_struct* buscarVersionRecursiva(version_struct* raiz, char* numeroVersion);
version_struct* buscarVersion(version_struct* raiz, char* numeroVersion);
version_struct* buscarUltimaVersion(version_struct* raiz);
char* obtenerPadre(char* version);
int obtenerNumeroSubversion(char* version);
void obtenerTodasHermanas(version_struct* padre, vector<version_struct*>& hermanas);
void renumerarVersionCompleta(version_struct* version, char* nuevoNumeroPadre);
void correrVersionesHermanas(version_struct* padre, int numeroDesde);
void mostrarVersionesRecursivo(version_struct* version, int nivel);
void eliminarVersionRecursiva(version_struct* version);

#endif