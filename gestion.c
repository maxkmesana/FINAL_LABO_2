#include <stdio.h>
#include <stdlib.h>
#include<ctype.h> // para usar funcion isalpha() / isdigit()
#include<string.h>
#include<time.h>
#include"gestion.h"

#define ARCHIVO_EMPLEADOS "archivoEmpleado.bin"


int nroIngresoGlobal;
int nroPxIGlobal;

void menu(){
    nodoPaciente *arbol = NULL;
    filesToEstructuras(&arbol);
    int opcion;
    do{
        system("cls");
        printf("** LABORATORIO CLINICO **\n");
        printf("-------------------------\n\n");
        printf("1- Acceder al sistema. \n");
        printf("2- Salir. \n");
        printf(">Ingrese una opcion: ");
        fflush(stdin);
        scanf("%i", &opcion);
        switch(opcion){
        case 1:
            menuPrincipal();
            break;
        case 2:
            persistenciaTotal(arbol);
            exit(0);
            break;
        default:
            printf("Opcion ingresada no es valida. Intente nuevamente. \n");
            system("pause");
            system("cls");
            break;
        }
    }while(opcion != 3);
}



/// -------------------------------------------------- FUNCIONES ADICIONALES -------------------------------------------------- ///


void listarPracticasPorInicio(char nombreArchivo[], Empleado empleadoLog) {
    int flag = 0; /// se pone en 1 si al menos un registro se encontro.
    Practica registro;
    char *buscada = validarNombrePractica2();
    if(strcmp(buscada, "-1") == 0){
        return;
    }
    FILE *archivo = fopen(nombreArchivo, "rb");
    if(archivo){
        while(fread(&registro, sizeof(registro), 1, archivo) == 1){
            if(cadenaContenida(registro.nombrePractica, buscada) == 1){
                if(empleadoLog.nroPerfil == 0 || (empleadoLog.nroPerfil != 0 && registro.eliminado == 0)){
                    flag = 1;
                    mostrarRegistroPractica(registro);
                }
            }
        }
        if(flag == 0){
            printf("\nNo se encontraron practicas que comiencen con lo ingresado. \n");
        }
    }else{
        printf("No se pudo abrir el archivo en listarPracticasPorInicio. \n");
    }
}

int cadenaContenida(const char* haystack, const char* needle){
    int i = 0;
    while(i<strlen(needle) && needle[i] != '\0' && haystack[i] != '\0'){
        if(tolower(haystack[i]) != tolower(needle[i])){
            return 0;
        }
        i++;
        haystack++;
        needle++;
    }
    return 1;
}

char *validarNombrePractica2()
{
    char nombre[30];
    int valido;
    do
    {
        valido = 1;
        printf(">Ingrese el nombre de practica deseado: ");
        fflush(stdin);
        gets(nombre);
        if(strcmp(nombre,"-1") == 0)
        {
            printf("Accion cancelada. \n");
            return strdup("-1");
        }
        if (strlen(nombre) > 30)
        {
            printf("Error. El nombre de la practica no debe superar los 30 caracteres.\n\n");
            valido = 0;
        }
        else if(esNombrePracValido(nombre) == 0)
        {
            printf("Error. El nombre ingresado contine caracteres no alfabeticos.\n\n");
            valido = 0;
        }
    }
    while(valido != 1);
    char *cadena = strdup(nombre);
    // strdup: asigna memoria dinamicamente y copia la cadena en la mem asignada.
    return cadena;
}

void testearListaPrac(Empleado empleadoLog)
{
    nodoListaPrac * listaOrdenadaPrac =NULL;
    listaOrdenadaPrac=cargarPracArchivo(listaOrdenadaPrac);
    mostrarListaPrac(listaOrdenadaPrac,empleadoLog);

}


void mostrarListaPrac(nodoListaPrac* lista, Empleado empleadoLog)
{
    nodoListaPrac* seg = lista;

    while (seg != NULL)
    {
        if(empleadoLog.nroPerfil == 0 || (empleadoLog.nroPerfil != 0 && seg->dato.eliminado == 0)){
            printNodoPractica(seg);
        }
        seg = seg->sig;
    }
}

void printNodoPractica(nodoListaPrac *nodo){
    printf("\n-------------------------------------\n");
    printf("Nro de practica: %i\n", nodo->dato.nroPractica);
    printf("Nombre de practica: %s\n", nodo->dato.nombrePractica);
    printf("Eliminado: %i\n", nodo->dato.eliminado);
    printf("---------------------------------------\n");
}

nodoListaPrac *crearNodoPrac(Practica nuevaPractica)
{
    nodoListaPrac* nuevoNodo=(nodoListaPrac*)malloc(sizeof(nodoListaPrac));
    nuevoNodo->dato=nuevaPractica;
    nuevoNodo->sig=NULL;
    return nuevoNodo;
}


nodoListaPrac* cargarPracArchivo(nodoListaPrac* listaOrdenadaPrac)
{
    Practica practicaArchivo;
    FILE* archivo=fopen("practicas.bin","rb");
    if(archivo!=NULL)
    {
        while(!feof(archivo))
        {
            fread(&practicaArchivo,sizeof(Practica),1,archivo);
            if(!feof(archivo))
            {
                listaOrdenadaPrac=agregarPracOrdenada(listaOrdenadaPrac,practicaArchivo);

            }
        }
        fclose(archivo);
    }
    return listaOrdenadaPrac;
}


nodoListaPrac* agregarPracOrdenada(nodoListaPrac* listaOrdenadaPrac, Practica nuevaPrac)
{
    nodoListaPrac* nuevoNodo=crearNodoPrac(nuevaPrac);
    nodoListaPrac* seg=listaOrdenadaPrac;
// si la lista esta vacia o el empleado a agregar va al principio
    if(listaOrdenadaPrac==NULL || strcmpi(nuevaPrac.nombrePractica, listaOrdenadaPrac->dato.nombrePractica) < 0)
    {
        nuevoNodo->sig = listaOrdenadaPrac;//si esta vacia queda apuntando a null y si hay algo apunta al nodo que estaba primero
        listaOrdenadaPrac=nuevoNodo;
    }
    else
    {
        while(seg->sig!=NULL && strcmpi(nuevaPrac.nombrePractica, seg->sig->dato.nombrePractica) >= 0)  //busca la posicion correcta donde deberia ser insertado
        {
            seg=seg->sig;
        }
        nuevoNodo->sig = seg->sig;
        seg->sig = nuevoNodo;
    }
    return listaOrdenadaPrac;
}


void printPacientesIngresos(nodoPaciente *arbol, Empleado empleadoLog){
/// NO TIENE QUE PRINTEAR LAS PXI
    if(arbol){
        printPacientesIngresos(arbol->izq, empleadoLog);
        printNodoPaciente2(arbol, empleadoLog);
        printListaIngreso2(arbol->listaIngreso, empleadoLog);
        printPacientesIngresos(arbol->der, empleadoLog);
    }
}

void printListaIngreso2(nodoIngreso *listaIngreso, Empleado empleadoLog){
    nodoIngreso *seg = listaIngreso;
    while(seg)
    {
        if(empleadoLog.nroPerfil == 0 || (empleadoLog.nroPerfil != 0 && seg->datoPcte.eliminado == 0)){
            printNodoIngreso(seg);
            printf("\t ---------------------------------------------------------\n");
        }
        seg = seg->sig;
    }
}

void printNodoPaciente2(nodoPaciente *nodo, Empleado empleadoLog)
{
        if(empleadoLog.nroPerfil == 0 || (empleadoLog.nroPerfil != 0 && nodo->datoPaciente.eliminado == 0)){
            printf("\n//////////////////////////// ** PACIENTE ** ////////////////////////////\n");
            printf("- Apellido nombre: %s\n",nodo->datoPaciente.apellidoNombre);
            printf("- Direccion: %s\n",nodo->datoPaciente.direccion);
            printf("- DNI: %i\n",nodo->datoPaciente.dni);
            printf("- Edad: %i\n",nodo->datoPaciente.edad);
            printf("- Eliminado: %i\n",nodo->datoPaciente.eliminado);
            printf("- Telefono: %s\n",nodo->datoPaciente.telefono);
        }
}


void buscarPacientePorDni(nodoPaciente *arbol, Empleado empleadoLog){
    system("cls");
    int flag = 0;
    int dni = validarDni0(arbol);
    if(dni == -1){
        return;
    }
    nodoPaciente *buscado = existePacienteNodo(arbol, dni);
    if(empleadoLog.nroPerfil == 0 || (empleadoLog.nroPerfil != 0 && buscado->datoPaciente.eliminado == 0)){
        printNodoPaciente(buscado);
        flag = 1;
    }
    printParcial(buscado->listaIngreso, empleadoLog);
    if(flag == 0){
        printf("No se encontraron registros disponibles para el paciente buscado. \n");
    }
}


void ordenarArbolAlfabetico(nodoPaciente *arbol, nodoPaciente **destino){
    if(arbol){
        ordenarArbolAlfabetico(arbol->izq, destino);
        insertarPacienteAlfabetico(destino,arbol);
        ordenarArbolAlfabetico(arbol->der, destino);
    }
}

void insertarPacienteAlfabetico(nodoPaciente **ordenado, nodoPaciente *nodoActual) {
    if (*ordenado == NULL) {
        *ordenado = crearNodoPacienteAlf(nodoActual->datoPaciente, nodoActual->listaIngreso);
    } else {
        int comparacion = strcmpi(nodoActual->datoPaciente.apellidoNombre, (*ordenado)->datoPaciente.apellidoNombre);

        if (comparacion > 0) {
            insertarPacienteAlfabetico(&(*ordenado)->der, nodoActual);
        } else if (comparacion < 0) {
            insertarPacienteAlfabetico(&(*ordenado)->izq, nodoActual);
        }
    }
}

nodoPaciente *crearNodoPacienteAlf(Paciente dato, nodoIngreso *listaIngreso)
{
    nodoPaciente *nuevo = (nodoPaciente*)malloc(sizeof(nodoPaciente));
    nuevo->datoPaciente = dato;
    nuevo->izq = NULL;
    nuevo->der = NULL;
    nuevo->listaIngreso = listaIngreso;
    return nuevo;
}

void menuBuscarIngreso(Empleado empleadoLog){
    nodoPaciente *arbol = NULL;
    filesToEstructuras(&arbol);
    int opcion;
    int nroIngreso;
    int dni;
    do
    {
        system("cls");
        printf("** BUSQUEDA DE INGRESO **\n");
        printf("-------------------------\n\n");
        printf("1- Buscar por numero de ingreso. \n");
        printf("2- Buscar por fecha de ingreso. \n");
        printf("3- Buscar por DNI de paciente. \n");
        printf("4- Volver. \n");
        printf("5- Salir. \n");
        printf(">Ingrese una opcion: ");
        fflush(stdin);
        scanf("%i",&opcion);

        switch(opcion)
        {
        case 1:
            nroIngreso = validarNroIngreso();
            if(nroIngreso == -1){
                break;
            }
            printIngresosXnro("ingresos.bin", nroIngreso, empleadoLog);
            system("pause");
            break;
        case 2:
            printIngresosXfecha("ingresos.bin", empleadoLog);
            system("pause");
            break;
        case 3:
            dni = validarDni0(arbol);
            if(dni == -1){
                break;
            }
            printIngresosXdni("ingresos.bin", empleadoLog, dni);
            system("pause");
            break;
        case 4:
            persistenciaTotal(arbol);
            return;
            break;
        case 5:
            persistenciaTotal(arbol);
            exit(0);
            break;
        default:
            printf("La opcion ingresada es incorrecta.\n");
            system("pause");
            break;

        }
    }
    while(opcion != 4); // opcion distinto a volver
}

void printIngresosXdni(char nombreArchivo[], Empleado empleadoLog, int dni){
    system("cls");
    FILE *archivo = fopen(nombreArchivo, "rb");
    stIngresos registro;
    int flag = 0; // se pone en 1 si encuentra al menos un registro
    // con el mismo DNI que el parametro

    if(archivo != NULL){
        while(!feof(archivo)){
            fread(&registro, sizeof(stIngresos), 1, archivo);
            if(!feof(archivo)){
                if(empleadoLog.nroPerfil == 0 || (empleadoLog.nroPerfil != 0 && registro.eliminado == 0)){
                    if(registro.dniPcte == dni){
                    mostrarRegistroIng(registro);
                    printPxIRegistros("pracXing.bin", registro.nroIngreso);
                    flag = 1;
                    }
                }
            }
        }
        if(flag == 0){
            printf("No se encontraron registros con ese DNI.\n");
        }
    }else{
        printf("No se pudo abrir el archivo %s en printIngresosXdni. \n", nombreArchivo);
    }
    fclose(archivo);
}

void printIngresosXfecha(char nombreArchivo[], Empleado empleadoLog){
    system("cls");
    FILE *archivo = fopen(nombreArchivo, "rb");
    stIngresos registro;
    int flag = 0; // se pone en 1 si encuentra al menos un registro
    // con la misma fecha que la buscada


    char* fechaBuscada = (char*)malloc(sizeof(char)*10);
    printf("Que fecha de ingresos quiere buscar? \n");
    fechaBuscada = validarFechaRandom();
    if(strcmp(fechaBuscada, "-1") == 0)
    {
        return;
    }

    struct tm tmFechaBuscada = convertirFecha(fechaBuscada);

    if(archivo != NULL){
        while (fread(&registro, sizeof(stIngresos), 1, archivo) == 1)
        {
            struct tm tmFechaRegistro = convertirFecha(registro.fechaIngreso);

            if(empleadoLog.nroPerfil == 0 || (empleadoLog.nroPerfil != 0 && registro.eliminado == 0)){
                if ((difftime(mktime(&tmFechaRegistro), mktime(&tmFechaBuscada)) == 0))
                {
                    mostrarRegistroIng(registro);
                    printPxIRegistros("pracXing.bin", registro.nroIngreso);
                    flag = 1;
                }
            }
        }
        if(flag == 0){
            printf("No se encontraron registros con esa fecha de ingreso.\n");
        }
    }else{
        printf("No se pudo abrir el archivo %s en printIngresosXfecha. \n", nombreArchivo);
    }
    fclose(archivo);
}

void printIngresosXnro(char nombreArchivo[], int nroIngreso, Empleado empleadoLog){
    system("cls");
    FILE *archivo = fopen(nombreArchivo, "rb");
    stIngresos registro;
    int flag = 0; // se pone en 1 si encuentra al menos un registro
    // con el mismo nroIngreso que el parametro.
    if(archivo != NULL){
        while(!feof(archivo)){
            fread(&registro, sizeof(stIngresos), 1, archivo);
            if(!feof(archivo)){
                if(empleadoLog.nroPerfil == 0 || (empleadoLog.nroPerfil != 0 && registro.eliminado == 0)){
                    if(registro.nroIngreso == nroIngreso){
                    mostrarRegistroIng(registro);
                    printPxIRegistros("pracXing.bin", nroIngreso);
                    flag = 1;
                    }
                }
            }
        }
        if(flag == 0){
            printf("No se encontraron registros con ese numero de ingreso.\n");
        }
    }else{
        printf("No se pudo abrir el archivo %s en printIngresosXnro. \n", nombreArchivo);
    }
    fclose(archivo);
}

void printPxIRegistros(char nombreArchivo[], int nroIngreso){
    FILE *archivo = fopen(nombreArchivo, "rb");
    PxI registro;
    if(archivo != NULL){
        while(!feof(archivo)){
            fread(&registro, sizeof(PxI), 1, archivo);
            if(!feof(archivo)){
                if(registro.nroIngreso == nroIngreso){
                    printf("\t----------------------------\n");
                    printf("\t- Numero de practica: %i\n", registro.nroPractica);
                    printf("\t- Nombre de practica: %s\n", buscarNombrePractica("practicas.bin",registro.nroPractica));
                    printf("\t- Resultado: %s\n", registro.resultado);
                }
            }
        }
    }else{
        printf("No se pudo abrir el archivo %s en printPxIRegistros. \n", nombreArchivo);
    }
    fclose(archivo);
}

char *buscarNombrePractica(char nombreArchivo[], int nroPractica){
    FILE *archivo = fopen(nombreArchivo, "rb");
    Practica registro;
    if(archivo != NULL){
        while(!feof(archivo)){
            fread(&registro, sizeof(Practica), 1, archivo);
            if(!feof(archivo)){
                if(registro.nroPractica == nroPractica){
                    return strdup(registro.nombrePractica);
                }
            }
        }
    }else{
        printf("No se pudo abrir el archivo %s en buscarNombrePractica. \n", nombreArchivo);
    }
    fclose(archivo);
    return strdup("-1");
}

void ingresosDesdeHasta()
{
    stIngresos registro;
    char* fechaDesde = (char*)malloc(sizeof(char)*10);
    char* fechaHasta = (char*)malloc(sizeof(char)*10);
    system("cls");
    printf("A partir de que fecha de ingreso quiere mostrar? \n");
    fechaDesde = validarFechaRandom();
    if(strcmp(fechaDesde, "-1") == 0)
    {
        return;
    }

    printf("Hasta que fecha de ingreso quiere mostrar? \n");
    fechaHasta = validarFechaRandom();
    if(strcmp(fechaDesde, "-1") == 0)
    {
        return;
    }
    struct tm tmFechaDesde = convertirFecha(fechaDesde);
    struct tm tmFechaHasta = convertirFecha(fechaHasta);
    FILE *archivo = fopen("ingresos.bin", "rb");
    if(archivo != NULL)
    {
        while (fread(&registro, sizeof(stIngresos), 1, archivo) == 1)
        {
            struct tm tmFechaIngreso = convertirFecha(registro.fechaIngreso);

            /// primera condicion: se fija si fecha de ingreso es mayor o igual a fecha desde
            /// segunda condicion: se fija si fecha de ingreso es menor o igual a fecha hasta
            if ((difftime(mktime(&tmFechaIngreso), mktime(&tmFechaDesde)) >= 0) && (difftime(mktime(&tmFechaIngreso), mktime(&tmFechaHasta)) <= 0))
            {
                mostrarRegistroIng(registro);
            }
        }

    }
    else
    {
        printf("No se pudo abrir archivo ingresos.bin en funcion ingresosDesdeHasta. \n");
        return;
    }
    fclose(archivo);
}

/// -------------------------------------------------- FUNCIONES ADICIONALES -------------------------------------------------- ///




///---------------------LOG IN---------------------------------///

Empleado logIn()
{
    int flag=0;
    int flag2=0;
    char *usuario;
    char *contra;
    Empleado empleadoLogueado;
    do
    {
        usuario = validarUsuario0();
        if(strcmp(usuario, "-1") == 0){
            empleadoLogueado.nroPerfil = -1;
            return empleadoLogueado;
        }
        empleadoLogueado= buscarUsuarioLog(usuario,&flag);
        if(flag==1){
            do
            {
                contra = validarContra();
                if(strcmp(contra, "-1") == 0){
                    empleadoLogueado.nroPerfil = -1;
                    return empleadoLogueado;
                }
                if(strcmp(empleadoLogueado.contra,contra)==0)
                {
                    flag2=1;
                    printf("Logueado con exito \n");
                    system("pause");
                }
                else
                {
                    printf("Contrasenia incorrecta, vuelve a intentarlo. \n");
                }
            }
            while(flag2==0);
        }
        else
        {
            printf("El nombre de usuario no se encuentra vinculado a ningun empleado, vuelve a intentarlo \n");
            system("pause");
        }


    }
    while(flag==0);

    return empleadoLogueado;

}

///---------------------------- MENU PRINCIPAL ---------------------------------------------------///
void menuPrincipal()
{
    system("cls");
    Empleado empleadoLogueado;
    int opcion;
    empleadoLogueado = logIn();
    opcion = empleadoLogueado.nroPerfil;
    switch(opcion)
    {
    case 0:
        menuAdministrador(empleadoLogueado);
        break;
    case 1:
        menuProfesionales(empleadoLogueado);
        break;
    case 2:
        menuAdministrativos(empleadoLogueado);
        break;
    case -1:
        system("pause");
        return;
        break;
    default:
        printf("Hubo un error en el login. Volviendo al menu principal. \n");
        system("pause");
        return;
    }
}

///buscar usuario
Empleado buscarUsuarioLog(char usuario[20], int* flag)
{
    Empleado empleadoArchivo;

    FILE*archivo=fopen(ARCHIVO_EMPLEADOS,"rb");
    if(archivo!=NULL)
    {
        while(!feof(archivo)&&*flag==0)
        {
            fread(&empleadoArchivo,sizeof(Empleado),1,archivo);
            if(!feof(archivo))
            {
                if(strcmpi(usuario,empleadoArchivo.usuario)==0)
                {
                    *flag=1;
                }
            }
        }
        fclose(archivo);
    }
    return empleadoArchivo;
}

//validarUsuario()

///--------------------menu profesional---------------------------------///
void menuProfesionales(Empleado empleadoLog)
{
    nodoPaciente* arbol=NULL;
    filesToEstructuras(&arbol);
    int opcion;
    do
    {
        system("cls");
        printf("** MENU PROFESIONALES **\n");
        printf("--------------------\n\n");
        printf("1- Modificar una practica por ingreso. \n");
        printf("2- Menu de practicas. \n");
        printf("3- Mostrar base de datos completa. \n");
        printf("4- Volver. \n");
        printf("5- Salir. \n");
        printf(">Ingrese una opcion: ");
        fflush(stdin);
        scanf("%i",&opcion);

        switch(opcion)
        {
        case 1:
            modificacion_de_PxI(arbol, empleadoLog);
            break;
        case 2:
            menuPracticasRestringido(arbol, empleadoLog);
            break;
        case 3:
            printTotal(arbol,empleadoLog);
            system("pause");
            break;
        case 4:
            persistenciaTotal(arbol);
            return;
            break;
        case 5:
            persistenciaTotal(arbol);
            exit(0);
            break;
        default:
            printf("La opcion ingresada es incorrecta.\n");
            system("pause");
            break;

        }
    }
    while(opcion != 4); // opcion distinto a volver
}
/// ------------------------------ menu administrativo ---------------------------------------///
void menuAdministrativos(Empleado empleadoLog)
{
    int opcion;
    int dni;
    nodoPaciente *arbol= NULL;
    filesToEstructuras(&arbol);
    nodoPaciente *nodovalido = NULL;
    do
    {
        system("cls");
        printf("** MENU ADMINISTRATIVOS **\n");
        printf("--------------------------\n\n");
        printf("1- Gestion de pacientes. \n");
        printf("2- Gestion de ingresos. \n");
        printf("3- Alta de practicas por ingreso. \n");
        printf("4- Baja de practicas por ingreso. \n");
        printf("5- Menu de practicas. \n");
        printf("6- Mostrar base de datos completa. \n");
        printf("7- Volver. \n");
        printf("8- Salir. \n");
        printf(">Ingrese una opcion: ");
        fflush(stdin);
        scanf("%i",&opcion);

        switch(opcion)
        {
        case 1:
            menuGestionPacientes(arbol, empleadoLog);
            break;
        case 2:
            menuGestionIngresos(arbol, empleadoLog);
            break;
        case 3:
            system("cls");
            dni = validarDni0(arbol);
            if(dni == -1){
                break;
            }
            nodovalido = existePacienteNodo(arbol, dni);
            if(nodovalido->listaIngreso != NULL){
                printListaIngreso2(nodovalido->listaIngreso, empleadoLog);
                alta_de_PxIAdministrativo(nodovalido);
            }else{
                printf("El paciente no contiene ingresos. \n");
                system("pause");
                break;
            }
            break;
        case 4:
            baja_de_pXi(arbol, empleadoLog);
            break;
        case 5:
            menuPracticasRestringido(arbol, empleadoLog);
            break;
        case 6:
            printTotal(arbol,empleadoLog);
            system("pause");
            break;
        case 7:
            persistenciaTotal(arbol);
            return;
            break;
        case 8:
            persistenciaTotal(arbol);
            exit(0);
            break;
        default:
            printf("La opcion ingresada es incorrecta.\n");
            system("pause");
            break;
        }
    }while(opcion != 7); // opcion distinto a volver

}


///FUNCIONES DE EMPLEADO
void crearAdministrador()  // ejecutarla 1 sola vez para cargar los perfiles
{
    Empleado admin1;
    strcpy(admin1.apellidoNombre,"Aroca Adrian");
    strcpy(admin1.telefono,"2231564521");
    strcpy(admin1.usuario,"adrian");
    strcpy(admin1.contra,"comision6");
    strcpy(admin1.perfil,"administrador");
    admin1.nroPerfil= 0;


    FILE* archivo=fopen(ARCHIVO_EMPLEADOS,"wb");
    if(archivo!=NULL)
    {
        fwrite(&admin1,sizeof(Empleado),1,archivo);

    }
    else
    {
        printf("no se pude abrir el archivo correctamente\n");
    }
    fclose(archivo);

}

int esUsuarioValido(const char *cadena)
{
    for (int i = 0; i < strlen(cadena); i++)
    {
        if (!isalpha(cadena[i]) && !isdigit(cadena[i]) && cadena[i] != '-' && cadena[i] != '_' && cadena[i] != '.')
        {
            // isalpha() devuelve 0 si no es caracter numerico
            return 0;
        }
    }
    return 1;
}

Empleado cargarEmpleadoTeclado()
{
    Empleado nuevoEmpleado;
    system("cls");
    printf("** CREACION DE USUARIO **\n");
    printf("-------------------------\n\n");

    nuevoEmpleado.dni=validarDniEmpleado1();
    if(nuevoEmpleado.dni == -1){
        return nuevoEmpleado;
    }

    strncpy(nuevoEmpleado.apellidoNombre,validarNombreEmpleado(), sizeof(nuevoEmpleado.apellidoNombre));
    if(strcmp(nuevoEmpleado.apellidoNombre, "-1") == 0){
        return nuevoEmpleado;
    }

    strncpy(nuevoEmpleado.telefono, validarTelefonoPaciente(), sizeof(nuevoEmpleado.telefono));
    if(strcmp(nuevoEmpleado.telefono, "-1") == 0){
        return nuevoEmpleado;
    }

    strncpy(nuevoEmpleado.usuario,validarUsuario1(), sizeof(nuevoEmpleado.usuario));
    if(strcmp(nuevoEmpleado.usuario, "-1") == 0){
        return nuevoEmpleado;
    }

    strncpy(nuevoEmpleado.contra, validarContra(), sizeof(nuevoEmpleado.contra));
    if(strcmp(nuevoEmpleado.contra, "-1") == 0){
        return nuevoEmpleado;
    }

    nuevoEmpleado.nroPerfil = eleccionPerfil();
    if(nuevoEmpleado.nroPerfil == -1){
        return nuevoEmpleado;
    }

    if(nuevoEmpleado.nroPerfil == 1)
    {
        strcpy(nuevoEmpleado.perfil,"profesional");
    }
    else if(nuevoEmpleado.nroPerfil==2)
    {
        strcpy(nuevoEmpleado.perfil,"administrativo");
    }

    return nuevoEmpleado;
}

int eleccionPerfil() {
    int nroPerfil;
    do {
        printf("1- Profesional de Laboratorio.\n");
        printf("2- Administrativo.\n");
        printf("Ingrese el tipo de perfil: ");
        fflush(stdin);
        scanf("%i", &nroPerfil);

        if(nroPerfil == -1){
            return -1;
        }
        if (nroPerfil != 1 && nroPerfil != 2) {
            printf("Opción no válida. Por favor, ingrese '1' o '2'.\n");
        }

    } while (nroPerfil != 1 && nroPerfil != 2);

    return nroPerfil;
}

int esEmpleadoValido(Empleado registro)
{
    if(registro.dni == -1)
    {
        return 0;
    }
    if(strcmp(registro.apellidoNombre, "-1") == 0)
    {
        return 0;
    }
    if(strcmp(registro.telefono, "-1") == 0)
    {
        return 0;
    }
    if(strcmp(registro.usuario, "-1") == 0){
        return 0;
    }
    if(strcmp(registro.contra, "-1") == 0){
        return 0;
    }
    if(registro.nroPerfil == -1){
        return 0;
    }
    return 1;
}

char *validarContra()
{
    char contra[20];
    int valido;
    do
    {
        valido = 1;
        printf(">Ingrese la contrasenia deseada: ");
        fflush(stdin);
        gets(contra);
        if(strcmp(contra,"-1") == 0)
        {
            return strdup("-1");
        }
        if(strlen(contra) < 8){
            printf("Error. La contrasenia no puede tener menos de 8 caracteres.\n\n");
            valido = 0;
        }else if (strlen(contra) > 20){
            printf("Error. La contrasenia no debe superar los 20 caracteres.\n\n");
            valido = 0;
        }else if(esContraValida(contra) == 0){
            printf("Error. La contrasenia contiene caracteres no validos. \n");
            valido = 0;
        }
    }
    while(valido != 1);
    char *cadena = strdup(contra);
    // strdup: asigna memoria dinamicamente y copia la cadena en la mem asignada.
    return cadena;
}

int esContraValida(const char *cadena)
{
    for (int i = 0; i < strlen(cadena); i++)
    {
        if (!isalnum(cadena[i]) && !isspace(cadena[i]) && !strchr(",./-!@#$%^&*()_+=[]{};:'\"<>?\\|`~", cadena[i])){
            // isalpha() devuelve 0 si no es caracter numerico
            return 0;
        }
    }
    return 1;
}

char *validarNombreEmpleado()
{
    char nombre[40];
    int valido;
    do
    {
        valido = 1;
        printf(">Ingrese el nombre y apellido deseado: ");
        fflush(stdin);
        gets(nombre);
        if(strcmp(nombre, "-1") == 0)
        {
            return strdup("-1");
        }
        if (strlen(nombre) > 40)
        {
            printf("Error. El nombre del empleado no debe superar los 40 caracteres.\n\n");
            valido = 0;
        }
        else if(esNombreValido(nombre) == 0)
        {
            printf("Error. El nombre ingresado contine caracteres no alfabeticos.\n\n");
            valido = 0;
        }
    }
    while(valido != 1);
    char *cadena = strdup(nombre);
    // strdup: asigna memoria dinamicamente y copia la cadena en la mem asignada.
    return cadena;
}

char *validarUsuario0()
{
    char usuario[20];
    int valido;
    do
    {
        valido = 1;
        printf(">Ingrese el nombre de usuario: ");
        fflush(stdin);
        gets(usuario);

        if(strcmp(usuario, "-1") == 0)
        {
            printf("Accion cancelada. Volviendo al menu.\n");
            return strdup("-1");
        }
        if(buscarUsuario(usuario)==0)
        {
            printf("Error. El usuario no existe.\n");
            valido=0;
            continue;
        }
        if (strlen(usuario) > 20)
        {
            printf("Error. El usuario no debe superar los 20 caracteres.\n");
            valido = 0;
            continue;
        }else if (strlen(usuario) < 4)
        {
            printf("Error. El usuario no puede ser menor de 4 caracteres.\n");
            valido = 0;
            continue;
        }else if(esUsuarioValido(usuario) == 0){
            printf("Error. El nombre ingresado contine caracteres no validos.\n");
            valido = 0;
            continue;
        }
    }
    while(valido != 1);
    char *cadena = strdup(usuario);
    return cadena;
}

///validar usuario
char *validarUsuario1()
{
    char usuario[20];
    int valido;
    do
    {
        valido = 1;
        printf(">Ingrese el nombre de usuario: ");
        fflush(stdin);
        gets(usuario);

        if(strcmp(usuario, "-1") == 0)
        {
            return strdup("-1");
        }
        if(buscarUsuario(usuario)==1)
        {
            printf("Error, el nombre de usuario ya esta en uso.\n");
            valido=0;
            continue;
        }
        if (strlen(usuario) > 20)
        {
            printf("Error. El usuario no debe superar los 20 caracteres.\n");
            valido = 0;
            continue;
        }else if (strlen(usuario) < 4)
        {
            printf("Error. El usuario no puede ser menor de 4 caracteres.\n");
            valido = 0;
            continue;
        }else if(esUsuarioValido(usuario) == 0){
            printf("Error. El nombre ingresado contine caracteres no validos.\n");
            valido = 0;
            continue;
        }
    }
    while(valido != 1);
    char *cadena = strdup(usuario);
    return cadena;
}

/// buscar usuario
int buscarUsuario(char usuario[20])
{
    Empleado empleadoArchivo;
    int flag=0;
    FILE*archivo=fopen(ARCHIVO_EMPLEADOS,"rb");
    if(archivo!=NULL)
    {
        while(!feof(archivo)&&flag==0)
        {
            fread(&empleadoArchivo,sizeof(Empleado),1,archivo);
            if(!feof(archivo))
            {
                if(strcmpi(usuario,empleadoArchivo.usuario)==0)
                {
                    flag=1;
                }
            }
        }
        fclose(archivo);
    }
    return flag;
}
///busacr empleado por dni// Detalle de funcionalidad adicional: [30 puntos]

int buscarEmpleado(int dni)
{
    int flag= 0;
    Empleado empleadoArchivo;
    FILE *archivo=fopen(ARCHIVO_EMPLEADOS,"rb");
    if(archivo!=NULL)
    {
        while(!feof(archivo)&&flag==0)
        {
            fread(&empleadoArchivo,sizeof(Empleado),1,archivo);
            if(!feof(archivo))
            {
                if(dni==empleadoArchivo.dni)
                {
                    flag=1;
                }

            }
        }
        fclose(archivo);
    }
    return flag;
}

int validarDniEmpleado0()
{
    int nroDni;
    int valido;
    do
    {
        printf(">Ingrese el numero de DNI deseado: ");
        fflush(stdin);
        valido = scanf("%i", &nroDni);
        if(nroDni== -1)
        {
            return -1;
        }
        if(valido != 1)
        {
            printf("Error. El numero de DNI ingresado no es un entero valido. \n\n");
        }
        else if(buscarEmpleado(nroDni) == 0)
        {
            printf("Error. El empleado no existe. Ingrese uno distinto.\n\n");
            valido = 0;
        }
        if (nroDni <= 9999 || nroDni > 99999999)
        {
            printf("Error. El DNI debe tener entre 5 y 8 digitos. Ingrese uno valido.\n\n");
            valido = 0;
        }
    }
    while(valido != 1);
    return nroDni;
}

int validarDniEmpleado1()
{
    int nroDni;
    int valido;
    do
    {
        printf(">Ingrese el numero de DNI deseado: ");
        fflush(stdin);
        valido = scanf("%i", &nroDni);
        if(nroDni== -1)
        {
            return -1;
        }
        if(valido != 1)
        {
            printf("Error. El numero de DNI ingresado no es un entero valido. \n\n");
        }
        else if(buscarEmpleado(nroDni) == 1)
        {
            printf("Error. El empleado ya existe. Ingrese uno distinto.\n\n");
            valido = 0;
        }
        if (nroDni <= 9999 || nroDni > 99999999)
        {
            printf("Error. El DNI debe tener entre 5 y 8 digitos. Ingrese uno valido.\n\n");
            valido = 0;
        }
    }
    while(valido != 1);
    return nroDni;
}
///----------------------------------  MENÚ PARA ADMINISTRADORES --------------------------------------------------------------------///
void menuAdministrador(Empleado empleadoLog)
{
    int opcion;

    nodoPaciente *arbol= NULL;
    filesToEstructuras(&arbol);
    do
    {
        system("cls");
        printf("** MENU ADMINISTRADOR **\n");
        printf("------------------------\n\n");
        printf("1- Crear nuevo usuario. \n");
        printf("2- Gestion de empleados. \n");
        printf("3- Gestion de pacientes. \n");
        printf("4- Gestion de practicas. \n");
        printf("5- Gestion de ingresos. \n");
        printf("6- Gestion de practicas por ingresos. \n");
        printf("7- Mostrar base de datos completa. \n");
        printf("8- Volver. \n");
        printf("9- Salir. \n");
        printf(">Ingrese una opcion: ");
        fflush(stdin);
        scanf("%i",&opcion);

        switch(opcion)
        {
        case 1:
            alta_de_empleado();
            break;
        case 2:
            menuGestionEmpleados(arbol, empleadoLog);
            break;
        case 3:
            menuGestionPacientes(arbol, empleadoLog);
            break;
        case 4:
            menuGestionPracticas(arbol, empleadoLog);
            break;
        case 5:
            menuGestionIngresos(arbol, empleadoLog);
            break;
        case 6:
            menuGestionPxI(arbol, empleadoLog);
            break;
        case 7:
            printTotal(arbol,empleadoLog);
            system("pause");
            break;
        case 8:
            persistenciaTotal(arbol);
            return;
            break;
        case 9:
            persistenciaTotal(arbol);
            exit(0);
            break;
        default:
            printf("La opcion ingresada es incorrecta.\n");
            system("pause");
            break;
        }
    }
    while(opcion != 8);

}

///menu gestion de practicas por ingresos
void menuGestionPxI(nodoPaciente*arbol, Empleado empleadoLog)
{
    int opcion;
    nodoPaciente *nodovalido;
    int dni;
    do
    {
        system("cls");
        printf("** GESTION DE PXI **\n");
        printf("--------------------\n\n");
        printf("1- Dar de alta una practica por ingreso. \n"); /// se va
        printf("2- Modificacion de una practica por ingreso. \n");
        printf("3- Dar de baja una practica por ingreso.\n"); /// se va
        printf("4- Mostrar base de datos completa. \n");
        printf("5- Volver. \n");
        printf("6- Salir. \n");
        printf(">Ingrese una opcion: ");
        fflush(stdin);
        scanf("%i",&opcion);

        switch(opcion)
        {
        case 1:
            system("cls");
            dni = validarDni0(arbol);
            if(dni == -1){
                break;
            }
            nodovalido = existePacienteNodo(arbol, dni);
            if(nodovalido->listaIngreso != NULL){
                printListaIngreso2(nodovalido->listaIngreso, empleadoLog);
                alta_de_PxI(nodovalido);
            }else{
                printf("El paciente no contiene ingresos. \n");
                system("pause");
                break;
            }
            break;
        case 2:
            modificacion_de_PxI(arbol, empleadoLog);
            break;
        case 3:
            baja_de_pXi(arbol, empleadoLog);
            break;
        case 4:
            printTotal(arbol,empleadoLog);
            system("pause");
            break;
        case 5:
            persistenciaTotal(arbol);
            return;
            break;
        case 6:
            persistenciaTotal(arbol);
            exit(0);
            break;
        default:
            printf("La opcion ingresada es incorrecta.\n");
            system("pause");
            break;

        }
    }
    while(opcion != 5); // opcion distinto a volver
}
///menu gestion ingresos
void menuGestionIngresos(nodoPaciente *arbol, Empleado empleadoLog)
{
    nodoPaciente *nodovalido;
    int opcion;
    int dni;
    do
    {
        system("cls");
        printf("** GESTION INGRESOS **\n");
        printf("----------------------\n\n");
        printf("1- Dar de alta un ingreso.\n");
        printf("2- Modificacion de un ingreso. \n");
        printf("3- Dar de baja un ingreso.\n");
        printf("4- Filtrar ingresos con fecha desde y hasta.\n");
        printf("5- Buscar un ingreso.\n");
        printf("6- Volver.\n");
        printf("7- Salir.\n");
        printf(">Ingrese una opcion: ");
        fflush(stdin);
        scanf("%i",&opcion);

        switch(opcion)
        {
        case 1:
            system("cls");
            dni = validarDni0(arbol);
            if(dni == -1){
                system("pause");
                break;
            }
            nodovalido = existePacienteNodo(arbol, dni);
            alta_de_ingreso(nodovalido);
            break;
        case 2:
            modificacion_de_ingresos(arbol, empleadoLog);
            break;
        case 3:
            baja_de_ingresos(arbol, empleadoLog);
            break;
        case 4:
            persistenciaTotal(arbol);
            ingresosDesdeHasta();
            system("pause");
            break;
        case 5:
            persistenciaTotal(arbol);
            menuBuscarIngreso(empleadoLog);
            break;
        case 6:
            return;
            break;
        case 7:
            persistenciaTotal(arbol);
            exit(0);
            break;
        default:
            printf("La opcion ingresada es incorrecta.\n");
            system("pause");
            break;
        }
    }
    while(opcion != 6); // opcion distinto a volver


}

void menuPracticasRestringido(nodoPaciente *arbol, Empleado empleadoLog){
    int opcion;
    do
    {
        system("cls");
        printf("** PRACTICAS **\n");
        printf("---------------\n\n");
        printf("1- Mostrar archivo de practicas. \n");
        printf("2- Listado de practicas ordenado alfabeticamente. \n");
        printf("3- Buscar practicas por comienzo de cadena. \n");
        printf("4- Volver. \n");
        printf("5- Salir. \n");
        printf(">Ingrese una opcion: ");
        fflush(stdin);
        scanf("%i",&opcion);
        switch(opcion)
        {
        case 1:
            mostrarArchivoPractica("practicas.bin", empleadoLog);
            system("pause");
            break;
        case 2:
            testearListaPrac(empleadoLog);
            system("pause");
            break;
        case 3:
            listarPracticasPorInicio("practicas.bin",empleadoLog);
            system("pause");
            break;
        case 4:
            return;
            break;
        case 5:
            persistenciaTotal(arbol);
            exit(0);
            break;
        default:
            printf("La opcion ingresada es incorrecta.\n");
            system("pause");
            break;
        }
    }while(opcion != 4); // opcion distinto a volver
}

///menu gestion practicas
void menuGestionPracticas(nodoPaciente *arbol, Empleado empleadoLog)
{
    int opcion;
    do
    {
        system("cls");
        printf("** GESTION DE PRACTICAS **\n");
        printf("--------------------------\n\n");
        printf("1- Dar de alta una practica. \n");
        printf("2- Modificacion de una practica. \n");
        printf("3- Dar de baja una practica. \n");
        printf("4- Mostrar archivo de practicas. \n");
        printf("5- Listado de practicas ordenado alfabeticamente. \n");
        printf("6- Buscar practicas por comienzo de cadena. \n");
        printf("7- Volver. \n");
        printf("8- Salir. \n");
        printf(">Ingrese una opcion: ");
        fflush(stdin);
        scanf("%i",&opcion);
        switch(opcion)
        {
        case 1:
            alta_de_practica("practicas.bin");
            break;
        case 2:
            modificacion_de_practica("practicas.bin");
            break;
        case 3:
            baja_de_practica("practicas.bin",arbol);
            break;
        case 4:
            mostrarArchivoPractica("practicas.bin", empleadoLog);
            system("pause");
            break;
        case 5:
            testearListaPrac(empleadoLog);
            system("pause");
            break;
        case 6:
            listarPracticasPorInicio("practicas.bin",empleadoLog);
            system("pause");
            break;
        case 7:
            return;
            break;
        case 8:
            persistenciaTotal(arbol);
            exit(0);
            break;
        default:
            printf("La opcion ingresada es incorrecta.\n");
            system("pause");
            break;
        }
    }while(opcion != 7); // opcion distinto a volver
}
/// menu gestion pacientes
void menuGestionPacientes(nodoPaciente*arbol, Empleado empleadoLog)
{
    int opcion;
    nodoPaciente *ordenado = NULL;
    do
    {
        system("cls");
        printf("** GESTION DE PACIENTES **\n");
        printf("--------------------------\n\n");
        printf("1- Dar de alta a un paciente. \n");
        printf("2- Modificacion de un paciente. \n");
        printf("3- Dar de baja a un paciente. \n");
        printf("4- Listar pacientes ordenados alfabeticamente.\n");
        printf("5- Buscar paciente por DNI. \n");
        printf("6- Mostrar pacientes y sus ingresos. \n");
        printf("7- Volver. \n");
        printf("8- Salir. \n");
        printf("Ingrese una opcion: ");
        fflush(stdin);
        scanf("%i",&opcion);

        switch(opcion)
        {
        case 1:
            alta_de_paciente(&arbol);
            break;
        case 2:
            modificacion_de_paciente(arbol);
            break;
        case 3:
            baja_de_paciente(arbol);
            break;
        case 4:
            ordenarArbolAlfabetico(arbol, &ordenado);
            printTotal(ordenado, empleadoLog);
            system("pause");
            break;
        case 5:
            buscarPacientePorDni(arbol,empleadoLog);
            system("pause");
            break;
        case 6:
            printPacientesIngresos(arbol,empleadoLog);
            system("pause");
            break;
        case 7:
            return;
            break;
            break;
        case 8:
            persistenciaTotal(arbol);
            exit(0);
            break;
        default:
            printf("La opcion ingresada es incorrecta.\n");
            system("pause");
            break;
        }
    }
    while(opcion != 7); // opcion distinto a volver


}

///menu gestion empleados
void menuGestionEmpleados(nodoPaciente*arbol, Empleado empleadoLog)
{
    int dni;
    int opcion;

    do
    {
        system("cls");
        printf("** GESTION DE EMPLEADOS **\n");
        printf("--------------------------\n\n");
        printf("1- Dar de alta a un empleado.\n");
        printf("2- Modificacion de un empleado.\n");
        printf("3- Dar de baja a un empleado.\n");
        printf("4- Mostrar lista empleados.\n");
        printf("5- Consultar empleado por dni.\n");
        printf("6- Volver. \n");
        printf("7- Salir. \n");
        printf(">Ingrese una opcion: ");
        fflush(stdin);
        scanf("%i",&opcion);

        switch(opcion)
        {
        case 1:
            alta_de_empleado();
            break;
        case 2:
            modificacion_de_empleado(arbol);
            break;
        case 3:
            baja_de_empleado();
            break;
        case 4:
            system("cls");
            testearListaEmp(empleadoLog.nroPerfil);
            system("pause");
            break;
        case 5:
            dni = validarDniEmpleado0();
            if(dni == -1){
                system("pause");
                break;
            }
            buscarEmpleadoAdicional(dni,empleadoLog.nroPerfil);
            system("pause");
            break;
        case 6:
            persistenciaTotal(arbol);
            return;
            break;
        case 7:
            persistenciaTotal(arbol);
            exit(0);
            break;
        default:
            printf("La opcion ingresada es incorrecta\n");
            system("pause");
            break;
        }
    }
    while(opcion != 6); // opcion distinto a volver
}


/// alta empleado
void alta_de_empleado()
{
    int flag=0;
    Empleado empleadoArchivo;
    Empleado nuevoEmpleado;
    nuevoEmpleado=cargarEmpleadoTeclado();
    if(esEmpleadoValido(nuevoEmpleado) == 0){
        printf("Accion cancelada. \n");
        system("pause");
        return;
    }
    FILE *archivo=fopen(ARCHIVO_EMPLEADOS,"r+b");
    if(archivo!=NULL)
    {
        while(!feof(archivo)&& flag==0)
        {
            fread(&empleadoArchivo,sizeof(Empleado),1,archivo);
            if(!feof(archivo))
            {
                if(empleadoArchivo.dni == nuevoEmpleado.dni)
                {
                    flag=1;
                    printf("ERROR, el empleado ya existe\n");
                }
            }

        }
        if(flag==0)
        {
            fseek(archivo,0,SEEK_END);
            fwrite(&nuevoEmpleado,sizeof(Empleado),1,archivo);
            printf("EL EMPLEADO FUE DADO DE ALTA CON EXITO\n");
        }
        fclose(archivo);
    }
}

void mostrarArchivoEmpleado(char nombreArchivo[]){
    FILE *archivo = fopen(nombreArchivo, "rb");
    Empleado registro;
    if(archivo != NULL)
    {
        while(!feof(archivo))
        {
            fread(&registro, sizeof(Empleado), 1, archivo);
            if(!feof(archivo))
            {
                printf("DNI: %d\n",registro.dni);
                printf("Apellido y Nombre: %s\n",registro.apellidoNombre);
                printf("Telefono: %s\n",registro.telefono);
                printf("Usuario: %s\n",registro.usuario);
                printf("Tipo perfil: %s\n", registro.perfil);
                printf("Contrasenia: %s\n", registro.contra);
                printf("------------------------------\n");
            }
        }
    }
    else
    {
        printf("Error. No se pudo abrir el archivo de empleados. \n");
    }
    fclose(archivo);
}

/// modificacion empleado
void modificacion_de_empleado(nodoPaciente *arbol)
{
    char *nombre;
    char *usuario;
    char *contra;
    char *telefono;

    int opcion;
    int opcion2;
    int flag=0;
    Empleado empleadoArchivo;
    int dni;

    dni=validarDniEmpleado0();
    if(dni == -1){
        return;
    }

    FILE *archivo=fopen(ARCHIVO_EMPLEADOS,"r+b");
    if(archivo!=NULL)
    {
        while(!feof(archivo)&& flag==0)
        {
            fread(&empleadoArchivo,sizeof(Empleado),1,archivo);
            if(!feof(archivo))
            {
                if(empleadoArchivo.dni == dni)
                {
                    flag=1;
                }

            }
        }
        if(flag==1)
        {
            do
            {
                system("cls");
                mostrarEmpleado(empleadoArchivo, empleadoArchivo.nroPerfil);
                printf("** MODIFICACION DE EMPLEADOS **\n");
                printf("-------------------------------\n\n");
                printf("1- Usuario.\n");
                printf("2- Contrasenia.\n");
                printf("3- Apellido y Nombre.\n");
                printf("4- Dni.\n");
                printf("5- Perfil.\n");
                printf("6- Telefono.\n");
                printf("7- Volver.\n");
                printf("8- Salir.\n");
                printf("Ingrese el campo a modificar: ");
                fflush(stdin);
                scanf("%i",&opcion);
                switch(opcion)
                {
                case 1:
                    usuario = validarUsuario1();
                    if(strcmp(usuario, "-1") == 0){
                        break;
                    }
                    strncpy(empleadoArchivo.usuario,usuario, sizeof(empleadoArchivo.usuario));
                    break;
                case 2:
                    contra = validarContra();
                    if(strcmp(contra, "-1") == 0){
                        break;
                    }
                    strncpy(empleadoArchivo.contra,contra, sizeof(empleadoArchivo.contra));
                    break;
                case 3:
                    nombre = validarNombreEmpleado();
                    if(strcmp(nombre, "-1") == 0){
                        break;
                    }
                    strncpy(empleadoArchivo.apellidoNombre,nombre, sizeof(empleadoArchivo.apellidoNombre));
                    break;
                case 4:
                    dni = validarDniEmpleado1();
                    if(dni == -1){
                        break;
                    }
                    empleadoArchivo.dni = dni;
                    break;
                case 5:
                    opcion2 = eleccionPerfil();
                    if(opcion2 == -1){
                        break;
                    }
                    if(opcion2==1)
                    {
                        strcpy(empleadoArchivo.perfil,"Profesional");
                        empleadoArchivo.nroPerfil=1;
                    }
                    else if(opcion2==2)
                    {
                        strcpy(empleadoArchivo.perfil,"Administrativo");
                        empleadoArchivo.nroPerfil=2;
                    }
                    break;
                case 6:
                    telefono = validarTelefonoPaciente();
                    if(strcmp(telefono, "-1") == 0){
                        break;
                    }
                    strncpy(empleadoArchivo.telefono,telefono, sizeof(empleadoArchivo.telefono));
                    break;
                case 7:
                    fseek(archivo,-sizeof(Empleado),SEEK_CUR);
                    fwrite(&empleadoArchivo,sizeof(Empleado),1,archivo);
                    fclose(archivo);
                    return;
                    break;
                case 8:
                    persistenciaTotal(arbol);
                    fseek(archivo,-sizeof(Empleado),SEEK_CUR);
                    fwrite(&empleadoArchivo,sizeof(Empleado),1,archivo);
                    fclose(archivo);
                    exit(0);
                    break;
                default:
                    printf("Opcion incorrecta\n");
                    break;
                }
            }
            while(opcion != 7); // opcion distinto a volver

            fseek(archivo,-sizeof(Empleado),SEEK_CUR);
            fwrite(&empleadoArchivo,sizeof(Empleado),1,archivo);

        }
        else
        {
            printf("El dni que ingreso no esta vinculado a ningun empleado existente. \n");
        }

        fclose(archivo);
    }
}


///baja de empleado
void baja_de_empleado()
{
    Empleado empleadoArchivo;
    int dni;
    dni = validarDniEmpleado0();
    if(dni == -1){
        return;
    }

    FILE *archivo = fopen(ARCHIVO_EMPLEADOS, "r+b");
    if (archivo != NULL)
    {
        FILE *temporalEmp = fopen("temporalEmp.bin", "wb");  // Archivo temporal para escribir los registros válidos
        if (temporalEmp != NULL)
        {
            while(!feof(archivo))
            {
                fread(&empleadoArchivo, sizeof(Empleado), 1, archivo);
                if(!feof(archivo))
                {
                    if (empleadoArchivo.dni != dni)
                    {
                        fwrite(&empleadoArchivo, sizeof(Empleado), 1, temporalEmp); // Se copian los registros distintos al archivo temporal
                    }
                }
            }

            fclose(temporalEmp);
            fclose(archivo);

            remove(ARCHIVO_EMPLEADOS);           // Elimina el archivo original
            rename("temporalEmp.bin", ARCHIVO_EMPLEADOS);  // Renombra el archivo temporal
            printf("Empleado dado de baja exitosamente\n");
            system("pause");
        }
        else
        {
            printf("Error al abrir el archivo temporal\n");
        }
    }
    else
    {
        printf("Error al abrir el archivo de empleados\n");
    }
}

Empleado buscarEmpleadoAdicional(int dni,int nroPerfil)
{
    int flag= 0;
    Empleado empleadoArchivo;
    FILE *archivo=fopen(ARCHIVO_EMPLEADOS,"rb");
    if(archivo!=NULL)
    {
        while(!feof(archivo)&&flag==0)
        {
            fread(&empleadoArchivo,sizeof(Empleado),1,archivo);
            if(!feof(archivo))
            {
                if(dni==empleadoArchivo.dni)
                {
                    flag=1;
                    mostrarEmpleado(empleadoArchivo,nroPerfil);

                }

            }
        }
        fclose(archivo);
    }
    return empleadoArchivo;
}

//mostrar empleaod por dni
void mostrarEmpleado(Empleado empleadoNuevo,int nroPerfil)
{

    printf("-----------------\n");
    printf(" DNI: %d\n",empleadoNuevo.dni);
    printf(" Apellido y Nombre: %s\n",empleadoNuevo.apellidoNombre);
    printf(" Telefono: %s\n",empleadoNuevo.telefono);
    printf(" Usuario: %s\n",empleadoNuevo.usuario);
    if(nroPerfil==0)
    {
        printf(" Contrasenia: %s\n",empleadoNuevo.contra);
    }
    else
    {
        printf(" Contra: ");
        for(int i=0; i<strlen(empleadoNuevo.contra); i++)
        {
            printf("*");
        }
        printf("\n");
    }

    printf(" Perfil: %s\n",empleadoNuevo.perfil);
    printf(" NroPerfil: %d\n\n",empleadoNuevo.nroPerfil);
}

///----------LISTA EMPLEADOS-------------------------///
//probar lista empleado
void testearListaEmp(int nroPerfil)
{
    nodoListaEmp * listaOrdenadaEmp =NULL;
    listaOrdenadaEmp=cargarEmpleadoArchivo(listaOrdenadaEmp);
    mostrarListaEmp(listaOrdenadaEmp,nroPerfil);

}

// mostrarlista emp
void mostrarListaEmp(nodoListaEmp* lista, int nroPerfil)
{
    nodoListaEmp* seg = lista;

    while (seg != NULL)
    {
        mostrarEmpleado(seg->dato, nroPerfil);
        seg = seg->sig;
    }
}

//CREAR NODO
nodoListaEmp *crearNodoEmp(Empleado nuevoEmpleado)
{
    nodoListaEmp* nuevoNodo=(nodoListaEmp*)malloc(sizeof(nodoListaEmp));
    nuevoNodo->dato=nuevoEmpleado;
    nuevoNodo->sig=NULL;
    return nuevoNodo;
}
//cargar empleado desde el archivo
nodoListaEmp* cargarEmpleadoArchivo(nodoListaEmp* listaOrdenadaEmp)
{
    Empleado empleadoArchivo;
    FILE* archivo=fopen(ARCHIVO_EMPLEADOS,"rb");
    if(archivo!=NULL)
    {
        while(!feof(archivo))
        {
            fread(&empleadoArchivo,sizeof(Empleado),1,archivo);
            if(!feof(archivo))
            {
                listaOrdenadaEmp=agregarEmpOrdenado(listaOrdenadaEmp,empleadoArchivo);

            }
        }
        fclose(archivo);
    }
    return listaOrdenadaEmp;
}
//agregar empleados ordenados
nodoListaEmp* agregarEmpOrdenado(nodoListaEmp* listaOrdenadaEmp, Empleado nuevoEmpleado)
{
    nodoListaEmp* nuevoNodo=crearNodoEmp(nuevoEmpleado);
    nodoListaEmp* seg=listaOrdenadaEmp;
// si la lista esta vacia o el empleado a agregar va al principio
    if(listaOrdenadaEmp==NULL || strcmpi(nuevoEmpleado.apellidoNombre, listaOrdenadaEmp->dato.apellidoNombre) < 0)
    {
        nuevoNodo->sig = listaOrdenadaEmp;//si esta vacia queda apuntando a null y si hay algo apunta al nodo que estaba primero
        listaOrdenadaEmp=nuevoNodo;
    }
    else
    {
        while(seg->sig!=NULL && strcmpi(nuevoEmpleado.apellidoNombre, seg->sig->dato.apellidoNombre) >= 0)  //busca la posicion correcta donde deberia ser insertado
        {
            seg=seg->sig;
        }
        nuevoNodo->sig = seg->sig;
        seg->sig = nuevoNodo;
    }
    return listaOrdenadaEmp;
}
///----------LISTA EMPLEADOS-------------------------///


/// -------------------------------------------------- FUNCIONES DE PRINTEO TOTAL -------------------------------------------------- ///
void printTotal(nodoPaciente *arbol, Empleado empleadoLog)
{
    if(arbol)
    {
        printTotal(arbol->izq, empleadoLog);
        if(empleadoLog.nroPerfil == 0 || (empleadoLog.nroPerfil != 0 && arbol->datoPaciente.eliminado == 0)){
            printf("\n//////////////////////////// ** PACIENTE ** ////////////////////////////\n");
            printNodoPaciente(arbol);
            printParcial(arbol->listaIngreso, empleadoLog);
        }
        printTotal(arbol->der, empleadoLog);
    }
}

void printNodoPaciente(nodoPaciente *nodo)
{
        printf("- Apellido nombre: %s\n",nodo->datoPaciente.apellidoNombre);
        printf("- Direccion: %s\n",nodo->datoPaciente.direccion);
        printf("- DNI: %i\n",nodo->datoPaciente.dni);
        printf("- Edad: %i\n",nodo->datoPaciente.edad);
        printf("- Eliminado: %i\n",nodo->datoPaciente.eliminado);
        printf("- Telefono: %s\n",nodo->datoPaciente.telefono);

}

void printParcial(nodoIngreso *lista, Empleado empleadoLog)
{
    nodoIngreso *seg = lista;
    while(seg != NULL)
    {
        if(empleadoLog.nroPerfil == 0 || (empleadoLog.nroPerfil != 0 && seg->datoPcte.eliminado == 0)){
        printf("\n\t================= ** INGRESO ** =================\n");
        printNodoIngreso(seg);
        printListaPxI(seg->lista);
        }
        seg = seg->sig;
    }
}

void printNodoIngreso(nodoIngreso* nodoPrinteo)
{
    printf("\t- Numero de ingreso: %i\n", nodoPrinteo->datoPcte.nroIngreso);
    printf("\t- Fecha de ingreso: %s\n", nodoPrinteo->datoPcte.fechaIngreso);
    printf("\t- Fecha de retiro: %s\n", nodoPrinteo->datoPcte.fechaRetiro);
    printf("\t- Numero de DNI: %i\n", nodoPrinteo->datoPcte.dniPcte);
    printf("\t- Numero de matricula del profesional solicitante: %i\n", nodoPrinteo->datoPcte.matrProfesional);
    printf("\t- Eliminado: %i\n", nodoPrinteo->datoPcte.eliminado);
}

void printListaPxI(nodoPxI *subLista)
{
    nodoPxI *seg = subLista;
    while(seg != NULL)
    {
        printf("\n\t\t---------- ** PXI ** ----------\n");
        printNodoPxI(seg);
        seg = seg->sig;
    }

}

void printNodoPxI(nodoPxI *nodo)
{
    printf("\t\t- Numero de practica por ingreso: %i\n", nodo->pracXing.nroPxI);
    printf("\t\t- Numero de ingreso: %i\n", nodo->pracXing.nroIngreso);
    printf("\t\t- Numero de practica: %i\n", nodo->pracXing.nroPractica);
    printf("\t\t- Nombre de practica: %s\n", buscarNombrePractica("practicas.bin", nodo->pracXing.nroPractica));
    printf("\t\t- Resultado: %s\n", nodo->pracXing.resultado);
}


/// -------------------------------------------------- FUNCIONES DE PRINTEO TOTAL -------------------------------------------------- ///




/// -------------------------------------------------- FUNCIONES DE PERSISTENCIA --------------------------------------------------- ///

void filesToEstructuras(nodoPaciente **arbol)
{
    FILE *archivoPac = fopen("pacientes.bin", "rb");
    FILE *archivoIng = fopen("ingresos.bin", "rb");
    FILE *archivoPrac = fopen("pracXing.bin", "rb");
    if (archivoPac == NULL || archivoIng == NULL || archivoPrac == NULL)
    {
        perror("Error al abrir archivos");
        exit(EXIT_FAILURE);
    }
    Paciente paciente;
    stIngresos ingreso;
    PxI pXi;
    while(fread(&paciente, sizeof(Paciente), 1, archivoPac) == 1)
    {
        insertarNodoPaciente(arbol, paciente);
    }
    fclose(archivoPac);
    while(fread(&ingreso, sizeof(stIngresos), 1, archivoIng) == 1)
    {
        nodoPaciente *pacienteEncontrado = existePacienteNodo(*arbol, ingreso.dniPcte);
        if(pacienteEncontrado != NULL)
        {
            nodoIngreso *nuevoNodoIng = crearNodoIngreso(ingreso);
            agregarIngresoAlFinal(&(pacienteEncontrado->listaIngreso), nuevoNodoIng);
        }
    }
    fclose(archivoIng);
    while(fread(&pXi, sizeof(PxI), 1, archivoPrac) == 1)
    {
        nodoPaciente *pacienteEncontrado = buscarPacientePorIngreso(*arbol, pXi.nroIngreso); /// ERROR ACA
        if(pacienteEncontrado != NULL)
        {
            nodoIngreso *ingresoEncontrado = existeIngresoNodo(pacienteEncontrado->listaIngreso, pXi.nroIngreso);
            if (ingresoEncontrado != NULL)
            {
                nodoPxI *nuevoNodoPxi = crearNodoPxI(pXi);
                agregarPxIAlFinal(&(ingresoEncontrado->lista), nuevoNodoPxi);
            }
        }
    }
    fclose(archivoPrac);
}

void persistenciaTotal(nodoPaciente *arbol)
{
    FILE *archivoAux = fopen("temporalPac.bin", "ab");
    if(archivoAux != NULL)
    {
        escribirPaciente(archivoAux, arbol);
        fclose(archivoAux);

        remove("pacientes.bin"); /// original
        rename("temporalPac.bin", "pacientes.bin");
        remove("temporalPac.bin");

        remove("ingresos.bin"); /// original
        rename("temporalIng.bin", "ingresos.bin");
        remove("temporalIng.bin");

        remove("pracXing.bin"); /// original
        rename("temporalPxI.bin", "pracXing.bin");
        remove("temporalPxI.bin");
    }
    else
    {
        printf("Error. No se pudo abrir temporalPac.bin en gestion.c/persistenciaPacientes\n");
    }

}

void escribirPaciente(FILE *archivo, nodoPaciente *arbol)
{
    if(arbol != NULL)
    {
        escribirPaciente(archivo, arbol->izq);
        persistenciaIngYpXi(arbol->listaIngreso);
        fwrite(&arbol->datoPaciente, sizeof(Paciente), 1, archivo);
        escribirPaciente(archivo, arbol->der);
    }
}

void persistenciaIngYpXi(nodoIngreso *lista)
{
    FILE *archivoIng = fopen("temporalIng.bin", "ab");
    if(archivoIng != NULL)
    {
        FILE *archivoPxI = fopen("temporalPxI.bin", "ab");
        if(archivoPxI != NULL)
        {
            escribirIngYpXi(archivoIng,archivoPxI,lista);
            fclose(archivoIng);
            fclose(archivoPxI);
        }
        else
        {
            printf("Error. No se pudo abrir temporalPxI.bin en gestion.c/persistenciaIngreso\n");
        }
    }
    else
    {
        printf("Error. No se pudo abrir temporalIng.bin en gestion.c/persistenciaIngreso\n");
    }
}


void escribirIngYpXi(FILE *archivoIng, FILE *archivoPxI, nodoIngreso *lista)
{
    nodoIngreso *seg = lista;
    while(seg != NULL)
    {
        fwrite(&seg->datoPcte, sizeof(stIngresos), 1, archivoIng);
        escribirPxI(archivoPxI, seg->lista);
        seg = seg->sig;
    }
}

void escribirPxI(FILE *archivoPxI, nodoPxI *subLista)
{
    nodoPxI *seg = subLista;
    while(seg != NULL)
    {
        fwrite(&seg->pracXing, sizeof(PxI), 1, archivoPxI);
        seg = seg->sig;
    }
}

/// -------------------------------------------------- FUNCIONES DE PERSISTENCIA --------------------------------------------------- ///



/// -------------------------------------------------- FUNCIONES DE PRACXINGRESO --------------------------------------------------- ///
void alta_de_PxIAdministrativo(nodoPaciente *pacienteValido)
{
    char control;
    nodoIngreso *nodoIngEncontrado;
    int nroIngreso;
    printf("\n**ALTA DE PRACTICA POR INGRESO**\n");
    printf("--------------------------------\n");
    nroIngreso = validarNroIngreso(); // es nroIngreso valido, puede o no existir.
    if(nroIngreso == -1)
    {
        return;
    }
    nodoIngEncontrado = existeNodoIngreso(pacienteValido->listaIngreso, nroIngreso);
    if(nodoIngEncontrado != NULL && nodoIngEncontrado->datoPcte.eliminado == 0)
    {
        agregarPxIAdministrativo(&(nodoIngEncontrado->lista),nodoIngEncontrado->datoPcte.nroIngreso);
    }
    else   /// no existe
    {
        printf("Ingreso no existe. Quiere crear un ingreso? (s/n): ");
        fflush(stdin);
        scanf("%c", &control);
        if(control == 's' || control == 'S'){
            alta_de_ingreso(pacienteValido);
        }
    }
}

void alta_de_PxI(nodoPaciente *pacienteValido)
{
    char control;
    nodoIngreso *nodoIngEncontrado;
    int nroIngreso;
    printf("\n**ALTA DE PRACTICA POR INGRESO**\n");
    nroIngreso = validarNroIngreso(); // es nroIngreso valido, puede o no existir.
    if(nroIngreso == -1)
    {
        return;
    }
    nodoIngEncontrado = existeNodoIngreso(pacienteValido->listaIngreso, nroIngreso);
    if(nodoIngEncontrado != NULL)
    {
        agregarPxI(&(nodoIngEncontrado->lista),nodoIngEncontrado->datoPcte.nroIngreso);
    }
    else   /// no existe
    {
        printf("Ingreso no existe. Quiere crear un ingreso? (s/n): ");
        fflush(stdin);
        scanf("%c", &control);
        if(control == 's' || control == 'S'){
            alta_de_ingreso(pacienteValido);
        }
    }
}

nodoIngreso *existeNodoIngreso(nodoIngreso *lista, int nroIngreso)
{
    nodoIngreso *seg = lista;
    while(seg != NULL)
    {
        if(seg->datoPcte.nroIngreso == nroIngreso)
        {
            return seg;
        }
        seg = seg->sig;
    }
    return NULL;
}

int validarNroIngreso()
{
    int nroIngreso;
    int valido;
    int iterar;
    do
    {
        iterar = 0;
        printf(">Ingrese el numero de ingreso deseado: ");
        fflush(stdin);
        valido = scanf("%i", &nroIngreso);
        if(nroIngreso == -1)
        {
            printf("Accion cancelada. \n");
            return -1;
        }
        if(valido != 1)
        {
            printf("Error. El numero de ingreso no es un entero valido. \n\n");
            iterar = 1;
        }
        else if(nroIngreso < 0)
        {
            printf("Error. El numero de ingreso no puede ser negativo. Intente nuevamente.\n\n");
            iterar = 1;
        }
    }
    while(iterar == 1);
    return nroIngreso;
}

/// Funcion que muestra todos los registros de un archivo
/// de practicas por ingreso
void mostrarArchivoPxI(char nombreArchivo[])
{
    FILE *archivo = fopen(nombreArchivo, "rb");
    PxI registro;
    if(archivo != NULL)
    {
        printf("\n**REGISTROS DE PXI**\n");
        while(!feof(archivo))
        {
            fread(&registro, sizeof(PxI), 1, archivo);
            if(!feof(archivo))
            {
                mostrarRegistroPxI(registro);
                printf("-------------------------------------\n");
            }
        }
    }
    else
    {
        printf("Error. No se pudo abrir %s en gestion.c/mostrarArchivoPxI\n", nombreArchivo);
    }
    fclose(archivo);
}

/// Muestra un registro del archivo de practicas por ingreso
void mostrarRegistroPxI(PxI registro)
{
    printf("-------------------------------------\n");
    printf("Nro de ingreso al que pertence: %i\n", registro.nroIngreso);
    printf("Nro de ingreso por practica: %i\n", registro.nroPxI);
    printf("Nro de practica: %i\n", registro.nroPractica);
    printf("Resultado: %s\n", registro.resultado);
}

void baja_de_pXi(nodoPaciente *arbol, Empleado empleadoLog)
{
    char control;
    int nroPxIBorrar;
    int iterar;
    nodoIngreso *listaIngreso;
    do
    {
        system("cls");
        printf("** BAJA DE PRACTICA POR INGRESO **\n");
        printf("----------------------------------\n");
        listaIngreso = buscarIngreso(arbol, empleadoLog);
        if(listaIngreso != NULL)
        {
            if(listaIngreso->lista != NULL)
            {
                nroPxIBorrar = validarNroPxI(listaIngreso->lista);
                if(nroPxIBorrar == -1)
                {
                    return;
                }
                borrarNodo(&(listaIngreso->lista),nroPxIBorrar);
                if(listaIngreso->lista == NULL){
                    printf("El ingreso no contiene practicas despues de borrar. Se elimino el ingreso. \n");
                    listaIngreso->datoPcte.eliminado = 1;
                    system("pause");
                }
                iterar = 0;
            }
        }
        else
        {
            printf(">Quiere intentar de nuevo? (s/n): ");
            fflush(stdin);
            scanf("%c", &control);
            if(control == 's' || control == 'S')
            {
                iterar = 1;
            }
            else
            {
                iterar = 0;
            }
        }
    }
    while(iterar == 1);
}

nodoPxI *validarNodoPxI(nodoPaciente *arbol, Empleado empleadoLog)
{
    nodoPaciente *pacienteBuscado = NULL;
    nodoIngreso *ingresoBuscado = NULL;
    nodoPxI *pXiBuscado;

    pacienteBuscado = validarDniModificacion(arbol);
    if(pacienteBuscado == NULL)
    {
        return NULL;
    }

    if(pacienteBuscado->listaIngreso == NULL){
        printf("El paciente no contiene ningun ingreso. \n");
        return NULL;
    }

    printParcial(pacienteBuscado->listaIngreso, empleadoLog);
    ingresoBuscado = validarIngresoModificacion(pacienteBuscado->listaIngreso);
    if(ingresoBuscado == NULL)
    {
        return NULL;
    }

    pXiBuscado = buscarNodoPxI(ingresoBuscado->lista);
    if(pXiBuscado == NULL)
    {
        return NULL;
    }

    return pXiBuscado;
}

void borrarNodo(nodoPxI **subLista, int nroPxIBorrar)
{
    if((*subLista != NULL) && (nroPxIBorrar == (*subLista)->pracXing.nroPxI))
    {
        nodoPxI *aux = *subLista;
        *subLista = (*subLista)->sig;
        free(aux);
        return;
    }
    else
    {
        nodoPxI* ante = *subLista;
		nodoPxI* seg = (*subLista)->sig;
        while((seg != NULL) && (nroPxIBorrar != seg->pracXing.nroPxI))
        {
            ante = seg;
            seg = seg->sig;
        }
        if(seg != NULL)
        {
            ante->sig = seg->sig;
            free(seg);
            return;
        }
    }
}

void modificacion_de_PxI(nodoPaciente *arbol, Empleado empleadoLog)
{
    char *resultado;
    int nroPractica;
    int eleccion;
    char control;
    nodoPxI *nodoAmodificar = NULL;
    do{
        system("cls");
        if(nodoAmodificar == NULL)
        {
            nodoAmodificar = validarNodoPxI(arbol, empleadoLog);
            if(nodoAmodificar == NULL){
                printf("No se pudo encontrar el registro a modificar. Intente nuevamente. \n");
                system("pause");
                return;
            }
        }
        system("cls");
        printNodoPxI(nodoAmodificar);
        printf("\n---------------\n");
        printf("Campo a modificar\n");
        printf("-----------------\n");
        printf("1- Resultado.\n");
        printf("2- Numero de practica.\n");
        printf("3- Volver.\n");
        printf("4- Salir.\n");
        fflush(stdin);
        printf(">Ingrese la opcion que desee: ");
        scanf("%i", &eleccion);
        switch(eleccion)
        {
        case 1:
            resultado = validarResultado();
            if(strcmp(resultado,"-1") == 0)
            {
                return;
            }
            strcpy(nodoAmodificar->pracXing.resultado, resultado);
            break;
        case 2:
            nroPractica = validarNroPractica0();
            if(nroPractica == -1)
            {
                return;
            }
            buscarRegistroPractica(nroPractica);
            printf(">Es esta la practica con la que quiere reemplazar? (s/n): ");
            fflush(stdin);
            scanf("%c", &control);
            if(control != 's' && control != 'S'){
                printf("No se modifico la practica. \n");
                system("pause");
                break;
            }
            nodoAmodificar->pracXing.nroPractica = nroPractica;
            break;
        case 3:
            return;
            break;
        case 4:
            persistenciaTotal(arbol);
            exit(0);
            break;
        default:
            printf("Opcion ingresada no es valida. \n");
            return;
            break;
        }
    }while(eleccion != 3);
}

void buscarRegistroPractica(int nroPractica){
    FILE *archivo = fopen("practicas.bin", "rb");
    Practica registro;
    if(archivo){
        while(fread(&registro, sizeof(Practica), 1, archivo) == 1){
            if(registro.nroPractica == nroPractica){
                mostrarRegistroPractica(registro);
                return;
            }
        }
    }else{
        printf("No se pudo abrir practicas.bin en buscarRegistroPractica. \n");
    }
    fclose(archivo);
}

//void modificacion_de_PxI(nodoPaciente *arbol, Empleado empleadoLog)
//{
//    char *resultado;
//    int nroPractica;
//    int eleccion;
//    nodoPxI *nodoAmodificar = NULL;
//    do{
//        system("cls");
//        if(nodoAmodificar == NULL)
//        {
//            nodoAmodificar = validarNodoPxI(arbol, empleadoLog);
//            if(nodoAmodificar == NULL){
//                printf("No se pudo encontrar el registro a modificar. Intente nuevamente. \n");
//                system("pause");
//                return;
//            }
//        }
//        system("cls");
//        printNodoPxI(nodoAmodificar);
//        printf("\n---------------\n");
//        printf("Campo a modificar\n");
//        printf("-----------------\n");
//        printf("1- Resultado.\n");
//        printf("2- Numero de practica.\n");
//        printf("3- Volver.\n");
//        printf("4- Salir.\n");
//        fflush(stdin);
//        printf(">Ingrese la opcion que desee: ");
//        scanf("%i", &eleccion);
//        switch(eleccion)
//        {
//        case 1:
//            resultado = validarResultado();
//            if(strcmp(resultado,"-1") == 0)
//            {
//                return;
//            }
//            strcpy(nodoAmodificar->pracXing.resultado, resultado);
//            break;
//        case 2:
//            nroPractica = validarNroPractica0();
//            if(nroPractica == -1)
//            {
//                return;
//            }
//            nodoAmodificar->pracXing.nroPractica = nroPractica;
//            break;
//        case 3:
//            return;
//            break;
//        case 4:
//            persistenciaTotal(arbol);
//            exit(0);
//            break;
//        default:
//            printf("Opcion ingresada no es valida. \n");
//            return;
//            break;
//        }
//    }while(eleccion != 3);
//}

/// VALIDA INPUT DE NRO PXI POR TECLADO
/// ***SOLO PARA MODIFICACION/BAJA PXI***
int validarNroPxI(nodoPxI *subLista)
{
    int nroPxI;
    int valido;
    do
    {
        printf(">Ingrese el numero de practica por ingreso: ");
        fflush(stdin);
        valido = scanf("%i", &nroPxI);
        if(nroPxI == -1)
        {
            printf("Accion cancelada. \n");
            return -1;
        }
        if(valido != 1)
        {
            printf("Error. El numero de practica por ingreso no es un entero valido. \n\n");
        }
        else if(existeNroPxI(subLista, nroPxI) == 0)
        {
            printf("Error. El numero de practica por ingreso no existe. \n\n");
            valido = 0;
        }
    }
    while(valido != 1);
    return nroPxI;
}

nodoPxI *buscarNodoPxI(nodoPxI *subLista)
{
    nodoPxI *seg = subLista;
    int nroPxIBuscado = validarNroPxI(subLista);
    if(nroPxIBuscado == -1)
    {
        return NULL;
    }
    while(seg != NULL)
    {
        if(seg->pracXing.nroPxI == nroPxIBuscado)
        {
            return seg;
        }
        seg = seg->sig;
    }
    return NULL;
}

void agregarPxIAdministrativo(nodoPxI **subLista, int nroIngresoPadre)
{
    PxI PxInuevo;
    int iterar;
    int esValido; // si la carga de la PxI fue cancelada (1) o no (0)
    char control;
    do
    {
        PxInuevo = crearPxIAdministrativo(nroIngresoPadre);
        esValido = esPxIValido(PxInuevo);
        if(esValido == 1)
        {
            agregarPxIAlFinal(subLista, crearNodoPxI(PxInuevo));
            iterar = 0;
            printf(">Quiere ingresar otra Practica para el mismo ingreso? (s/n): ");
        }
        else
        {
            printf(">Quiere intentar de nuevo? (s/n): ");
        }
        fflush(stdin);
        scanf("%c", &control);
        if(control == 's' || control == 'S')  // control n
        {
            iterar = 1;
        }
        else
        {
            iterar = 0;
        }
    }
    while(iterar == 1);
}

/// ****FUNCIONA
void agregarPxI(nodoPxI **subLista, int nroIngresoPadre)
{
    PxI PxInuevo;
    int iterar;
    int esValido; // si la carga de la PxI fue cancelada (1) o no (0)
    char control;
    do
    {
        PxInuevo = crearPxI(nroIngresoPadre);
        esValido = esPxIValido(PxInuevo);
        if(esValido == 1)
        {
            agregarPxIAlFinal(subLista, crearNodoPxI(PxInuevo));
            iterar = 0;
            printf(">Quiere ingresar otra Practica para el mismo ingreso? (s/n): ");
        }
        else
        {
            printf(">Quiere intentar cargar una Practica al ingreso nuevamente? (s/n): ");
        }
        fflush(stdin);
        scanf("%c", &control);
        if(control == 's' || control == 'S')  // control n
        {
            iterar = 1;
        }
        else
        {
            iterar = 0;
        }
    }
    while(iterar == 1);
}

/// ****FUNCIONA
void agregarPxIAlFinal(nodoPxI **subLista, nodoPxI *nuevoNodo)
{
    if(*subLista == NULL)
    {
        *subLista = nuevoNodo;
    }
    else
    {
        nodoPxI *seg = *subLista;
        while(seg->sig != NULL)
        {
            seg = seg->sig;
        }
        seg->sig = nuevoNodo;
    }
}

/// Verifica si el paciente creado es valido o si se
/// cancelo la carga de ese
int esPxIValido(PxI pracXing)
{

    if (pracXing.nroPractica == -1)
    {
        return 0;
    }

    if (strcmp(pracXing.resultado, "-1") == 0)
    {
        return 0;
    }

    return 1;
}

nodoPxI *crearNodoPxI(PxI pracXing)
{
    nodoPxI *nuevo = (nodoPxI*)malloc(sizeof(nodoPxI));
    nuevo->pracXing = pracXing;
    nuevo->sig = NULL;
    return nuevo;
}

PxI crearPxI(int nroIngresoPadre)
{
    printf("\n** Practica por Ingreso **\n");
    printf("--------------------------\n");
    char control;
    PxI nuevo;

    nuevo.nroIngreso = nroIngresoPadre;

    actualizarNroGlobal("nroPxI.bin");
    nroPxIGlobal++;
    nuevo.nroPxI = nroPxIGlobal;

    nuevo.nroPractica = validarNroPractica0();
    if (nuevo.nroPractica == -1)
    {
        return nuevo;
    }
    buscarRegistroPractica(nuevo.nroPractica);
    printf(">Es esta la practica que quiere ingresar? (s/n): ");
    fflush(stdin);
    scanf("%c", &control);
    if(control != 's' && control != 'S'){
        nuevo.nroPractica = -1;
        printf("No se dio de alta la practica \n");
        system("pause");
        return nuevo;
    }

    strcpy(nuevo.resultado, validarResultado());
    if (strcmp(nuevo.resultado, "-1") == 0)
    {
        return nuevo;
    }

    escribirNroArchivo("nroPxI.bin");
    return nuevo;
}

PxI crearPxIAdministrativo(int nroIngresoPadre)
{
    printf("\n** Practica por Ingreso **\n");
    printf("--------------------------\n");
    PxI nuevo;
    char control;

    nuevo.nroIngreso = nroIngresoPadre;

    actualizarNroGlobal("nroPxI.bin");
    nroPxIGlobal++;
    nuevo.nroPxI = nroPxIGlobal;

    nuevo.nroPractica = validarNroPractica0();
    if (nuevo.nroPractica == -1)
    {
        return nuevo;
    }
    buscarRegistroPractica(nuevo.nroPractica);
    printf(">Es esta la practica que quiere ingresar? (s/n): ");
    fflush(stdin);
    scanf("%c", &control);
    if(control != 's' && control != 'S'){
        printf("No se dio de alta la practica \n");
        system("pause");
        return nuevo;
    }


    strcpy(nuevo.resultado, "");

    escribirNroArchivo("nroPxI.bin");
    return nuevo;
}

char *validarResultado()
{
    char resultado[40];
    int valido;
    do
    {
        valido = 1;
        printf(">Ingrese el resultado deseado: ");
        fflush(stdin);
//        fgets(resultado, sizeof(resultado), stdin);
//        resultado[strcspn(resultado, "\n")] = '\0';
        gets(resultado);
        if(strcmp(resultado, "-1") == 0)
        {
            printf("Accion cancelada. \n");
            return strdup("-1");
        }
        if (strlen(resultado) > 40)
        {
            printf("Error. El nombre de la practica no debe superar los 30 caracteres.\n\n");
            valido = 0;
        }
        else if(esResultadoValido(resultado) == 0)
        {
            printf("Error. El resultado ingresado contiene caracteres no validos.\n\n");
            valido = 0;
        }
    }
    while(valido != 1);
    char *cadena = strdup(resultado);
    // strdup: asigna memoria dinamicamente y copia la cadena en la mem asignada.
    return cadena;
}

/// Verifica que cada caracter del resultado sea valido
/// Los espacios y los numeros no son considerados invalidos
/// Retorna 1 si la cadena es valida, 0 si no lo es.
int esResultadoValido(const char *cadena)
{
    for (int i = 0; i < strlen(cadena); i++)
    {
        if (!isalpha(cadena[i]) && !isdigit(cadena[i]) && cadena[i] != ' ')
        {
            // isalpha() devuelve 0 si no es caracter numerico
            return 0;
        }
    }
    return 1;
}

int existeNroPxI(nodoPxI *subLista, int nroPxIBuscado)
{
    nodoPxI *seg = subLista;
    while(seg != NULL)
    {
        if(seg->pracXing.nroPxI == nroPxIBuscado)
        {
            return 1;
        }
        seg = seg->sig;
    }
    return 0;
}

/// -------------------------------------------- FUNCIONES DE PRACTICAXINGRESO ---------------------------------------------- ///



/// -------------------------------------------------- FUNCIONES DE FECHA --------------------------------------------------- ///



int obtenerAnioRandom()
{
    int anio, valido;
    do
    {
        printf(">Ingrese el anio: ");
        fflush(stdin);
        valido = scanf("%i", &anio);

        if (anio == -1)
        {
            return -1;
        }

        if (valido != 1 || (anio < 2000 || anio > 2050))
        {
            printf("Error. El anio ingresado no corresponde a uno valido. Ingrese la fecha nuevamente.\n\n");
        }

    }
    while (valido != 1 || (anio < 2000 || anio > 2050));

    return anio;
}
char* validarFechaRandom()
{
    int anio, mes, dia;
    anio = obtenerAnioRandom();

    if (anio == -1)
    {
        return "-1";
    }

    mes = obtenerMes(anio);

    if (mes == -1)
    {
        return "-1";
    }

    dia = obtenerDia(mes, anio);

    if (dia == -1)
    {
        return "-1";
    }

    return fechaFormatear(dia, mes, anio);
}

struct tm convertirFecha(char *fecha)
{
    struct tm tmFecha = {0};
    sscanf(fecha, "%d/%d/%d", &tmFecha.tm_mday, &tmFecha.tm_mon, &tmFecha.tm_year);
    tmFecha.tm_mon -= 1;  // Ajustar al rango de 0-11
    tmFecha.tm_year -= 1900;  // Ajustar al año desde 1900
    return tmFecha;
}

char* validarFecha()
{
    int anio, mes, dia;
    anio = obtenerAnio();

    if (anio == -1)
    {
        return "-1";
    }

    mes = obtenerMes(anio);

    if (mes == -1)
    {
        return "-1";
    }

    dia = obtenerDia(mes, anio);

    if (dia == -1)
    {
        return "-1";
    }

    return fechaFormatear(dia, mes, anio);
}

int obtenerAnio()
{
    int anio, valido;
    do
    {
        printf(">Ingrese el anio de retiro: ");
        fflush(stdin);
        valido = scanf("%i", &anio);

        if (anio == -1)
        {
            return -1;
        }

        if (valido != 1 || (anio < fGetAnioLarge() || anio > 2050))
        {
            printf("Error. El anio ingresado no corresponde a uno valido. Ingrese la fecha completa nuevamente.\n\n");
        }

    }
    while (valido != 1 || (anio < fGetAnioLarge() || anio > 2050));

    return anio;
}

int obtenerMes(int anio)
{
    int mes, valido, iterar;
    do
    {
        iterar = 0;
        printf(">Ingrese el mes: ");
        fflush(stdin);
        valido = scanf("%i", &mes);

        if (mes == -1)
        {
            return -1;
        }

        if (valido != 1 || (mes > 12 || mes < 1))
        {
            printf("Error. El mes ingresado no corresponde a uno valido. Ingrese la fecha completa nuevamente.\n\n");
            iterar = 1;
        }else if(anio == fGetAnioLarge() && mes < fGetMonthNumber()){
            printf("La fecha no puede ser anterior a la fecha actual. \n\n");
            iterar = 1;
        }

    }
    while (iterar == 1);

    return mes;
}

int obtenerDia(int mes, int anio)
{
    int dia, valido;
    int iterar;
    do
    {
        iterar = 0;
        printf(">Ingrese el numero del dia: ");
        valido = scanf("%i", &dia);

        if (dia == -1)
        {
            return -1;
        }

        if(mes == fGetMonthNumber() && anio == fGetAnioLarge() && dia < fGetDayNumber()){
            iterar = 1;
            printf("La fecha no puede ser anterior a la fecha actual. \n");
        }

        if (valido != 1 || (dia > obtenerDiasMes(mes, anio) || dia < 1))
        {
            iterar = 1;
            printf("Error. El dia ingresado no corresponde a uno valido. Ingrese el dia nuevamente.\n\n");
        }

    }
    while (iterar == 1);

    return dia;
}


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

    char* date = (char*)malloc(10 * sizeof(char));
    if (date == NULL)
    {
        return NULL;
    }
    date[0] = '\0';

    sprintf(date + strlen(date), "%d/", dia);
    sprintf(date + strlen(date), "%d/", mes);
    sprintf(date + strlen(date), "%d", anio);

    return date;
}

char* fechaFormatear(int dia, int mes, int anio)
{
    char* fecha = (char*)malloc(10 * sizeof(char));
    if (fecha == NULL)
    {
        return NULL;
    }
    fecha[0] = '\0';

    sprintf(fecha + strlen(fecha), "%d/", dia);
    sprintf(fecha + strlen(fecha), "%d/", mes);
    sprintf(fecha + strlen(fecha), "%d", anio);

    return strdup(fecha);
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

/// Funcion que muestra todos los registros de un archivo
/// de practicas por ingreso
void mostrarArchivoIng(char nombreArchivo[])
{
    FILE *archivo = fopen(nombreArchivo, "rb");
    stIngresos registro;
    if(archivo != NULL)
    {
        printf("\n**REGISTROS DE INGRESOS**\n");
        while(!feof(archivo))
        {
            fread(&registro, sizeof(stIngresos), 1, archivo);
            if(!feof(archivo))
            {
                mostrarRegistroIng(registro);
                printf("\n===================================\n");
            }
        }
    }
    else
    {
        printf("Error. No se pudo abrir %s en gestion.c/mostrarArchivoPxI\n", nombreArchivo);
    }
    fclose(archivo);
}

/// Muestra un registro del archivo de practicas por ingreso
void mostrarRegistroIng(stIngresos registro)
{
    printf("===================================\n");
    printf("Nro de ingreso: %i\n", registro.nroIngreso);
    printf("Fecha de ingreso: %s\n", registro.fechaIngreso);
    printf("Fecha de retiro: %s\n", registro.fechaRetiro);
    printf("DNI del paciente: %i\n", registro.dniPcte);
    printf("Matricula del profesional: %i\n", registro.matrProfesional);
    printf("Eliminado: %i\n", registro.eliminado);

}

nodoIngreso *buscarIngreso(nodoPaciente *arbol, Empleado empleadoLog)
{
    nodoPaciente *pacienteBuscado = NULL;
    nodoIngreso *ingresoBuscado = NULL;

    pacienteBuscado = validarDniModificacion(arbol);
    if(pacienteBuscado == NULL)
    {
        return NULL;
    }

    if(pacienteBuscado->listaIngreso == NULL){
        printf("El paciente no tiene ingresos. \n");
        return NULL;
    }

    printParcial(pacienteBuscado->listaIngreso, empleadoLog);

    ingresoBuscado = validarIngresoModificacion(pacienteBuscado->listaIngreso);
    if(ingresoBuscado == NULL)
    {
        return NULL;
    }

    return ingresoBuscado;
}

/// HACERLE DO WHILE A ESTA FUNCION (FIJARSE SI ESTA BIEN HACERLE ESO)
nodoIngreso *validarIngresoModificacion(nodoIngreso *lista)
{
    int nroIngreso;
    nodoIngreso *nodoAmodificar;
    printf(">Ingrese el numero de ingreso: ");
    fflush(stdin);
    int validoNroIngreso = scanf("%i", &nroIngreso);
    if(nroIngreso == -1)
    {
        printf("Accion cancelada. \n");
        return NULL;
    }
    if(validoNroIngreso != 1)
    {
        printf("Error. El numero de ingreso contiene caracteres no numericos. Intente nuevamente.\n\n");
        return NULL;
    }
    else if(nroIngreso < 0)
    {
        printf("Error. El numero de ingreso no puede ser menor a 0. Intente nuevamente.\n\n");
        return NULL;
    }
    nodoAmodificar = existeIngresoNodo(lista, nroIngreso);
    if(nodoAmodificar == NULL)
    {
        printf("Error. El numero de ingreso no existe.\n\n");
        return NULL;
    }
    return nodoAmodificar;
}

/// pacienteValido es el paciente que se busco anterior a esta funcion
/// que es valido en todos sus sentidos y existe creado en el arbol.
void alta_de_ingreso(nodoPaciente *pacienteValido)
{
    int iterar;
    char control;
    stIngresos nuevoIngreso;
    nodoIngreso *nodoIngNuevo;
    do
    {
        if(pacienteValido->datoPaciente.eliminado == 1){
            printf("El paciente se encuentra eliminado. No se puede cargar ingresos. \n");
            return;
        }
        printf("\n** CARGANDO INGRESO **\n");
        printf("----------------------\n");
        nuevoIngreso = cargaIngreso(pacienteValido);
        if(esIngresoValido(nuevoIngreso) == 1)
        {
            nodoIngNuevo = crearNodoIngreso(nuevoIngreso);
            agregarPxIAdministrativo(&(nodoIngNuevo->lista), nodoIngNuevo->datoPcte.nroIngreso);
            if(nodoIngNuevo->lista == NULL){
                printf("\nNo se puede cargar el ingreso sin una practica. Intente nuevamente. \n");
                system("pause");
                return;
            }
            agregarIngresoAlFinal(&(pacienteValido->listaIngreso), nodoIngNuevo);
            printf(">Quiere generar otro ingreso para el mismo paciente? (s/n): ");
        }
        else
        {
            printf(">Quiere intentar cargar un ingreso nuevamente? (s/n): ");
        }
        fflush(stdin);
        scanf("%c", &control);
        if(control == 's' || control == 'S')  // control n
        {
            iterar = 1;
        }
        else
        {
            iterar = 0;
        }
    }
    while(iterar == 1);
}

nodoIngreso *existeIngresoNodo(nodoIngreso *lista, int nroIngresoBuscado)
{
    nodoIngreso *seg = lista;
    while(seg != NULL)
    {
        if(nroIngresoBuscado == seg->datoPcte.nroIngreso)
        {
            return seg;
        }
        seg = seg->sig;
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
    nuevoNodo->datoPcte.matrProfesional = registro.matrProfesional;
    nuevoNodo->datoPcte.eliminado = registro.eliminado;
    nuevoNodo->sig = NULL;
    nuevoNodo->lista = NULL;

    return nuevoNodo;
}

int esIngresoValido(stIngresos ingreso)
{

    if (strcmp(ingreso.fechaIngreso, "-1") == 0)
    {
        return 0;
    }

    if (strcmp(ingreso.fechaRetiro, "-1") == 0)
    {
        return 0;
    }

    if(ingreso.matrProfesional == -1)
    {
        return 0;
    }

    return 1;
}

stIngresos cargaIngreso(nodoPaciente *raiz)
{
    stIngresos nuevo;

    nuevo.dniPcte = raiz->datoPaciente.dni;

    actualizarNroGlobal("nroIngreso.bin");
    nuevo.nroIngreso = nroIngresoGlobal++;

    /// meterse en esta funcion y dar la opcion de -1 y retornar -1
    char* fechaInterna = (char*)malloc(sizeof(char)*11); /// revisar
    fechaInterna = fechaConcatenada();
    strncpy(nuevo.fechaIngreso, fechaInterna, sizeof(nuevo.fechaIngreso) -1);
    nuevo.fechaIngreso[sizeof(nuevo.fechaIngreso) -1] = '\0';
    free(fechaInterna);

    if(strcmp(nuevo.fechaIngreso, "-1") == 0)
    {
        return nuevo;
    }
    /// meterse en esta funcion y dar la opcion de -1 y retornar -1
    char* fechaUsuario = (char*)malloc(sizeof(char)*11); /// revisar
    fechaUsuario = validarFecha();
    strncpy(nuevo.fechaRetiro, fechaUsuario, sizeof(nuevo.fechaRetiro) -1);
    nuevo.fechaRetiro[sizeof(nuevo.fechaRetiro) -1] = '\0';
    free(fechaUsuario);

    if(strcmp(nuevo.fechaRetiro, "-1") == 0)
    {
        return nuevo;
    }

    nuevo.matrProfesional = validarMatricula();
    if(nuevo.matrProfesional == -1)
    {
        return nuevo;
    }

    nuevo.eliminado = 0;

    escribirNroArchivo("nroIngreso.bin");

    return nuevo;
}

void agregarIngresoAlFinal(nodoIngreso **lista, nodoIngreso *nuevoNodo)
{
    if(*lista == NULL)
    {
        *lista = nuevoNodo;
    }
    else
    {
        nodoIngreso* seg = *lista;
        while(seg->sig != NULL)
        {
            seg = seg->sig;
        }
        seg->sig = nuevoNodo;
    }
}

int validarMatricula()
{
    int numMatricula;
    int valido;
    int iterar;
    do
    {
        iterar = 0;
        printf(">Ingrese la matricula: ");
        fflush(stdin);
        valido = scanf("%i", &numMatricula);
        if(numMatricula == -1)
        {
            return -1;
        }
        if(valido != 1)
        {
            printf("Error. El numero de ingreso no es un entero valido. \n\n");
            iterar = 1;
        }
        else if(numMatricula <= 9999 || numMatricula > 99999999)
        {
            printf("Error. El numero de matricula debe tener entre 5 y 8 digitos. Ingrese uno válido.\n\n");
            iterar = 1;
        }
    }
    while(iterar == 1);
    return numMatricula;
}


nodoIngreso *validarNodoIngreso(nodoPaciente *arbol, Empleado empleadoLog)
{
    nodoPaciente *pacienteBuscado = NULL;
    nodoIngreso *ingresoBuscado;

    pacienteBuscado = validarDniModificacion(arbol);
    if(pacienteBuscado == NULL)
    {
        return NULL;
    }

    if(pacienteBuscado->listaIngreso == NULL){
        printf("El paciente no cuenta con ingresos. \n");
        return NULL;
    }

    printParcial(pacienteBuscado->listaIngreso, empleadoLog);

    ingresoBuscado = validarIngresoModificacion(pacienteBuscado->listaIngreso);
    if(ingresoBuscado == NULL)
    {
        return NULL;
    }
    return ingresoBuscado;
}

void modificacion_de_ingresos(nodoPaciente *arbol, Empleado empleadoLog)
{
    char* fechaIngreso = (char*)malloc(sizeof(char)*10);
    char* fechaRetiro = (char*)malloc(sizeof(char)*10);
    int nroMatricula;
    int eleccion;
    nodoIngreso *nodoModificar = NULL;
    do{
        system("cls");
        if(nodoModificar == NULL)
        {
            nodoModificar = validarNodoIngreso(arbol, empleadoLog);
            if(nodoModificar == NULL){
                system("pause");
                return;
            }
        }
        system("cls");
        printNodoIngreso(nodoModificar);
        printf("\n-----------------\n");
        printf("Campo a modificar\n");
        printf("-----------------\n");
        printf("1- Fecha de ingreso.\n");
        printf("2- Fecha de retiro.\n");
        printf("3- Numero de matricula.\n");
        printf("4- Volver.\n");
        printf("5- Salir.\n");
        fflush(stdin);
        printf(">Ingrese la opcion que desee: ");
        scanf("%i", &eleccion);
        switch(eleccion)
        {
        case 1:
            fechaIngreso = validarFechaRandom();
            if(strcmp(fechaIngreso, "-1") == 0)
            {
                return;
            }
            strncpy(nodoModificar->datoPcte.fechaIngreso, fechaIngreso, sizeof(nodoModificar->datoPcte.fechaIngreso) - 1);
            nodoModificar->datoPcte.fechaIngreso[sizeof(nodoModificar->datoPcte.fechaIngreso) -1] = '\0';
            free(fechaIngreso);
            break;
        case 2:
            fechaRetiro = validarFechaRandom();
            if(strcmp(fechaRetiro, "-1") == 0)
            {
                return;
            }
            strncpy(nodoModificar->datoPcte.fechaRetiro, fechaRetiro, sizeof(nodoModificar->datoPcte.fechaRetiro) - 1);
            nodoModificar->datoPcte.fechaRetiro[sizeof(nodoModificar->datoPcte.fechaRetiro) -1] = '\0';
            free(fechaRetiro);
            break;
        case 3:
            nroMatricula = validarMatricula();
            if(nroMatricula == -1)
            {
                return;
            }
            nodoModificar->datoPcte.matrProfesional = nroMatricula;
            break;
        case 4:
            return;
            break;
        case 5:
            persistenciaTotal(arbol);
            exit(0);
            break;
        default:
            printf("La opcion ingresada es incorrecta.\n");
            system("pause");
            break;
        }
    }while(eleccion != 4);
}

void eliminarNodo(nodoIngreso** lista, int nroIngreso)
{
    if((*lista) && ((*lista)->datoPcte.nroIngreso) == nroIngreso)
    {
        nodoIngreso* aux = *lista;
        *lista = (*lista)->sig;
        free(aux);
        return;
    }else{
        nodoIngreso* ante = *lista;
		nodoIngreso* seg = (*lista)->sig;
		while(seg && seg->datoPcte.nroIngreso != nroIngreso){
            ante = seg;
            seg = seg->sig;
        }
        if(seg){
            ante->sig = seg->sig;
            free(seg);
            return;
        }
    }
}

void liberarListaPxI(nodoPxI** subLista)
{
    nodoPxI* ante = *subLista;
    nodoPxI* prox;

    while (ante != NULL) {
        prox = ante->sig;
        free(ante);
        ante = prox;
    }

    *subLista = NULL;
}

void printListaIngreso(nodoIngreso *listaIngreso){
    nodoIngreso *seg = listaIngreso;
    while(seg)
    {
        printNodoIngreso(seg);
        seg = seg->sig;
        printf("\t ---------------------------------------------------------\n");
    }
}

/// MODIFICADA POR MI.
void baja_de_ingresos(nodoPaciente *arbol, Empleado empleadoLog){
    char control;
    int iterar;
    int nroDni;
    nodoPaciente *pcteIngreso;
    nodoIngreso *nodoModificar;
    do{
        system("cls");
        nroDni = validarDni0(arbol);
        if(nroDni == -1){
            return;
        }
        pcteIngreso = existePacienteNodo(arbol,nroDni);
        nodoIngreso* seg = pcteIngreso->listaIngreso;
        system("cls");

        while(seg)
        {
            if(empleadoLog.nroPerfil == 0 || (empleadoLog.nroPerfil != 0 && seg->datoPcte.eliminado == 0)){
                printNodoIngreso(seg);
                printf("\t ---------------------------------------------------------\n");
            }
            seg = seg->sig;
        }

        if(pcteIngreso != NULL && pcteIngreso->listaIngreso != NULL){
            nodoModificar = validarIngresoModificacion(pcteIngreso->listaIngreso);
            if(nodoModificar == NULL){
                return;
            }
            liberarListaPxI(&nodoModificar->lista);
            if(nodoModificar->lista == NULL){
                pcteIngreso->listaIngreso->datoPcte.eliminado = 1;
            }
            iterar = 0;
            printf("Ingreso eliminado. \n");
            printf(">Quiere eliminar otro ingreso? (s/n): ");
        }else if(pcteIngreso->listaIngreso == NULL){
                printf("El paciente no tiene ingresos. \n");
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
    if(strcmp(nombreArchivo, "nroIngreso.bin") == 0)
    {
        num = nroIngresoGlobal;
    }
    else if(strcmp(nombreArchivo, "nroPxI.bin") == 0)
    {
        num = nroPxIGlobal;
    }
    else
    {
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
        if(strcmp(nombreArchivo, "nroIngreso.bin") == 0)
        {
            nroIngresoGlobal = num;
        }
        else if(strcmp(nombreArchivo, "nroPxI.bin") == 0)
        {
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

nodoPaciente *buscarPacientePorIngreso(nodoPaciente *arbol, int nroIngreso)
{
    if (arbol == NULL)
    {
        return NULL;
    }

    nodoIngreso *seg = arbol->listaIngreso;
    while (seg != NULL)
    {
        if (seg->datoPcte.nroIngreso == nroIngreso)
        {
            return arbol;
        }
        seg = seg->sig;
    }
    nodoPaciente *encontradoEnIzquierda = buscarPacientePorIngreso(arbol->izq, nroIngreso);
    nodoPaciente *encontradoEnDerecha = buscarPacientePorIngreso(arbol->der, nroIngreso);
    return (encontradoEnIzquierda != NULL) ? encontradoEnIzquierda : encontradoEnDerecha;
}


void alta_de_paciente(nodoPaciente **arbol)
{
    Paciente pacienteNuevo;
    int iterar;
    int esValido; // si la carga del paciente fue cancelada (1) o no (0)
    char control;
    do
    {
        system("cls");
        printf("**ALTA DE PACIENTE**\n");
        pacienteNuevo = crearPaciente(*arbol);
        esValido = esPacienteValido(pacienteNuevo);
        if(esValido == 1)
        {
            insertarNodoPaciente(arbol, pacienteNuevo);
            iterar = 0;
            printf(">Quiere ingresar de nuevo? (s/n): ");
        }
        else
        {
            printf("Accion cancelada. \n");
            printf(">Quiere intentar de nuevo? (s/n): ");
        }
        fflush(stdin);
        scanf("%c", &control);
        if(control == 's' || control == 'S')  // control n
        {
            iterar = 1;
        }
        else
        {
            iterar = 0;
        }
    }
    while(iterar == 1);  //  iterar = 1 itera de nuevo
}

/// Printea menu para cambiar el estado del paciente (eliminado = 1 || 0).
/// Hace las validaciones pertinentes.
/// Devuelve un 1 o 0 dependiendo del estado elegido para el paciente.
void baja_de_paciente(nodoPaciente *arbol)
{
    int eleccion;
    nodoPaciente *nodoAmodificar = NULL;
    do{
        system("cls");
        if(nodoAmodificar == NULL)
        {
            nodoAmodificar = validarDniModificacion(arbol);
            if(nodoAmodificar == NULL){
                printf("No se pudo encontrar el paciente a eliminar. Intente nuevamente. \n");
                system("pause");
                return;
            }
        }
        system("cls");
        printNodoPaciente(nodoAmodificar);
        printf("\n------------------------\n");
        printf("** Eliminar un paciente **\n");
        printf("--------------------------\n");
        printf("1- Elimando.\n");
        printf("2- No eliminado.\n");
        printf("3- Volver. \n");
        printf("4- Salir. \n");

        fflush(stdin);
        printf(">Ingrese la opcion que desee: ");
        scanf("%i", &eleccion);
        switch(eleccion)
        {
        case 1:
            nodoAmodificar->datoPaciente.eliminado = 1;
            break;
        case 2:
            nodoAmodificar->datoPaciente.eliminado = 0;
            break;
        case 3:
            return;
            break;
        case 4:
            persistenciaTotal(arbol);
            exit(0);
            break;
        default:
            printf("La opcion ingresada es incorrecta.\n");
            system("pause");
            break;
        }
    }while(eleccion != 3); // opcion distinto a volver
}

/// Modularizacion, modifica el campo elegido del paciente.
void modificacion_de_paciente(nodoPaciente *arbol)
{
    char *nombre;
    int edad;
    char *direccion;
    char *telefono;
    int eleccion;
    nodoPaciente *nodoAmodificar = NULL;
    do{
        system("cls");
        if(nodoAmodificar == NULL)
        {
            nodoAmodificar = validarDniModificacion(arbol);
            if(nodoAmodificar == NULL){
                system("pause");
                return;
            }
        }
        system("cls");
        printNodoPaciente(nodoAmodificar);
        printf("\n---------------\n");
        printf("Campo a modificar\n");
        printf("-----------------\n");
        printf("1- Apellido y nombre.\n");
        printf("2- Edad.\n");
        printf("3- Direccion.\n");
        printf("4- Telefono. \n");
        printf("5- Volver. \n");
        printf("6- Salir. \n");

        fflush(stdin);
        printf(">Ingrese la opcion que desee: ");
        scanf("%i", &eleccion);
        switch(eleccion)
        {
        case 1:
            nombre = validarNombrePaciente();
            if(strcmp(nombre,"-1") == 0)
            {
                break;
            }
            strcpy(nodoAmodificar->datoPaciente.apellidoNombre, nombre);
            break;
        case 2:
            edad = validarEdad();
            if(edad == -1)
            {
                break;
            }
            nodoAmodificar->datoPaciente.edad = edad;
            break;
        case 3:
            direccion = validarDireccionPaciente();
            if(strcmp(direccion,"-1") == 0)
            {
                break;
            }
            strcpy(nodoAmodificar->datoPaciente.direccion, direccion);
            break;
        case 4:
            telefono = validarTelefonoPaciente();
            if(strcmp(telefono,"-1") == 0)
            {
                break;
            }
            strcpy(nodoAmodificar->datoPaciente.telefono, telefono);
            break;
        case 5:
            return;
            break;
        case 6:
            persistenciaTotal(arbol);
            exit(0);
            break;
        }
    }while(eleccion != 5);
}

/// Se valida el Dni de manera distinta a la validacion ya hecha
/// especifica y exclusivamente para usarse en funcion de modificacion
nodoPaciente *validarDniModificacion(nodoPaciente *arbol)
{
    int dni;
    printf(">Ingrese el DNI del paciente: ");
    fflush(stdin);
    int validoDni = scanf("%i", &dni);
    if(dni == -1)
    {
        printf("Accion cancelada. \n");
        return NULL;
    }
    nodoPaciente *nodoAmodificar = existePacienteNodo(arbol, dni);
    if(validoDni != 1)
    {
        printf("Error. El dni ingresado contiene caracteres no numericos. Intente nuevamente. \n\n");
        return NULL;
    }
    else if (dni <= 9999 || dni > 99999999)
    {
        printf("Error. El DNI debe tener entre 5 y 8 digitos. Ingrese uno valido.\n\n");
        return NULL;
    }
    else if(nodoAmodificar == NULL)
    {
        printf("Error. El DNI ingresado no pertenece a un paciente. Intente nuevamente. \n\n");
        return NULL;
    }
    return nodoAmodificar;
}

/// Printea cada nodo del arbol en orden por DNI
/// PARA TESTEOS DE FUNCIONES
void printPacientes(nodoPaciente *arbol)
{
    if(arbol != NULL)
    {
        printPacientes(arbol->izq);
        printNodoPaciente(arbol);
        printPacientes(arbol->der);
    }
}

/// Funcion que muestra todos los registros de un archivo
/// de pacientes
/// PARA TESTEOS DE FUNCIONES
void mostrarArchivoPacientes(char nombreArchivo[])
{
    FILE *archivo = fopen(nombreArchivo, "rb");
    Paciente registro;
    printf("\n**REGISTROS DE PACIENTES**\n");
    if(archivo != NULL)
    {
        while(!feof(archivo))
        {
            fread(&registro, sizeof(Paciente), 1, archivo);
            if(!feof(archivo))
            {
                mostrarRegistroPacientes(registro);
                printf("===================================\n");
            }
        }
    }
    else
    {
        printf("Error. No se pudo abrir %s en pacientes.c/mostrarArchivoPacientes\n", nombreArchivo);
    }
    fclose(archivo);
}

/// Muestra un registro de pacientes
/// PARA TESTEOS DE FUNCIONES
void mostrarRegistroPacientes(Paciente registro)
{
    printf("===================================\n");
    printf("Apellido nombre: %s\n",registro.apellidoNombre);
    printf("Direccion: %s\n",registro.direccion);
    printf("DNI: %i\n",registro.dni);
    printf("Edad: %i\n",registro.edad);
    printf("Eliminado: %i\n",registro.eliminado);
    printf("Telefono: %s\n",registro.telefono);
}

/// Inserta un nodo paciente en el arbol de pacientes
void insertarNodoPaciente(nodoPaciente **arbol, Paciente dato)
{
    if(*arbol == NULL)
    {
        *arbol = crearNodoPaciente(dato);
    }
    else
    {
        if(dato.dni > (*arbol)->datoPaciente.dni)
        {
            insertarNodoPaciente(&(*arbol)->der, dato);
        }
        else if (dato.dni < (*arbol)->datoPaciente.dni)
        {
            insertarNodoPaciente(&(*arbol)->izq, dato);
        }
    }
}

/// Crea un nodo de tipo NodoPaciente
nodoPaciente *crearNodoPaciente(Paciente dato)
{
    nodoPaciente *nuevo = (nodoPaciente*)malloc(sizeof(nodoPaciente));
    nuevo->datoPaciente = dato;
    nuevo->izq = NULL;
    nuevo->der = NULL;
    nuevo->listaIngreso = NULL;
    return nuevo;
}

/// Verifica si el paciente creado es valido o si se
/// cancelo la carga de ese
int esPacienteValido(Paciente pcte)
{
    if (strcmp(pcte.apellidoNombre, "-1") == 0)
    {
        return 0;
    }
    if (pcte.edad == -1)
    {
        return 0;
    }
    if (pcte.dni == -1)
    {
        return 0;
    }
    if (strcmp(pcte.direccion, "-1") == 0)
    {
        return 0;
    }
    if (strcmp(pcte.telefono, "-1") == 0)
    {
        return 0;
    }
    return 1;
}

/// Crea un paciente utilizando todas las funciones de validacion
Paciente crearPaciente(nodoPaciente *arbol)
{
    Paciente nuevo;

    strcpy(nuevo.apellidoNombre, validarNombrePaciente());
    if (strcmp(nuevo.apellidoNombre, "-1") == 0)
    {
        return nuevo;
    }

    nuevo.edad = validarEdad();
    if (nuevo.edad == -1)
    {
        return nuevo;
    }

    nuevo.dni = validarDni1(arbol);
    if (nuevo.dni == -1)
    {
        return nuevo;
    }

    strcpy(nuevo.direccion, validarDireccionPaciente());
    if (strcmp(nuevo.direccion, "-1") == 0)
    {
        return nuevo;
    }

    strcpy(nuevo.telefono, validarTelefonoPaciente());
    if (strcmp(nuevo.telefono, "-1") == 0)
    {
        return nuevo;
    }

    nuevo.eliminado = 0;
    return nuevo;
}

/// Se pide una cadena de caracteres y se verifica si es valida (alfabetica) y
/// Si es valida, se retorna.
/// Caso contrario, se pide la cadena de nuevo hasta que se cumplan los criterios.
char *validarTelefonoPaciente()
{
    char telefono[15];
    int valido;
    do
    {
        valido = 1;
        printf(">Ingrese el numero de telefono deseado: ");
        fflush(stdin);
//        fgets(telefono, sizeof(telefono), stdin);
//        telefono[strcspn(telefono, "\n")] = '\0';
        gets(telefono);
        if(strcmp(telefono, "-1") == 0)
        {
            return strdup("-1");
        }
        if (strlen(telefono) > 15)
        {
            printf("Error. El telefono no debe superar los 15 caracteres.\n\n");
            valido = 0;
        }
        else if(esTelefonoValido(telefono) == 0)
        {
            printf("Error. El numero de telefono ingresado contiene caracteres no validos.\n\n");
            valido = 0;
        }
    }
    while(valido != 1);
    char *cadena = strdup(telefono);
    // strdup: asigna memoria dinamicamente y copia la cadena en la mem asignada.
    return cadena;
}

/// Verifica que cada caracter de la direccion sea valido (alfabetico)
/// Los espacios y los numeros no son considerados invalidos
/// Retorna 1 si la cadena es valida, 0 si no lo es.
int esTelefonoValido(const char *cadena)
{
    for (int i = 0; i < strlen(cadena); i++)
    {
        if (!isdigit(cadena[i]) && cadena[i] != ' ')
        {
            // isalpha() devuelve 0 si no es caracter numerico
            return 0;
        }
    }
    return 1;
}

/// Se pide una cadena de caracteres y se verifica si es valida (alfabetica) y
/// Si es valida, se retorna.
/// Caso contrario, se pide la cadena de nuevo hasta que se cumplan los criterios.
char *validarDireccionPaciente()
{
    char direccion[30];
    int valido;
    do
    {
        valido = 1;
        printf(">Ingrese la direccion deseada: ");
        fflush(stdin);
//        fgets(direccion, sizeof(direccion), stdin);
//        direccion[strcspn(direccion, "\n")] = '\0';
        gets(direccion);
        if(strcmp(direccion, "-1") == 0)
        {
            return strdup("-1");
        }
        if (strlen(direccion) > 30)
        {
            printf("Error. La direccion no debe superar los 30 caracteres.\n\n");
            valido = 0;
        }
        else if(esDireccionValida(direccion) == 0)
        {
            printf("Error. La direccion ingresada contiene caracteres no validos.\n\n");
            valido = 0;
        }
    }
    while(valido != 1);
    char *cadena = strdup(direccion);
    // strdup: asigna memoria dinamicamente y copia la cadena en la mem asignada.
    return cadena;
}

/// Verifica que cada caracter de la direccion sea valido (alfabetico)
/// Los espacios, el punto y los numeros no son considerados invalidos
/// Retorna 1 si la cadena es valida, 0 si no lo es.
int esDireccionValida(const char *cadena)
{
    for (int i = 0; i < strlen(cadena); i++)
    {
        if (!isalpha(cadena[i]) && !isdigit(cadena[i]) && cadena[i] != ' ' && cadena[i] != '.')
        {
            // isalpha() devuelve 0 si no es caracter numerico
            return 0;
        }
    }
    return 1;
}

/// Se pide una cadena de caracteres y se verifica si es valida (alfabetica) y
/// Si es valida, se retorna.
/// Caso contrario, se pide la cadena de nuevo hasta que se cumplan los criterios.
char *validarNombrePaciente()
{
    char nombre[40];
    int valido;
    do
    {
        valido = 1;
        printf(">Ingrese el nombre y apellido deseado: ");
        fflush(stdin);
//        fgets(nombre, sizeof(nombre), stdin);
//        nombre[strcspn(nombre, "\n")] = '\0';
        gets(nombre);
        if(strcmp(nombre, "-1") == 0)
        {
            return strdup("-1");
        }
        if (strlen(nombre) > 40)
        {
            printf("Error. El nombre del paciente no debe superar los 40 caracteres.\n\n");
            valido = 0;
        }
        else if(esNombreValido(nombre) == 0)
        {
            printf("Error. El nombre ingresado contine caracteres no alfabeticos.\n\n");
            valido = 0;
        }
    }
    while(valido != 1);
    char *cadena = strdup(nombre);
    // strdup: asigna memoria dinamicamente y copia la cadena en la mem asignada.
    return cadena;
}

/// Verifica que cada caracter del nombre sea valido (alfabetico)
/// Los espacios no son considerados invalidos
/// Retorna 1 si la cadena es valida, 0 si no lo es.
int esNombreValido(const char *cadena)
{
    for (int i = 0; i < strlen(cadena); i++)
    {
        if (!isalpha(cadena[i]) && cadena[i] != ' ')
        {
            // isalpha() devuelve 0 si no es caracter numerico
            return 0;
        }
    }
    return 1;
}


/// Pide la edad del paciente a ser ingresado y hace las validaciones
/// pertinentes (si es un entero valido, si esta en un rango humanamente posible)
int validarEdad()
{
    int edad;
    int valido;
    do
    {
        printf(">Ingrese la edad deseada: ");
        fflush(stdin);
        valido = scanf("%i", &edad);
        if(edad == -1)
        {
            return -1;
        }
        if(valido != 1)
        {
            printf("Error. La edad ingresada no es un entero valido. \n\n");
        }
        else if(edad < 1 || edad > 114)
        {
            printf("Error. La edad ingresada no esta en un rango valido. \n\n");
            valido = 0;
        }
    }
    while(valido != 1);
    return edad;
}

int validarDni0(nodoPaciente *arbol)
{
    int nroDni;
    int valido;
    do
    {
        printf(">Ingrese el numero de DNI deseado: ");
        fflush(stdin);
        valido = scanf("%i", &nroDni);
        if(nroDni== -1)
        {
            printf("Accion cancelada. \n");
            return -1;
        }
        if(valido != 1)
        {
            printf("Error. El numero de DNI ingresado no es un entero valido. \n\n");
        }
        else if(existePaciente(arbol, nroDni) == 0)
        {
            printf("Error. El paciente no existe. Ingrese uno distinto.\n\n");
            valido = 0;
        }
        else if (nroDni <= 9999 || nroDni > 99999999)
        {
            printf("Error. El DNI debe tener entre 5 y 8 digitos. Ingrese uno valido.\n\n");
            valido = 0;
        }
    }
    while(valido != 1);
    return nroDni;
}

/// Pide el DNI del paciente a ser ingresado y hace las validaciones
/// pertinentes (si el paciente ya existe, si el DNI es entero valido,
/// y que el dni este entre 5 y 8 digitos);
int validarDni1(nodoPaciente *arbol)
{
    int nroDni;
    int valido;
    do
    {
        printf(">Ingrese el numero de DNI deseado: ");
        fflush(stdin);
        valido = scanf("%i", &nroDni);
        if(nroDni== -1)
        {
            return -1;
        }
        if(valido != 1)
        {
            printf("Error. El numero de DNI ingresado no es un entero valido. \n\n");
        }
        else if(existePaciente(arbol, nroDni) == 1)
        {
            printf("Error. El paciente ya existe. Ingrese uno distinto.\n\n");
            valido = 0;
        }
        else if (nroDni <= 9999 || nroDni > 99999999)
        {
            printf("Error. El DNI debe tener entre 5 y 8 digitos. Ingrese uno valido.\n\n");
            valido = 0;
        }
    }
    while(valido != 1);
    return nroDni;
}

/// Verifica si el paciente buscado existe en el arbol o no.
/// retorna el nodo si es encontrado, NULL si no lo encuentra.
nodoPaciente *existePacienteNodo(nodoPaciente* arbol, int dni)
{
    if (arbol == NULL || arbol->datoPaciente.dni == dni)
    {
        return arbol;
    }
    if (dni < arbol->datoPaciente.dni)
    {
        return existePacienteNodo(arbol->izq, dni);
    }
    else
    {
        return existePacienteNodo(arbol->der, dni);
    }
}

/// Verifica si el paciente ingresado por teclado existe o no en el arbol cargado.
/// Devuelve 1 si existe, 0 si no existe.
int existePaciente(nodoPaciente *arbol, int dni)
{
    if(arbol)
    {
        return(arbol->datoPaciente.dni == dni) || existePaciente(arbol->izq, dni) || existePaciente(arbol->der, dni);
    }
    else
    {
        return 0;
    }
}
/// -------------------------------------------------- FUNCIONES DE PACIENTES --------------------------------------------------- ///





/// -------------------------------------------------- FUNCIONES DE PRACTICAS -------------------------------------------------- ///

/// Da de baja una practica. Hace todas las validaciones pertinentes.
void baja_de_practica(char nombreArchivo[], nodoPaciente *arbol)
{
    FILE *archivo = fopen(nombreArchivo, "r+b");
    char control;
    char control2;
    Practica registro;
    int desplazamientoBytes;
    int valido;
    int nroPractica;
    do
    {
        system("cls");
        valido = 1;
        nroPractica = validarNroPractica0();
        if(nroPractica == -1)
        {
            printf("Accion cancelada. \n");
            return;
        }
        desplazamientoBytes = buscarPracticaXnumero(nroPractica);
        fseek(archivo, desplazamientoBytes, SEEK_SET);
        fread(&registro, sizeof(Practica), 1, archivo);
        mostrarRegistroPractica(registro);
        printf(">Es esta la practica que quiere eliminar? (s/n): ");
        fflush(stdin);
        scanf("%c", &control);
        if(control != 's' && control != 'S')  // si pones no
        {
            valido = 0;
        }else if(buscarPracticaArbol(arbol,nroPractica) == 1){
            printf("Hay ingresos asociados con esa practica. No se puede borrar. \n");
            valido = 0;
        }
        if(valido == 0)
        {
            printf(">Quiere intentar borrar una practica distinta? (s/n): ");
            fflush(stdin);
            scanf("%c", &control2);
            if(control2 != 's' && control2 != 'S')
            {
                return;
            }
        }
    }
    while(valido != 1);
    fseek(archivo, desplazamientoBytes, SEEK_SET);
    registro.eliminado = 1;
    fwrite(&registro,sizeof(Practica), 1, archivo);
    fclose(archivo);
}


/// Busca en cada Paciente para ver si el nroPractica existe en alguna de sus sublistas.
/// retorna un 1 o un 0 dependiendo de si existe o no.
int buscarPracticaArbol(nodoPaciente *arbol, int nroPractica)
{
    if(arbol)
    {
        return buscarPracticaLista(arbol->listaIngreso,nroPractica) || buscarPracticaArbol(arbol->izq, nroPractica) || buscarPracticaArbol(arbol->der, nroPractica);
    }
    else
    {
        return 0;
    }
}

/// Busca en cada Ingreso para ver si el nroPractica existe en su sublista.
/// retorna un 1 o un 0 dependiendo de si existe o no.
int buscarPracticaLista(nodoIngreso *lista, int nroPractica)
{
    nodoIngreso *seg = lista;
    while(seg != NULL)
    {
        if(buscarPracticaSub(seg->lista,nroPractica) == 1)
        {
            return 1;
        }
        seg = seg->sig;
    }
    return 0;
}

/// Busca en cada PxI para ver si el nroPractica existe.
/// retorna un 1 o un 0 dependiendo de si existe o no.
int buscarPracticaSub(nodoPxI *subLista, int nroPractica)
{
    nodoPxI *seg = subLista;
    while(seg != NULL)
    {
        if(seg->pracXing.nroPractica == nroPractica)
        {
            return 1;
        }
        seg = seg->sig;
    }
    return 0;
}

/// Funcion principal de modificacion de practica
/// Busca la practica ingresada, la muestra para corroborar que se quiera modificar,
/// se lee el registro a modificar, se suplanta el campo de nombre y se reemplaza el registro
void modificacion_de_practica(char nombreArchivo[])
{
    FILE *archivo = fopen(nombreArchivo, "r+b");
    char control;
    Practica registro;
    int desplazamientoBytes;
    int nroPractica;
    do
    {
        system("cls");
        nroPractica = validarNroPractica0();
        if(nroPractica == -1)
        {
            system("pause");
            return;
        }
        desplazamientoBytes = buscarPracticaXnumero(nroPractica);
        fseek(archivo, desplazamientoBytes, SEEK_SET);
        fread(&registro, sizeof(Practica), 1, archivo);
        mostrarRegistroPractica(registro);
        printf(">Es esta la practica que quiere modificar? (s/n): ");
        fflush(stdin);
        scanf("%c", &control);
    }
    while(control != 's' && control != 'S');
    char *nombre = validarNombrePractica();
    if(strcmp(nombre, "-1") == 0)
    {
        system("pause");
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
int buscarPracticaXnumero(int nroPractica)
{
    int nroRegistro = NroRegistroXnroPractica("practicas.bin", nroPractica);
    return nroRegistro * sizeof(Practica);
}

/// Cuenta por cada registro hasta llegar a uno que coincida con el nroPractica ingresado
/// Es usada por la funcion buscarPracticaXnumero para poder saber el numero
/// de bytes necesarios hasta ese registro (para poder usar fseek en otra funcion)
int NroRegistroXnroPractica(char nombreArchivo[], int nroPractica)
{
    FILE *archivo = fopen(nombreArchivo, "rb");
    Practica registro;
    int i=0;
    if(archivo != NULL)
    {
        while(!feof(archivo))
        {
            fread(&registro, sizeof(Practica), 1, archivo);
            if(!feof(archivo))
            {
                if(registro.nroPractica == nroPractica)
                {
                    return i;
                }
                i++;
            }
        }
    }
    else
    {
        printf("Error. No se pudo abrir archivo en practicas.c/existePractica\n");
    }
    fclose(archivo);
    return -1;
}


/// Crea una practica nueva (mediante otra funcion) y la ingresa al archivo
void alta_de_practica(char nombreArchivo[])
{
    Practica nueva;
    char control;
    int iterar;
    do
    {
        nueva.nroPractica = validarNroPractica1();
        if(nueva.nroPractica != -1)
        {
            strcpy(nueva.nombrePractica,validarNombrePractica());
            if(strcmp(nueva.nombrePractica, "-1") != 0)
            {
                nueva.eliminado = 0;
                FILE *archivo = fopen(nombreArchivo, "ab");
                if(archivo != NULL)
                {
                    fwrite(&nueva, sizeof(Practica), 1, archivo);
                }
                else
                {
                    printf("No se pudo abrir %s en alta_de_practica", nombreArchivo);
                }
                fclose(archivo);
                printf("Quiere ingresar otro? (s/n): ");
                fflush(stdin);
                scanf("%c", &control);
                if(control == 's' || control == 'S')
                {
                    iterar = 1;
                }
                else
                {
                    iterar = 0;
                }
            }else{
                printf("Accion cancelada. \n");
                system("pause");
            }
        }else{
            printf("Accion cancelada. \n");
            system("pause");
        }
    }
    while(iterar == 1);
}

char *inputNombrePrac()
{
    char nombre[30];
    int valido;
    do
    {
        valido = 1;
        printf(">Ingrese el nombre de practica deseado: ");
        fflush(stdin);
//        fgets(nombre, sizeof(nombre), stdin);
//        nombre[strcspn(nombre, "\n")] = '\0';
        gets(nombre);
        if(strcmp(nombre,"-1") == 0)
        {
            printf("Accion cancelada. \n");
            return strdup("-1");
        }
        if (strlen(nombre) > 30)
        {
            printf("Error. El nombre de la practica no debe superar los 30 caracteres.\n\n");
            valido = 0;
        }
        else if(esNombrePracValido(nombre) == 0)
        {
            printf("Error. El nombre ingresado contine caracteres no alfabeticos.\n\n");
            valido = 0;
        }
    }
    while(valido != 1);
    char *cadena = strdup(nombre);
    // strdup: asigna memoria dinamicamente y copia la cadena en la mem asignada.
    return cadena;
}

/// Se pide una cadena de caracteres y se verifica si es valida (alfabetica) y
/// si existe o no en el archivo de practicas. Si es valida y no existe, se retorna.
/// Caso contrario, se pide la cadena de nuevo hasta que se cumplan los criterios.
char *validarNombrePractica()
{
    char nombre[30];
    int valido;
    do
    {
        valido = 1;
        printf(">Ingrese el nombre de practica deseado: ");
        fflush(stdin);
//        fgets(nombre, sizeof(nombre), stdin);
//        nombre[strcspn(nombre, "\n")] = '\0';
        gets(nombre);
        if(strcmp(nombre,"-1") == 0)
        {
            printf("Accion cancelada. \n");
            return strdup("-1");
        }
        if (strlen(nombre) > 30)
        {
            printf("Error. El nombre de la practica no debe superar los 30 caracteres.\n\n");
            valido = 0;
        }
        else if(esNombrePracValido(nombre) == 0)
        {
            printf("Error. El nombre ingresado contine caracteres no alfabeticos.\n\n");
            valido = 0;
        }
        else if(existeNombrePractica(nombre, "practicas.bin") == 1)
        {
            printf("Error. El nombre de la practica ya existe. Ingrese uno distinto. \n\n");
            valido = 0;
        }
    }
    while(valido != 1);
    char *cadena = strdup(nombre);
    // strdup: asigna memoria dinamicamente y copia la cadena en la mem asignada.
    return cadena;
}

/// Verifica que cada caracter de la cadena sea valido (alfabetico)
/// Los espacios, comas y puntos, numeros, "/" y "-" no son considerados invalidos
/// Retorna 1 si la cadena es valida, 0 si no lo es.
int esNombrePracValido(const char *cadena)
{
    for (int i = 0; i < strlen(cadena); i++)
    {
        if (!isalpha(cadena[i]) && !isdigit(cadena[i]) && cadena[i] != ' ' && cadena[i] != ',' && cadena[i] != '.' && cadena[i] != '/' && cadena[i] != '-')
        {
            // isalpha() devuelve 0 si no es caracter numerico
            return 0;
        }
    }
    return 1;
}

/// Verifica si la cadena (ingresada en validarNombrePractica()
/// existe o no en el registro de practicas (archivo practicas.bin)
/// Devuelve 1 si existe, 0 si no existe.
int existeNombrePractica(const char *cadena, char nombreArchivo[])
{
    FILE *archivo = fopen(nombreArchivo, "rb");
    Practica registro;
    if(archivo != NULL)
    {
        while(!feof(archivo))
        {
            fread(&registro, sizeof(Practica), 1, archivo);
            if(!feof(archivo))
            {
                if(strcmpi(cadena, registro.nombrePractica) == 0)
                {
                    return 1;
                }
            }
        }
    }
    else
    {
        printf("Error. No se pudo abrir archivo en practicas.c/existeNombre\n");
    }
    fclose(archivo);
    return 0;
}

int validarNroPractica0()
{
    int nroPractica;
    int valido;
    do
    {
        printf(">Ingrese el numero de practica deseado: ");
        fflush(stdin);
        valido = scanf("%i", &nroPractica);
        if(nroPractica == -1)
        {
            printf("Accion cancelada. \n\n");
            return -1;
        }
        if(valido != 1)
        {
            printf("Error. El numero de practica ingresado no es un entero valido. \n\n");
        }
        else if(nroPractica < 0)
        {
            printf("Error. El numero de practica no puede ser negativo. \n\n");
            valido = 0;
        }
        else if(existeNroPractica("practicas.bin",nroPractica) == 0)
        {
            printf("Error. El numero de practica ingresado no existe. \n\n");
            valido = 0;
        }else if(estaEliminada("practicas.bin", nroPractica) == 1){
            printf("Error. El numero de practica ingresado no existe. \n\n");
            valido = 0;
        }
    }
    while(valido != 1);
    return nroPractica;
}

int estaEliminada(char nombreArchivo[], int nroPractica){
    FILE *archivo = fopen(nombreArchivo, "rb");
    Practica registro;
    if(archivo != NULL)
    {
        while(!feof(archivo))
        {
            fread(&registro, sizeof(Practica), 1, archivo);
            if(!feof(archivo))
            {
                if(registro.nroPractica == nroPractica && registro.eliminado == 1)
                {
                    return 1;
                }
            }
        }
    }
    else
    {
        printf("Error. No existen practicas. Se creara el archivo. \n");
        archivo = fopen(nombreArchivo,"ab");
        return 0;
    }
    fclose(archivo);
    return 0;
}

/// Se pide un numero de practica y se verifica si es un entero valido y si
/// el numero de practica ya existe. Se tiene que ingresar un numero hasta que
/// ese numero sea valido mediante esos dos criterios.
int validarNroPractica1()
{
    int nroPractica;
    int valido;
    do
    {
        printf(">Ingrese el numero de practica deseado: ");
        fflush(stdin);
        valido = scanf("%i", &nroPractica);
        if(nroPractica == -1)
        {

            return -1;
        }
        if(valido != 1)
        {
            printf("Error. El numero de practica ingresado no es un entero valido. \n\n");
        }
        else if(nroPractica < 0)
        {
            printf("Error. El numero de practica no puede ser negativo. \n\n");
            valido = 0;
        }
        else if(existeNroPractica("practicas.bin",nroPractica) == 1)
        {
            printf("Error. El numero de practica ingresado ya existe. \n\n");
            valido = 0;
        }
    }
    while(valido != 1);
    return nroPractica;
}


/// Recorre el archivo de practicas para verificar si un numero de practica
/// ya existe o no. Es usada en validarNroPractica
/// Devuelve 1 si existe, 0 si no existe.
int existeNroPractica(char nombreArchivo[], int nroPractica)
{
    FILE *archivo = fopen(nombreArchivo, "rb");
    Practica registro;
    if(archivo != NULL)
    {
        while(!feof(archivo))
        {
            fread(&registro, sizeof(Practica), 1, archivo);
            if(!feof(archivo))
            {
                if(registro.nroPractica == nroPractica)
                {
                    return 1;
                }
            }
        }
    }
    else
    {
        printf("Error. No existen practicas. Se creara el archivo. \n");
        archivo = fopen(nombreArchivo,"ab");
        return 0;
    }
    fclose(archivo);
    return 0;
}

/// Funcion que muestra todos los registros de un archivo
/// de practicas
void mostrarArchivoPractica(char nombreArchivo[], Empleado empleadoLog)
{
    FILE *archivo = fopen(nombreArchivo, "rb");
    Practica registro;
    if(archivo != NULL)
    {
        while(!feof(archivo))
        {
            fread(&registro, sizeof(Practica), 1, archivo);
            if(!feof(archivo))
            {
                if(empleadoLog.nroPerfil == 0 || (empleadoLog.nroPerfil != 0 && registro.eliminado == 0)){
                    mostrarRegistroPractica(registro);
                }
            }
        }
    }
    else
    {
        printf("Error. No se pudo abrir %s en practicas.c/mostrarArchivoPractica\n", nombreArchivo);
    }
    fclose(archivo);
}

/// Muestra un registro del archivo de practicas
void mostrarRegistroPractica(Practica registro)
{
    printf("\n-------------------------------------\n");
    printf("Nro de practica: %i\n", registro.nroPractica);
    printf("Nombre de practica: %s\n", registro.nombrePractica);
    printf("Eliminado: %i\n", registro.eliminado);
    printf("---------------------------------------\n");
}
/// -------------------------------------------------- FUNCIONES DE PRACTICAS -------------------------------------------------- ///
