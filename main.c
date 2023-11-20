#include <stdio.h>
#include <stdlib.h>
#include"gestion.h"
extern int nroPxIGlobal;

int main()
{
    char control;
    nodoPaciente *arbol = NULL;

/// testeo de alta paciente y alta PxI y persistencia total: funcionando
//    alta_de_paciente(&arbol);
//    do{
//        nodoPaciente *nodovalido = existePacienteNodo(arbol, validarDni0(arbol));
//        alta_de_PxI(nodovalido);
//        printf("pxi a paciente distinto?????\n");
//        fflush(stdin);
//        scanf("%c", &control);
//    }while(control == 's');
//    printTotal(arbol);
//    persistenciaTotal(arbol);
//    mostrarArchivoPacientes("pacientes.bin");
//    mostrarArchivoIng("ingresos.bin");
//    mostrarArchivoPxI("pracXing.bin");
//    system("pause");
/// testeo de filesToEstructuras: funcionando
//    filesToEstructuras(&arbol);
//    printTotal(arbol);

    alta_de_paciente(&arbol);
    return 0;
}
