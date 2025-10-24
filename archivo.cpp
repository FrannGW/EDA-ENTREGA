// Estructuras de Datos y Algoritmos - Obligatorio 2025
// Tecnologo en Informatica FIng - DGETP - UTEC
//
// archivo.cpp
// Modulo de implementacion de archivo.

#include "archivo.hpp"
#include <iostream>
#include <string.h>
#include "version.hpp"

using namespace std;

#define NO_IMPLEMENTADA_FUNC ((void)0, NO_IMPLEMENTADA)


Archivo CrearArchivo(char* nombre)
{
    // Crea el archivo con el nombre especificado y lo inicializa sin contenido (vacío).
    // El archivo creado es retornado.
    // Esta operación se ejecuta al inicio de una sesión de trabajo con un archivo.

    Archivo a = new (nodo_archivo);

    if (a == NULL) {
        return NULL; // en caso de error
    }

    a->nombre = new (char[MAX_NOMBRE]);
    strncpy(a->nombre, nombre, MAX_NOMBRE - 1);
    a->nombre[MAX_NOMBRE - 1] = '\0';
    // a->v = NULL;

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

    // Validar si la versión ya existe
    if (buscarVersion(a->primeraVersion, version) != NULL) {
        strcpy(error, "Version ya existe");
        return ERROR;
    }

    char* ultimoPunto = strrchr(version, '.');
    if (ultimoPunto != NULL) {
        // Es una subversión (tiene padre)
        char versionPadre[100];
        strncpy(versionPadre, version, ultimoPunto - version);
        versionPadre[ultimoPunto - version] = '\0';
        
        // Verificar que el padre exista
        version_struct* padre = buscarVersion(a->primeraVersion, versionPadre);
        if (padre == NULL) {
            strcpy(error, "Version padre no existe");
            return ERROR;
        }

        // 🔥 NUEVA VALIDACIÓN: Verificar que no haya huecos
        // Extraer el número de la subversión actual (después del último punto)
        int numSubversionActual = atoi(ultimoPunto + 1);
        
        // Verificar que todas las subversiones anteriores existan
        for (int i = 1; i < numSubversionActual; i++) {
            char subversionAnterior[100];
            sprintf(subversionAnterior, "%s.%d", versionPadre, i);
            
            if (buscarVersion(a->primeraVersion, subversionAnterior) == NULL) {
                strcpy(error, "Existen huecos en las versiones hermanas");
                return ERROR;
            }
        }
    }

    // Crear nueva versión
    version_struct* nuevaVersion = crearVersionSimple(version);

    // Insertar al final de la lista
    if (a->primeraVersion == NULL) {
        a->primeraVersion = nuevaVersion;
    } else {
        version_struct* ultima = buscarUltimaVersion(a->primeraVersion);
        ultima->sigVersion = nuevaVersion;
    }

    return OK;
}

TipoRet BorrarVersion(Archivo& a, char* version)
{
    // Elimina una versión del archivo si la version pasada por parámetro existe. En otro caso la operación quedará sin
    // efecto. Si la versión a eliminar posee subversiones, éstas deberán ser eliminadas también, así como el texto
    // asociado a cada una de las versiones. No deben quedar números de versiones libres sin usar. Por lo tanto cuando
    // se elimina una versión, las versiones hermanas que le siguen deben decrementar su numeración (así también sus
    // subversiones dependientes). Por ejemplo, si existen las versiones 2.15.1, 2.15.2 y 2.15.3, y elimino la 2.15.1,
    // la versión 2.15.2 y la 2.15.3 pasan a ser 2.15.1 y 2.15.2 respectivamente, esto incluye a todas las subversiones
    // de estas versiones.

    (void)a; (void)version;  // ← Elimina warnings
    return NO_IMPLEMENTADA;
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
    
    // Versión SIMPLIFICADA - mostrar lista plana
    version_struct* actual = a->primeraVersion;
    while (actual != NULL) {
        cout << actual->numero << endl;
        actual = actual->sigVersion;
    }
    
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
    // Esta función muestra el texto completo de la version, teniendo en cuenta los cambios realizados en dicha versión
    // y en las versiones ancestras, de la cual ella depende.
    if (a == NULL){
        return ERROR;
    }

    version_struct* versionEncontrada = buscarVersion(a->primeraVersion, version);
    if (versionEncontrada == NULL){
        return ERROR;
    }
    cout << a->nombre << " - " << version << endl;
    cout << endl;


    if (versionEncontrada -> textoVersion == NULL||
        versionEncontrada -> textoVersion -> primeralineas == NULL){
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