#include "Lineas.hpp"
#include "NavegacionVersiones.hpp"
#include <string.h>
#include <iostream>

using namespace std;

// PRE: 
//   - a es un archivo válido creado con CrearArchivo
//   - version existe en el archivo y es editable (sin subversiones)
//   - contenidoLinea no es NULL ni vacío
//   - nroLinea entre 1 y (cantidad_líneas + 1)
//   - error es buffer para mensajes de error
// POST:
//   - Inserta la línea en la posición especificada
//   - Las líneas existentes se desplazan hacia abajo
//   - Retorna OK si éxito, ERROR (con mensaje en buffer error) si falla
TipoRet InsertarLineaModulo(Archivo& a, char* version, char* contenidoLinea, unsigned int nroLinea, char* error) {
    // Validar archivo existente
    if (a == NULL) {
        strcpy(error, "Archivo no existe");
        return ERROR;
    }

    // Buscar la versión especificada
    version_struct* versionEncontrada = buscarVersionRecursiva(a->primeraVersion, version);
    if (versionEncontrada == NULL) {
        strcpy(error, "Version no existe");
        return ERROR;
    }

    // Validar que la versión no tenga subversiones (no editable según letra)
    if (versionEncontrada->primeraSubversion != NULL) {
        strcpy(error, "No se puede editar version con subversiones");
        return ERROR;
    }

    // Validar contenido de línea no vacío
    if (contenidoLinea == NULL || strlen(contenidoLinea) == 0) {
        strcpy(error, "Linea Vacia");
        return ERROR;
    }

    // Validar número de línea mínimo
    if (nroLinea < 1) {
        strcpy(error, "Numero de linea invalido");
        return ERROR;
    }

    // Inicializar estructura de texto si no existe
    if (versionEncontrada->textoVersion == NULL) {
        versionEncontrada->textoVersion = new texto;
        versionEncontrada->textoVersion->primeralineas = NULL;
        versionEncontrada->textoVersion->cantidadLineas = 0;
    }

    texto* txt = versionEncontrada->textoVersion;

    // Validar rango del número de línea (1 a n+1 según letra)
    if (nroLinea > txt->cantidadLineas + 1) {
        strcpy(error, "Número de línea fuera de rango");
        return ERROR;
    }

    // Crear nueva línea
    linea* nuevaLinea = new linea;
    nuevaLinea->contenido = new char[strlen(contenidoLinea) + 1];
    strcpy(nuevaLinea->contenido, contenidoLinea);
    nuevaLinea->sig = NULL;

    // Caso 1: Insertar al principio (nroLinea == 1)
    if (nroLinea == 1) {
        nuevaLinea->sig = txt->primeralineas;
        txt->primeralineas = nuevaLinea;
    } 
    // Caso 2: Insertar al final (nroLinea == cantidadLineas + 1)
    else if (nroLinea == txt->cantidadLineas + 1) {
        linea* actual = txt->primeralineas;
        // Avanzar hasta la última línea
        while (actual != NULL && actual->sig != NULL) {
            actual = actual->sig;
        }
        if (actual == NULL) {
            // Lista vacía - este caso no debería pasar por la validación anterior
            txt->primeralineas = nuevaLinea;
        } else {
            actual->sig = nuevaLinea;
        }
    }
    // Caso 3: Insertar en medio
    else {
        linea* actual = txt->primeralineas;
        // Avanzar hasta la posición anterior a donde insertaremos
        for (unsigned int i = 1; i < nroLinea - 1; i++) {
            actual = actual->sig;
        }
        nuevaLinea->sig = actual->sig;
        actual->sig = nuevaLinea;
    }

    // Actualizar contador de líneas
    txt->cantidadLineas++;
    return OK;
}

// PRE:
//   - a es un archivo válido creado con CrearArchivo
//   - version existe en el archivo y es editable (sin subversiones)
//   - nroLinea entre 1 y cantidad_líneas
//   - error es buffer para mensajes de error
// POST:
//   - Elimina la línea en la posición especificada
//   - Las líneas siguientes se desplazan hacia arriba
//   - Retorna OK si éxito, ERROR (con mensaje en buffer error) si falla
TipoRet BorrarLineaModulo(Archivo& a, char* version, unsigned int nroLinea, char* error) {
    // Validar archivo existente
    if (a == NULL) {
        strcpy(error, "Archivo no existe");
        return ERROR;
    }

    // Buscar la versión especificada
    version_struct* versionEncontrada = buscarVersionRecursiva(a->primeraVersion, version);
    if (versionEncontrada == NULL) {
        strcpy(error, "Version no existe");
        return ERROR;
    }

    // Validar que la versión no tenga subversiones (no editable según letra)
    if (versionEncontrada->primeraSubversion != NULL) {
        strcpy(error, "No se puede editar version con subversiones");
        return ERROR;
    }

    // Validar que exista texto en la versión
    if (versionEncontrada->textoVersion == NULL || 
        versionEncontrada->textoVersion->primeralineas == NULL) {
        strcpy(error, "El texto de la version esta vacio");
        return ERROR;
    }

    texto* txt = versionEncontrada->textoVersion;

    // Validar rango del número de línea (1 a n según letra)
    if (nroLinea < 1 || nroLinea > txt->cantidadLineas) {
        strcpy(error, "Número de línea fuera de rango");
        return ERROR;
    }

    linea* actual = txt->primeralineas;
    linea* anterior = NULL;

    // Caso 1: Eliminar primera línea
    if (nroLinea == 1) {
        txt->primeralineas = actual->sig;
        delete[] actual->contenido;
        delete actual;
    }
    // Caso 2: Eliminar línea del medio o final
    else {
        // Avanzar hasta la línea a eliminar
        for (unsigned int i = 1; i < nroLinea; i++) {
            anterior = actual;
            actual = actual->sig;
        }

        // Reorganizar punteros para saltar la línea a eliminar
        if (anterior != NULL) {
            anterior->sig = actual->sig;
        }

        // Liberar memoria de la línea eliminada
        delete[] actual->contenido;
        delete actual;
    }

    // Actualizar contador de líneas
    txt->cantidadLineas--;

    // Si no quedan líneas, limpiar la estructura de texto
    if (txt->cantidadLineas == 0) {
        txt->primeralineas = NULL;
    }

    return OK;
}