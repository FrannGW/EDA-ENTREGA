#ifndef ARBOL_VERSIONES_H
#define ARBOL_VERSIONES_H

#include "version.hpp"
#include <vector>

using namespace std;

/**
 * @file arbol_versiones.h
 * @brief Operaciones sobre la estructura jerárquica de versiones
 */

// Búsqueda y navegación
version_struct* buscarVersionRecursiva(version_struct* raiz, char* numeroVersion);
version_struct* buscarVersion(version_struct* raiz, char* numeroVersion);
version_struct* buscarUltimaVersion(version_struct* raiz);
char* obtenerPadre(char* version);
int obtenerNumeroSubversion(char* version);

// Manipulación de versiones hermanas
void obtenerTodasHermanas(version_struct* padre, vector<version_struct*>& hermanas);
void renumerarVersionCompleta(version_struct* version, char* nuevoNumeroPadre);
void correrVersionesHermanas(version_struct* padre, int numeroDesde);

// Visualización y eliminación
void mostrarVersionesRecursivo(version_struct* version, int nivel);
void eliminarVersionRecursiva(version_struct* version);

#endif