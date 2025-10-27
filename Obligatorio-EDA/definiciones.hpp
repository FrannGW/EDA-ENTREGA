#ifndef DEFINICIONES_H
#define DEFINICIONES_H



// Tipos de retorno para las operaciones del sistema
enum _retorno { 
    OK,                 // Operación exitosa
    ERROR,              // Error en la operación  
    NO_IMPLEMENTADA     // Función no implementada
};

typedef enum _retorno TipoRet;

// Constantes para límites del sistema
#define MAX_NOMBRE 100      // maximo para el nombre del archivo
#define MAX_COMANDO 200     // maximo para comando de entrada

#endif