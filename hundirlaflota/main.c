#include <stdio.h>
#include <stdlib.h> //rand()
#include <string.h>
#include <time.h>
#include <stdbool.h> //true/false

//KeyboardKontroll
#include <unistd.h>
#include <termios.h>

//COLORES

void black(char a[]);
void red(char a[]);
void green(char a[]);
void yellow(char a[]);
void blue(char a[]);
void purple(char a[]);
void cian(char a[]);
void white(char a[]);
void reset();


#define N 7 //tamano del tablero
#define PUNTUACIONMAXIMA 16

void crearTableroVacio(char tablero[N][N]); //crear un tablero de N*N con . representando agua
void iaBarcosAleatorios(char tablero[N][N]); //dado un tablero genera los barcos de manera aleatoria
bool mirarPosicionDeBarco(char tablero[N][N],int x,int y, int direccion, int tamaino); //dada la coordenada orientacion y el tamano del barco devuelve verdadero si el barco se puede colocar en esas coordenadas
void colocarBarco(char tablero[N][N],int x,int y, int direccion, int tamaino); //dada la coordenada orientacion y el tamano del barco, coloca el barco en el tablero asignado
void iaMovimiento(int ordendisparos[N*N][2]); //crear una matriz de [N*N] [2] que representa todos los disparos que va a hacer la ia a lo largo de toda la partida
void jugadorColocarBarco(char tablero[N][N]); // colocar los barcos del jugador
bool mirarDisparo(char tablero[N][N],int x,int y); //dando las coordeanadas de dispara devuelve verdadero si ha impactado en un barco
void imprimirJuego(char tablero1[N][N],char tablero2[N][N],int puntosJugador, int puntosIA); //imprime los dos tableros del juego y la puntuacion de cada uno
void imprimirTablero(char tablero[N][N], bool oculto); //imprimir un tablero por pantalla,  se puede hacer de manera oculta no imprimiendo los barcos

void limpiarPantalla();

//puntuaciones
char modoAperturaFicheroLeerFichero[2] = "r+";
char nombreFicheroPuntuaciones[] = "puntuaciones.txt";
void tratarOpcionPuntuaciones();
void insertarPuntuacion(char puntuacion[]);

//FUNCIONES

char getch(void);    
void clearScreen();   
//void delay(int timer);
void createEmptyBoard(char [N][N]);
void printBoard(char [N][N]);
void startGame(char [N][N]);
void printLogo(int i,int b,bool a);
void setShip(char [N][N]);
void placeShip(char [N][N], int ship);
//void printLoadMenu();
void printStart(bool q,bool w,bool e);

int main(){
    char input = ' '; //variable para controlar los imputs del teclado
    char b[N][N]; //matriz del jugador 1
    char c[N][N]; // matriz del jugador 2
    int count = 0; //controlar la opcion selecionada
    printLogo(1,0,true); //dibujar el logo de battleship 
    printStart(1,0,0); //dibujar el menu con sus opciones en el color de la opcion selecionada
    while(true){
        input = tolower(getch()); //mirar si ha pulsado a/A o d/D para moverse entre las opciones
        clearScreen(); //borrar todo el contenido de la terminal(la funcion system("clear") no me funciona en el compilador online)
        printLogo(1,0,true); //dibujar el logo de battleship
        switch(input){
        case 'a': //mover la seleccion hacia la izquierda
            count --;
            if(count == -1){ //resetear la selecion y pasar al lado contrario
                count =2;
            }
            if(count == 0){
                printStart(1,0,0);
            }
            else if(count == 1){
                printStart(0,1,0);
            }
            else if(count == 2){
                printStart(0,0,1);
            }
            break;
        case 'd': //mover la seleccion hacia la derecha
            count ++;
            if(count == 3){ //resetear la selecion y pasar al lado contrario
                count =0;
            }
            if(count == 0){
                printStart(1,0,0);
            }
            else if(count == 1){
                printStart(0,1,0);
            }
            else if(count == 2){
                printStart(0,0,1);
            }
            
            break;
        default:
            if(count==0){ //si la opcion selecionada es start, empezar el juego
                startGame(b);
            }
            else if(count==1){ //si la opcion selecionada es score, mostrar la puntuacion
                //ver puntaciones
                tratarOpcionPuntuaciones();
                printf("\npress a or d to go back");
            }
            else if(count==3){ //si la opcion selecionada es exit, salir del juego
                _exit;
            }
        }
    }
    return 0;
}
    

void startGame(char b[N][N]){
    clearScreen();
    int ordendisparos[N*N][2];
    iaMovimiento(ordendisparos); //declarar todos los movimientos que va a tener la ia en la partida

    printf("Hundir la flota");
    char tablero1[N][N]; //crear los tableros del jugador y de la ia
    char tablero2[N][N];
    
    int puntosJugador = 0;
    int puntosIA = 0;
    int ronda = 0;
    int numTurnos = 0;
    
    int disparoX,disparoY;
    char disparo2Y;
    
    bool ganar = false;
    bool turno = true;

    crearTableroVacio(tablero1); //asignar valores al tablero representando el agua
    crearTableroVacio(tablero2);
    
    //jugadorColocarBarco(tablero2); //colocar los barcos del jugador
    iaBarcosAleatorios(tablero2);
    
    iaBarcosAleatorios(tablero1); //colocar los barcos de la ia
    
    //imprimirJuego(tablero1,tablero2,puntosJugador,puntosIA); //imprimir los dos tableros del juego con los aciertos de cada uno  

    while(!ganar){ //jugar partida

        // Reinicializamos los disparos
        disparoX = 0;
        disparoY = 0;
        disparo2Y = "";

        if(turno){
            imprimirJuego(tablero1,tablero2,puntosJugador,puntosIA);
            printf("\nIntruce el disparo en X(0-6): ");
            scanf("%d",&disparoX);
            printf("\nIntruce el disparo en Y(A-G): ");
            scanf(" %c",&disparo2Y);
            disparoY = tolower(disparo2Y) - 97;
        }
        else{
            disparoX = ordendisparos[ronda][0];
            disparoY = ordendisparos[ronda][1];
            printf("\nIA dispara a [%d %d]", disparoX,disparoY);
        }
        if(disparoY <= 6 && disparoY >= 0 && disparoX <= 6 && disparoY >= 0){
            
            if(turno && mirarDisparo(tablero1,disparoX,disparoY)){
                puntosJugador++;
                tablero1[disparoX][disparoY] = 'T';
                printf("\nBarco!!");
            }
            else if(turno && !mirarDisparo(tablero1,disparoX,disparoY)){
                tablero1[disparoX][disparoY] = 'F';
                printf("\nAgua!!");
                turno = !turno;
            }
            else if(!turno && mirarDisparo(tablero2,disparoX,disparoY)){
                puntosIA++;
                printf("\nLa IA a DADO A TU barco!!");
                tablero2[disparoX][disparoY] = 'T';
            }
            else if(!turno && !mirarDisparo(tablero2,disparoX,disparoY)){
                printf("\nLa IA a DADO Agua!!");
                tablero2[disparoX][disparoY] = 'F';
                turno = !turno;
            }
            if(!turno){
                printf("\nRonda actual: %d", ronda);
                ronda ++;
            }

            numTurnos++;

        }
        else{
            printf("\nPosicion fuera del tablero");
        }
        
        if(puntosJugador == PUNTUACIONMAXIMA){
            printf("\nEL Jugador ha ganado la partida");
            ganar = true;
            // Guardamos la puntuacion
            char iniciales[4];
            printf("\nIntroduzca sus iniciales: ");
            scanf("%3s",iniciales);

            char buf[BUFSIZ];
            sprintf(buf, "%d ", numTurnos);

            strcat(buf, iniciales);
            
            insertarPuntuacion(buf);
        }
        else if(puntosIA == PUNTUACIONMAXIMA){
            printf("\nLa IA ha ganado la partida");
            ganar = true;
        }
            
    }
    
}
void setShip(char b[N][N]){ //funcion para selecionar que barco quieres colocar
    int aircraftCarrier = 1;
    int cruiseShips = 2;
    int patrolBoats = 3;
    int ship;
    printf("\nRemaining (%d) aircraftCarrier : 1 \nRemaining (%d) cruiseShips: 2 \nRemaining (%d) patrolBoats: 3 \nwhat do you want to place: ",aircraftCarrier,cruiseShips,patrolBoats);
    scanf("%d",&ship); //selecionar el tipo de barco
    switch(ship){
        case 1:
            aircraftCarrier--; //restar a la cantidad de barcos restantes
            if(aircraftCarrier<0){ //si no queda ningun borco de este tipo
                printf("\n0 aircraftCarrier remaining");
                printf("\nRemaining %d cruiseShips: 2 \nRemaining %d patrolBoats: 3 \nWhat do you want to place: ",cruiseShips,patrolBoats);
                scanf("%d",&ship);
            }
            placeShip(b,ship);
            break;
        case 2:
            cruiseShips--; //restar a la cantidad de barcos restantes
            if(cruiseShips<0){
                printf("\n0 cruiseShips remaining");
                printf("\nRemaining %d aircraftCarrier : 1 \nRemaining %d patrolBoats: 3 \nWhat do you want to place: ",aircraftCarrier,patrolBoats);
                scanf("%d",&ship);
            }
            placeShip(b,ship);
            break;
        case 3:
            patrolBoats--; //restar a la cantidad de barcos restantes
            if(patrolBoats<0){
                printf("\n0 patrolBoats remaining");
                printf(" \nRemaining %d aircraftCarrier : 1 \nRemaining %d cruiseShips: 2 \nWhat do you want to place?",aircraftCarrier,cruiseShips);
                scanf("%d",&ship);
            }
            placeShip(b,ship);
            break;
        default:
            printf("\nRemaining (%d) aircraftCarrier : 1 \nRemaining (%d) cruiseShips: 2 \nRemaining (%d) patrolBoats: 3 \nwhat do you want to place: ",aircraftCarrier,cruiseShips,patrolBoats);
            scanf("%d",&ship);
            break;
    }
}
void placeShip(char b[N][N],int ship){ //funcion para colocar el barco en la posicion de la matriz selecionada
    clearScreen;
    printBoard(b);
    int x;
    int y;
    int shiplong;
    int direction;
    int a;
    //int selectedPos[2] = [-1,-1];
    
    //definir la longitud de cada barco(estaria bien hacerlo con una libreria)
    if(ship == 1){
        shiplong = 4;
    }
    else if(ship == 2){
        shiplong = 3;
    }
    else if(ship == 2){
        shiplong = 2;
    }
    


    printf("\nwhere do you want to place your ship?");
    printf("\nx: ");

    scanf("%d",&x); //selecionar la fila que quieres colocar el barco
    printf("\ny: ");
    scanf("%d",&y); //selecionar la columna que quieres colocar el barco
    //en progreso
    
    // if (mode = 1){
    //     do{
    //         a = getch();
    //         printf("%d",a);
    //         if(a == 'w'){  
    //             printf("↑");
        
    //         }
    //         else if(a == 'a'){ 
    //             printf("←");
                
    //         }
    //         else if(a == 's'){ 
    //             printf("↓");
                
    //         }
    //         else if(a == 'd'){ 
    //             printf("→");
                
    //         }
    //         else {
                
    //         }
    //     }while(true)
        
    //}
    
}
void clearScreen(){
  printf("\e[1;1H\e[2J");//limpiar la pantalla
}

// void delay(int timer){
    
//     timer *= 10000;
//     for (int i = 0; i<= timer; i++){
//         for (int j = 0; j<= timer; j++){
            
//         }
//     }

// }

char getch(void){
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if(tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if(tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if(read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if(tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    printf("%c\n", buf);
    return buf;
}
void createEmptyBoard(char b[N][N]){
    for (int i=0; i<N; i++){
        for (int j=0; j<N; j++){
            b[i][j] = '.';
        }
    }
}

void printBoard(char b[N][N]){
    clearScreen();
    //system("clear");

    printf("\n    ");
    for (int i=0; i<N; i++){ //imprimir la fila de arriba con las letras
        cian("");
        printf(" %c ",i+65);
    }
    printf("\n    ");
    for (int i=0; i<N; i++){ //decoracion
        cian("- -");
    }
    printf("\n");
    
    for (int i=0; i<N; i++){
        printf(" %d ",i);
        cian("|");
        for (int j=0; j<N; j++){
            reset();
            printf("|"); //decoracion
            printf("%c",b[i][j]); //imprimir la matriz de barcos
            printf("|"); //decoracion
        }
    cian("|"); //decoracion
    printf(" %d",i); //imprimir la columna con numeros
    printf("\n");
    }
    printf("    ");
    for (int i=0; i<N; i++){ //decoracion
        cian("- -"); 
    }
    printf("\n    ");
        for (int i=0; i<N; i++){ //imprimir la fila de arriba con las letras
        printf(" %c ",i+65);
    }

}

//colores
void black(char a[]){
    printf("\033[1;30m");
    printf("%s",a);
}
void red(char a[]){
    printf("\033[0;31m");
    printf("%s",a);
}
void green(char a[]){
    printf("\033[0;32m");
    printf("%s",a);
}
void yellow(char a[]){
    printf("\033[1;33m");
    printf("%s",a);
}
void blue(char a[]){
    printf("\033[0;34m");
    printf("%s",a);
    
}
void purple(char a[]){
    printf("\033[0;35m");
    printf("%s",a);
}
void cian(char a[]){
    printf("\033[0;36m");
    printf("%s",a);
}
void white(char a[]){
    printf("\033[0;37m");
    printf("%s",a);
}

void reset(){
    printf("\033[0m");
}
// void printLoadMenu(){
//     printf("Loaging...");
//     for(int i = 0; i <=20;i++){
//         for(int j= 0; j <=10000000;j++){
            
//         }
//         printf("░");
//     }
//     printf("\n");
// }

//decoracion para imprimir menus y demas
void printStart(bool q, bool w, bool e){
    cian("....................................................................................................................\n");
    for(int i =0;i<=4;i++){
        cian(".");
        printLogo(2,i,q);
        cian(" . ");
        printLogo(3,i,w);
        cian(" . ");
        printLogo(4,i,e);
        cian(".");
        printf("\n");
    }
    cian("....................................................................................................................\n");
}
void printLogo(int i,int b,bool a){
    if(a){
        green("");
    }
    else if(!a){
        red("");
    }
    switch(i){
        case 1:
            if(b == 0)
                
                cian(".......................................................................................\n");
                cian(".");
                blue("██████╗░░ █████╗░ ████████╗ ████████╗ ██╗░░░░░ ███████╗ ░██████╗ ██╗░░██╗ ██╗ ██████╗░");
                cian(".\n.");
                blue("██╔══██╗ ██╔══██╗ ╚══██╔══╝ ╚══██╔══╝ ██║░░░░░ ██╔════╝ ██╔════╝ ██║░░██║ ██║ ██╔══██╗");
                cian(".\n.");
                blue("██████╦╝ ███████║ ░░░██║░░░ ░░░██║░░░ ██║░░░░░ █████╗░░ ╚█████╗░ ███████║ ██║ ██████╔╝");
                cian(".\n.");
                blue("██╔══██╗ ██╔══██║ ░░░██║░░░ ░░░██║░░░ ██║░░░░░ ██╔--╝░░ ░╚═══██╗ ██╔══██║ ██║ ██╔═══╝░");
                cian(".\n.");
                blue("██████╦╝ ██║░░██║ ░░░██║░░░ ░░░██║░░░ ███████╗ ███████╗ ██████╔╝ ██║░░██║ ██║ ██║░░░░░");
                cian(".\n.");
                blue("╚═════╝░ ╚═╝░░╚═╝ ░░░╚═╝░░░ ░░░╚═╝░░░ ╚══════╝ ╚══════╝ ╚═════╝░ ╚═╝░░╚═╝ ╚═╝ ╚═╝░░░░░");
                cian(".\n.......................................................................................\n");
                reset();
            break;
        case 2:
            if(b == 0)
                printf("██████╗░ ██╗░░░░░ ░█████╗░ ██╗░░░██╗");
            else if(b == 1)
                printf("██╔══██╗ ██║░░░░░ ██╔══██╗ ╚██╗░██╔╝");
            else if(b == 2)
                printf("██████╔╝ ██║░░░░░ ███████║ ░╚████╔╝░");
            else if(b == 3)
                printf("██╔═══╝░ ██║░░░░░ ██╔══██║ ░░╚██╔╝░░");
            else if(b == 4)
                printf("██║░░░░░ ███████╗ ██║░░██║ ░░░██║░░░");
            else if(b == 5)
                printf("╚═╝░░░░░ ╚══════╝ ╚═╝░░╚═╝ ░░░╚═╝░░░");
            break;
        case 3:
            if(b == 0)
                printf("░██████╗ ░█████╗░ ░█████╗░ ██████╗░ ███████╗");
            else if(b == 1)
                printf("██╔════╝ ██╔══██╗ ██╔══██╗ ██╔══██╗ ██╔════╝");
            else if(b == 2)
                printf("╚█████╗░ ██║░░╚═╝ ██║░░██║ ██████╔╝ █████╗░░");
            else if(b == 3)
                printf("░╚═══██╗ ██║░░██╗ ██║░░██║ ██╔══██╗ ██╔══╝░░");
            else if(b == 4)
                printf("██████╔╝ ╚█████╔╝ ╚█████╔╝ ██║░░██║ ███████╗");
            else if(b == 5)
                printf("╚═════╝░ ░╚════╝░ ░╚════╝░ ╚═╝░░╚═╝ ╚══════╝");
            break;

        case 4:
            if(b == 0)
                printf("███████╗██╗░░██╗██╗████████╗");
            if(b == 1)
                printf("██╔════╝╚██╗██╔╝██║╚══██╔══╝");
            if(b == 2)
                printf("█████╗░░░╚███╔╝░██║░░░██║░░░");
            if(b == 3)
                printf("██╔══╝░░░██╔██╗░██║░░░██║░░░");
            if(b == 4)
                printf("███████╗██╔╝╚██╗██║░░░██║░░░");
            if(b == 5)
                printf("╚══════╝╚═╝░░╚═╝╚═╝░░░╚═╝░░░");
            break;
            
        case 5:
            if(b == 0)
                printf("░█████╗░ ██████╗░ ████████╗ ██╗ ░█████╗░ ███╗░░██╗");
            else if(b == 1)
                printf("██╔══██╗ ██╔══██╗ ╚══██╔══╝ ██║ ██╔══██╗ ████╗░██║");
            else if(b == 2)
                printf("██║░░██║ ██████╔╝ ░░░██║░░░ ██║ ██║░░██║ ██╔██╗██║");
            else if(b == 3)
                printf("██║░░██║ ██╔═══╝░ ░░░██║░░░ ██║ ██║░░██║ ██║╚████║");
            else if(b == 4)
                printf("╚█████╔╝ ██║░░░░░ ░░░██║░░░ ██║ ╚█████╔╝ ██║░╚███║");
            else if(b == 5)
                printf("░╚════╝░ ╚═╝░░░░░ ░░░╚═╝░░░ ╚═╝ ░╚════╝░ ╚═╝░░╚══╝");
            //printf("\n");
            break;
        default:
            //nop
            break;
            
    }
    
    reset();
}

void imprimirJuego(char tablero1[N][N],char tablero2[N][N], int puntosJugador, int puntosIA){
    printf("\ntablero jugador 1");
    imprimirTablero(tablero2,false);
    printf("\ntablero IA");
    imprimirTablero(tablero1,true);
    printf("\nBarcos acertados por el jugador: %d", puntosJugador);
    printf("\nBarcos acertados por el jugador: %d", puntosIA);
}
bool mirarDisparo(char tablero[N][N],int x,int y){
    bool tocado = false;
    if(tablero[x][y] == 'b'){
        tocado = true;
    }
    // else{
    //     //
    // }
    return tocado;
}
void jugadorColocarBarco(char tablero[N][N]){
    int barcos[3];
    barcos[0] = 1; //barcos de 4 
    barcos[1] = 2; //barcos de 3
    barcos[2] = 3; //barcos de 2
    
    for(int i = 0; i != 3;i++){ //bucle que recorre toda la lista de barcos
        int tamaino = 4-i;
        while(barcos[i] > 0){ //bucle que recorre la cantidad de cada tipo de barco
            for(int j = 0; j != 3;j++){ //imprimir la cantidad de barcos restantes para colocar
                printf("\ntamaino %d: %d",tamaino,barcos[j]);
            }
            imprimirTablero(tablero,false);
            int posicionX,direccion; //declarar variables para posicionar el barco del turno
            char posicionY;
            bool posible;
            printf("\nIntruce la posicion en X(0-6): ");
            scanf("%d",&posicionX);
            printf("\nIntruce la posicion en Y(A-G): ");
            scanf(" %c",&posicionY);
            posicionY = tolower(posicionY) - 97; //remapear el valor a 0-6
            printf("\nIntruce la direccion(0-1): ");
            scanf("%d",&direccion);
            printf("%d%d%d", posicionX,posicionY,direccion);
            if(posicionX <= 6 && posicionX >= 0 && posicionY <= 6 && posicionY >= 0 && (direccion == 0 || direccion == 1)){ //mirar si la coordenada esta dentro del tablero
                posible = mirarPosicionDeBarco(tablero,posicionX,posicionY,direccion,tamaino);
                if(posible){
                    barcos[i] --; //restar uno cada vez que la posicion sea correcta
                    colocarBarco(tablero,posicionX,posicionY,direccion,tamaino); //colocar el barco cada vez que la posicion sea correcta
                }
                else{
                    printf("\nPosicion Incompatible");
                }
            }

            else{
                printf("\nPosicion fuera del tablero");
            }
        }
    }
}
void iaMovimiento(int ordendisparos[N*N][2]){
    int array[N*N];
    int l= 0;
    int k= 0;
    for (int i = 0; i < 49; i++) { //generar una lista con todas las posibles opciones
        //printf("%d [",i);
        for (int j = 0; j < 2; j++){
            
            if(j == 0){
                ordendisparos[i][j] = l;
            }
            else if(j == 1){
                ordendisparos[i][j] = k;
                k++;
            }
            //printf("%d ", ordendisparos[i][j]);
        }

        if((i+1 )%7 == 0 && i !=0){
            l++;
            k = 0;
        }
        //printf("]\n");
    }
    
    srand(time(NULL));
    for(int i = 0; i < N*N; i++) {     //generar una lista con todas las posibles opciones
        array[i] = i;
        //printf("%d \n", array[i]);
    }

    for(int i = 0; i < N*N; i++) {    //generar aleatoriamente las coordenadas
        int temp = array[i];
        int randomIndex = rand() % 49;
    
        array[i] = array[randomIndex];
        array[randomIndex] = temp; 
        //printf("%d \n", array[i]);
        
        int temp1 = ordendisparos[array[i]][0];
        int temp2 = ordendisparos[array[i]][1];
        ordendisparos[array[i]][0] = ordendisparos[array[i]][0]; //generar el orden en el que la ia va a disparar en toda la paritda
        ordendisparos[array[i]][1] = ordendisparos[array[i]][1]; //generar el orden en el que la ia va a disparar en toda la paritda
        ordendisparos[i][0] = temp1;
        ordendisparos[i][1] = temp2; 
        //printf("[%d, %d]\n", ordendisparos[array[i]][0], ordendisparos[array[i]][1]);
    }
}
bool mirarPosicionDeBarco(char tablero[N][N],int x,int y, int direccion, int tamaino){
    bool posible = true;
    if(tablero[x][y] != '.'){ //mirar si la posicion del barco esta ya ocupada
        printf("esta ocupada");
        posible = false;
    }
    else if(tablero[x][y] == 'T' || tablero[x][y] == 'F'){
        printf("Disparo repetido, pierdes turno");
        posible = false;
    }
    else if(direccion == 0){
        if(y < (N-tamaino)+1){ //mirar si el barco choca con la pared
            //printf("siH");
            for(int i = 1; i != tamaino; i++){ //mirar si el barco choca con otro barco
                if(tablero[x][y+i]  != '.'){
                    posible = false;
                    //printf("NoH");
                }
            }
        }
        else{
            posible = false;
            printf("no entra el barco");
        } 
    }
    else if(direccion == 1){ //mirar si el barco choca con la pared
        if(x < (N-tamaino)+1){
            //printf("siV");
            for(int i = 1; i != tamaino; i++){ //mirar si el barco choca con otro barco
                if(tablero[x+i][y]  != '.'){
                    posible = false;
                    printf("esta ocupada");
                }
            }
        }
        else{
            posible = false;
            printf("no entra el barco");
        } 
    }
    return posible;
}
void iaBarcosAleatorios(char tablero[N][N]){
    int barcos[3];
    bool posible;
    barcos[0] = 1; //barcos de 4 
    barcos[1] = 2; //barcos de 3
    barcos[2] = 3; //barcos de 2
    
    srand(time(NULL));
    //printf("%d",sizeof(barcos)/sizeof(barcos[0])); //calcular longitud de una lista
    
    for(int i = 0; i != 3;i++){ //bucle que recorre toda la lista de barcos
        int tamaino = 4-i;
        while(barcos[i] > 0){ //bucle que recorre la cantidad de cada tipo de barco
            
            
            int direccion = rand()%2; //crear un valor aleatorio entre 0 y 1 para deciridir si la ia coloca un barco 0 = horizontal o 1 = vertical
            
            int posicionX = rand()%N;
            int posicionY = rand()%N;
            //printf("%d",i);
            //printf("%d,%d, (%d,%c)",direccion,tamaino,posicionX,posicionY+65);

            posible = mirarPosicionDeBarco(tablero,posicionX,posicionY,direccion,tamaino);
            if(posible){
                barcos[i] --; //restar uno cada vez que se ejecute
                colocarBarco(tablero,posicionX,posicionY,direccion,tamaino);
            }
            //printf("\n");

        }
    }
    
}
void colocarBarco(char tablero[N][N],int x,int y, int direccion, int tamaino){ //asigna un valor a las casillas que tienen un barco
    if(direccion == 0){
        for(int i = 0; i != tamaino; i++){
            tablero[x][y+i] = 'b';
        }
    }
    else if(direccion == 1){
        for(int i = 0; i != tamaino; i++){
            tablero[x+i][y] = 'b';
        }
    }
}
void crearTableroVacio(char tablero[N][N]){ //crea un tablero de N x N con . representando el valor de vacio/agua
    for (int i=0; i<N; i++){
        for (int j=0; j<N; j++){
            tablero[i][j] = '.';
        }
    }
}
void imprimirTablero(char tablero[N][N], bool oculto){
    //limpiarPantalla();
    //system("clear");

    printf("\n    ");
    for (int i=0; i<N; i++){ //imprimir la fila de arriba con las letras
        cian("");
        printf(" %c ",i+65);
    }
    printf("\n    ");
    for (int i=0; i<N; i++){ //decoracion
        cian("- -");
    }
    printf("\n");
    
    for (int i=0; i<N; i++){
        printf(" %d ",i);
        cian("|");
        if(!oculto){
            for (int j=0; j<N; j++){
                printf("|"); //decoracion
                printf("%c",tablero[i][j]); //imprimir la matriz de barcos
                printf("|"); //decoracion
            }
        }
        else{
            for (int j=0; j<N; j++){
                printf("|"); //decoracion
                if(tablero[i][j] == 'b'){
                    printf("%c",'.');
                }
                else{
                    printf("%c",tablero[i][j]); //imprimir la matriz de barcos
                }
                printf("|"); //decoracion

            }
        }
    cian("|"); //decoracion
    printf(" %d",i); //imprimir la columna con numeros
    printf("\n");
    }
    printf("    ");
    for (int i=0; i<N; i++){ //decoracion
        cian("- -"); 
    }
    printf("\n    ");
        for (int i=0; i<N; i++){ //imprimir la fila de arriba con las letras
        printf(" %c ",i+65);
    }

}

void limpiarPantalla(){
  printf("\e[1;1H\e[2J");//limpiar la pantalla
}

/**
 * Opcion de tratar las puntuaciones, leemos el archivo e imprimimos su contenido
*/
void tratarOpcionPuntuaciones(){

    /* Abrimos el archivo en modo lectura */
    FILE *fichero = fopen(nombreFicheroPuntuaciones, modoAperturaFicheroLeerFichero);
    char content;

    if(fichero != NULL){
        // Obtenemos la primera linea
        content = fgetc(fichero);

        // Mientras lo que nos devuelva no sea End of File, imprimimos por pantalla y volvemos a recoger contenido
        while(content != EOF){
            printf ( "%c", content);
            content = fgetc(fichero);
        }
        printf("\n");

        /* Cerramos el fichero para que no se corrompa */
        fclose(fichero);
    }else{
        printf("Error al abrir el fichero de puntuaciones.");
    }

}

/**
 * Inserta en el fichero especificado en constantes la cadena de char introducida
*/
void insertarPuntuacion(char puntuacion[]){

    /* Abre el fichero en modo escritura */
    FILE *fichero = fopen(nombreFicheroPuntuaciones, "a+");

    if(fichero != NULL){

        /* Insertamos un line break y el contenido */
        fputs(puntuacion, fichero);
        fputs("\n", fichero);

        fclose(fichero);
    }else{
        printf("Error al abrir el fichero de puntuaciones.");
    }

}