#include <stdio.h>

/*
* @author Antonio Suano Lara
*/

void mostrarMenu();

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
                break;
            case 2:
                // Opción jugar
                printf("Ha seleccionado jugar... COMING SOON.\n");
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
