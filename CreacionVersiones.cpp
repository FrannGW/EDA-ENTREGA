#include "CreacionVersiones.hpp"
#include "NavegacionVersiones.hpp"
#include "ManipulacionVersiones.hpp"  
#include "version.hpp"
#include <string.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

//crea nueva cersion en el archivo si cumplen las reglas (padre existe y no hay huecos)
// si version existe, corre las hermanas
TipoRet crearVersionModulo(Archivo& a, char* version, char* error) {
    if (a == NULL) {
        strcpy(error, "Archivo no existe");
        return ERROR;
    }

    // Validar formato de versión
    if (!validarVersion(version, error)) {
        return ERROR;
    }

    // caso aparte: version ya existe - correr hermanas
    version_struct* versionExistente = buscarVersionRecursiva(a->primeraVersion, version);
    if (versionExistente != NULL) {
        char* padreStr = obtenerPadre(version);
        int numInsertar = obtenerNumeroSubversion(version);
        
        if (padreStr == NULL) {
            strcpy(error, "No se puede insertar version raiz duplicada");
            return ERROR;
        }
        
        version_struct* padre = buscarVersionRecursiva(a->primeraVersion, padreStr);
        if (padre == NULL) {
            strcpy(error, "Error interno: padre no encontrado");
            delete[] padreStr;
            return ERROR;
        }
        
        // Correr versiones hermanas para hacer espacio
        correrVersionesHermanas(padre, numInsertar);
        delete[] padreStr;
    }

    // Validar existencia del padre (excepto para versiones raíz)
    char* ultimoPunto = strrchr(version, '.');
    if (ultimoPunto != NULL) {
        char versionPadre[100];
        strncpy(versionPadre, version, ultimoPunto - version);
        versionPadre[ultimoPunto - version] = '\0';
        
        version_struct* padre = buscarVersionRecursiva(a->primeraVersion, versionPadre);
        if (padre == NULL) {
            strcpy(error, "Version padre no existe");
            return ERROR;
        }

        // validar que no haya huecos entre versiones hermanas
        int numSubversionActual = atoi(ultimoPunto + 1);
        if (numSubversionActual > 1) {
            for (int i = 1; i < numSubversionActual; i++) {
                char subversionAnterior[100];
                sprintf(subversionAnterior, "%s.%d", versionPadre, i);
                
                if (buscarVersionRecursiva(a->primeraVersion, subversionAnterior) == NULL) {
                    strcpy(error, "Existen huecos en las versiones hermanas");
                    return ERROR;
                }
            }
        }
    }

    // crea e inserta la nueva versión
    version_struct* nuevaVersion = crearVersionSimple(version);

    if (a->primeraVersion == NULL) {
        // primera versión del archivo
        a->primeraVersion = nuevaVersion;
    } else {
        char* ultimoPunto = strrchr(version, '.');
        if (ultimoPunto != NULL) {
            // es subversión - insertar en el padre
            char versionPadre[100];
            strncpy(versionPadre, version, ultimoPunto - version);
            versionPadre[ultimoPunto - version] = '\0';
            
            version_struct* padre = buscarVersionRecursiva(a->primeraVersion, versionPadre);
            if (padre != NULL) {
                // insertar en orden numérico entre las subversiones del padre
                int numNuevo = obtenerNumeroSubversion(version);
                version_struct* actual = padre->primeraSubversion;
                version_struct* anterior = NULL;
                
                while (actual != NULL) {
                    int numActual = obtenerNumeroSubversion(actual->numero);
                    if (numActual >= numNuevo) break;
                    anterior = actual;
                    actual = actual->sigVersion;
                }
                
                if (anterior == NULL) {
                    // insertar al principio
                    nuevaVersion->sigVersion = padre->primeraSubversion;
                    padre->primeraSubversion = nuevaVersion;
                } else {
                    // Insertar en medio
                    anterior->sigVersion = nuevaVersion;
                    nuevaVersion->sigVersion = actual;
                }
                nuevaVersion->padre = padre;

                // HEREDAR TEXTO DEL PADRE - CORRECCIÓN CLAVE
                if (padre->textoVersion != NULL) {
                    // Crear nueva estructura de texto para la subversión
                    nuevaVersion->textoVersion = new texto;
                    nuevaVersion->textoVersion->primeralineas = NULL;
                    nuevaVersion->textoVersion->cantidadLineas = 0;
                    
                    // Copiar todas las líneas del padre
                    linea* actualPadre = padre->textoVersion->primeralineas;
                    linea* ultimaLinea = NULL;
                    
                    while (actualPadre != NULL) {
                        linea* nuevaLinea = new linea;
                        nuevaLinea->contenido = new char[strlen(actualPadre->contenido) + 1];
                        strcpy(nuevaLinea->contenido, actualPadre->contenido);
                        nuevaLinea->sig = NULL;
                        
                        if (nuevaVersion->textoVersion->primeralineas == NULL) {
                            nuevaVersion->textoVersion->primeralineas = nuevaLinea;
                            ultimaLinea = nuevaLinea;
                        } else {
                            ultimaLinea->sig = nuevaLinea;
                            ultimaLinea = nuevaLinea;
                        }
                        
                        nuevaVersion->textoVersion->cantidadLineas++;
                        actualPadre = actualPadre->sig;
                    }
                }
            }
        } else {
            // Es versión raíz - insertar al final
            version_struct* ultima = buscarUltimaVersion(a->primeraVersion);
            if (ultima != NULL) {
                ultima->sigVersion = nuevaVersion;
            } else {
                a->primeraVersion = nuevaVersion;
            }
        }
    }

    return OK;
}