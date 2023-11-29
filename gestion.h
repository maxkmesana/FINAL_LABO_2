#ifndef GESTION_H_INCLUDED
#define GESTION_H_INCLUDED

/// ====================================================== ESTRUCTURAS ====================================================== ///
typedef struct{
    int nroIngreso;
    int nroPxI;
    int nroPractica;
    char resultado[40];
}PxI;

typedef struct nodoPxI{
    PxI pracXing;
    struct nodoPxI *sig;
}nodoPxI;

typedef struct {
    int nroIngreso;
    char fechaIngreso[11];
    char fechaRetiro[11];
    int dniPcte;
    int matrProfesional;
    int eliminado;
} stIngresos;

typedef struct nodoIngreso{
    stIngresos datoPcte;
    struct nodoIngreso *sig;
    nodoPxI *lista;
}nodoIngreso;

typedef struct{
    char apellidoNombre[40];
    int edad;
    int dni;
    char direccion[30];
    char telefono[15];
    int eliminado;
}Paciente;

typedef struct nodoPaciente{
    Paciente datoPaciente;
    nodoIngreso *listaIngreso;
    struct nodoPaciente *izq;
    struct nodoPaciente *der;
}nodoPaciente;

typedef struct{
    int nroPractica;
    char nombrePractica[30];
    int eliminado;
} Practica;

typedef struct{
int dni;
char apellidoNombre[40];
char telefono[15];
char usuario[20];
char contra[20];
char perfil[20];
int nroPerfil; // 0: administrador, 1: profesional de lab, 2: administrativo
}Empleado;

typedef struct nodoListaEmp{
    Empleado dato;
    struct nodoListaEmp *sig;
}nodoListaEmp;

typedef struct nodoListaPrac{
    Practica dato;
    struct nodoListaPrac *sig;
}nodoListaPrac;


/// ====================================================== ESTRUCTURAS ====================================================== ///

/// -------------------------------------------------- FUNCIONES ADICIONALES ----------------------------------------------- ///


/// PRACTICAS ///
int cadenaContenida(const char* haystack, const char* needle);
void listarPracticasPorInicio(char nombreArchivo[], Empleado empleadoLog);
nodoListaPrac *crearNodoPrac(Practica nuevaPractica);
void printNodoPractica(nodoListaPrac *nodo);
void mostrarListaPrac(nodoListaPrac* lista, Empleado empleadoLog);
void testearListaPrac(Empleado empleadoLog);
nodoListaPrac* agregarPracOrdenada(nodoListaPrac* listaOrdenadaPrac, Practica nuevaPrac);
nodoListaPrac* cargarPracArchivo(nodoListaPrac* listaOrdenadaPrac);
char *validarNombrePractica2();

/// PRACTICAS ///

/// PACIENTES ///
void insertarPacienteAlfabetico(nodoPaciente **ordenado, nodoPaciente *nodoActual);
void ordenarArbolAlfabetico(nodoPaciente *arbol, nodoPaciente **destino);
nodoPaciente *crearNodoPacienteAlf(Paciente dato, nodoIngreso *listaIngreso);
void buscarPacientePorDni(nodoPaciente *arbol, Empleado empleadoLog);
void printPacientesIngresos(nodoPaciente *arbol, Empleado empleadoLog);
void printNodoPaciente2(nodoPaciente *nodo, Empleado empleadoLog);
void printListaIngreso2(nodoIngreso *listaIngreso, Empleado empleadoLog);
/// PACIENTES ///

/// INGRESOS ///
void printIngresosXdni(char nombreArchivo[], Empleado empleadoLog, int dni);
char *buscarNombrePractica(char nombreArchivo[], int nroPractica);
void printPxIRegistros(char nombreArchivo[], int nroIngreso);
void printIngresosXfecha(char nombreArchivo[], Empleado empleadoLog);
void printIngresosXnro(char nombreArchivo[], int nroIngreso, Empleado empleadoLog);
void ingresosDesdeHasta();
/// INGRESOS ///

/// -------------------------------------------------- FUNCIONES ADICIONALES ----------------------------------------------- ///


/// ------------------------------------------------ FUNCIONES DE PRINTEO TOTAL -------------------------------------------- ///
void printNodoPxI(nodoPxI *nodo);
void printListaPxI(nodoPxI *subLista);
void printNodoIngreso(nodoIngreso* nodoPrinteo);
void printParcial(nodoIngreso *lista, Empleado empleadoLog);
void printNodoPaciente(nodoPaciente *nodo);
void printTotal(nodoPaciente *arbol, Empleado empleadoLog);

/// ------------------------------------------------ FUNCIONES DE PRINTEO TOTAL -------------------------------------------- ///


/// ------------------------------------------------ FUNCIONES DE PERSISTENCIA --------------------------------------------- ///
void escribirPxI(FILE *archivoPxI, nodoPxI *subLista);
void escribirIngYpXi(FILE *archivoIng, FILE *archivoPxI, nodoIngreso *lista);
void escribirPaciente(FILE *archivo, nodoPaciente *arbol);
void persistenciaIngYpXi(nodoIngreso *lista);
void persistenciaTotal(nodoPaciente *arbol);
void filesToEstructuras(nodoPaciente **arbol);
/// ------------------------------------------------ FUNCIONES DE PERSISTENCIA --------------------------------------------- ///

/// -------------------------------------------------- FUNCIONES DE PRACXING ----------------------------------------------- ///
int esResultadoValido(const char *cadena);
char *validarResultado();
PxI crearPxI(int nroIngresoPadre);
nodoPxI *crearNodoPxI(PxI pracXing);
int esPxIValido(PxI pracXing);
void agregarPxIAlFinal(nodoPxI **subLista, nodoPxI *nuevoNodo);
nodoIngreso *validarIngresoModificacion(nodoIngreso *lista);
nodoIngreso *validarIngresoModificacion(nodoIngreso *lista);
nodoPxI *buscarNodoPxI(nodoPxI *subLista);
int validarNroPxI(nodoPxI *subLista);
int existeNroPxI(nodoPxI *subLista, int nroPxIBuscado);
void borrarNodo(nodoPxI **subLista, int nroPxIBorrar);
nodoPxI *validarNodoPxI(nodoPaciente *arbol, Empleado empleadoLog);
//void generarArchivoPxI(char nombreArchivo[]); /// OBSOLETA
void mostrarRegistroPxI(PxI registro);
void mostrarArchivoPxI(char nombreArchivo[]);
void agregarPxI(nodoPxI **subLista, int nroIngresoPadre);
void printNodoPxI(nodoPxI *nodo);
void alta_de_PxI(nodoPaciente *pacienteValido);
nodoIngreso *existeNodoIngreso(nodoIngreso *lista, int nroIngreso);
int validarNroIngreso();
void baja_de_pXi(nodoPaciente *arbol, Empleado empleadoLog);
void modificacion_de_PxI(nodoPaciente *arbol, Empleado empleadoLog);
void escribirPxI(FILE *archivo, nodoPxI *subLista);
void liberarListaPxI(nodoPxI** subLista);
void agregarPxIAdministrativo(nodoPxI **subLista, int nroIngresoPadre);
PxI crearPxIAdministrativo(int nroIngresoPadre);
void alta_de_PxIAdministrativo(nodoPaciente *pacienteValido);
void buscarRegistroPractica(int nroPractica);
/// -------------------------------------------------- FUNCIONES DE PRACXING ----------------------------------------------- ///

/// -------------------------------------------------- FUNCIONES DE FECHA -------------------------------------------------- ///
void fGetNow(char date[]);
int fGetAnioLarge();
int fGetMinute(char date[]);
int fGetHour(char date[]);
int fGetDayNumber();
int fGetMonthNumber();
int fGetAnioShort();
char* fechaConcatenada();
char* fechaFormatear(int dia, int mes, int anio);
int obtenerDiasMes(int mes, int anio);
int obtenerDia(int mes, int anio);
int obtenerMes(int anio);
int obtenerAnio();
char* validarFecha();
struct tm convertirFecha(char *fecha);
int obtenerAnioRandom();
char* validarFechaRandom();
/// -------------------------------------------------- FUNCIONES DE FECHA --------------------------------------------------- ///

/// -------------------------------------------------- FUNCIONES DE INGRESOS ------------------------------------------------ ///
nodoIngreso *crearNodoIngreso(stIngresos registro);
void agregarIngresoAlFinal(nodoIngreso **lista, nodoIngreso *nuevoNodo);
int validarMatricula();
int esNombrePracValido(const char *cadena);
void printNroArchivo(char nombreArchivo[]);
void escribirNroArchivo(char nombreArchivo[]);
void actualizarNroGlobal(char nombreArchivo[]);
void crearNroArchivo(char nombreArchivo[]);
stIngresos cargaIngreso(nodoPaciente *raiz);
nodoIngreso *existeIngresoNodo(nodoIngreso *lista, int nroIngresoBuscado);
nodoIngreso *validarIngresoModificacion(nodoIngreso *lista);
nodoIngreso *buscarIngreso(nodoPaciente *arbol, Empleado empleadoLog);
int esIngresoValido(stIngresos ingreso);
void alta_de_ingreso(nodoPaciente *pacienteValido);
void modificacion_de_ingresos(nodoPaciente *arbol, Empleado empleadoLog);
void printNodoIngreso(nodoIngreso* nodoPrinteo);
void mostrarRegistroIng(stIngresos registro);
void mostrarArchivoIng(char nombreArchivo[]);
void baja_de_ingresos(nodoPaciente *arbol, Empleado empleadoLog);
void eliminarNodo(nodoIngreso** lista, int dni);
void printListaIngreso(nodoIngreso *listaIngreso);
nodoIngreso *validarNodoIngreso(nodoPaciente *arbol, Empleado empleadoLog);
/// -------------------------------------------------- FUNCIONES DE INGRESOS ------------------------------------------------ ///

/// -------------------------------------------------- FUNCIONES DE PACIENTES ----------------------------------------------- ///
int existePaciente(nodoPaciente *arbol, int dni);
int validarDni1(nodoPaciente *arbol);
int validarDni0(nodoPaciente *arbol);
int validarEdad();
int esNombreValido(const char *cadena);
char *validarNombrePaciente();
int esDireccionValida(const char *cadena);
char *validarDireccionPaciente();
int esTelefonoValido(const char *cadena);
char *validarTelefonoPaciente();
Paciente crearPaciente(nodoPaciente *arbol);
nodoPaciente *crearNodoPaciente(Paciente dato);
void insertarNodoPaciente(nodoPaciente **arbol, Paciente dato);
void mostrarRegistroPacientes(Paciente registro);
void mostrarArchivoPacientes(char nombreArchivo[]);
void printPacientes(nodoPaciente *arbol);
nodoPaciente *existePacienteNodo(nodoPaciente* arbol, int dni);
int validarEliminado();
void modificacion_de_paciente(nodoPaciente *arbol);
nodoPaciente *validarDniModificacion(nodoPaciente *arbol);
void baja_de_paciente(nodoPaciente *arbol);
int esPacienteValido(Paciente pcte);
void alta_de_paciente(nodoPaciente **arbol);
nodoPaciente *buscarPacientePorIngreso(nodoPaciente *arbol, int nroIngreso);
/// -------------------------------------------------- FUNCIONES DE PACIENTES ----------------------------------------------- ///

/// -------------------------------------------------- FUNCIONES DE PRACTICAS ----------------------------------------------- ///
void mostrarRegistroPractica(Practica registro);
void mostrarArchivoPractica(char nombreArchivo[], Empleado empleadoLog);
int existeNroPractica(char nombreArchivo[], int nroPractica);
int validarNroPractica1();
int validarNroPractica0();
int existeNombrePractica(const char *cadena, char nombreArchivo[]);
int esNombreValido(const char *cadena);
char *validarNombrePractica();
void alta_de_practica(char nombreArchivo[]);
int NroRegistroXnroPractica(char nombreArchivo[], int nroPractica);
int buscarPracticaXnumero(int nroPractica);
int NroRegistroXnombre(char nombreArchivo[], char nombrePractica[]); //comentar o no dependiendo de respuesta del profe
int buscarPracticaXnombre(); // comentar o no dependiendo de respuesta del profe
//void modificacion_de_practica(char nombreArchivo[], int desplazamientoBytes); // descomentar y cambiar dependiendo de respuesta del profe
void modificacion_de_practica(char nombreArchivo[]);
int buscarPracticaSub(nodoPxI *subLista, int nroPractica);
int buscarPracticaLista(nodoIngreso *lista, int nroPractica);
int buscarPracticaArbol(nodoPaciente *arbol, int nroPractica);
void baja_de_practica(char nombreArchivo[], nodoPaciente *arbol);
char *inputNombrePrac();
int estaEliminada(char nombreArchivo[], int nroPractica);
/// -------------------------------------------------- FUNCIONES DE PRACTICAS ----------------------------------------------- ///

///---------------------------------------- FUNCIONES DE EMPLEADOS -------------------------------------------------------------///
void menuAdministrativos(Empleado empleadoLog);
void menuProfesionales();
void crearAdministrador();
Empleado cargarEmpleadoTeclado();
char *validarUsuario0();
char *validarUsuario1();
int buscarUsuario(char usuario[20]);
int buscarEmpleado(int dni);
int validarDniEmpleado();
void alta_de_empleado();
void modificacion_de_empleado(nodoPaciente *arbol);
void baja_de_empleado();
nodoListaEmp* agregarEmpOrdenado(nodoListaEmp* listaOrdenadaEmp, Empleado nuevoEmpleado);
nodoListaEmp* cargarEmpleadoArchivo(nodoListaEmp* listaOrdenadaEmp);
nodoListaEmp *crearNodoEmp(Empleado nuevoEmpleado);
void mostrarListaEmp(nodoListaEmp* lista, int nroPerfil);
//consultar empleado x dni
void mostrarEmpleado(Empleado empleadoNuevo,int nroPerfil);
void testearListaEmp(int nroPerfil);
Empleado buscarEmpleadoAdicional(int dni,int nroPerfil);
char *validarNombreEmpleado();
int esContraValida(const char *cadena);
int esEmpleadoValido(Empleado registro);
char *validarContra();
int eleccionPerfil();
int validarDniEmpleado0();
int validarDniEmpleado1();
void mostrarArchivoEmpleado(char nombreArchivo[]);
///---------------------------------------- FUNCIONES DE EMPLEADOS -------------------------------------------------------------///

///--------------------------------- FUNCIONES log in y menu principal ---------------------------------------------------------///
Empleado logIn();
void menuPrincipal();
void menuProfesionales(Empleado empleadoLog);
void menuAdministrador(Empleado empleadoLog);
void menuAdministrativos(Empleado empleadoLog);
void menuGestionPxI(nodoPaciente*arbol, Empleado empleadoLog);
void menuGestionIngresos(nodoPaciente *arbol, Empleado empleadoLog);
void menuGestionPracticas(nodoPaciente *arbol, Empleado empleadoLog);
void menuGestionPacientes(nodoPaciente*arbol, Empleado empleadoLog);
void menuGestionEmpleados(nodoPaciente*arbol, Empleado empleadoLog);
void menuPracticasRestringido(nodoPaciente *arbol, Empleado empleadoLog);
void menu();
Empleado buscarUsuarioLog(char usuario[20], int* flag);
void menuBuscarIngreso(Empleado empleadoLog);
///--------------------------------- FUNCIONES log in y menu principal ---------------------------------------------------------///

#endif // GESTION_H_INCLUDED
