#include <stdio.h>
#include "hashmap.h"

int main()
{
    int caso=2;
    while(caso!=0)
    {
        printf("1. Atrapar Pokemon\n");
        printf("2. Evolucionar Pokemon\n");
        printf("3. Buscar Pokemon por tipo\n");
        printf("4. Buscar Pokemon por nombre\n");
        printf("5. Buscar Pokemon por nombre en la Pokedex\n");
        printf("6. Mostrar Todos los Pokemon de la Pokedex\n");
        printf("7. Mostrar Pokemon ordenados por PC\n");
        printf("8. Liberar Pokemon\n");
        printf("9. Mostrar Pokemon por Region\n");
        printf("0. Salir\n");
        scanf("%d", &caso);

        switch(caso)
        {
            case 1:printf("NO IMPLEMENTADA\n");break;
            case 2:printf("NO IMPLEMENTADA\n");break;
            case 3:printf("NO IMPLEMENTADA\n");break;
            case 4:printf("NO IMPLEMENTADA\n");break;
            case 5:printf("NO IMPLEMENTADA\n");break;
            case 6:printf("NO IMPLEMENTADA\n");break;
            case 7:printf("NO IMPLEMENTADA\n");break;
            case 8:printf("NO IMPLEMENTADA\n");break;
            case 9:printf("NO IMPLEMENTADA\n");break;
        }
    }
    return 0;
}