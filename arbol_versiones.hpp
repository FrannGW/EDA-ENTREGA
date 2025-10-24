#ifndef ARBOL_VERSIONES_H
#define ARBOL_VERSIONES_H

#include "estructuras_comunes.hpp"  // ← Incluir estructuras

// Funciones de búsqueda en el árbol
version_struct* buscarVersion(version_struct* raiz, char* numeroVersion);
version_struct* buscarUltimaVersion(version_struct* raiz);

// Funciones para manipulación del árbol
char* obtenerPadre(char* version);
int obtenerNumeroSubversion(char* version);
version_struct* crearVersionConNumero(char* versionBase, int nuevoNumero);
void renumerarVersionYSubversiones(version_struct* version, char* nuevoNumero);

// Función para mostrar el árbol jerárquicamente
void mostrarVersionesRecursivo(version_struct* version, int nivel);

#endif