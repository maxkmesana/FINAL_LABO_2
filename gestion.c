#include <stdio.h>
#include <stdlib.h>
#include<ctype.h> // para usar funcion isalpha() / isdigit()
#include<string.h>
#include<time.h>
#include"gestion.h"

int nroIngresoGlobal;
int nroPxIGlobal;

/// -------------------------------------------------- FUNCIONES DE PRACXINGRESO--------------------------------------------------- ///

/// Funcion que muestra todos los registros de un archivo
/// de practicas
void mostrarArchivoPxI(char nombreArchivo[]){
    FILE *archivo = fopen(nombreArchivo, "rb");
    PxI registro;
    if(archivo != NULL){
        while(!feof(archivo)){
            fread(&registro, sizeof(PxI), 1, archivo);
            if(!feof(archivo)){
                mostrarRegistroPxI(registro);
            }
        }
    }else{
        printf("Error. No se pudo abrir %s en practicas.c/mostrarArchivoPractica\n", nombreArchivo);
    }
    fclose(archivo);
}

/// Muestra un registro del archivo de practicas
void mostrarRegistroPxI(PxI registro){
    printf("\n-------------------------------------\n");
    printf("Nro de ingreso por practica: %i\n", registro.nroIngreso);
    printf("Nro de practica: %i\n", registro.nroPractica);
    printf("Resultado: %s\n", registro.resultado);
    printf("-------------------------------------\n");
}

/// *** SE CARGA POR TECLADO ***
/// *** ES EXCLUSIVAMENTE PARA TESTEOS ***
void generarArchivoPxI(char nombreArchivo[]){
    char control;
    do{
        PxI nuevo = crearPxI();
        if(esPxIValido(nuevo) == 1){
            FILE *archivo = fopen(nombreArchivo, "ab");
            if(archivo != NULL){
                fwrite(&nuevo, sizeof(PxI), 1, archivo);
            }else{
                printf("No se pudo abrir archivo %s en generarArchivoPxI. \n", nombreArchivo);
            }
            fclose(archivo);
        }

        printf("Quiere ingresar otro/intentar de nuevo? (s/n): ");
        fflush(stdin);
        scanf("%c", &control);
    }while(control == 's' || control == 's');
}

void baja_de_pXi(nodoPaciente *arbol){
    char control;
    int nroPxIBorrar;
    int iterar;
    nodoPxI *listaPxI;
    do{
        listaPxI = validacionesModifPxI(arbol);
        if(listaPxI != NULL){
            nroPxIBorrar = validarNroPxI(listaPxI);
            if(nroPxIBorrar == -1){
                return;
            }
             borrarNodo(&listaPxI,nroPxIBorrar);
            iterar = 0;
        }else{
            printf(">Quiere intentar de nuevo? (s/n): ");
            fflush(stdin);
            scanf("%c", &control);
            if(control == 's' || control == 'S'){
                iterar = 1;
            }else{
                iterar = 0;
            }
        }
    }while(iterar == 1);
}

nodoPxI *validacionesBajaPxI(nodoPaciente *arbol){
    nodoPaciente *pacienteBuscado = NULL;
    nodoIngreso *ingresoBuscado = NULL;

    pacienteBuscado = validarDniModificacion(arbol);
    if(pacienteBuscado == NULL){
        return NULL;
    }

    ingresoBuscado = validarIngresoModificacion(pacienteBuscado->listaIngreso);
    if(ingresoBuscado == NULL){
        return NULL;
    }

    return ingresoBuscado->lista;
}

void borrarNodo(nodoPxI **subLista, int nroPxIBorrar){
    nodoPxI *seg;
    nodoPxI *ant;
    if((*subLista != NULL) && (nroPxIBorrar != (*subLista)->pracXing.nroIngreso)){
        nodoPxI *aux = *subLista;
        *subLista = (*subLista)->sig;
        free(aux);
    }else{
        seg = *subLista;
        while((seg != NULL) && (nroPxIBorrar != seg->pracXing.nroIngreso)){
            ant = seg;
            seg = seg->sig;
        }
        ant->sig = seg->sig;
        free(seg);
    }
}



void printNodoPxI(nodoPxI *nodo){
    printf("------------------------------\n");
    printf("Numero de practica por ingreso: %i\n", nodo->pracXing.nroIngreso);
    printf("Numero de practica: %i\n", nodo->pracXing.nroPractica);
    printf("Resultado: %s\n", nodo->pracXing.resultado);
    printf("------------------------------\n");
}

nodoPxI *validacionesModifPxI(nodoPaciente *arbol){
    nodoPaciente *pacienteBuscado = NULL;
    nodoIngreso *ingresoBuscado = NULL;
    nodoPxI *pXiBuscada = NULL;

    pacienteBuscado = validarDniModificacion(arbol);
    if(pacienteBuscado == NULL){
        return NULL;
    }

    ingresoBuscado = validarIngresoModificacion(pacienteBuscado->listaIngreso);
    if(ingresoBuscado == NULL){
        return NULL;
    }

    pXiBuscada = buscarNodoPxI(ingresoBuscado->lista);
    if(pXiBuscada == NULL){
        return NULL;
    }
    return pXiBuscada;
}

void modificacion_de_PxI(nodoPaciente *arbol, int eleccion){
    int iterar;
    char control;
    nodoPxI *nodoAmodificar = NULL;
    do{
        if(nodoAmodificar == NULL){
            nodoAmodificar = validacionesModifPxI(arbol);
        }
        if(nodoAmodificar != NULL){
            eleccionModifPxI(eleccion,nodoAmodificar);
            printf(">Quiere modificar otro campo de la misma practica por ingreso? (s/n): ");
        }else{
            printf(">Quiere intentar de nuevo? (s/n): ");
        }
        fflush(stdin);
        scanf("%c", &control);
        if(control == 's' || control == 'S'){
            iterar = 1;
        }else{
            iterar = 0;
        }
    }while(iterar == 1);
}

void eleccionModifPxI(int eleccion, nodoPxI *nodoAmodificar){
    char *resultado;
    int nroPractica;
    switch(eleccion){
    case 1:
        resultado = validarResultado();
        if(strcmp(resultado,"-1") == 0){
            return;
        }
        strcpy(nodoAmodificar->pracXing.resultado, resultado);
        break;
    case 2:
        nroPractica = validarNroPractica(0);
        if(nroPractica == -1){
            return;
        }
        nodoAmodificar->pracXing.nroPractica = nroPractica;
        break;
    }
}

/// VALIDA INPUT DE NRO PXI POR TECLADO
/// ***SOLO PARA MODIFICACION/BAJA PXI***
int validarNroPxI(nodoPxI *subLista){
    int nroPxI;
    int valido;
    do{
        printf(">Ingrese la edad deseada: ");
        fflush(stdin);
        valido = scanf("%i", &nroPxI);
        if(nroPxI == -1){
            printf("Accion cancelada. \n");
            return -1;
        }
        if(valido != 1){
            printf("Error. El numero de practica por ingreso no es un entero valido. \n\n");
        }else if(existeNroPxI(subLista, nroPxI) == 0){
            printf("Error. El numero de practica por ingreso no existe. \n\n");
            valido = 0;
        }
    }while(valido != 1);
    return nroPxI;
}

nodoPxI *buscarNodoPxI(nodoPxI *subLista){
    nodoPxI *seg = subLista;
    int nroPxIBuscado = validarNroPxI(subLista);
    if(nroPxIBuscado == -1){
        return NULL;
    }
    while(seg != NULL){
        if(seg->pracXing.nroIngreso == nroPxIBuscado){
            return seg;
        }
        seg = seg->sig;
    }
    return NULL;
}

nodoPxI *alta_de_PxI(nodoPxI *subLista){
    PxI PxInuevo;
    int iterar;
    int esValido; // si la carga de la PxI fue cancelada (1) o no (0)
    char control;
    do{
        PxInuevo = crearPxI();
        esValido = esPxIValido(PxInuevo);
        if(esValido == 1){
            subLista = agregarPxIAlFinal(subLista, crearNodoPxI(PxInuevo));
            iterar = 0;
            printf(">Quiere ingresar otra? (s/n): \n");
        }else{
            printf(">Quiere intentar de nuevo? (s/n): ");
        }
        fflush(stdin);
        scanf("%c", &control);
        if(control == 's' || control == 'S'){ // control n
            iterar = 1;
        }else{
            iterar = 0;
        }
    }while(iterar == 1);
    return subLista;
}

nodoPxI *agregarPxIAlFinal(nodoPxI *subLista, nodoPxI *nuevoNodo)
{
    if(subLista == NULL)
    {
        subLista = nuevoNodo;
    }
    else
    {
        nodoPxI *seg = subLista;
        while(seg->sig != NULL)
        {
            seg = seg->sig;
        }
        seg->sig = nuevoNodo;
    }
    return subLista;
}

/// Verifica si el paciente creado es valido o si se
/// cancelo la carga de ese
int esPxIValido(PxI pracXing){

    if (pracXing.nroPractica == -1){
        return 0;
    }

    if (strcmp(pracXing.resultado, "-1") == 0){
        return 0;
    }

    return 1;
}

nodoPxI *crearNodoPxI(PxI pracXing){
    nodoPxI *nuevo = (nodoPxI*)malloc(sizeof(nodoPxI));
    nuevo->pracXing = pracXing;
    nuevo->sig = NULL;
    return nuevo;
}

/// Crea un paciente utilizando todas las funciones de validacion
PxI crearPxI(){
    PxI nuevo;
    nroPxIGlobal++;
    nuevo.nroIngreso = nroIngresoGlobal;

    nuevo.nroPractica = validarNroPractica(1);
    if (nuevo.nroPractica == -1){
        return nuevo;
    }

    strcpy(nuevo.resultado, validarResultado());
    if (strcmp(nuevo.resultado, "-1") == 0){
        return nuevo;
    }

    return nuevo;
}

char *validarResultado(){
    char resultado[40];
    int valido;
    do{
        valido = 1;
        printf(">Ingrese el resultado deseado: ");
        fflush(stdin);
        fgets(resultado, sizeof(resultado), stdin);
        resultado[strcspn(resultado, "\n")] = '\0';
        if(strcmp(resultado, "-1") == 0){
            printf("Accion cancelada. \n");
            return strdup("-1");
        }
        if(esResultadoValido(resultado) == 0){
            printf("Error. El resultado ingresado contiene caracteres no validos.\n\n");
            valido = 0;
        }
    }while(valido != 1);
    char *cadena = strdup(resultado);
    // strdup: asigna memoria dinamicamente y copia la cadena en la mem asignada.
    return cadena;
}

/// Verifica que cada caracter del resultado sea valido
/// Los espacios y los numeros no son considerados invalidos
/// Retorna 1 si la cadena es valida, 0 si no lo es.
int esResultadoValido(const char *cadena){
    for (int i = 0; i < strlen(cadena); i++) {
        if (!isalpha(cadena[i]) && !isdigit(cadena[i]) && cadena[i] != ' '){
        // isalpha() devuelve 0 si no es caracter numerico
            return 0;
        }
    }
    return 1;
}

int existeNroPxI(nodoPxI *subLista, int nroPxIBuscado){
    nodoPxI *seg = subLista;
    while(seg != NULL){
        if(seg->pracXing.nroIngreso == nroPxIBuscado){
            return 1;
        }
        seg = seg->sig;
    }
    return 0;
}

/// -------------------------------------------------- FUNCIONES DE PRACTICAXINGRESO --------------------------------------------------- ///



/// -------------------------------------------------- FUNCIONES DE FECHA --------------------------------------------------- ///

void fGetNow(char date[])
{
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    strftime(date, 128,"%d-%m-%Y %w%H:%M:%S",tlocal);
}

int fGetAnioLarge()
{
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);

    int anio = tlocal->tm_year + 1900; // Obtiene el año actual desde la estructura tm

    return anio;
}

int fGetAnioShort()
{
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);

    int anio = tlocal->tm_year % 100; // Obtiene los dos últimos dígitos del año

    return anio;
}

int fGetMonthNumber()
{
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);

    int mes = tlocal->tm_mon + 1; // Se suma 1 porque tm_mon comienza desde 0

    return mes;
}

int fGetDayNumber()
{
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);

    int dia = tlocal->tm_mday; // Obtiene el día actual desde la estructura tm

    return dia;
}

int fGetHour(char date[])
{
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    strftime(date, 128,"%H:%M:%S",tlocal);

    int hora;
    sscanf(date, "%d", &hora);

    return hora;
}

int fGetMinute(char date[])
{
    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    strftime(date, 128,"%M",tlocal);

    int min;
    sscanf(date, "%d", &min);

    return min;
}

char* fechaConcatenada()
{
    int dia = fGetDayNumber();
    int mes = fGetMonthNumber();
    int anio = fGetAnioLarge();

    char date[10];
    date[0] = '\0';

    sprintf(date + strlen(date), "%d/", dia);
    sprintf(date + strlen(date), "%d/", mes);
    sprintf(date + strlen(date), "%d", anio);

    return strdup(date);
}

char* fechaFormatear(int dia, int mes, int anio)
{
    char date[10];
    date[0] = '\0';

    sprintf(date + strlen(date), "%d/", dia);
    sprintf(date + strlen(date), "%d/", mes);
    sprintf(date + strlen(date), "%d", anio);

    return strdup(date);
}

int obtenerDiasMes(int mes, int anio)
{
    int diasMes;

    if (mes == 2)
    {
        if ((anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0))
        {
            diasMes = 29;
        }
        else
        {
            diasMes = 28;
        }
    }
    else if (mes == 4 || mes == 6 || mes == 9 || mes == 11)
    {
        diasMes = 30;
    }
    else
    {
        diasMes = 31;
    }

    return diasMes;
}


/// -------------------------------------------------- FUNCIONES DE FECHA --------------------------------------------------- ///





/// -------------------------------------------------- FUNCIONES DE INGRESOS ----------------------------------------------------- ///

nodoIngreso *validarIngresoModificacion(nodoIngreso *lista){
    int nroIngreso;
    printf(">Ingrese el numero de ingreso: ");
    fflush(stdin);
    int validoNroIngreso = scanf("%i", &nroIngreso);
    if(nroIngreso == -1){
        printf("Accion cancelada. \n");
        return NULL;
    }
    if(validoNroIngreso != 1){
        printf("Error. El numero de ingreso contiene caracteres no numericos. \n\n");
        return NULL;
    }
    nodoIngreso *nodoAmodificar = existeIngresoNodo(lista, nroIngreso);
    if(nodoAmodificar == NULL){
        printf("Error. El numero de ingreso no existe.\n\n");
        return NULL;
    }
    return nodoAmodificar;
}

nodoIngreso *existeIngresoNodo(nodoIngreso *lista, int nroIngresoBuscado){
    nodoIngreso *seg = lista;
    while(seg != NULL){
        if(nroIngresoBuscado == seg->datoPcte.nroIngreso){
            return seg;
        }
    }
    return NULL;
}

nodoIngreso *crearNodoIngreso(stIngresos registro)
{
    nodoIngreso *nuevoNodo = (nodoIngreso*)malloc(sizeof(nodoIngreso));
    nuevoNodo->datoPcte.nroIngreso = registro.nroIngreso;
    nuevoNodo->datoPcte.dniPcte = registro.dniPcte;
    strcpy(nuevoNodo->datoPcte.fechaIngreso, registro.fechaIngreso);
    strcpy(nuevoNodo->datoPcte.fechaRetiro, registro.fechaRetiro);
    nuevoNodo->datoPcte.eliminado = registro.eliminado;
    nuevoNodo->sig = NULL;
    nuevoNodo->lista = NULL;

    return nuevoNodo;
}

stIngresos cargaIngreso(nodoPaciente *raiz)
{

    stIngresos nuevo;

    nuevo.dniPcte = raiz->datoPaciente.dni;

    nuevo.nroIngreso = nroIngresoGlobal++;

    char* fechaInterna = fechaConcatenada();
    strcpy(nuevo.fechaIngreso, fechaInterna);
    free(fechaInterna);

    char* fechaUsuario = validarFecha();
    strcpy(nuevo.fechaRetiro, fechaUsuario);
    free(fechaUsuario);

    nuevo.matrProfesional = validarMatricula();

    nuevo.eliminado = 0;

    return nuevo;
}

nodoIngreso *agregarIngresoAlFinal(nodoIngreso *lista, nodoIngreso *nuevoNodo)
{
    if(lista == NULL)
    {
        lista = nuevoNodo;
    }
    else
    {
        nodoIngreso* seg = lista;
        while(seg->sig != NULL)
        {
            seg = seg->sig;
        }
        seg->sig = nuevoNodo;
    }
    return lista;
}

int validarMatricula()
{
    int numMatricula;
    int valido;

    do
    {
        printf(">Ingrese la matricula: ");
        fflush(stdin);
        valido = scanf("%i", &numMatricula);
        if(valido != 1)
        {
            printf("Error. El numero de ingreso no es un entero valido. \n\n");
        }
        else if(numMatricula <= 1 || numMatricula > 99999999)
        {
            printf("Error. El numero de matricula debe tener entre 1 y 8 digitos. Ingrese uno válido.\n\n");
        }
    }
    while(valido != 1);
    return numMatricula;
}

char* validarFecha()
{
    int dia;
    int mes;
    int anio;
    int valido;
    do
    {
        printf(">Ingrese el anio: ");
        fflush(stdin);
        valido = scanf("%i", &anio);
        if(valido != 1 && (anio < fGetAnioLarge() || anio > 2050))
        {
            printf("Error. El anio ingresado no corresponde a uno valido.\n\n");
        }else
        {
            printf(">Ingreso el mes: ");
            fflush(stdin);
            valido = scanf("%i", &mes);
            if(valido != 1 && (mes < fGetMonthNumber() || mes > 12))
            {
                printf("Error. El mes ingresado no corresponde a uno valido.\n\n");
            }else
            {
                printf("Ingrese el numero dia dia: ");
                valido = scanf("%i", & dia);
                if(valido != 1 && (dia < fGetDayNumber() || dia > obtenerDiasMes(mes, anio)))
                {
                    printf("Error. El dia ingresado no corresponde a uno valido.\n\n");
                }
            }
        }
    }while(valido != 1);

    char* formateo = fechaFormatear(dia, mes, anio);
    return formateo;
}

/// TERMINAR
//nodoIngreso* alta_de_(nodoPaciente *arbol)
//{
//    int dniPcte;
//    int valido;
//    char control;
//
//    do
//    {
//        dniPcte = validarDni(arbol);
//        busqueda = existePacienteNodo(arbol, dniPcte);
//
//    }
//    while();
//}

/// printea el numero de ingreso que esta en el archivo.
void printNroArchivo(char nombreArchivo[])
{
    int registro;
    FILE *archivo = fopen(nombreArchivo, "rb");
    if(archivo != NULL)
    {
        fread(&registro, sizeof(int), 1, archivo);
        printf("%i", registro);
    }
    else
    {
        printf("Error. No se pudo abrir archivo %s \n", nombreArchivo);
    }
    fclose(archivo);
}

/// toma la variable global y la actualiza en el archivo.
void escribirNroArchivo(char nombreArchivo[])
{
    int num;
    if(strcmp(nombreArchivo, "nroIngreso.bin") == 0){
        num = nroIngresoGlobal;
    }else if(strcmp(nombreArchivo, "nroPxI.bin") == 0){
        num = nroPxIGlobal;
    }else{
        printf("nombreArchivo ingresado en escribirNroArchivo no es valido. \n");
        return;
    }

    FILE *archivo = fopen(nombreArchivo, "w+b");
    if(archivo == NULL)
    {
        printf("Error. No se pudo abrir archivo %s \n", nombreArchivo);
    }
    else
    {
        fwrite(&num, sizeof(int), 1, archivo);
    }
    fclose(archivo);
}

/// actualiza la variable global con el registro del archivo.
void actualizarNroGlobal(char nombreArchivo[])
{
    int num;
    FILE *archivo = fopen(nombreArchivo, "rb");
    if(archivo == NULL)
    {
        printf("Error. No se pudo abrir archivo %s \n", nombreArchivo);
    }
    else
    {
        fread(&num, sizeof(int), 1, archivo);
        if(strcmp(nombreArchivo, "nroIngreso.bin") == 0){
            nroIngresoGlobal = num;
        }else if(strcmp(nombreArchivo, "nroPxI.bin") == 0){
            nroPxIGlobal = num;
        }
    }
    fclose(archivo);

}

/// Crea el archivo de nroIngreso inicializado en 0
void crearNroArchivo(char nombreArchivo[])
{
    int num = 0;
    FILE *archivo = fopen(nombreArchivo, "wb");
    if(archivo == NULL)
    {
        printf("Error. No se pudo abrir archivo %s \n", nombreArchivo);
    }
    else
    {
        fwrite(&num, sizeof(int), 1, archivo);
    }
    fclose(archivo);
}


/// -------------------------------------------------- FUNCIONES DE INGRESOS ----------------------------------------------------- ///




/// -------------------------------------------------- FUNCIONES DE PACIENTES --------------------------------------------------- ///

nodoPaciente *alta_de_paciente(nodoPaciente *arbol){
    Paciente pacienteNuevo;
    int iterar;
    int esValido; // si la carga del paciente fue cancelada (1) o no (0)
    char control;
    do{
        pacienteNuevo = crearPaciente(arbol);
        esValido = esPacienteValido(pacienteNuevo);
        if(esValido == 1){
            arbol = insertarNodoPaciente(arbol, pacienteNuevo);
            iterar = 0;
        }else{
            printf(">Quiere intentar de nuevo? (s/n): ");
            fflush(stdin);
            scanf("%c", &control);
            if(control == 's' || control == 'S'){ // control n
                iterar = 1;
            }else{
                iterar = 0;
            }
        }

    }while(iterar == 1); //  iterar = 1 itera de nuevo
    return arbol;
}

void baja_de_paciente(nodoPaciente *arbol){
    nodoPaciente *nodoAmodificar = NULL;
    char control;
    int valido;
    do{
        nodoAmodificar = validarDniModificacion(arbol);
        if(nodoAmodificar == NULL){
            printf(">Quiere intentar nuevamente? (s/n): ");
            fflush(stdin);
            scanf("%c", &control);
            if(control == 's' || control == 'S'){
                valido = 0;
            }else{
                return;
            }
        }else{
            valido = 1;
        }
    }while(valido != 1);
    int eliminado = validarEliminado();
    if(eliminado == -1){
        return;
    }
    nodoAmodificar->datoPaciente.eliminado = eliminado;
}

/// Funcion principal de modificacion de pacientes
/// acabo de refactorizar esta funcion (18/11/23, 04.11 am)
/// y me siento un crack de lo hermosa que quedo (a comparacion de como estaba)
/// si cuando se testea esta funcion, hay algun error: max es un salame.
void modificacion_de_paciente(nodoPaciente *arbol, int eleccion){
    int iterar;
    char control;
    nodoPaciente *nodoAmodificar = NULL;
    do{
        if(nodoAmodificar == NULL){
           nodoAmodificar = validarDniModificacion(arbol);
        }
        if(nodoAmodificar != NULL){
            eleccionModifPaciente(eleccion,nodoAmodificar);
            printf(">Quiere modificar otro campo del mismo paciente? (s/n): ");
        }else{
            printf(">Quiere intentar de nuevo? (s/n): ");
        }
        fflush(stdin);
        scanf("%c", &control);
        if(control == 's' || control == 'S'){
            iterar = 1;
        }else{
            iterar = 0;
        }
    }while(iterar == 1);
}

/// Modularizacion, modifica el campo elegido del paciente.
void eleccionModifPaciente(int eleccion, nodoPaciente *nodoAmodificar){
    char *nombre;
    int edad;
    char *direccion;
    char *telefono;
    switch(eleccion){
    case 1:
        nombre = validarNombrePaciente();
        if(strcmp(nombre,"-1") == 0){
            return;
        }
        strcpy(nodoAmodificar->datoPaciente.apellidoNombre, nombre);
        break;
    case 2:
        edad = validarEdad();
        if(edad == -1){
            return;
        }
        nodoAmodificar->datoPaciente.edad = edad;
        break;
    case 3:
        direccion = validarDireccionPaciente();
        if(strcmp(direccion,"-1") == 0){
            return;
        }
        strcpy(nodoAmodificar->datoPaciente.direccion, direccion);
        break;
    case 4:
        telefono = validarTelefonoPaciente();
        if(strcmp(telefono,"-1") == 0){
            return;
        }
        strcpy(nodoAmodificar->datoPaciente.telefono, telefono);
        break;
    }
}

/// Se valida el Dni de manera distinta a la validacion ya hecha
/// especifica y exclusivamente para usarse en funcion de modificacion
nodoPaciente *validarDniModificacion(nodoPaciente *arbol){
    int dni;
    printf(">Ingrese el DNI del paciente: ");
    fflush(stdin);
    int validoDni = scanf("%i", &dni);
    if(dni == -1){
        printf("Accion cancelada. \n");
        return NULL;
    }
    nodoPaciente *nodoAmodificar = existePacienteNodo(arbol, dni);
    if(validoDni != 1){
        printf("Error. El dni ingresado contiene caracteres no numericos. Intente nuevamente. \n\n");
        return NULL;
    }else if (dni <= 9999 || dni > 99999999) {
        printf("Error. El DNI debe tener entre 5 y 8 digitos. Ingrese uno valido.\n\n");
        return NULL;
        }else if(nodoAmodificar == NULL){
            printf("Error. El DNI ingresado no pertenece a un paciente. Intente nuevamente. \n\n");
            return NULL;
        }
    return nodoAmodificar;
}

/// Printea menu para cambiar el estado del paciente (eliminado = 1 || 0).
/// Hace las validaciones pertinentes.
/// Devuelve un 1 o 0 dependiendo del estado elegido para el paciente.
int validarEliminado(){
    int eleccion;
    int valido;
    do{
        printf("\nCual quiere que sea el estado del paciente?\n");
        printf("1- Eliminado \n");
        printf("2- No eliminado \n");
        printf(">Ingrese su eleccion: ");
        fflush(stdin);
        valido = scanf("%i", &eleccion);
        if(eleccion == -1){
            printf("Accion cancelada. \n");
            return -1;
        }
        if(valido != 1){
            printf("Error. Eleccion ingresada no es entero valido. Intente nuevamente.\n\n");
        }else if(eleccion < 1 || eleccion > 2){
            printf("Error. Eleccion ingresada no es una opcion valida. Intente nuevamente.\n\n");
            valido = 0;
        }

    }while(valido != 1);
    if(eleccion == 1){
        return 1;
    }else{
        return 0;
    }
}

/// Printea cada nodo del arbol en orden por DNI
/// PARA TESTEOS DE FUNCIONES
void inorder(nodoPaciente *arbol){
    if(arbol != NULL){
        inorder(arbol->izq);
        printf("\n-------------------------------------\n");
        printf("Apellido nombre: %s\n",arbol->datoPaciente.apellidoNombre);
        printf("Direccion: %s\n",arbol->datoPaciente.direccion);
        printf("DNI: %i\n",arbol->datoPaciente.dni);
        printf("Edad: %i\n",arbol->datoPaciente.edad);
        printf("Eliminado: %i\n",arbol->datoPaciente.eliminado);
        printf("Telefono: %s\n",arbol->datoPaciente.telefono);
        printf("-------------------------------------\n");
        inorder(arbol->der);
    }
}

/// Pasa los registros del archivo de pacientes a un arbol de pacientes.
/// PARA TESTEOS DE FUNCIONES
nodoPaciente *archivoToArbolPacientes(char nombreArchivo[], nodoPaciente *arbol){
    FILE *archivo = fopen(nombreArchivo, "rb");
    Paciente registro;
    if(archivo != NULL){
        while(!feof(archivo)){
            fread(&registro, sizeof(Paciente), 1, archivo);
            if(!feof(archivo)){
                arbol = insertarNodoPaciente(arbol, registro);
            }
        }
    }else{
        printf("Error. No se pudo abrir %s en pacientes.c/mostrarArchivoPacientes\n", nombreArchivo);
    }
    fclose(archivo);
    return arbol;
}

/// Funcion que muestra todos los registros de un archivo
/// de pacientes
/// PARA TESTEOS DE FUNCIONES
void mostrarArchivoPacientes(char nombreArchivo[]){
    FILE *archivo = fopen(nombreArchivo, "rb");
    Paciente registro;
    if(archivo != NULL){
        while(!feof(archivo)){
            fread(&registro, sizeof(Paciente), 1, archivo);
            if(!feof(archivo)){
                mostrarRegistroPacientes(registro);
            }
        }
    }else{
        printf("Error. No se pudo abrir %s en pacientes.c/mostrarArchivoPacientes\n", nombreArchivo);
    }
    fclose(archivo);
}

/// Muestra un registro de pacientes
/// PARA TESTEOS DE FUNCIONES
void mostrarRegistroPacientes(Paciente registro){
    printf("\n-------------------------------------\n");
    printf("Apellido nombre: %s\n",registro.apellidoNombre);
    printf("Direccion: %s\n",registro.direccion);
    printf("DNI: %i\n",registro.dni);
    printf("Edad: %i\n",registro.edad);
    printf("Eliminado: %i\n",registro.eliminado);
    printf("Telefono: %s\n",registro.telefono);
    printf("-------------------------------------\n");
}

// Crea un paciente y lo mete a un archivo de pacientes
// para poder testear funciones de pacientes.
//  ****SOLO**** PARA TESTEOS DE FUNCIONES
/// LA LOGICA DE ESTA FUNCION NO TIENE SENTIDO => COMENTADA. REHACER POR PELOTUDO
//nodoPaciente *pacienteToarchivo(char nombreArchivo[], nodoPaciente *arbol){
//    FILE *archivo = fopen(nombreArchivo, "ab");
//    if(archivo != NULL){
//        Paciente nuevo = crearPaciente(arbol);
//        if(esPacienteValido(nuevo) == 0){
//            printf("Paciente ingresado no es valido. \n");
//        }
//        arbol = insertarNodoPaciente(arbol, nuevo);
//        fwrite(&nuevo, sizeof(Paciente), 1, archivo);
//    }else{
//        printf("No se pudo abrir %s en pacientes.c/pacienteToArchivo", nombreArchivo);
//    }
//    fclose(archivo);
//    return arbol;
//}


/// Inserta un nodo paciente en el arbol de pacientes
nodoPaciente *insertarNodoPaciente(nodoPaciente *arbol, Paciente dato){
    if(arbol == NULL){
        arbol = crearNodoPaciente(dato);
    }else{
        if(dato.dni > arbol->datoPaciente.dni){
            arbol->der = insertarNodoPaciente(arbol->der,dato);
        }else if(dato.dni < arbol->datoPaciente.dni){
            arbol->izq = insertarNodoPaciente(arbol->izq,dato);
        }
    }
    return arbol;
}

/// Crea un nodo de tipo NodoPaciente
nodoPaciente *crearNodoPaciente(Paciente dato){
    nodoPaciente *nuevo = (nodoPaciente*)malloc(sizeof(nodoPaciente));
    nuevo->datoPaciente = dato;
    nuevo->izq = NULL;
    nuevo->der = NULL;
    nuevo->listaIngreso = NULL;
    return nuevo;
}

/// Verifica si el paciente creado es valido o si se
/// cancelo la carga de ese
int esPacienteValido(Paciente pcte){
    if (strcmp(pcte.apellidoNombre, "-1") == 0){
        return 0;
    }
    if (pcte.edad == -1){
        return 0;
    }
    if (pcte.dni == -1){
        return 0;
    }
    if (strcmp(pcte.direccion, "-1") == 0){
        return 0;
    }
    if (strcmp(pcte.telefono, "-1") == 0){
        return 0;
    }
    return 1;
}

/// Crea un paciente utilizando todas las funciones de validacion
Paciente crearPaciente(nodoPaciente *arbol){
    Paciente nuevo;

    strcpy(nuevo.apellidoNombre, validarNombrePaciente());
    if (strcmp(nuevo.apellidoNombre, "-1") == 0){
        return nuevo;
    }

    nuevo.edad = validarEdad();
    if (nuevo.edad == -1){
        return nuevo;
    }

    nuevo.dni = validarDni(arbol);
    if (nuevo.dni == -1){
        return nuevo;
    }

    strcpy(nuevo.direccion, validarDireccionPaciente());
    if (strcmp(nuevo.direccion, "-1") == 0){
        return nuevo;
    }

    strcpy(nuevo.telefono, validarTelefonoPaciente());
    if (strcmp(nuevo.telefono, "-1") == 0){
        return nuevo;
    }

    nuevo.eliminado = 0;
    return nuevo;
}

/// Se pide una cadena de caracteres y se verifica si es valida (alfabetica) y
/// Si es valida, se retorna.
/// Caso contrario, se pide la cadena de nuevo hasta que se cumplan los criterios.
char *validarTelefonoPaciente(){
    char telefono[15];
    int valido;
    do{
        valido = 1;
        printf(">Ingrese el numero de telefono deseado: ");
        fflush(stdin);
        fgets(telefono, sizeof(telefono), stdin);
        telefono[strcspn(telefono, "\n")] = '\0';
        if(strcmp(telefono, "-1") == 0){
            printf("Accion cancelada. \n");
            return strdup("-1");
        }
        if(esTelefonoValido(telefono) == 0){
            printf("Error. El numero de telefono ingresado contiene caracteres no validos.\n\n");
            valido = 0;
        }
    }while(valido != 1);
    char *cadena = strdup(telefono);
    // strdup: asigna memoria dinamicamente y copia la cadena en la mem asignada.
    return cadena;
}

/// Verifica que cada caracter de la direccion sea valido (alfabetico)
/// Los espacios y los numeros no son considerados invalidos
/// Retorna 1 si la cadena es valida, 0 si no lo es.
int esTelefonoValido(const char *cadena){
    for (int i = 0; i < strlen(cadena); i++) {
        if (!isdigit(cadena[i]) && cadena[i] != ' '){
        // isalpha() devuelve 0 si no es caracter numerico
            return 0;
        }
    }
    return 1;
}

/// Se pide una cadena de caracteres y se verifica si es valida (alfabetica) y
/// Si es valida, se retorna.
/// Caso contrario, se pide la cadena de nuevo hasta que se cumplan los criterios.
char *validarDireccionPaciente(){
    char direccion[30];
    int valido;
    do{
        valido = 1;
        printf(">Ingrese la direccion deseada: ");
        fflush(stdin);
        fgets(direccion, sizeof(direccion), stdin);
        direccion[strcspn(direccion, "\n")] = '\0';
        if(strcmp(direccion, "-1") == 0){
            printf("Accion cancelada. \n");
            return strdup("-1");
        }
        if(esDireccionValida(direccion) == 0){
            printf("Error. La direccion ingresada contiene caracteres no validos.\n\n");
            valido = 0;
        }
    }while(valido != 1);
    char *cadena = strdup(direccion);
    // strdup: asigna memoria dinamicamente y copia la cadena en la mem asignada.
    return cadena;
}

/// Verifica que cada caracter de la direccion sea valido (alfabetico)
/// Los espacios, el punto y los numeros no son considerados invalidos
/// Retorna 1 si la cadena es valida, 0 si no lo es.
int esDireccionValida(const char *cadena){
    for (int i = 0; i < strlen(cadena); i++) {
        if (!isalpha(cadena[i]) && !isdigit(cadena[i]) && cadena[i] != ' ' && cadena[i] != '.'){
        // isalpha() devuelve 0 si no es caracter numerico
            return 0;
        }
    }
    return 1;
}

/// Se pide una cadena de caracteres y se verifica si es valida (alfabetica) y
/// Si es valida, se retorna.
/// Caso contrario, se pide la cadena de nuevo hasta que se cumplan los criterios.
char *validarNombrePaciente(){
    char nombre[40];
    int valido;
    do{
        valido = 1;
        printf(">Ingrese el nombre y apellido deseado: ");
        fflush(stdin);
        fgets(nombre, sizeof(nombre), stdin);
        nombre[strcspn(nombre, "\n")] = '\0';
        if(strcmp(nombre, "-1") == 0){
            printf("Accion cancelada. \n");
            return strdup("-1");
        }
        if(esNombreValido(nombre) == 0){
            printf("Error. El nombre ingresado contine caracteres no alfabeticos.\n\n");
            valido = 0;
        }
    }while(valido != 1);
    char *cadena = strdup(nombre);
    // strdup: asigna memoria dinamicamente y copia la cadena en la mem asignada.
    return cadena;
}

/// Verifica que cada caracter del nombre sea valido (alfabetico)
/// Los espacios no son considerados invalidos
/// Retorna 1 si la cadena es valida, 0 si no lo es.
int esNombreValido(const char *cadena){
    for (int i = 0; i < strlen(cadena); i++) {
        if (!isalpha(cadena[i]) && cadena[i] != ' '){
        // isalpha() devuelve 0 si no es caracter numerico
            return 0;
        }
    }
    return 1;
}


/// Pide la edad del paciente a ser ingresado y hace las validaciones
/// pertinentes (si es un entero valido, si esta en un rango humanamente posible)
int validarEdad(){
    int edad;
    int valido;
    do{
        printf(">Ingrese la edad deseada: ");
        fflush(stdin);
        valido = scanf("%i", &edad);
        if(edad == -1){
            printf("Accion cancelada. \n");
            return -1;
        }
        if(valido != 1){
            printf("Error. La edad ingresada no es un entero valido. \n\n");
        }else if(edad < 1 || edad > 114){
            printf("Error. La edad ingresada no esta en un rango valido. \n\n");
            valido = 0;
        }
    }while(valido != 1);
    return edad;
}


/// Pide el DNI del paciente a ser ingresado y hace las validaciones
/// pertinentes (si el paciente ya existe, si el DNI es entero valido,
/// y que el dni este entre 5 y 8 digitos);
int validarDni(nodoPaciente *arbol){
    int nroDni;
    int valido;
    do{
        printf(">Ingrese el numero de DNI deseado: ");
        fflush(stdin);
        valido = scanf("%i", &nroDni);
        if(nroDni== -1){
            printf("Accion cancelada. \n");
            return -1;
        }
        if(valido != 1){
            printf("Error. El numero de DNI ingresado no es un entero valido. \n\n");
        }else if(existePaciente(arbol, nroDni) == 1){
            printf("Error. El paciente ya existe. Ingrese uno distinto.\n\n");
            valido = 0;
            }else if (nroDni <= 9999 || nroDni > 99999999) {
            printf("Error. El DNI debe tener entre 5 y 8 digitos. Ingrese uno valido.\n\n");
            valido = 0;
            }
    }while(valido != 1);
    return nroDni;
}




/// Verifica si el paciente buscado existe en el arbol o no.
/// retorna el nodo si es encontrado, NULL si no lo encuentra.
nodoPaciente *existePacienteNodo(nodoPaciente* arbol, int dni){
    if (arbol == NULL || arbol->datoPaciente.dni == dni){
        return arbol;
    }
    if (dni < arbol->datoPaciente.dni){
        return existePacienteNodo(arbol->izq, dni);
    }
    else{
        return existePacienteNodo(arbol->der, dni);
    }
}

/// Verifica si el paciente ingresado por teclado existe o no en el arbol cargado.
/// Devuelve 1 si existe, 0 si no existe.
int existePaciente(nodoPaciente *arbol, int dni){
    if(arbol){
        return(arbol->datoPaciente.dni == dni) || existePaciente(arbol->izq, dni) || existePaciente(arbol->der, dni);
    }else{
        return 0;
    }
}


/// -------------------------------------------------- FUNCIONES DE PACIENTES --------------------------------------------------- ///





/// -------------------------------------------------- FUNCIONES DE PRACTICAS -------------------------------------------------- ///

/// Da de baja una practica. Hace todas las validaciones pertinentes.
void baja_de_practica(char nombreArchivo[], nodoPaciente *arbol){
    FILE *archivo = fopen(nombreArchivo, "r+b");
    char control;
    char control2;
    Practica registro;
    int desplazamientoBytes;
    int valido;
    int nroPractica;
    do{
        nroPractica = validarNroPractica(0);
        if(nroPractica == -1){
            return;
        }
        desplazamientoBytes = buscarPracticaXnumero(nroPractica);
        fseek(archivo, desplazamientoBytes, SEEK_SET);
        fread(&registro, sizeof(Practica), 1, archivo);
        mostrarRegistroPractica(registro);
        printf(">Es esta la practica que quiere eliminar? (s/n): ");
        fflush(stdin);
        scanf("%c", &control);
        if(control != 's' && control != 'S'){ // si pones no
            valido = 0;
        }else if(buscarPracticaArbol(arbol,nroPractica) == 1){
            printf("Hay ingresos asociados con esa practica. No se puede borrar. \n");
            valido = 0;
        }
        if(valido == 0){
            printf(">Quiere intentar borrar una practica distinta? (s/n): ");
            fflush(stdin);
            scanf("%c", &control2);
            if(control2 != 's' && control2 != 'S'){
                return;
            }
        }
    }while(valido != 1);
    fseek(archivo, desplazamientoBytes, SEEK_SET);
    registro.eliminado = 1;
    fwrite(&registro,sizeof(Practica), 1, archivo);
    fclose(archivo);
}


/// Busca en cada Paciente para ver si el nroPractica existe en alguna de sus sublistas.
/// retorna un 1 o un 0 dependiendo de si existe o no.
int buscarPracticaArbol(nodoPaciente *arbol, int nroPractica){
    if(arbol){
        return buscarPracticaLista(arbol->listaIngreso,nroPractica) || buscarPracticaArbol(arbol->izq, nroPractica) || buscarPracticaArbol(arbol->der, nroPractica);
    }else{
        return 0;
    }
}

/// Busca en cada Ingreso para ver si el nroPractica existe en su sublista.
/// retorna un 1 o un 0 dependiendo de si existe o no.
int buscarPracticaLista(nodoIngreso *lista, int nroPractica){
    nodoIngreso *seg = lista;
    while(seg != NULL){
        if(buscarPracticaSub(seg->lista,nroPractica) == 1){
            return 1;
        }
        seg = seg->sig;
    }
    return 0;
}

/// Busca en cada PxI para ver si el nroPractica existe.
/// retorna un 1 o un 0 dependiendo de si existe o no.
int buscarPracticaSub(nodoPxI *subLista, int nroPractica){
    nodoPxI *seg = subLista;
    while(seg != NULL){
        if(seg->pracXing.nroPractica == nroPractica){
            return 1;
        }
        seg = seg->sig;
    }
    return 0;
}

/// Funcion principal de modificacion de practica
/// Busca la practica ingresada, la muestra para corroborar que se quiera modificar,
/// se lee el registro a modificar, se suplanta el campo de nombre y se reemplaza el registro
void modificacion_de_practica(char nombreArchivo[]){
    FILE *archivo = fopen(nombreArchivo, "r+b");
    char control;
    Practica registro;
    int desplazamientoBytes;
    int nroPractica;
    do{
        nroPractica = validarNroPractica(0);
        if(nroPractica == -1){
            return;
        }
        desplazamientoBytes = buscarPracticaXnumero(nroPractica);
        fseek(archivo, desplazamientoBytes, SEEK_SET);
        fread(&registro, sizeof(Practica), 1, archivo);
        mostrarRegistroPractica(registro);
        printf(">Es esta la practica que quiere reemplazar? (s/n): ");
        fflush(stdin);
        scanf("%c", &control);
    }while(control != 's' && control != 'S');
    char *nombre = validarNombrePractica();
    if(strcmp(nombre, "-1") == 0){
        return;
    }
    fseek(archivo, desplazamientoBytes, SEEK_SET);
    strcpy(registro.nombrePractica, nombre);
    fwrite(&registro,sizeof(Practica), 1, archivo);
    fclose(archivo);
}

/// Busca la practica que se quiere modificar (por num de practica),
/// hace las validaciones correspondientes al numero pedido,
/// retorna numero de bytes hasta ese registro del archivo.
int buscarPracticaXnumero(int nroPractica){
    int nroRegistro = NroRegistroXnroPractica("practicas.bin", nroPractica);
    return nroRegistro * sizeof(Practica);
}

/// Cuenta por cada registro hasta llegar a uno que coincida con el nroPractica ingresado
/// Es usada por la funcion buscarPracticaXnumero para poder saber el numero
/// de bytes necesarios hasta ese registro (para poder usar fseek en otra funcion)
int NroRegistroXnroPractica(char nombreArchivo[], int nroPractica){
    FILE *archivo = fopen(nombreArchivo, "rb");
    Practica registro;
    int i=0;
    if(archivo != NULL){
        while(!feof(archivo)){
            fread(&registro, sizeof(Practica), 1, archivo);
            if(!feof(archivo)){
                if(registro.nroPractica == nroPractica){
                    return i;
                }
                i++;
            }
        }
    }else{
        printf("Error. No se pudo abrir archivo en practicas.c/existePractica\n");
    }
    fclose(archivo);
    return -1;
}


/// Crea una practica nueva (mediante otra funcion) y la ingresa al archivo
void alta_de_practica(char nombreArchivo[]){
    Practica nueva;
    FILE *archivo = fopen(nombreArchivo, "ab");
    if(archivo != NULL){

        /// ASIGNA CAMPOS A 'nueva'
        nueva.nroPractica = validarNroPractica(1);
        if(nueva.nroPractica == -1){
            return;
        }
        strcpy(nueva.nombrePractica,validarNombrePractica());
        if(strcmp(nueva.nombrePractica, "-1") == 0){
            return;
        }

        nueva.eliminado = 0;
        /// ASIGNA CAMPOS A 'nueva'

        fwrite(&nueva, sizeof(Practica), 1, archivo);
    }else{
        printf("No se pudo abrir %s en practicas.c/alta_de_practica", nombreArchivo);
    }
    fclose(archivo);
}

/// Se pide una cadena de caracteres y se verifica si es valida (alfabetica) y
/// si existe o no en el archivo de practicas. Si es valida y no existe, se retorna.
/// Caso contrario, se pide la cadena de nuevo hasta que se cumplan los criterios.
char *validarNombrePractica(){
    char nombre[30];
    int valido;
    do{
        valido = 1;
        printf(">Ingrese el nombre de practica deseado: ");
        fflush(stdin);
        fgets(nombre, sizeof(nombre), stdin);
        nombre[strcspn(nombre, "\n")] = '\0';
        if(strcmp(nombre,"-1") == 0){
            printf("Accion cancelada. \n");
            return strdup("-1");
        }
        if(esNombrePracValido(nombre) == 0){
            printf("Error. El nombre ingresado contine caracteres no alfabeticos.\n\n");
            valido = 0;
        }else if(existeNombrePractica(nombre, "practicas.bin") == 1){
            printf("Error. El nombre de la practica ya existe. Ingrese uno distinto. \n\n");
            valido = 0;
        }
    }while(valido != 1);
    char *cadena = strdup(nombre);
    // strdup: asigna memoria dinamicamente y copia la cadena en la mem asignada.
    return cadena;
}

/// Verifica que cada caracter de la cadena sea valido (alfabetico)
/// Los espacios, comas y puntos, numeros, "/" y "-" no son considerados invalidos
/// Retorna 1 si la cadena es valida, 0 si no lo es.
int esNombrePracValido(const char *cadena){
    for (int i = 0; i < strlen(cadena); i++) {
        if (!isalpha(cadena[i]) && !isdigit(cadena[i]) && cadena[i] != ' ' && cadena[i] != ',' && cadena[i] != '.' && cadena[i] != '/' && cadena[i] != '-') {
        // isalpha() devuelve 0 si no es caracter numerico
            return 0;
        }
    }
    return 1;
}

/// Verifica si la cadena (ingresada en validarNombrePractica()
/// existe o no en el registro de practicas (archivo practicas.bin)
/// Devuelve 1 si existe, 0 si no existe.
int existeNombrePractica(const char *cadena, char nombreArchivo[]){
    FILE *archivo = fopen(nombreArchivo, "rb");
    Practica registro;
    if(archivo != NULL){
        while(!feof(archivo)){
            fread(&registro, sizeof(Practica), 1, archivo);
            if(!feof(archivo)){
                if(strcmpi(cadena, registro.nombrePractica) == 0){
                    return 1;
                }
            }
        }
    }else{
        printf("Error. No se pudo abrir archivo en practicas.c/existeNombre\n");
    }
    fclose(archivo);
    return 0;
}

/// Se pide un numero de practica y se verifica si es un entero valido y si
/// el numero de practica ya existe. Se tiene que ingresar un numero hasta que
/// ese numero sea valido mediante esos dos criterios.
int validarNroPractica(int error){
    int nroPractica;
    int valido;
    do{
        printf(">Ingrese el numero de practica deseado: ");
        fflush(stdin);
        valido = scanf("%i", &nroPractica);
        if(nroPractica == -1){
            printf("Accion cancelada. \n");
            return -1;
        }
        if(valido != 1){
            printf("Error. El numero de practica ingresado no es un entero valido. \n\n");
        }else{
            /// solucionar esta poronga
        }
    }while(valido != 1);
    return nroPractica;
}


/// Recorre el archivo de practicas para verificar si un numero de practica
/// ya existe o no. Es usada en validarNroPractica
/// Devuelve 1 si existe, 0 si no existe.
int existeNroPractica(char nombreArchivo[], int nroPractica){
    FILE *archivo = fopen(nombreArchivo, "rb");
    Practica registro;
    if(archivo != NULL){
        while(!feof(archivo)){
            fread(&registro, sizeof(Practica), 1, archivo);
            if(!feof(archivo)){
                if(registro.nroPractica == nroPractica){
                    return 1;
                }
            }
        }
    }else{
        printf("Error. No se pudo abrir archivo en practicas.c/existePractica\n");
    }
    fclose(archivo);
    return 0;
}

/// Funcion que muestra todos los registros de un archivo
/// de practicas
void mostrarArchivoPractica(char nombreArchivo[]){
    FILE *archivo = fopen(nombreArchivo, "rb");
    Practica registro;
    if(archivo != NULL){
        while(!feof(archivo)){
            fread(&registro, sizeof(Practica), 1, archivo);
            if(!feof(archivo)){
                mostrarRegistroPractica(registro);
            }
        }
    }else{
        printf("Error. No se pudo abrir %s en practicas.c/mostrarArchivoPractica\n", nombreArchivo);
    }
    fclose(archivo);
}

/// Muestra un registro del archivo de practicas
void mostrarRegistroPractica(Practica registro){
    printf("\n-------------------------------------\n");
    printf("Nro de practica: %i\n", registro.nroPractica);
    printf("Nombre de practica: %s\n", registro.nombrePractica);
    printf("Eliminado: %i\n", registro.eliminado);
    printf("-------------------------------------\n");
}
/// -------------------------------------------------- FUNCIONES DE PRACTICAS -------------------------------------------------- ///
