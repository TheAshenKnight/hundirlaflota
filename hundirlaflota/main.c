#include <stdio.h>

/*
* @author Antonio Suano Lara
*/

char modoAperturaFicheroInsertarPuntuacion[1] = "a";
char modoAperturaFicheroLeerFichero[2] = "r+";
char nombreFicheroPuntuaciones[] = "puntuaciones.txt";

void mostrarMenu();
void tratarOpcionPuntuaciones();
void insertarPuntuacion(char* puntuacion);

int main()
{
    printf("--- HUNDIR LA FLOTA ---\n");
    int opcion = 0;
    do{
        mostrarMenu();
        scanf("%d", &opcion);
        
        switch(opcion){
            case 1:
                // Opción mostrar puntuaciones
                printf("Ha seleccionado ver puntuaciones... COMING SOON.\n");
                tratarOpcionPuntuaciones();
                break;
            case 2:
                // Opción jugar
                printf("Ha seleccionado jugar... COMING SOON.\n");
                insertarPuntuacion("HELLO MY FRIEND");
                break;
            case 3:
                printf("Esperamos haya disfrutado.\n");
                break;
        }
        
    }while(opcion != 3);
    
    return 0;
}

void mostrarMenu(){
    
    printf(" MENU PRINCIPAL \n");
    printf("1. Mostrar puntuaciones\n");
    printf("2. Jugar\n");
    printf("3. Salir\n");
    printf("Introduzca la opción deseada: ");
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
void insertarPuntuacion(char* puntuacion){

    /* Abre el fichero en modo escritura */
    FILE *fichero = fopen(nombreFicheroPuntuaciones, modoAperturaFicheroInsertarPuntuacion);

    if(fichero != NULL){
        
        /* Insertamos un line break primero y luego el contenido */
        fputs("\n", fichero);
        fputs(puntuacion, fichero);

        fclose(fichero);
    }else{
        printf("Error al abrir el fichero de puntuaciones.");
    }

}