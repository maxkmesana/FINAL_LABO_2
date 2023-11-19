#include <stdio.h>
#include <stdlib.h>
#include"gestion.h"
extern int nroPxIGlobal;

int main()
{
    nodoPaciente *arbol = NULL;
    arbol = archivoToArbolPacientes("pacientes.bin",arbol);
    mostrarArchivoPacientes("pacientes.bin");

    nodoPaciente *nodoValido = existePacienteNodo(arbol,78654321);
    nodoIngreso *lista = NULL;
//    if(nodoValido != NULL){
//        alta_de_PxI(nodoValido);
//    }
//    printIngresos(nodoValido->listaIngreso);
//    baja_de_pXi(arbol);
//    modificacion_de_PxI(arbol);
//    printIngresos(nodoValido->listaIngreso);
    alta_de_ingreso(nodoValido);
    printIngresos(lista);
    return 0;
}
