/*
 Proyecto: Pac-Man con caracteres
 Equipo 2  Horario: 8:00-9:00hrs.
 Integrantes del equipo:
    --Diaz Cruz Esteban David
    --Echeverria Valencia Greta ALitzel
    --Gomez Almazan Martín
    --Serafín Velazquez Andrea Lizeth
    --Torres Gallardo Angel Gabriel
    --Vargas Hernandez Luis Abraham
*/

#include <iostream>
#include <ctime>
#include <cstdlib>

#ifdef _WIN32
#include <conio.h> // Para la función _getch() en sistemas Windows
#else
#include <termios.h> // Para la función getch() en sistemas Linux
#include <unistd.h>
#endif

using namespace std;

int puntaje = 0; //el puntaje
const int filas = 5; //filas del tablero
const int columnas = 5; //columnas del tablero
char tablero[filas][columnas]; //Tablero
int posX_Pacman, posY_Pacman; //Posicion de pacman
int posX_Pastilla, posY_Pastilla; //Posicion de la pastilla
const int cantidad_fantasmas = 2;
char prevfantasma[cantidad_fantasmas]; //es para ver si el fantasma esta sobre un punto o en un espacio en blanco
int posX_Fantasma[cantidad_fantasmas], posY_Fantasma[cantidad_fantasmas];//Posicion de fantasma
bool s_pastilla = false; //Está bandera la usaremos para saber si pacman se comió la pastilla
int cont_pastilla = 0;
bool fantasma_exist[cantidad_fantasmas];
/*
P = Pacman
. = bolitas para comer
F = Fantasma
# = Pared
o = Superpastilla
*/


#ifdef _WIN32
void inicializarConsola() {
    // No se necesita inicializar nada en Windows
}
void detenerConsola() {
    // No se necesita detener nada en Windows
}

//Para moverse con la pura letra sin tener que estar apretando enter a cada rato
char obtenerEntrada() {
    // Obtener entrada de teclado en sistemas Windows
    return _getch();
}
#else
void inicializarConsola() {
    // Configurar la terminal en sistemas Linux (Linux, macOS)
    struct termios t; // Estructura para configuración de la terminal
    tcgetattr(STDIN_FILENO, &t); // Obtener la configuración actual de la terminal
    t.c_lflag &= ~ICANON; // Deshabilitar el modo canónico (entrada de línea)
    tcsetattr(STDIN_FILENO, TCSANOW, &t); // Aplicar la configuración modificada a la terminal
    //Profe esto lo vimos en un tutorial de youtube porque nos salían errores, le entendimos más o menos poco
}

void detenerConsola() {
    // Restaurar la configuración de la terminal en sistemas Linux (Linux, macOS)
    struct termios t; // Estructura para configuración de la terminal
    tcgetattr(STDIN_FILENO, &t); // Obtener la configuración actual de la terminal
    t.c_lflag |= ICANON; // Habilitar el modo canónico (entrada de línea)
    tcsetattr(STDIN_FILENO, TCSANOW, &t); // Aplicar la configuración modificada a la terminal
    //Profe esto lo vimos en un tutorial de youtube porque nos salían errores, le entendimos más o menos poco
}

//Para moverse con la pura letra sin tener que estar apretando enter a cada rato
char obtenerEntrada() {
    // Obtener entrada de teclado en sistemas Linux (Linux, macOS)
    char buf = 0; // Variable para almacenar el carácter de entrada
    struct termios old = {0}; // Estructura para la configuración original de la terminal
    fflush(stdout); // Vaciar el búfer de salida
    // Obtener y modificar la configuración actual de la terminal
    if (tcgetattr(0, &old) < 0) {
        perror("tcsetattr()");
    }
    // Deshabilitar el modo canónico y el eco de caracteres
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    // Configurar la lectura de un carácter sin esperar por "Enter"
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    // Aplicar la configuración modificada a la terminal
    if (tcsetattr(0, TCSANOW, &old) < 0) {
        perror("tcsetattr ICANON");
    }
    // Leer un carácter desde la entrada estándar
    if (read(0, &buf, 1) < 0) {
        perror("read()");
    }
    // Restaurar la configuración original de la terminal
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0) {
        perror("tcsetattr ~ICANON");
    }
    return buf; // Devolver el carácter de entrada
}

#endif

void inicializarTablero() {
    // Inicializar el tablero y las posiciones de Pacman y el Fantasma

    // Llenar el tablero con puntos (representados por '.')
    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            tablero[i][j] = '.';
        }
    }

    // Colocar a Pacman en el centro del tablero
    posX_Pacman = filas / 2;
    posY_Pacman = columnas / 2;
    tablero[posX_Pacman][posY_Pacman] = 'P';

    // Generar posiciones aleatorias para el Fantasma
    srand(time(nullptr)); //semilla para numeros aleatorios
    for(int i = 0; i <= cantidad_fantasmas-1; i++){
        posX_Fantasma[i] = rand() % filas + i;
        posY_Fantasma[i] = rand() % columnas + i;
        tablero[posX_Fantasma[i]][posY_Fantasma[i]] = 'F';
    }
    // Generar posiciones aleatorias para la pastilla
    srand(time(nullptr) + 5 ); //semilla para numeros aleatorios
    posX_Pastilla = rand() % filas;
    posY_Pastilla = rand() % columnas;
    tablero[posX_Pastilla][posY_Pastilla] = 'o';
}

void imprimirTablero() {
    // Limpiar la pantalla y mostrar el tablero con colores
    system("cls || clear"); // Limpiar la pantalla en Windows o sistemas Linux
    //Imprimimos el tablero según lo que contenga el elemento de la matriz
    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            if (tablero[i][j] == 'P') {
                cout << "\x1B[33m" << "P" << "\x1B[0m "; // Amarillo para Pacman
            } else if (tablero[i][j] == 'F') {
                if(!s_pastilla){
                    cout << "\x1B[31m" << "F" << "\x1B[0m "; // Rojo para el fantasma
                }else{
                    cout << "\x1B[34m" << "F" << "\x1B[0m ";// Rojo para el fantasma que se pueden comer
                }
            } else {
                cout << "\x1B[37m" << tablero[i][j] << "\x1B[0m "; // Blanco para el resto
            }
        }
        cout << endl;
    }
}

void moverPacman(char direccion) {
    // Mover a Pacman y actualizar el puntaje
    int prevx = posX_Pacman, prevy = posY_Pacman;
    tablero[posX_Pacman][posY_Pacman] = ' ';
    // La posicion la sacamos de esta manera para que en caso de que este en el borde y quiera avanzar más se teletransporte al otro lado
    if (direccion == 'w') {
        posX_Pacman = (posX_Pacman - 1 + filas) % filas;
    } else if (direccion == 's') {
        posX_Pacman = (posX_Pacman + 1) % filas;
    } else if (direccion == 'a') {
        posY_Pacman = (posY_Pacman - 1 + columnas) % columnas;
    } else if (direccion == 'd') {
        posY_Pacman = (posY_Pacman + 1) % columnas;
    }
    //Cuando pacman se come un punto, su puntaje aumenta
    if (tablero[posX_Pacman][posY_Pacman] == '.'){
        puntaje++;
    }else if(tablero[posX_Pacman][posY_Pacman] == 'o'){ //Cuando pacman se topa con una pastilla ahora puede comer a los fantasmas
        s_pastilla = true;
        cont_pastilla = 10;
    }else if(tablero[posX_Pacman][posY_Pacman] == '#'){//Cuando pacman topa con una pared no puede avanzar
        posX_Pacman = prevx;
        posY_Pacman = prevy;
    }
    //Ponemos a pacman en la matriz tablero
    tablero[posX_Pacman][posY_Pacman] = 'P';
}

void moverFantasma() {
    // Mover al Fantasma aleatoriamente cantidad_fantasmas
    for(int i = 0; i<=cantidad_fantasmas-1;i++){
        if(fantasma_exist[i]){
            tablero[posX_Fantasma[i]][posY_Fantasma[i]] = prevfantasma[i];
            int direccion = rand() % 4; //sacamos un numero aleatorio para que sea la direccion
            // La posicion la sacamos de esta manera para que en caso de que este en el borde y quiera avanzar más se teletransporte al otro lado
            if (direccion == 0) {
                posX_Fantasma[i] = (posX_Fantasma[i] - 1 + filas) % filas;
            } else if (direccion == 1) {
                posX_Fantasma[i] = (posX_Fantasma[i] + 1) % filas;
            } else if (direccion == 2) {
                posY_Fantasma[i] = (posY_Fantasma[i] - 1 + columnas) % columnas;
            } else if (direccion == 3) {
                posY_Fantasma[i] = (posY_Fantasma[i] + 1) % columnas;
            }

            prevfantasma[i] = tablero[posX_Fantasma[i]][posY_Fantasma[i]];//Para que si había un punto aunque el fantasma pase por encima se mantenga
            tablero[posX_Fantasma[i]][posY_Fantasma[i]] = 'F';//Ponemos al fantasma en el tablero
        }else{
            posX_Fantasma[i] = -1;   
            posY_Fantasma[i] = -1;
        }
    }
}

void juegoPerdido(){
    system("cls || clear");
    cout<<"  ____                         ___                 _ "<<endl;
    cout<<" / ___| __ _ _ __ ___   ___   / _ \\__   _____ _ __| |"<<endl;
    cout<<"| |  _ / _` | '_ ` _ \\ / _ \\ | | | \\ \\ / / _ \\ '__| |"<<endl;
    cout<<"| |_| | (_| | | | | | |  __/ | |_| |\\ V /  __/ |  |_|"<<endl;
    cout<<" \\____|\\__,_|_| |_| |_|\\___|  \\___/  \\_/ \\___|_|  (_)"<<endl;
    cout << "Has sido atrapado por el fantasma. Juego terminado." << endl;
    cout << "Tu puntaje es: " << puntaje << endl;
}

bool pacmanColisionaFantasma (){
    for(int i=0; i<=cantidad_fantasmas-1; i++){
        if (posX_Pacman == posX_Fantasma[i] && posY_Pacman == posY_Fantasma[i]) {
            if(!s_pastilla){
                // Juego perdido
                juegoPerdido();
                return true;
                break;
            }else if(s_pastilla && fantasma_exist[i]){
                fantasma_exist[i] = false;
                if(prevfantasma[i] == '.'){
                    puntaje++;
                }
            }
        }
    }
    return false;
}

int main() {
    for (int i=0;i<=cantidad_fantasmas-1;i++){
        prevfantasma[i] = '.';
        fantasma_exist[i] = true;
    }
    
    // Inicialización del juego
    system("cls || clear");
    inicializarConsola();
    try{
        cout<<" _ __   __ _  ___ _ __ ___   __ _ _ __  "<<endl;
        cout<<"| '_ \\ / _` |/ __| '_ ` _ \\ / _` | '_ \\ "<<endl;
        cout<<"| |_) | (_| | (__| | | | | | (_| | | | |"<<endl;
        cout<<"| .__/ \\__,_|\\___|_| |_| |_|\\__,_|_| |_|"<<endl;
        cout<<"| |                                     "<<endl;
        cout<<"|_|               "<<endl;
        cout<<"Presione cualquier tecla para iniciar..."<<endl;
        char direccion = obtenerEntrada();
        direccion = ' ';
    } catch (const std::exception &e) {
        std::cerr << "Se produjo una excepción: " << e.what() << std::endl;
    }
    inicializarTablero();
    imprimirTablero();
    
    // Bucle principal del juego
    while (true) {
        if (cont_pastilla >0){
            cont_pastilla--;
        }else{
            s_pastilla = false;
        }
        if(puntaje >= (filas*columnas-cantidad_fantasmas - 1) ) {
            // Juego ganado
            system("cls || clear");
            cout<<"__   __                                _ "<<endl;
            cout<<"\\ \\ / /__  _   _  __      _____  _ __ | |"<<endl;
            cout<<" \\ V / _ \\| | | | \\ \\ /\\ / / _ \\| '_ \\| |"<<endl;
            cout<<"  | | (_) | |_| |  \\ V  V / (_) | | | |_|"<<endl;
            cout<<"  |_|\\___/ \\__,_|   \\_/\\_/ \\___/|_| |_(_)"<<endl;
            break;
        }
        char direccion = obtenerEntrada();
        if (direccion == 'w' || direccion == 'a' || direccion == 's' || direccion == 'd' ||direccion == 'W' || direccion == 'A' || direccion == 'S' || direccion == 'D') {
            moverPacman(direccion);
            imprimirTablero();
            cout << "Tu puntaje es: " << puntaje << endl;
            if (pacmanColisionaFantasma()) {
                // Juego perdido
                break;
            }
        }
        moverFantasma();
        imprimirTablero();
        cout << "Tu puntaje es: " << puntaje << endl;
        //cout << "Fantasma: " << fantasma_exist[0] << endl;

        //El if indica si el fantasma y pacman estan en la misma posicion
         if (pacmanColisionaFantasma()) {
                // Juego perdido
                break;
            }
    }

    detenerConsola();
    char direccion = obtenerEntrada();
    direccion = ' ';
    return 0;
}
