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

char prevfantasma = '.'; //es para ver si el fantasma esta sobre un punto o en un espacio en blanco
int puntaje = 0; //el puntaje
const int filas = 15; //filas del tablero
const int columnas = 15; //columnas del tablero
char tablero[filas][columnas]; //Tablero
int posX_Pacman, posY_Pacman; //Posicion de pacman
int posX_Fantasma, posY_Fantasma;//Posicion de fantasma

#ifdef _WIN32
void inicializarConsola() {
    // No se necesita inicializar nada en Windows
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

#endif

#ifdef _WIN32
void detenerConsola() {
    // No se necesita detener nada en Windows
}
#else
void detenerConsola() {
    // Restaurar la configuración de la terminal en sistemas Linux (Linux, macOS)
    struct termios t; // Estructura para configuración de la terminal
    tcgetattr(STDIN_FILENO, &t); // Obtener la configuración actual de la terminal
    t.c_lflag |= ICANON; // Habilitar el modo canónico (entrada de línea)
    tcsetattr(STDIN_FILENO, TCSANOW, &t); // Aplicar la configuración modificada a la terminal
    //Profe esto lo vimos en un tutorial de youtube porque nos salían errores, le entendimos más o menos poco
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
    posX_Fantasma = rand() % filas;
    posY_Fantasma = rand() % columnas;
    tablero[posX_Fantasma][posY_Fantasma] = 'F';
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
                cout << "\x1B[31m" << "F" << "\x1B[0m "; // Rojo para el fantasma
            } else {
                cout << "\x1B[37m" << tablero[i][j] << "\x1B[0m "; // Blanco para el resto
            }
        }
        cout << endl;
    }
}

void moverPacman(char direccion) {
    // Mover a Pacman y actualizar el puntaje
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
    }
    //Ponemos a pacman en la matriz tablero
    tablero[posX_Pacman][posY_Pacman] = 'P';
}

void moverFantasma() {
    // Mover al Fantasma aleatoriamente
    tablero[posX_Fantasma][posY_Fantasma] = prevfantasma;
    int direccion = rand() % 4; //sacamos un numero aleatorio para que sea la direccion
    // La posicion la sacamos de esta manera para que en caso de que este en el borde y quiera avanzar más se teletransporte al otro lado
    if (direccion == 0) {
        posX_Fantasma = (posX_Fantasma - 1 + filas) % filas;
    } else if (direccion == 1) {
        posX_Fantasma = (posX_Fantasma + 1) % filas;
    } else if (direccion == 2) {
        posY_Fantasma = (posY_Fantasma - 1 + columnas) % columnas;
    } else if (direccion == 3) {
        posY_Fantasma = (posY_Fantasma + 1) % columnas;
    }

    prevfantasma = tablero[posX_Fantasma][posY_Fantasma];//Para que si había un punto aunque el fantasma pase por encima se mantenga
    tablero[posX_Fantasma][posY_Fantasma] = 'F';//Ponemos al fantasma en el tablero
}

#ifdef _WIN32
//Para moverse con la pura letra sin tener que estar apretando enter a cada rato
char obtenerEntrada() {
    // Obtener entrada de teclado en sistemas Windows
    return _getch();
}
#else
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
int main() {
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
        if(puntaje >= (filas*columnas-1) ) {
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
            if (posX_Pacman == posX_Fantasma && posY_Pacman == posY_Fantasma) {
                // Juego perdido
                juegoPerdido();
                break;
            }
        }
        moverFantasma();
        imprimirTablero();
        cout << "Tu puntaje es: " << puntaje << endl;
        //El if indica si el fantasma y pacman estan en la misma posicion
        if (posX_Pacman == posX_Fantasma && posY_Pacman == posY_Fantasma) {
            // Juego perdido
            juegoPerdido();
            break;
        }
    }

    detenerConsola();

    return 0;
}
