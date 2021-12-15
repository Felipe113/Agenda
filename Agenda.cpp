#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <conio.h>


using namespace std;
const char DIR_ARCHIVO[] = "contactos.dat";

struct Contacto{
  char nombre[30];
  char apellido[45];
  int edad;
  int dia;
  int mes;
  int ano;
  char telefono[16];
};
//Variables globales
  int anioAct;  //Año Actual
  int mesAct; //Mes actual
  int fechAct; //Fecha actual

/**FUNCIONES DEL PROGRAMA**/
void ingresarContacto(Contacto c);
int tamadat();
void creardata();
Contacto leerContacto();
bool existeContacto(char nombre[], char apellido[]);
Contacto buscarContacto(char nombre[], char apellido[]);
void mostrarContacto(Contacto c);
void listarContactos();
void listarPorEdad();
void listarPorMes();
void burbuja(Contacto[], int);
void burbujames(Contacto[], int);
void burbujaApellido(Contacto[], int);
void eliminarContacto(char nombre[],char apellido[]);
void acctualizarContacto(char nombre[],char apellido[]);
int calcularEdad( int anioAct , int mesAct, int fechAct, int ano, int mes, int dia);
/*****************************/

/**FUNCIONES PARA EL MENÚ**/
void menuIngresar();
void menuBuscar();
void menuListado();
void menuEliminar();
void menuActualizar();
/*****************************/
/**Estetica**/
void hastaPronto();
/*****************************/
int main(int argc, char const *argv[]){
  system("color ff");
  int op, MAXCON=20;//Define el Maximo de Contactos
  int yn = 0;
    system("cls");
    cout << "Ingrese Año Actual AAAA: ";   cin >> anioAct;
    cout << "Ingrese Mes Actual MM  : ";     cin >> mesAct;
    cout << "Ingrese Dia Actual DD  : ";     cin >> fechAct;
  do {
    system("cls");
    cout << "\t\t\t=======================================" << '\n';
    cout << "\t\t\t||                                   ||" << '\n';
    cout << "\t\t\t||             AGENDA                ||" << '\n';
    cout << "\t\t\t||                                   ||" << '\n';
    cout << "\t\t\t||  Ingresar Contacto...........[1]  ||" << '\n';
    cout << "\t\t\t||  Buscar Contacto.............[2]  ||" << '\n';
    cout << "\t\t\t||  Listar Contactos............[3]  ||" << '\n';
    cout << "\t\t\t||  Eliminar Contacto...........[4]  ||" << '\n';
    cout << "\t\t\t||  Actualizar Contacto.........[5]  ||" << '\n';
    cout << "\t\t\t||  SALIR.......................[6]  ||" << '\n';
    cout << "\t\t\t||                                   ||" << '\n';
    cout << "\t\t\t=======================================" << '\n';
    cout << "\t\t\t    Ingrese una opcion ->";
    cin >> op;
    cin.ignore();
    switch (op) {
      case 1: system("cls"); //CLS = CLean Screen (en la lib <cstdlib> )
              creardata();
              if(tamadat()==(MAXCON - 2)){
                cout << "Solo te quedan 2 lugares en la agenda" << endl;
                system("pause");
              }
              if(tamadat()<MAXCON){
                    menuIngresar();
                  system("pause");
              }else{
                cout << "Maximo de contactos en agenda" << endl;
                system("pause");
                }
              
              //cout << tamadat() << endl;
              break;
      case 2: system("cls");
              menuBuscar();
              break;
      case 3: system("cls");
              menuListado();
              break;
      case 4: system("cls");
              menuEliminar();
              break;
      case 5: system("cls");
              menuActualizar();
              break;
      case 6: system("cls");
              do {
                cout << "\t\t\tSeguro que quieres SALIR? SI[1].....NO[2]: ";
                cin >> yn;
                if (yn != 1 &&  yn != 2) {
                  cout << "\t\t\t" << yn << " NO es una opcion valida" << '\n';
                }
              } while(yn != 1 && yn !=2);
              system("cls");
              hastaPronto();
              break;

      default: cout << "\t\t\tOpcion incorrecta" << '\n';
               break;
    }
  } while(op != 6 || yn == 0 || yn ==2);

  return 0;
}

/*PROGRAMA*/
void creardata(){
  ofstream salida;

	salida.open(DIR_ARCHIVO,ios::binary | ios::app);
	salida.close();
}

//Ingresar Contacto
void ingresarContacto(Contacto c){
	ofstream salida;

	salida.open(DIR_ARCHIVO,ios::binary | ios::app);
	salida.write((char*)&c, sizeof(Contacto));
	salida.close();
}

int tamadat(){
  
  	Contacto registro;
    int tamanyo = sizeof(Contacto); // Tamaño de un registro
    int i = 0, num_registros = 0;
    ifstream in;
    in.open(DIR_ARCHIVO, ios::binary);
    if(in.fail())
    {
       cout << "Error al abrir el fichero" << endl;
       system("pause");
       exit(1);
    }

    //cálculo del número de registros del fichero
    in.seekg(0,ios::end); //nos posicionamos al final del fichero
    //número de registros = bytes / tamaño en bytes de un registro
    num_registros=in.tellg()/tamanyo;
  in.close();
  return num_registros;
}


Contacto leerContacto(){
  setlocale(LC_CTYPE, "Spanish");
  Contacto c;
  cout << "\t\t\tNOMBRE   : ";
  cin.getline(c.nombre,30);
  cout << "\t\t\tAPELLIDO : ";
  cin.getline(c.apellido,45);
  cin.ignore();
  cout << "\t\t\t-FECHA DE NACIMIENTO-"<< endl;;
  cout << "\t\t\tDia   DD: ";
  cin >> c.dia;
  cin.ignore();
  cout << "\t\t\tMes   MM: ";
  cin >> c.mes;
  cin.ignore();
  cout << "\t\t\tAño AAAA: ";
  cin >> c.ano;
  cin.ignore();
  c.edad = calcularEdad( anioAct , mesAct, fechAct, c.ano, c.mes, c.dia);
  cout << "\t\t\tTELEFONO : ";
  cin.getline(c.telefono,16);
  return c;
}

//Buscar Contacto
bool existeContacto(char nombre[], char apellido[]){
  Contacto c;
  ifstream entrada;
  entrada.open(DIR_ARCHIVO,ios::binary);
  if (entrada.good()) {
    while(entrada.read( (char*) &c, sizeof(Contacto) )){
      /*strmcp --- string comparation --- comparacion de cadena*/
      if ( strcmp (nombre, c.nombre) == 0 && strcmp (apellido, c.apellido) == 0) {
        entrada.close();
        return true;
      }
    }
    entrada.close();
    return false;
  }

  else{
    entrada.close();
    return false;
  }
}

Contacto buscarContacto(char nombre[], char apellido[]){
  Contacto c;
  ifstream entrada;
  entrada.open(DIR_ARCHIVO,ios::binary);
  if (entrada.good()) {
    while(entrada.read( (char*) &c, sizeof(Contacto) )){
      /*strmcp --- string comparation --- comparacion de cadena*/
      if ( strcmp (nombre, c.nombre) == 0 && strcmp (apellido, c.apellido) == 0) {
        entrada.close();
        return c;
      }
    }
  }
}

void mostrarContacto(Contacto c){
  setlocale(LC_CTYPE, "Spanish");
  cout << '\n';
  cout << "\t\t\tNOMBRE               : " << c.nombre << '\n';
  cout << "\t\t\tAPELLIDO             : " << c.apellido << '\n';
  cout << "\t\t\tEDAD                 : " << c.edad << '\n';
  cout << "\t\t\tCUMPLEAÑOS DD/MM/AAAA: " << c.dia << "/" << c.mes << "/" << c.ano << '\n';
  cout << "\t\t\tTELEFONO             : " << c.telefono << '\n';
  cout << '\n';
}

//Listar Contactos
void listarContactos(){
  ifstream entrada;
  entrada.open(DIR_ARCHIVO,ios::binary);
  Contacto c;
  if(entrada.good()){
    while(entrada.read( (char*) &c , sizeof (Contacto) )){
      mostrarContacto(c);
    }
    entrada.close();
  }

  else{
    cout << "NO se puede leer el archivo" << '\n';
    entrada.close();
  }
}

// Listar por edad
void listarPorEdad(){
  setlocale(LC_CTYPE, "Spanish");
	Contacto registro, *array;
    int tamanyo = sizeof(Contacto); // Tamaño de un registro
    int i = 0, num_registros;
    ifstream in;
    in.open(DIR_ARCHIVO, ios::binary);
    if(in.fail())
    {
       cout << "Error al abrir el fichero" << endl;
       system("pause");
       exit(1);
    }

    //cálculo del número de registros del fichero
    in.seekg(0,ios::end); //nos posicionamos al final del fichero
    //número de registros = bytes / tamaño en bytes de un registro
    num_registros=in.tellg()/tamanyo;

    //creamos el array con el tamaño calculado
    array = new Contacto[num_registros];
    if(array==NULL)
    {
       cout << "Error en la asignación de memoria\n";
       system("pause");
       exit(1);
    }

    in.seekg(0); //volvemos al pincipio del fichero
    // Pasamos los registros al array
    in.read((char *) &array[i], tamanyo);
    while(!in.eof())
    {
         i++;
         in.read((char *) &array[i], tamanyo);
    }
    burbuja(array,num_registros); //ordenar el array
    system("cls");
    //mostrar todos los contactos ordenados por edad
    for(i=0;i<num_registros;i++)
    {
            cout << "\nCONTACTO: " << i+1 << endl;
            cout << "\t\t\tNOMBRE               : " << array[i].nombre << endl;
            cout << "\t\t\tAPELLIDO             : " << array[i].apellido << endl;
            cout << "\t\t\tEDAD                 : " << array[i].edad << endl;
            cout << "\t\t\tCUMPEAÑOS DD/MM/AAAA: " << array[i].dia<< "/" << array[i].mes << "/" << array[i].ano << endl;
			      cout << "\t\t\tTELEFONO             : " << array[i].telefono << endl;
			
    }
    in.close();
    delete [] array;
}

// Listar por apellido
void listarPorApellido(){
  setlocale(LC_CTYPE, "Spanish");
	Contacto registro, *array;
    int tamanyo = sizeof(Contacto); // Tamaño de un registro
    int i = 0, num_registros;
    ifstream in;
    in.open(DIR_ARCHIVO, ios::binary);
    if(in.fail())
    {
       cout << "Error al abrir el fichero" << endl;
       system("pause");
       exit(1);
    }

    //cálculo del número de registros del fichero
    in.seekg(0,ios::end); //nos posicionamos al final del fichero
    //número de registros = bytes / tamaño en bytes de un registro
    num_registros=in.tellg()/tamanyo;

    //creamos el array con el tamaño calculado
    array = new Contacto[num_registros];
    if(array==NULL)
    {
       cout << "Error en la asignación de memoria\n";
       system("pause");
       exit(1);
    }

    in.seekg(0); //volvemos al pincipio del fichero
    // Pasamos los registros al array
    in.read((char *) &array[i], tamanyo);
    while(!in.eof())
    {
         i++;
         in.read((char *) &array[i], tamanyo);
    }
    burbujaApellido(array,num_registros); //ordenar el array
    system("cls");
    //mostrar todos los contactos ordenados por edad
    for(i=0;i<num_registros;i++)
    {
            cout << "\nCONTACTO: " << i+1 << endl;
            cout << "\t\t\tNOMBRE               : " << array[i].nombre << endl;
            cout << "\t\t\tAPELLIDO             : " << array[i].apellido << endl;
            cout << "\t\t\tEDAD                 : " << array[i].edad << endl;
            cout << "\t\t\tCUMPEAÑOS DD/MM/AAAA: " << array[i].dia<< "/" << array[i].mes << "/" << array[i].ano << endl;
			      cout << "\t\t\tTELEFONO             : " << array[i].telefono << endl;
			
    }
    in.close();
    delete [] array;
}

// Listar por mes
void listarPorMes(){
  setlocale(LC_CTYPE, "Spanish");
	Contacto registro, *array;
    int tamanyo = sizeof(Contacto); // Tamaño de un registro
    int i = 0, num_registros;
    ifstream in;
    in.open(DIR_ARCHIVO, ios::binary);
    if(in.fail())
    {
       cout << "Error al abrir el fichero" << endl;
       system("pause");
       exit(1);
    }

    //cálculo del número de registros del fichero
    in.seekg(0,ios::end); //nos posicionamos al final del fichero
    //número de registros = bytes / tamaño en bytes de un registro
    num_registros=in.tellg()/tamanyo;

    //creamos el array con el tamaño calculado
    array = new Contacto[num_registros];
    if(array==NULL)
    {
       cout << "Error en la asignación de memoria\n";
       exit(1);
    }

    in.seekg(0); //volvemos al pincipio del fichero
    // Pasamos los registros al array
    in.read((char *) &array[i], tamanyo);
    while(!in.eof())
    {
         i++;
         in.read((char *) &array[i], tamanyo);
    }
    burbujames(array,num_registros); //ordenar el array
    system("cls");
    //mostrar todos los contactos ordenados por edad
    for(i=0;i<num_registros;i++)
    {
            cout << "\nCONTACTO: " << i+1 << endl;
            cout << "\t\t\tNOMBRE               : " << array[i].nombre << endl;
            cout << "\t\t\tAPELLIDO             : " << array[i].apellido << endl;
            cout << "\t\t\tEDAD                 : " << array[i].edad << endl;
            cout << "\t\t\tCUMPEAÑOS DD/MM/AAAA: " << array[i].dia<< "/" << array[i].mes << "/" << array[i].ano << endl;
			      cout << "\t\t\tTELEFONO             : " << array[i].telefono << endl;
			
    }
    in.close();
    delete [] array;
}

//función para ordenar el array de contactos por el método de la burbuja por edad
void burbuja(Contacto A[], int N)
{
    int i, j;
    Contacto aux;
    for(i=0;i<N-1;i++)      
        for(j=0;j<N-i-1;j++)
            if(A[j+1].edad<A[j].edad) 
            {               
              aux=A[j+1];   
              A[j+1]=A[j];  
              A[j]=aux;
            }
}

//función para ordenar el array de contactos por el método de la burbuja por mes
void burbujames(Contacto A[], int N)
{
    int i, j;
    Contacto aux;
    for(i=0;i<N-1;i++)      
        for(j=0;j<N-i-1;j++)
            if(A[j+1].mes<A[j].mes) 
            {               
              aux=A[j+1];   
              A[j+1]=A[j];  
              A[j]=aux;
            }
}
//función para ordenar el array de contactos por el método de la burbuja por apellido
void burbujaApellido(Contacto A[], int N)
{
    int i, j;
    Contacto aux;
    int salto = N / 2;

while(salto > 0){
    for(int i = salto; i < N; i++){
        int j = i - salto;

        while(j >= 0){
            int k = j + salto;

            if(strcmp(A[j].apellido, A[k].apellido) < 0){
                j = -1;
            }else{
                 aux = A[j];
                A[j] = A[k];
                A[k] = aux;
            }
        }           
    }
    salto /= 2;
}
}
//Eliminar Contacto
void eliminarContacto(char nombre[],char apellido[]){
  ifstream entrada;
  ofstream temporal;

  entrada.open(DIR_ARCHIVO,ios::binary);
  temporal.open("temp.dat",ios::binary);

  Contacto c;
  if(entrada.good()){
    while(entrada.read( (char*) &c , sizeof (Contacto) )){
      if ( strcmp (nombre, c.nombre) != 0 || strcmp (apellido, c.apellido) != 0) {
        temporal.write( (char*) &c, sizeof(Contacto) );
      }
    }
  }
  entrada.close();
  temporal.close();

  remove(DIR_ARCHIVO);
  rename("temp.dat",DIR_ARCHIVO);
}

//Acctualizar Contacto
void acctualizarContacto(char nombre[],char apellido[]){
  ifstream entrada;
  ofstream temporal;

  entrada.open(DIR_ARCHIVO,ios::binary);
  temporal.open("temp.dat",ios::binary);

  Contacto c;
  if(entrada.good()){
    while(entrada.read( (char*) &c , sizeof (Contacto) )){
      if ( strcmp (nombre, c.nombre) == 0 && strcmp (apellido, c.apellido) == 0) {
        Contacto actualizado = leerContacto();
        temporal.write( (char*) &actualizado, sizeof(Contacto) );
      }
      else{
        temporal.write( (char*) &c, sizeof(Contacto) );
      }
    }
  }
  entrada.close();
  temporal.close();

  remove(DIR_ARCHIVO);
  rename("temp.dat",DIR_ARCHIVO);
}

int calcularEdad( int anioAct , int mesAct, int fechAct, int ano, int mes, int dia){
   int respFech=0, respMes=0, edadFinal=0;
 
    if ( fechAct < dia  )
    {   //En caso de ser menor la fecha actual que el nacimiento
        fechAct = fechAct + 30; // Se le suma los 30 días (1 mes) a la fecha actual
        mesAct = mesAct - 1; // Se le resta un mes (30 días) al mes actual
        respFech =  fechAct - dia; //Se le resta fecha nacimiento al actual
    }
    else //En caso de ser mayor la fecha actual que el nacimiento
        respFech =  fechAct - dia;  //Se le resta fecha nacimiento al actual
 
    if( mesAct < mes )
    {   //En caso de ser menor el mes actual que el nacimiento
        mesAct = mesAct + 12; // Se le suma los 12 meses (1 año) al mes actual
        anioAct = anioAct - 1 ; // Se le resta 1 año ( 12 meses) al año actual
        respMes = mesAct - mes; //Se le resta año nacimiento al actual
    }
    else //En caso de ser mayor el mes actual que el nacimiento
        respMes = mesAct - mes; //Se le resta año nacimiento al actual
 
    
    edadFinal = (anioAct - ano);
  return edadFinal;
}


/**********************************************************/

/*MENU*/
void menuIngresar(){
  cout << "\t\t\t\t---------------------------" << '\n';
  cout << "\t\t\t\t INGRESO DE NUEVO CONTACTO" << '\n';
  cout << "\t\t\t\t---------------------------" << '\n';
  Contacto nuevo = leerContacto();
  ingresarContacto(nuevo);
  cout << "\t\t\tContacto agregado con exito" << '\n';
  getch();
}

void menuBuscar(){
  Contacto buscado;
  cout << "\t\t\t\t-----------------" << '\n';
  cout << "\t\t\t\t BUSCAR CONTACTO" << '\n';
  cout << "\t\t\t\t-----------------" << '\n';
  char nombre[30],apellido[45];
  cout << "\t\t\tIngrese los datos de busqueda:" << '\n';
  cout << "\t\t\tNOMBRE  :";
  cin.getline(nombre,30);
  cout << "\t\t\tAPELLIDO:";
  cin.getline(apellido,45);
  if (existeContacto(nombre,apellido)) {
    Contacto buscado = buscarContacto(nombre,apellido);
    mostrarContacto(buscado);
    getch();
  }
  else{
    cout << "\t\t\tNO se encuentra ese contacto";
    getch();
  }
}

void menuListado(){
  int opcion;
    bool repetir = true;
    
    do {
        system("cls");
        
        // Texto del menú que se verá cada vez
        cout << "\n\nMenu de Opciones" << endl;
        cout << "1. Listar por registro" << endl;
        cout << "2. Listar por edad" << endl;
        cout << "3. Listar por apellido" << endl;
        cout << "4. Listar por mes de cumplenaños" << endl;
        cout << "0. SALIR" << endl;
        
        cout << "\nIngrese una opcion: ";
        cin >> opcion;
        
        switch (opcion) {
            case 1:
                // Lista de instrucciones de la opción 1                
                listarContactos();
                system("pause");
                break;
                
            case 2:
                // Lista de instrucciones de la opción 2                
                listarPorEdad();
                system("pause");
                break;
                
            case 3:
                // Lista de instrucciones de la opción 3 
                listarPorApellido(); 
                system("pause");                         
                break;
                
            case 4:
                // Lista de instrucciones de la opción 4                
                listarPorMes();
                system("pause");                
                break;
            
            case 0:
            	repetir = false;
            	break;
        }        
    } while (repetir);
}

void menuEliminar(){
  cout << "\t\t\t\t-------------------" << '\n';
  cout << "\t\t\t\t ELIMINAR CONTACTO" << '\n';
  cout << "\t\t\t\t-------------------" << '\n';
  char nombre[30],apellido[45];
  cout << "\t\t\tIngrese los datos de eliminacion:" << '\n';
  cout << "\t\t\tNOMBRE  :";
  cin.getline(nombre,30);
  cout << "\t\t\tAPELLIDO:";
  cin.getline(apellido,45);
  if (existeContacto(nombre,apellido)) {

    Contacto eliminar = buscarContacto(nombre,apellido);
    int opcion;
    cout << "\t\t\t-Seguro que desea eliminar el siquiente contacto?";
    mostrarContacto(eliminar);

    do {
      cout << "\t\t\tSI[1].....NO[2]: ";
      cin >> opcion;
      switch (opcion) {
        case 1: eliminarContacto(nombre,apellido);
                cout << "\t\t\tContacto ELIMINADO con exito" << '\n';
                getch();
                break;

        case 2: cout << "\t\t\tSe ha CANCELADO la eliminacion" << '\n';
                getch();
                break;

        default: cout << "\t\t\tOPCION INCORRECTA" << '\n';
      }
    } while(opcion != 1 && opcion != 2);
  }
  else{
    cout << "\t\t\tNO EXISTE EL CONTACTO" << '\n';
    getch();
  }
}

void menuActualizar(){
  cout << "\t\t\t\t--------------------" << '\n';
  cout << "\t\t\t\t ACTUALIZAR CONTACTO" << '\n';
  cout << "\t\t\t\t--------------------" << '\n';
  char nombre[30],apellido[45];
  cout << "\t\t\tIngrese los datos de modificacion:" << '\n';
  cout << "\t\t\tNOMBRE  :";
  cin.getline(nombre,30);
  cout << "\t\t\tAPELLIDO:";
  cin.getline(apellido,45);
  cout <<'\n';

  if (existeContacto(nombre,apellido)) {
    Contacto buscado = buscarContacto(nombre,apellido);
    cout << "\t\t\tRegistro a modificar:";
    mostrarContacto(buscado);
    acctualizarContacto(nombre,apellido);
    cout << "\t\t\tSE ACTUALIZO EL CONTACTO" << '\n';
    getch();
  }
  else{
    cout << "\t\t\tNO EXISTE EL CONTACTO" << '\n';
    getch();
  }
}

/*Estetica*/
void hastaPronto(){
  cout << "\t\t\t      HHH   HHH   AAAAAAAAA    SSSSSSS    TTTTTTTTTTT   AAAAAAAAA" << '\n';
  cout << "\t\t\t      HHH   HHH   AAA   AAA   SSS             TTT       AAA   AAA" << '\n';
  cout << "\t\t\t      HHHHHHHHH   AAAAAAAAA     SSS           TTT       AAAAAAAAA" << '\n';
  cout << "\t\t\t      HHH   AAA   AAA   AAA      SSS          TTT       AAA   AAA" << '\n';
  cout << "\t\t\t      HHH   HHH   AAA   AAA   SSSSSSS         TTT       AAA   AAA"  << '\n';
  cout << '\n';
  cout << "\t\t\tPPPPPPPPP   RRRRRRRRR   OOOOOOOOO   NNN     NNN   TTTTTTTTTTT   OOOOOOOOO" << '\n';
  cout << "\t\t\tPPP   PPP   RRR   RR    OOO   OOO   NNNNN   NNN       TTT       OOO   OOO" << '\n';
  cout << "\t\t\tPPPPPPPPP   RRRRRRR     OOO   OOO   NNN NN  NNN       TTT       OOO   OOO" << '\n';
  cout << "\t\t\tPPP         RRR  RR     OOO   OOO   NNN  NN NNN       TTT       OOO   OOO" << '\n';
  cout << "\t\t\tPPP         RRR   RRR   OOOOOOOOO   NNN   NNNNN       TTT       OOOOOOOOO"  << '\n';
}
/******************/

