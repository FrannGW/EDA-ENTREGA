#include "MostrarVersiones.hpp"
#include "arbol_versiones.hpp"
#include <iostream>

using namespace std;

TipoRet mostrarVersionesModulo(Archivo a) {
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