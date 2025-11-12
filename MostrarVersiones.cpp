#include "MostrarVersiones.hpp"
#include <iostream>
#include "NavegacionVersiones.hpp"
#include <string.h>

using namespace std;


//version es un puntero a version_struct
//muestra de forma recusriva las versiones con indentancion segun nivel
void mostrarVersionesRecursivo(version_struct* version, int nivel) {
    if (version == NULL) return;
    
    for (int i = 0; i < nivel; i++) {
        cout << "\t";
    }
    
    cout << version->numero << endl;
    
    mostrarVersionesRecursivo(version->primeraSubversion, nivel + 1);
    mostrarVersionesRecursivo(version->sigVersion, nivel);
}


//muestra el nombre del arcbivo y todas las vers de forma jerarquica
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
