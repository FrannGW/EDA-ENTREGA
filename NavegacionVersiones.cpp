#include "NavegacionVersiones.hpp"
#include <string.h>
#include <stdlib.h>

using namespace std;


//retorna la version del numero especificado, busca recursivamente en subversiones y hermanas. Retorna NULL si no se encuentra
version_struct* buscarVersionRecursiva(version_struct* raiz, char* numeroVersion) {
    if (raiz == NULL) return NULL;
    
    if (strcmp(raiz->numero, numeroVersion) == 0) {
        return raiz;
    }
    
    version_struct* encontrada = buscarVersionRecursiva(raiz->primeraSubversion, numeroVersion);
    if (encontrada != NULL) return encontrada;
    
    return buscarVersionRecursiva(raiz->sigVersion, numeroVersion);
}



//retorna version con numero especificado, solo busca en el nviel de hermanas, retorna NULl si no encuentra
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

//retorna la ultima ersion de lista hermana (la que no tiene sigVersion)
version_struct* buscarUltimaVersion(version_struct* raiz) {
    if (raiz == NULL) return NULL;
    version_struct* actual = raiz;
    while (actual->sigVersion != NULL) {
        actual = actual->sigVersion;
    }
    return actual;
}


//retornamos string del padre de la version (todo hasta el ultumo punto). Retorna NULL si no tiene padre (es raiz)
char* obtenerPadre(char* version) {
    char* ultimoPunto = strrchr(version, '.');
    if (ultimoPunto == NULL) return NULL;
    
    char* padre = new char[ultimoPunto - version + 1];
    strncpy(padre, version, ultimoPunto - version);
    padre[ultimoPunto - version] = '\0';
    return padre;
}


//retorna el numero de la subversion (el ultimo numero despues del ultimo punto). si es raiz retorna numero entero
int obtenerNumeroSubversion(char* version) {
    char* ultimoPunto = strrchr(version, '.');
    if (ultimoPunto == NULL) return atoi(version);
    return atoi(ultimoPunto + 1);
}
