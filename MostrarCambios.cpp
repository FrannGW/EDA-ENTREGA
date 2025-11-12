#include "MostrarCambios.hpp"
#include "NavegacionVersiones.hpp"
#include <iostream>

using namespace std;

TipoRet MostrarCambiosModulo(Archivo a, char* version) {
    if (a == NULL) {
        return ERROR;
    }

    version_struct* versionEncontrada = buscarVersionRecursiva(a->primeraVersion, version);
    if (versionEncontrada == NULL) {
        return ERROR;
    }

    cout << a->nombre << " - " << version << endl;
    cout << endl;

    if (versionEncontrada->cambios == NULL) {
        cout << "No se realizaron modificaciones" << endl;
        return OK;
    }

    // Mostrar cambios en orden inverso (los más recientes primero)
    // Para mostrarlos en orden cronológico, necesitaríamos invertir la lista
    // Pero la letra no especifica el orden, así que mostramos como están almacenados
    
    cambio* actual = versionEncontrada->cambios;
    while (actual != NULL) {
        if (actual->tipo == 'I') {
            cout << "IL\t" << actual->nroLinea << "\t" << actual->contenido << endl;
        } else if (actual->tipo == 'B') {
            cout << "BL\t" << actual->nroLinea << endl;
        }
        actual = actual->sig;
    }

    return OK;
}