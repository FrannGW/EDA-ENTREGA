#include "archivo.hpp"
#include <iostream>
#include <string.h>

using namespace std;

//crea archivo y le asigna un nombre
Archivo CrearArchivo(char* nombre) {
    if (nombre == NULL) {
        return NULL;
    }

    
    Archivo a = new nodo_archivo;
    if (a == NULL) {
        return NULL;
    }

    // Asignar y copiar nombre
    a->nombre = new char[strlen(nombre) + 1];
    strcpy(a->nombre, nombre);
    
    // Inicializar sin versiones
    a->primeraVersion = NULL;

    return a;
}

// elmina archivo y libera memoria
TipoRet BorrarArchivo(Archivo& a) {
    if (a == NULL) {
        return ERROR;
    }

    // Liberar todas las versiones raíz recursivamente
    version_struct* versionActual = a->primeraVersion;
    while (versionActual != NULL) {
        version_struct* siguiente = versionActual->sigVersion;
        eliminarVersionRecursiva(versionActual);
        versionActual = siguiente;
    }

    // Liberar nombre y estructura principal
    delete[] a->nombre;
    delete a;
    a = NULL;

    return OK;
}

//creacion de nueva version
TipoRet CrearVersion(Archivo& a, char* version, char* error) {
    if (a == NULL) {
        strcpy(error, "Archivo no existe");
        return ERROR;
    }

    // Validar formato de versión
    if (!validarVersion(version, error)) {
        return ERROR;
    }

    // caso aparte: Versión ya existe - correr hermanas
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

        // Validar que no haya huecos entre versiones hermanas
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
        // Primera versión del archivo
        a->primeraVersion = nuevaVersion;
    } else {
        char* ultimoPunto = strrchr(version, '.');
        if (ultimoPunto != NULL) {
            // Es subversión - insertar en el padre
            char versionPadre[100];
            strncpy(versionPadre, version, ultimoPunto - version);
            versionPadre[ultimoPunto - version] = '\0';
            
            version_struct* padre = buscarVersionRecursiva(a->primeraVersion, versionPadre);
            if (padre != NULL) {
                // Insertar en orden numérico entre las subversiones del padre
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
                    // Insertar al principio
                    nuevaVersion->sigVersion = padre->primeraSubversion;
                    padre->primeraSubversion = nuevaVersion;
                } else {
                    // Insertar en medio
                    anterior->sigVersion = nuevaVersion;
                    nuevaVersion->sigVersion = actual;
                }
                nuevaVersion->padre = padre;
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

//elminamos version y subversiones del padre
TipoRet BorrarVersion(Archivo& a, char* version) {
    if (a == NULL) {
        return ERROR;
    }

    version_struct* versionEliminar = buscarVersionRecursiva(a->primeraVersion, version);
    if (versionEliminar == NULL) {
        return ERROR;
    }

    // Reorganizar punteros del padre/hermanos
    if (versionEliminar->padre == NULL) {
        // Es versión raíz
        version_struct* actual = a->primeraVersion;
        version_struct* anterior = NULL;
        
        while (actual != NULL && actual != versionEliminar) {
            anterior = actual;
            actual = actual->sigVersion;
        }
        
        if (anterior == NULL) {
            a->primeraVersion = versionEliminar->sigVersion;
        } else {
            anterior->sigVersion = versionEliminar->sigVersion;
        }
    } else {
        // ss subversión
        version_struct* padre = versionEliminar->padre;
        version_struct* actual = padre->primeraSubversion;
        version_struct* anterior = NULL;
        
        while (actual != NULL && actual != versionEliminar) {
            anterior = actual;
            actual = actual->sigVersion;
        }
        
        if (anterior == NULL) {
            padre->primeraSubversion = versionEliminar->sigVersion;
        } else {
            anterior->sigVersion = versionEliminar->sigVersion;
        }
    }

    // Eliminar recursivamente
    eliminarVersionRecursiva(versionEliminar);
    return OK;
}

//mostramos versiones de forma jerarquica siguiendo la letra
TipoRet MostrarVersiones(Archivo a) {
    if (a == NULL) {
        cout << "ERROR: Archivo es NULL" << endl;
        return ERROR;
    }
    
    cout << a->nombre << endl << endl;
    
    if (a->primeraVersion == NULL) {
        cout << "No hay versiones creadas" << endl;
    } else {
        mostrarVersionesRecursivo(a->primeraVersion, 0);
    }
    
    return OK;
}

// Funciones de texto (implementadas parcialmente)
// COMENTAR AL PROFE QUE ESTO NO ESTA IMPLEMENTADO                                       
TipoRet InsertarLinea(Archivo& a, char* version, char* contenidoLinea, unsigned int nroLinea, char* error) {
    if (a == NULL) {
        strcpy(error, "Archivo no existe");
        return ERROR;
    }

    if (contenidoLinea == NULL || strlen(contenidoLinea) == 0){
        strcpy(error, "Linea Vacia");
        return ERROR;
    }
    
    if (nroLinea < 1){
        strcpy(error, "Numero de linea invalido");
        return ERROR;
    }
    
    version_struct* versionEncontrada = buscarVersion(a->primeraVersion, version);
    if (versionEncontrada == NULL) {
        strcpy(error, "Version no existe");
        return ERROR;
    }

    if (versionEncontrada->primeraSubversion != NULL){
        strcpy(error, "No se puede editar version con subversiones");
        return ERROR;
    }
    
    if (versionEncontrada->textoVersion == NULL) {
        versionEncontrada->textoVersion = new texto;
        versionEncontrada->textoVersion->primeralineas = NULL;
        versionEncontrada->textoVersion->cantidadLineas = 0;
    }
    
    texto* txt = versionEncontrada->textoVersion;

    if (nroLinea > txt->cantidadLineas + 1) {
        strcpy(error, "Número de línea fuera de rango");
        return ERROR;
    }

    linea* nuevaLinea = new linea;
    nuevaLinea->contenido = new char[strlen(contenidoLinea) + 1];
    strcpy(nuevaLinea->contenido, contenidoLinea);
    nuevaLinea->sig = NULL;

    if (nroLinea == 1) {
        // Insertar al principio
        nuevaLinea->sig = txt->primeralineas;
        txt->primeralineas = nuevaLinea;
    } else {
        // Insertar en medio o al final
        linea* actual = txt->primeralineas;
        for (unsigned int i = 1; i < nroLinea - 1; i++) {
            actual = actual->sig;
        }
        nuevaLinea->sig = actual->sig;
        actual->sig = nuevaLinea;
    }

    txt->cantidadLineas++;
    return OK;
}                               

TipoRet BorrarLinea(Archivo& a, char* version, unsigned int nroLinea, char* error) {
    // TODO: Implementar eliminación de línea
    (void)a; (void)version; (void)nroLinea; (void)error; // Al compilar generaba warnings, pusimos este void para evitarlos en el codigo
    return NO_IMPLEMENTADA;
}

TipoRet MostrarTexto(Archivo a, char* version) {
    if (a == NULL){
        return ERROR;
    }

    version_struct* versionEncontrada = buscarVersion(a->primeraVersion, version);
    if (versionEncontrada == NULL){
        return ERROR;
    }
    
    cout << a->nombre << " - " << version << endl;
    cout << endl;

    if (versionEncontrada->textoVersion == NULL || 
        versionEncontrada->textoVersion->primeralineas == NULL) {
        cout << "No contiene lineas" << endl;
        return OK;
    }

    linea* actual = versionEncontrada->textoVersion->primeralineas;
    unsigned int numeroLinea = 1;

    while (actual != NULL){
        cout << numeroLinea << "\t" << actual->contenido << endl;
        actual = actual->sig;
        numeroLinea++;
    }
    
    return OK;
}

// Funciones no implementadas (marcadas explícitamente)

TipoRet MostrarCambios(Archivo a, char* version) { 
    (void)a; (void)version; // Eliminar warnings
    return NO_IMPLEMENTADA; 
}

TipoRet Iguales(Archivo a, char* version1, char* version2, bool& iguales) { 
    (void)a; (void)version1; (void)version2; (void)iguales; // Al compilar generaba warnings, pusimos este void para evitarlos en el codigo
    return NO_IMPLEMENTADA; 
}

TipoRet VersionIndependiente(Archivo& a, char* version) { 
    (void)a; (void)version; // Al compilar generaba warnings, pusimos este void para evitarlos en el codigo
    return NO_IMPLEMENTADA; 
}