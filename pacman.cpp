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
const int filas = 10; //filas del tablero
const int columnas = 10; //columnas del tablero
char tablero[filas][columnas]; //Tablero
int posX_Pacman, posY_Pacman; //Posicion de pacman
int posX_Pastilla, posY_Pastilla; //Posicion de la pastilla
const int cantidad_fantasmas = 2; //La cantidad de fantasmas
char prevfantasma[cantidad_fantasmas]; //es para ver si el fantasma esta sobre un punto o en un espacio en blanco
int posX_Fantasma[cantidad_fantasmas], posY_Fantasma[cantidad_fantasmas];//Posicion de fantasma
bool s_pastilla = false; //Está bandera la usaremos para saber si pacman se comió la pastilla
int cont_pastilla = 0; //El contador de turnos que dura el efecto de la pastilla
bool fantasma_exist[cantidad_fantasmas]; // Para ver si el fantasma existe o ya fue comido
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
    for(int i=0;i<4;i++){//Hacer 4 pastillas
        // Generar posiciones aleatorias para la pastilla
        srand(time(nullptr) + 5 + i ); //semilla para numeros aleatorios
        posX_Pastilla = rand() % filas;
        posY_Pastilla = rand() % columnas;
        tablero[posX_Pastilla][posY_Pastilla] = 'o';//Ponemos la pastilla
    }
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
                if(!s_pastilla){//Indica si la pastilla no ha sido comida
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
    int prevx = posX_Pacman, prevy = posY_Pacman; //Declaramos dos variables prev como auxiliares de dónde estaba pacman
    tablero[posX_Pacman][posY_Pacman] = ' '; //La posición en la que se encontraba pacman ahora estará vacia porque ya se comió el punto
    // La posicion la sacamos de esta manera para que en caso de que este en el borde y quiera avanzar más se teletransporte al otro lado
    if (direccion == 'w' || direccion == 'W') {//Arriba
        posX_Pacman = (posX_Pacman - 1 + filas) % filas;
    } else if (direccion == 's'|| direccion == 'S') {//Abajo
        posX_Pacman = (posX_Pacman + 1) % filas;
    } else if (direccion == 'a'|| direccion == 'A') {//Izquierda
        posY_Pacman = (posY_Pacman - 1 + columnas) % columnas;
    } else if (direccion == 'd'|| direccion == 'D') {//Derecha
        posY_Pacman = (posY_Pacman + 1) % columnas;
    }
    //Cuando pacman se come un punto, su puntaje aumenta
    if (tablero[posX_Pacman][posY_Pacman] == '.'){
        puntaje++;
    }else if(tablero[posX_Pacman][posY_Pacman] == 'o'){ //Cuando pacman se topa con una pastilla ahora puede comer a los fantasmas
        s_pastilla = true;//activamos la bandera para poder comer a los fantasmas
        cont_pastilla = 10;//Le damos 10 turnos para que se acabe el efecto
        puntaje++;//Se le da puntaje
    }else if(tablero[posX_Pacman][posY_Pacman] == '#'){//Cuando pacman topa con una pared no puede avanzar
        //En caso de que haya una pared regresa a la posición en la que estaba porque no la puede pasar
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
            int prevx = posX_Fantasma[i], prevy = posY_Fantasma[i]; //Declaramos dos variables prev como auxiliares de dónde estaba
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

            if(tablero[posX_Fantasma[i]][posY_Fantasma[i]] == '#'){//Cuando se topa con una pared no puede avanzar
                //En caso de que haya una pared regresa a la posición en la que estaba porque no la puede pasar
                posX_Fantasma[i] = prevx;
                posY_Fantasma[i] = prevy;
            }
            prevfantasma[i] = tablero[posX_Fantasma[i]][posY_Fantasma[i]];//Para que si había un punto aunque el fantasma pase por encima se mantenga
            tablero[posX_Fantasma[i]][posY_Fantasma[i]] = 'F';//Ponemos al fantasma en el tablero
        }else{
            //Si no existe lo sacamos del arreglo
            posX_Fantasma[i] = -1;   
            posY_Fantasma[i] = -1;
        }
    }
}

void juegoPerdido(){
    //Imprimimos un banner de que perdió el jugador y le decimos su puntaje
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
    //Iteramos para todos los fantasmas que existen
    for(int i=0; i<=cantidad_fantasmas-1; i++){
        //Si están en la misma posición
        if (posX_Pacman == posX_Fantasma[i] && posY_Pacman == posY_Fantasma[i]) {
            if(!s_pastilla){//Si no tiene el efecto de la pastilla
                // Juego perdido
                juegoPerdido();
                return true;
                break;
            }else if(s_pastilla && fantasma_exist[i]){
                //Si pacman tiene el efecto de la pastilla y el fantasma existe entonces deja de existir
                fantasma_exist[i] = false;
                if(prevfantasma[i] == '.'){
                    puntaje++;//Si había un punto dónde se comió al fantasma se come el punto también
                }
            }
        }
    }
    return false;
}

int main() {
    for (int i=0;i<=cantidad_fantasmas-1;i++){
        //Llenamos los arreglos con sus valores por defecto
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
        //Para no usar más storage usamos la variable char que ya teníamos y sólamente la limpiamos después de usarla
        //Esto para no tener que presionar enter
        char direccion = obtenerEntrada();
        direccion = ' ';
    } catch (const std::exception &e) {//En ciertos casos que no reconoce los caracteres del banner retorna error
        std::cerr << "Se produjo una excepción: " << e.what() << std::endl;
    }
    inicializarTablero();
    imprimirTablero();
    
    // Bucle principal del juego (infinito)
    while (true) {
        //Si tenemos el efecto de la pastilla le vamos quitando un turno
        if (cont_pastilla >0){
            cont_pastilla--;
        }else{
            s_pastilla = false;
        }
        if(puntaje >= (filas*columnas - 1) ) { //Aqui sacamos la cantidad de puntaje que necesita según los  puntos, que es los espacios de la matriz menos pacman menos las paredes
            // Juego ganado
            system("cls || clear");
            cout<<"__   __                                _ "<<endl;
            cout<<"\\ \\ / /__  _   _  __      _____  _ __ | |"<<endl;
            cout<<" \\ V / _ \\| | | | \\ \\ /\\ / / _ \\| '_ \\| |"<<endl;
            cout<<"  | | (_) | |_| |  \\ V  V / (_) | | | |_|"<<endl;
            cout<<"  |_|\\___/ \\__,_|   \\_/\\_/ \\___/|_| |_(_)"<<endl;
            break;
        }
        char direccion = obtenerEntrada();//Obtenemos la tecla presionada
        if (direccion == 'w' || direccion == 'a' || direccion == 's' || direccion == 'd' ||direccion == 'W' || direccion == 'A' || direccion == 'S' || direccion == 'D') {
            moverPacman(direccion);//Movemos a pacman según la dirección
            imprimirTablero();//Imprimimos el tablero otra vez y ponemos el puntaje que lleva
            cout << "Tu puntaje es: " << puntaje << endl;
            if (pacmanColisionaFantasma()) {//Revisamos si hay colision
                // Juego perdido
                break;
            }
        }
        moverFantasma();//Movemos al fantasma
        imprimirTablero();//Volvemos a imprimir el tablero
        cout << "Tu puntaje es: " << puntaje << endl;

        //El if indica si el fantasma y pacman estan en la misma posicion
         if (pacmanColisionaFantasma()) {
                // Juego perdido
                break;
            }
    }

    detenerConsola();//Detenemos consola
    char direccion = obtenerEntrada();//Volvemos a que presione una tecla para terminar el programa
    direccion = ' ';
    return 0;
}
