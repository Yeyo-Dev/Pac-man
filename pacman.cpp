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
#include <conio.h> // Para la función getch() en sistemas Windows
#endif

using namespace std;

int puntaje = 0; //el puntaje
const int filas = 13; //filas del tablero
const int columnas = 31; //columnas del tablero
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
//Para moverse con la pura letra sin tener que estar apretando enter a cada rato
char obtenerEntrada() {
    // Obtener entrada de teclado en sistemas Windows
    return getch();
}
#else
char obtenerEntrada() {
    char ch;
    // Lee el carácter
    std::cin.get(ch);
    return ch;
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
    for(int i=0;i<8;i++){//Hacer varias pastillas
        // Generar posiciones aleatorias para la pastilla
        srand(time(nullptr) + 5 + i ); //semilla para numeros aleatorios
        posX_Pastilla = rand() % filas;
        posY_Pastilla = rand() % columnas;
        tablero[posX_Pastilla][posY_Pastilla] = 'o';//Ponemos la pastilla
    }
    //Mapa
    //Vertical Derecho
    tablero[0][30]='#';
    tablero[1][30]='#';
    tablero[2][30]='#';
    tablero[3][30]='#';
    tablero[4][30]='#';
    tablero[5][30]='#'; 
    tablero[6][30]='.'; 
    tablero[7][30]='#'; 
    tablero[8][30]='#'; 
    tablero[9][30] = '#'; 
    tablero[10][30] = '#'; 
    tablero[11][30] = '#'; 
    tablero[12][30] = '#';
    //Vertical izquierdo
    tablero[0][0]='#'; 
    tablero[1][0]='#'; 
    tablero[2][0]='#'; 
    tablero[3][0]='#'; 
    tablero[4][0]='#'; 
    tablero[5][0]='#'; 
    tablero[6][0]='.'; 
    tablero[7][0]='#'; 
    tablero[8][0]='#'; 
    tablero[9][0] = '#'; 
    tablero[10][0] = '#'; 
    tablero[11][0] = '#'; 
    tablero[12][0] = '#';
    //Horizontal superior
    tablero[0][1]='#'; tablero[0][2]='#'; tablero[0][3]='#'; tablero[0][4]='#'; tablero[0][5]='#'; tablero[0][6]='#'; tablero[0][7]='#'; tablero[0][8]='#'; tablero[0][9]='#'; tablero[0][10]='#'; tablero[0][11]='#'; tablero[0][12]='#';
    tablero[0][13]='#'; tablero[0][14]='#'; tablero[0][15]='.';tablero[0][16]='#'; tablero[0][17]='#'; tablero[0][18]='#'; tablero[0][19]='#'; tablero[0][20]='#'; tablero[0][21]='#'; tablero[0][22]='#'; tablero[0][23]='#'; tablero[0][24]='#'; tablero[0][25]='#'; tablero[0][26]='#';tablero[0][27]='#';tablero[0][28]='#';tablero[0][29]='#';tablero[0][30]='#';
    //Horizontal inferior
    tablero[12][1]='#'; tablero[12][2]='#'; tablero[12][3]='#'; tablero[12][4]='#'; tablero[12][5]='#'; tablero[12][6]='#'; tablero[12][7]='#'; tablero[12][8]='#'; tablero[12][9]='#'; tablero[12][10]='#'; tablero[12][11]='#'; tablero[12][12]='#';
    tablero[12][13]='#'; tablero[12][14]='#'; tablero[12][15]='.';tablero[12][16]='#'; tablero[12][17]='#'; tablero[12][18]='#'; tablero[12][19]='#'; tablero[12][20]='#'; tablero[12][21]='#'; tablero[12][22]='#'; tablero[12][23]='#'; tablero[12][24]='#'; tablero[12][25]='#'; tablero[12][26]='#';tablero[12][27]='#';tablero[12][28]='#';tablero[12][29]='#';tablero[12][30]='#';
    //Letra E
    tablero[4][2]='#';tablero[5][2]='#';tablero[6][2]='#';tablero[7][2]='#';tablero[8][2]='#';tablero[4][3]='#';tablero[4][4]='#';tablero[6][3]='#';tablero[6][4]='#';tablero[8][3]='#';tablero[8][4]='#';
    
    //Letra Q
    tablero[4][6]='#';tablero[4][7]='#';tablero[4][8]='#';tablero[5][6]='#';/*tablero[6][6]='#'*/;tablero[7][6]='#';tablero[8][6]='#';tablero[5][8]='#';/*tablero[6][8]='#'*/;tablero[7][8]='#';tablero[8][8]='#';tablero[9][8]='#';tablero[10][8]='#';tablero[8][7]='#';//tablero[6][7]='*';tablero[7][7]='*';tablero[5][7]='*';
    
    //Letra u
    tablero[4][10]='#';tablero[5][10]='#';tablero[6][10]='#';tablero[7][10]='#';tablero[8][10]='#';tablero[8][11]='#';tablero[8][12]='#';tablero[7][12]='#';tablero[6][12]='#';tablero[5][12]='#';tablero[4][12]='#';

    //Letra i
    tablero[4][14]='#';tablero[5][14]='#';tablero[6][14]='#';tablero[7][14]='#';tablero[8][14]='#';
    
    //Letra P
    tablero[4][16]='#';/*tablero[5][16]='#'*/;tablero[6][16]='#';tablero[7][16]='#';tablero[8][16]='#';tablero[4][17]='#';tablero[6][17]='#';tablero[4][18]='#';tablero[5][18]='#';tablero[6][18]='#';
    
    //Letra O
    /*tablero[4][20]='#';*/tablero[5][20]='#';/*tablero[6][20]='#'*/;tablero[7][20]='#';/*tablero[8][20]='#';*/tablero[8][21]='#';tablero[4][21]='#';/*tablero[4][22]='#';*/tablero[5][22]='#';/*tablero[6][22]='#'*/;tablero[7][22]='#';/*tablero[8][22]='#';*/
    
    //Numero II
    tablero[4][24]='#';tablero[4][25]='#';/*tablero[4][26]='#'*/;tablero[4][27]='#';tablero[4][28]='#';tablero[5][25]='#';tablero[5][27]='#';tablero[6][25]='#';tablero[6][27]='#';tablero[7][25]='#';tablero[7][27]='#';tablero[8][24]='#';tablero[8][25]='#';/*tablero[8][26]='#'*/;tablero[8][27]='#';tablero[8][28]='#';
    //Paredes externas a la frase
    tablero[2][2]='#';tablero[2][3]='#';tablero[2][4]='#';tablero[2][6]='#';tablero[2][7]='#';tablero[2][8]='#';tablero[2][10]='#';tablero[2][11]='#';tablero[2][12]='#';tablero[2][14]='#';tablero[2][16]='#';tablero[2][17]='#';tablero[2][18]='#';tablero[2][20]='#';tablero[2][21]='#';tablero[2][22]='#';tablero[2][24]='#';tablero[2][25]='#';tablero[2][26]='#';tablero[2][27]='#';tablero[2][28]='#';
    tablero[10][2]='#';tablero[10][3]='#';tablero[10][4]='#';tablero[10][5]='#';tablero[10][6]='#';tablero[10][10]='#';tablero[10][11]='#';tablero[10][12]='#';tablero[10][14]='#';tablero[10][16]='#';tablero[10][17]='#';tablero[10][18]='#';tablero[10][20]='#';tablero[10][21]='#';tablero[10][22]='#';tablero[10][24]='#';tablero[10][25]='#';tablero[10][26]='#';tablero[10][27]='#';tablero[10][28]='#';
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
            } else if (tablero[i][j] == '#') {
                cout << "\x1B[32m" << "#" << "\x1B[0m ";//Verde para las paredes
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
    if(direccion == 'r'){//reiniciamos el juego
        for (int i=0;i<=cantidad_fantasmas-1;i++){
        //Llenamos los arreglos con sus valores por defecto
            prevfantasma[i] = '.';
            fantasma_exist[i] = true;
        }
        inicializarTablero();
        cont_pastilla == 0;
        s_pastilla = true;
    }else if(direccion == 'i'){//Modo invencible, no tiene gran funcionalidad sólo por diversión
        s_pastilla = true;
        cont_pastilla = 100;
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
            char prev_fantasma2 = prevfantasma[i];
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
            //Soluciona el bug de que se multiplique la imagen de los fantasmas
            if(prevfantasma[i] =='F'){
                prevfantasma[i] = prev_fantasma2;
            }
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
    try{
        cout<<" _ __   __ _  ___ _ __ ___   __ _ _ __  "<<endl;
        cout<<"| '_ \\ / _` |/ __| '_ ` _ \\ / _` | '_ \\ "<<endl;
        cout<<"| |_) | (_| | (__| | | | | | (_| | | | |"<<endl;
        cout<<"| .__/ \\__,_|\\___|_| |_| |_|\\__,_|_| |_|"<<endl;
        cout<<"| |                                     "<<endl;
        cout<<"|_|               "<<endl;
        cout<<"Para jugar usa las teclas WASD"<<endl;
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
        if(puntaje >= (filas*columnas - 190) ) { //Aqui sacamos la cantidad de puntaje que necesita según los  puntos, que es los espacios de la matriz menos pacman menos las paredes
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
        if (direccion == 'w' || direccion == 'a' || direccion == 's' || direccion == 'd' ||direccion == 'W' || direccion == 'A' || direccion == 'S' || direccion == 'D' || direccion == 'r' || direccion == 'i') {
            moverPacman(direccion);//Movemos a pacman según la dirección
            imprimirTablero();//Imprimimos el tablero otra vez y ponemos el puntaje que lleva
            cout << "Tu puntaje es: " << puntaje << endl;
            if (pacmanColisionaFantasma()) {//Revisamos si hay colision si es verdadero significa que se comieron a pacman
                // Juego perdido
                break;//Salimos del while
            }
        }
        moverFantasma();//Movemos al fantasma
        imprimirTablero();//Volvemos a imprimir el tablero
        cout << "Tu puntaje es: " << puntaje << endl;
        if(cont_pastilla > 0) {//Si tenemos el efecto de la super pastilla mostramos los turnos que nos quedan
            cout << "Tienes " <<cont_pastilla << " turnos antes de que se termine el efecto" <<endl;
        }

        //El if indica si el fantasma y pacman estan en la misma posicion
         if (pacmanColisionaFantasma()) {//Si es verdadero significa que se comieron a pacman
                // Juego perdido
                break;//Salimos del while infinito
            }
    }
    char direccion = obtenerEntrada();//Volvemos a que presione una tecla para terminar el programa
    direccion = ' '; //Limpiamos la variable es que me da toc por el debuger
    return 0;
}
