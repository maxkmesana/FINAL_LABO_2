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
    char fechaIngreso[10];
    char fechaRetiro[10];
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
    char telefono[30];
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
/// ====================================================== ESTRUCTURAS ====================================================== ///


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
nodoPxI *validarNodoPxI(nodoPaciente *arbol);
//void generarArchivoPxI(char nombreArchivo[]); /// OBSOLETA
void mostrarRegistroPxI(PxI registro);
void mostrarArchivoPxI(char nombreArchivo[]);
void agregarPxI(nodoPxI **subLista, int nroIngresoPadre);
void printListaPxI(nodoPxI *subLista);
void printNodoPxI(nodoPxI *nodo);
void printIngresos(nodoIngreso *lista);
void alta_de_PxI(nodoPaciente *pacienteValido);
nodoIngreso *existeNodoIngreso(nodoIngreso *lista, int nroIngreso);
int validarNroIngreso();
void baja_de_pXi(nodoPaciente *arbol);
void modificacion_de_PxI(nodoPaciente *arbol);
void eleccionModifPxI(nodoPxI *nodoAmodificar);
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
/// -------------------------------------------------- FUNCIONES DE FECHA --------------------------------------------------- ///

/// -------------------------------------------------- FUNCIONES DE INGRESOS ------------------------------------------------ ///
nodoIngreso *crearNodoIngreso(stIngresos registro);
void agregarIngresoAlFinal(nodoIngreso **lista, nodoIngreso *nuevoNodo);
int validarMatricula();
char* validarFecha();
int esNombrePracValido(const char *cadena);
void printNroArchivo(char nombreArchivo[]);
void escribirNroArchivo(char nombreArchivo[]);
void actualizarNroGlobal(char nombreArchivo[]);
void crearNroArchivo(char nombreArchivo[]);
stIngresos cargaIngreso(nodoPaciente *raiz);
nodoIngreso *existeIngresoNodo(nodoIngreso *lista, int nroIngresoBuscado);
nodoIngreso *validarIngresoModificacion(nodoIngreso *lista);
nodoIngreso *buscarIngreso(nodoPaciente *arbol);
int esIngresoValido(stIngresos ingreso);
void alta_de_ingreso(nodoPaciente *pacienteValido);
void eleccionModifIngreso(nodoIngreso *nodoModificar);
void modificacion_de_ingresos(nodoPaciente *arbol);
void printNodoIngreso(nodoIngreso* nodoPrinteo);
/// -------------------------------------------------- FUNCIONES DE INGRESOS ------------------------------------------------ ///

/// -------------------------------------------------- FUNCIONES DE PACIENTES ----------------------------------------------- ///
int existePaciente(nodoPaciente *arbol, int dni);
int validarDni(nodoPaciente *arbol);
int validarEdad();
int esNombreValido(const char *cadena);
char *validarNombrePaciente();
int esDireccionValida(const char *cadena);
char *validarDireccionPaciente();
int esTelefonoValido(const char *cadena);
char *validarTelefonoPaciente();
Paciente crearPaciente(nodoPaciente *arbol);
nodoPaciente *crearNodoPaciente(Paciente dato);
nodoPaciente *insertarNodoPaciente(nodoPaciente *arbol, Paciente dato);
nodoPaciente *pacienteToarchivo(char nombreArchivo[], nodoPaciente *arbol);
void mostrarRegistroPacientes(Paciente registro);
void mostrarArchivoPacientes(char nombreArchivo[]);
void inorder(nodoPaciente *arbol);
nodoPaciente *archivoToArbolPacientes(char nombreArchivo[], nodoPaciente *arbol);
nodoPaciente *existePacienteNodo(nodoPaciente* arbol, int dni);
int validarEliminado();
void eleccionModifPaciente(int eleccion, nodoPaciente *nodoAmodificar);
void modificacion_de_paciente(nodoPaciente *arbol, int eleccion);
nodoPaciente *validarDniModificacion(nodoPaciente *arbol);
void baja_de_paciente(nodoPaciente *arbol);
int esPacienteValido(Paciente pcte);
nodoPaciente *alta_de_paciente(nodoPaciente *arbol);
/// -------------------------------------------------- FUNCIONES DE PACIENTES ----------------------------------------------- ///

/// -------------------------------------------------- FUNCIONES DE PRACTICAS ----------------------------------------------- ///
void mostrarRegistroPractica(Practica registro);
void mostrarArchivoPractica(char nombreArchivo[]);
int existeNroPractica(char nombreArchivo[], int nroPractica);
int validarNroPractica1();
int validarNroPractica0();
int existeNombrePractica(const char *cadena, char nombreArchivo[]);
int esCadenaValida(const char *cadena);
char *validarNombrePractica();
void alta_de_practica(char nombreArchivo[]);
int NroRegistroXnroPractica(char nombreArchivo[], int nroPractica);
int buscarPracticaXnumero(int nroPractica);
int NroRegistroXnombre(char nombreArchivo[], char nombrePractica[]); //comentar o no dependiendo de respuesta del profe
int buscarPracticaXnombre(); // comentar o no dependiendo de respuesta del profe
//void modificacion_de_practica(char nombreArchivo[], int desplazamientoBytes); // descomentar y cambiar dependiendo de respuesta del profe
void menu_modificacion();
void modificacion_de_practica(char nombreArchivo[]);
int buscarPracticaSub(nodoPxI *subLista, int nroPractica);
int buscarPracticaLista(nodoIngreso *lista, int nroPractica);
int buscarPracticaArbol(nodoPaciente *arbol, int nroPractica);
void baja_de_practica(char nombreArchivo[], nodoPaciente *arbol);
/// -------------------------------------------------- FUNCIONES DE PRACTICAS ----------------------------------------------- ///

#endif // GESTION_H_INCLUDED
