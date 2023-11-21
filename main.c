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

//    printTotal(arbol);

//    alta_de_paciente(&arbol);


//    baja_de_ingresos(arbol);
//    printTotal(arbol);
//    ingresosDesdeHasta();
/// retestear modif y si funciona testearla con persistencia (cambiar fechas de ingreso)
//    modificacion_de_ingresos(arbol);
//    printTotal(arbol);
//    persistenciaTotal(arbol);
//    mostrarArchivoIng("ingresos.bin");
//    filesToEstructuras(&arbol);
//    printTotal(arbol);
//    modificacion_de_ingresos(arbol);
//    printTotal(arbol);
//    persistenciaTotal(arbol);
//    mostrarArchivoIng("ingresos.bin");
//    ingresosDesdeHasta();
    return 0;
}
