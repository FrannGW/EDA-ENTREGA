// Estructuras de Datos y Algoritmos - Obligatorio 2025
// Tecnologo en Informatica FIng - DGETP - UTEC
//
// archivo.cpp
// Modulo de implementacion de archivo.

#include "archivo.hpp"
#include <iostream>
#include <string.h>
#include "version.hpp"
#include "arbol_versiones.hpp"

using namespace std;

#define NO_IMPLEMENTADA_FUNC ((void)0, NO_IMPLEMENTADA)



Archivo CrearArchivo(char* nombre)
{
    // Verificar que el nombre no sea NULL
    if (nombre == NULL) {
        return NULL;
    }

    Archivo a = new nodo_archivo;

    if (a == NULL) {
        return NULL;
    }

    // Asignar memoria para el nombre e inicializar a cero
    a->nombre = new char[MAX_NOMBRE]();  // Los paréntesis inicializan a cero
    
    // Copiar el nombre asegurando la terminación nula
    strncpy(a->nombre, nombre, MAX_NOMBRE - 1);
    a->nombre[MAX_NOMBRE - 1] = '\0';  // Garantizar terminación nula
    
    // Inicializar el árbol de versiones
    a->primeraVersion = NULL;

    return a;
}

/* re: a no es NULL.
 * pos: Se libera toda la memoria asociada al Archivo y se asigna NULL. */
TipoRet BorrarArchivo(Archivo& a)
{
    // Elimina toda la memoria utilizada por el archivo y asigna NULL al puntero a.
    // Se asume como precondición que a referencia a un archivo (en particular a es distinto a NULL).
    // Esta operación se ejecuta al final de una sesión de trabajo con un archivo.

    // 1. verificar si el archivo es NULL (precondición fallida)
    if (a == NULL) {
        // nota: El mensaje de error lo imprime main.c si retorna ERROR.
        return ERROR;
    }

    // 2. Liberar memoria del árbol de versiones (DEBE implementarse a futuro)
    // ssumiendo que existe void liberar_versiones(struct nodo_version *&raiz)
    // liberar_versiones(a->raiz_versiones);

    // 3. kiberar el nodo raíz del Archivo
    free(a);
    a = NULL;

    // 4. devolver OK. main.c se encarga de imprimir el mensaje de éxito.
    return OK;
}


TipoRet CrearVersion(Archivo& a, char* version, char* error) {
    if (a == NULL) {
        strcpy(error, "Archivo no existe");
        return ERROR;
    }

    // Validar formato básico de la versión
    if (!validarVersion(version, error)) {
        return ERROR;
    }

    // Verificar si la versión ya existe
    version_struct* versionExistente = buscarVersionRecursiva(a->primeraVersion, version);
    
    if (versionExistente != NULL) {
        // CASO: La versión ya existe - hay que correr hermanas
        char* padreStr = obtenerPadre(version);
        int numInsertar = obtenerNumeroSubversion(version);
        
        if (padreStr == NULL) {
            strcpy(error, "No se puede insertar version raiz duplicada");
            return ERROR;
        }
        
        // Buscar el padre
        version_struct* padre = buscarVersionRecursiva(a->primeraVersion, padreStr);
        if (padre == NULL) {
            strcpy(error, "Error interno: padre no encontrado");
            delete[] padreStr;
            return ERROR;
        }
        
        // CORRER VERSIONES HERMANAS
        correrVersionesHermanas(padre, numInsertar);
        
        delete[] padreStr;
        
        // Después de correr, verificar nuevamente si la versión existe
        versionExistente = buscarVersionRecursiva(a->primeraVersion, version);
        if (versionExistente != NULL) {
            strcpy(error, "La version sigue existiendo despues de correr hermanas");
            return ERROR;
        }
    }

    // Resto de validaciones (padre y huecos)...
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

        // Validación de huecos
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

    // Crear nueva versión
    version_struct* nuevaVersion = crearVersionSimple(version);

    // Insertar en la posición correcta
    if (a->primeraVersion == NULL) {
        a->primeraVersion = nuevaVersion;
    } else {
        char* ultimoPunto = strrchr(version, '.');
        if (ultimoPunto != NULL) {
            char versionPadre[100];
            strncpy(versionPadre, version, ultimoPunto - version);
            versionPadre[ultimoPunto - version] = '\0';
            
            version_struct* padre = buscarVersionRecursiva(a->primeraVersion, versionPadre);
            if (padre != NULL) {
                // Insertar en orden numérico en las subversiones del padre
                int numNuevo = obtenerNumeroSubversion(version);
                version_struct* actual = padre->primeraSubversion;
                version_struct* anterior = NULL;
                
                while (actual != NULL) {
                    int numActual = obtenerNumeroSubversion(actual->numero);
                    if (numActual >= numNuevo) {
                        break;
                    }
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
    

    TipoRet BorrarVersion(Archivo& a, char* version)
{
    if (a == NULL) {
        return ERROR;
    }

    // Buscar la versión a eliminar
    version_struct* versionEliminar = buscarVersionRecursiva(a->primeraVersion, version);
    if (versionEliminar == NULL) {
        return ERROR; // La versión no existe
    }

    // Caso especial: si es la primera versión raíz
    if (versionEliminar->padre == NULL) {
        // Buscar la versión anterior en la lista de raíces
        version_struct* actual = a->primeraVersion;
        version_struct* anterior = NULL;
        
        while (actual != NULL && actual != versionEliminar) {
            anterior = actual;
            actual = actual->sigVersion;
        }
        
        if (anterior == NULL) {
            // Es la primera versión raíz
            a->primeraVersion = versionEliminar->sigVersion;
        } else {
            // Es una versión raíz pero no la primera
            anterior->sigVersion = versionEliminar->sigVersion;
        }
    } else {
        // Es una subversión, buscar en la lista del padre
        version_struct* padre = versionEliminar->padre;
        version_struct* actual = padre->primeraSubversion;
        version_struct* anterior = NULL;
        
        while (actual != NULL && actual != versionEliminar) {
            anterior = actual;
            actual = actual->sigVersion;
        }
        
        if (anterior == NULL) {
            // Es la primera subversión
            padre->primeraSubversion = versionEliminar->sigVersion;
        } else {
            // Es una subversión pero no la primera
            anterior->sigVersion = versionEliminar->sigVersion;
        }
    }

    // Eliminar la versión y todas sus subversiones recursivamente
    eliminarVersionRecursiva(versionEliminar);

    return OK;
}



TipoRet MostrarVersiones(Archivo a) {
    if (a == NULL) {
        cout << "ERROR: Archivo es NULL" << endl;
        return ERROR;
    }
    
    cout << a->nombre << endl;
    cout << endl;
    
    if (a->primeraVersion == NULL) {
        cout << "No hay versiones creadas" << endl;
        return OK;
    }
    
    // ✅ USAR la función recursiva del módulo arbol_versiones
    mostrarVersionesRecursivo(a->primeraVersion, 0);
    
    return OK;
}
TipoRet InsertarLinea(Archivo& a, char* version, char* contenidoLinea, unsigned int nroLinea, char* error)
{
    // Esta función inserta una linea de texto a la version parámetro en la posición nroLinea.
    // El número de línea debe estar entre 1 y n+1, siendo n la cantidad de líneas del archivo. Por ejemplo, si el texto
    // tiene 7 líneas, se podráinsertar en las posiciones 1 (al comienzo) a 8 (al final). Si se inserta en un número de
    // línea existente, ésta y las siguientes líneas se correrán hacia adelante (abajo) dejando el espacio para la nueva
    // línea. No se puede insertar una línea en una versión que tenga subversiones. Notar que el crear un archivo, éste
    // no es editable hasta que no se crea al menos una versión del mismo. Sólo las versiones de un archivo son
    // editables (se pueden insertar o suprimir líneas), siempre que no tengan subversiones creadas. En caso que TipoRet
    // sea ERROR, en error se debe cargar cuál es el mismo.

    // Notas para la implementación futura:
    // 1. Comprobar que la version exista sino retornar ERROR
    // 2. Comprobar que la version si existe no tenga subversiones, sino retornar ERROR
    // 3. Comprobar que nroLinea esté entre 1 y n+1, siendo n la cantidad de líneas del archivo. No puede ser menor a 1.

    if (a == NULL) {
        strcpy(error, "Archivo no existe");
        return ERROR;
    }

    if (contenidoLinea == NULL || strlen(contenidoLinea) == 0){
            strcpy(error, "Linea Vacia");
            return ERROR;
    }
    
    if (nroLinea < 1){
            strcpy(error, "Version no existe");
            return ERROR;
    }
    
    version_struct* versionEncontrada = buscarVersion(a->primeraVersion, version);

    if (versionEncontrada == NULL) {
            strcpy(error, "Version no existe");
            return ERROR;
    }

     if (versionEncontrada->primeraSubversion != NULL){
        strcpy(error, "No se puede editar veresion con subversiones");
        return ERROR;
     }
    
    if (versionEncontrada->textoVersion == NULL) {
       versionEncontrada->textoVersion= new texto;
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

TipoRet BorrarLinea(Archivo& a, char* version, unsigned int nroLinea, char* error)
{
    // Esta función elimina una línea de texto de la version del archivo a en la posición nroLinea.
    // El número de línea debe estar entre 1 y n, siendo n la cantidad de líneas del archivo. Por ejemplo, si el texto
    // tiene 7 líneas, se podrán eliminar líneas de las posiciones 1 a 7. Cuando se elimina una línea, las siguientes
    // líneas se corren, decrementando en una unidad sus posiciones para ocupar el lugar de la línea borrada. No se
    // puede borrar una línea de una versión que tenga subversiones creadas. En caso que TipoRet sea ERROR, en error se
    // debe cargar cuál es el mismo.

    (void)a; (void)version; (void)nroLinea; (void)error;
    return NO_IMPLEMENTADA;
}

TipoRet MostrarTexto(Archivo a, char* version)
{
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

TipoRet MostrarCambios(Archivo a, char* version)
{
    // Esta función muestra los cambios que se realizaron en el texto de la version parámetro, sin incluir los cambios
    // realizados en las versiones ancestras de la cual dicha versión depende.

    (void)a; (void)version;
    return NO_IMPLEMENTADA;
}

TipoRet Iguales(Archivo a, char* version1, char* version2, bool& iguales)
{
    // Esta función asigna al parámetro booleano (iguales) el valor true si ambas versiones (version1 y version2) del
    // archivo tienen exactamente el mismo texto, y false en caso contrario.

    (void)a; (void)version1; (void)version2; (void)iguales;
    return NO_IMPLEMENTADA;
}

TipoRet VersionIndependiente(Archivo& a, char* version)
{
    // Esta función crea una nueva versión al final del primer nivel con todos los cambios de la version especificada y
    // de sus versiones ancestras. La versión que se crea debe ser independiente de cualquier otra versión. Por ejemplo,
    // si creamos una versión independiente a partir de la 2.11.3, entonces se crea una nueva versión al final del
    // primer nivel (si existen las versiones 1, 2, 3 y 4, entonces se crea la 5) con los cambios realizados a las
    // versiones 2, 2.11 y 2.11.3.

    (void)a; (void)version;
    return NO_IMPLEMENTADA;
}