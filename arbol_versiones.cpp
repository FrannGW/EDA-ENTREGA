#include "arbol_versiones.hpp"
#include "version.hpp"
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

using namespace std;

//busca version en todo el arbol de forma recursiva
version_struct* buscarVersionRecursiva(version_struct* raiz, char* numeroVersion) {
    if (raiz == NULL) return NULL;
    
    // Buscar en el nodo actual
    if (strcmp(raiz->numero, numeroVersion) == 0) {
        return raiz;
    }
    
    // buscar en subversiones (hijos)
    version_struct* encontrada = buscarVersionRecursiva(raiz->primeraSubversion, numeroVersion);
    if (encontrada != NULL) return encontrada;
    
    // buscar en hermanos
    return buscarVersionRecursiva(raiz->sigVersion, numeroVersion);
}


 //Busca una versión solo en el nivel actual (no recursivo)
version_struct* buscarVersion(version_struct* raiz, char* numeroVersion) {
    version_struct* actual = raiz;
    while (actual != NULL) {
        if (strcmp(actual->numero, numeroVersion) == 0) {
            return actual;
        }
        actual = actual->sigVersion;
    }
    return NULL;
}

//encuentra ultima version en lista hermana
version_struct* buscarUltimaVersion(version_struct* raiz) {
    if (raiz == NULL) return NULL;
    version_struct* actual = raiz;
    while (actual->sigVersion != NULL) {
        actual = actual->sigVersion;
    }
    return actual;
}


  //Extrae el padre de una versión a partir de su número
  //Nuevo string con el número del padre (debe ser liberado)
 char* obtenerPadre(char* version) {
    char* ultimoPunto = strrchr(version, '.');
    if (ultimoPunto == NULL) return NULL; // es versión raíz
    
    char* padre = new char[ultimoPunto - version + 1];
    strncpy(padre, version, ultimoPunto - version);
    padre[ultimoPunto - version] = '\0';
    return padre;
}


//Obtiene el último número de una versión (subversión)

int obtenerNumeroSubversion(char* version) {
    char* ultimoPunto = strrchr(version, '.');
    if (ultimoPunto == NULL) return atoi(version); // versión raíz
    
    return atoi(ultimoPunto + 1); // número después del último punto
}


//obtiene todas las versiones hermanas de un padre
void obtenerTodasHermanas(version_struct* padre, vector<version_struct*>& hermanas) {
    if (padre == NULL) return;
    
    version_struct* actual = padre->primeraSubversion;
    while (actual != NULL) {
        hermanas.push_back(actual);
        actual = actual->sigVersion;
    }
}


//renumeración completa de una versión y todas sus subversiones
void renumerarVersionCompleta(version_struct* version, char* nuevoNumeroPadre) {
    if (version == NULL) return;
    
    char* numeroAntiguo = version->numero;
    
    // calcular nuevo número para esta versión
    char* ultimoPunto = strrchr(numeroAntiguo, '.');
    char* parteFinal = ultimoPunto + 1;
    
    char nuevoNumero[100];
    if (strcmp(nuevoNumeroPadre, "") == 0) {
        sprintf(nuevoNumero, "%s", parteFinal);
    } else {
        sprintf(nuevoNumero, "%s.%s", nuevoNumeroPadre, parteFinal);
    }
    
    // actualizar número de esta versión
    delete[] version->numero;
    version->numero = new char[strlen(nuevoNumero) + 1];
    strcpy(version->numero, nuevoNumero);
    
    // renumerar recursivamente todas las subversiones
    version_struct* subversion = version->primeraSubversion;
    while (subversion != NULL) {
        renumerarVersionCompleta(subversion, nuevoNumero);
        subversion = subversion->sigVersion;
    }
}

// si se inserta una version hermana duplicada corre a las her,anas
void correrVersionesHermanas(version_struct* padre, int numeroDesde) {
    if (padre == NULL) return;
    
    vector<version_struct*> hermanas;
    obtenerTodasHermanas(padre, hermanas);
    
    // Procesar de mayor a menor para evitar conflictos
    for (int i = hermanas.size() - 1; i >= 0; i--) {
        version_struct* hermana = hermanas[i];
        int numHermana = obtenerNumeroSubversion(hermana->numero);
        
        if (numHermana >= numeroDesde) {
            char nuevoNumeroPadre[100];
            if (padre->numero != NULL) {
                strcpy(nuevoNumeroPadre, padre->numero);
            } else {
                strcpy(nuevoNumeroPadre, "");
            }
            
            char nuevoNumero[100];
            sprintf(nuevoNumero, "%s.%d", nuevoNumeroPadre, numHermana + 1);
            renumerarVersionCompleta(hermana, nuevoNumeroPadre);
            
            delete[] hermana->numero;
            hermana->numero = new char[strlen(nuevoNumero) + 1];
            strcpy(hermana->numero, nuevoNumero);
        }
    }
}

//muestra el arbol jeraraquicamente
void mostrarVersionesRecursivo(version_struct* version, int nivel) {
    if (version == NULL) return;
    
    // Indentación según nivel jerárquico
    for (int i = 0; i < nivel; i++) {
        cout << "\t";
    }
    
    cout << version->numero << endl;
    
    // Mostrar subversiones (aumentar nivel)
    mostrarVersionesRecursivo(version->primeraSubversion, nivel + 1);
    // Mostrar hermanas (mismo nivel)
    mostrarVersionesRecursivo(version->sigVersion, nivel);
}

//elimina una version y subversiones de forma recursiva
void eliminarVersionRecursiva(version_struct* version) {
    if (version == NULL) return;
    
    // eliminamos subversiones todas las subversiones
    version_struct* subversion = version->primeraSubversion;
    while (subversion != NULL) {
        version_struct* siguiente = subversion->sigVersion;
        eliminarVersionRecursiva(subversion);
        subversion = siguiente;
    }
    
                    /*ESTO NO ESTA IMPLEMENTADO, AVISARLE AL PROFE
    if (version->textoVersion != NULL) {
        linea* actualLinea = version->textoVersion->primeralineas;
        while (actualLinea != NULL) {
            linea* siguienteLinea = actualLinea->sig;
            delete[] actualLinea->contenido;
            delete actualLinea;
            actualLinea = siguienteLinea;
        }
        delete version->textoVersion;
    
    
    // Finalmente liberar la versión
    delete[] version->numero;
    delete version;}               ESTO NO ESTA IMPLEMENTADO, AVISARLE AL PROFE*/
}