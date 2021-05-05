#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "hashmap.h"
#include "list.h"

//Estructura Pokemon con los datos de combate
typedef struct{
    char nombre[50];
    char id[3];
    int PC;
    int PS;
    char sexo[2];
}Pokemon;

//Estructura pokemon atrapado con datos por pokemon respecto a la pokedex
typedef struct{
    char pokemon[50];
    int existencia;
    char * tipo[50];
    char previa[50];
    char posterior[50];
    char region[20];
}PokemonAtrapado;


Pokemon* createPokemon(char* nombre, char* id, int PC, int PS){
    Pokemon* p = (Pokemon*) malloc(sizeof(Pokemon));
    strcpy(p->nombre,nombre);
    strcpy(p->id,id);
    p->PC=PC;
    p->PS=PS;
    return p;
}

PokemonAtrapado* createPokemonAtrapado(char* pokemon, char** tipo, char* previa, char* posterior, char* region){
    PokemonAtrapado* pa = (PokemonAtrapado*) malloc (sizeof(PokemonAtrapado));
    strcpy(pa->pokemon,pokemon);
    strcpy(pa->previa,previa);
    strcpy(pa->posterior,posterior);
    strcpy(pa->region,region);
    pa->existencia = 1;
    return pa;
}

//Conversor de char a int
int conversorInt(char * cadena){
    int i=cadena[0]-'0';
    if(cadena[1]-'0'>=0){
        i=i*10;
        i+=cadena[1]-'0';
    }
    return i;
}


//Funcion para leer el k-esimo elemento de un string (separado por comas) nos ayuda para cargar el archivo
char*get_csv_field (char * tmp, int k){
    int open_mark = 0;
    char* ret=(char*) malloc(100*sizeof(char));
    int ini_i=0, i=0;
    int j=0;
    while(tmp[i+1]!='\0'){
        if(tmp[i]== '\"'){
            open_mark = 1-open_mark;
            if(open_mark) ini_i = i+1;
            i++;
            continue;
        }
        if(open_mark || tmp[i]!= ','){
            if(k==j) ret[i-ini_i] = tmp[i];
            i++;
            continue;
        }
        if(tmp[i]== ','){
            if(k==j) {
               ret[i-ini_i] = 0;
               return ret;
            }
            j++; ini_i = i+1;
        }
        i++;
    }
    if(k==j) {
       ret[i-ini_i] = 0;
       return ret;
    }
    return NULL;
}

//Guardamos los pokemon en nuestro almacenamiento pokemon y pokedex
void Storage(HashMap * map,HashMap * Pokedexs,char* nombre, char * id,char** tipos, int PC, int PS, char* sexo, char* evoPrev, char* evoPos, char* nPokedex, char* region){
    Pokemon *p = createPokemon(nombre,id,PC,PS);
    insertMap(map,id,p);
    List * pokedex = searchMap(Pokedexs,nPokedex);
    if(!pokedex){
        PokemonAtrapado * pa = createPokemonAtrapado(nombre,tipos,evoPrev,evoPos,region);
        List * poke = createList();
        pushBack(poke,pa);
        insertMap(Pokedexs,nPokedex,poke);
    }
    else{
        PokemonAtrapado * aux = firstList(pokedex);
        while(aux){
            if(!strcmp(aux->pokemon,nombre))break;
            aux=nextList(pokedex);
        }
        if(!aux){
            PokemonAtrapado * pa = createPokemonAtrapado(nombre,tipos,evoPrev,evoPos,region);
            pushBack(pokedex,pa);
        }
        else{
            aux->existencia+=1;
        }
    }
}


//Cargamos pokemon a partir de uno o mas archivos .csv ingresados por el usuario
void cargarPokemon(HashMap * map, HashMap * pokedex){
    char nameFile[101],answer[2];
    bool flag=true;
    FILE *file,*newfile;
    gets(nameFile);
    //Abrimos el archivo ingresado por el usuario
    do{
        do{
            printf("\nIngrese el nombre del archivo sin extension:");
            gets(nameFile);
            strcat(nameFile,".csv");
            file = fopen (nameFile, "r" ); 
        }while(!file);

        // Cadena para guardar la linea completa del archivo csv
        char linea[1024];
        //Obtenemos la primera linea del archivo sin datos y copiamos en el nuevo 
        fgets (linea, 1023, file);

        newfile=fopen("storage.csv","a");
        fputs(linea,newfile);

        char *nombre,*sexo,*ePrev,*ePos,*region,*id,*tipo[4],*nPok;
        int i,pc=0,ps=0,k=0;

        while (fgets (linea, 1023, file) != NULL) { 
        // Se lee la linea y se copia en el nuevo archivo
            fputs(linea,newfile);
        //Creamos un bombero por linea de archivo y guardamos en el los datos 
            for(int i=0;i<10;i++){
                char *aux = get_csv_field(linea, i); 
                if(i==0)id=aux;
                if(i==1)nombre=aux;
                if(i==2)tipo[0]=aux;
                if(i==3)pc=conversorInt(aux);
                if(i==4)ps=conversorInt(aux);
                if(i==5)sexo=aux;
                if(i==6)ePrev=aux;
                if(i==7)ePos=aux;
                if(i==8)nPok=aux;
                if(i==9)region=aux;
            }
            Storage(map,pokedex,nombre,id,tipo,pc,ps,sexo,ePrev,ePos,nPok,region);
            if(sizeMap(map)==100){
                printf("Has llegado al limite de pokemon en tu almacenamiento\n");
                flag=false;
                break;
            }
        }

        fclose(file);

        //Preguntamos si quiere agregar mas pokemon desde otro archivo
        printf("Desea cargar otro archivo? (responda S/N segun corresponda) ");
        gets(answer);
        answer[0]=toupper(answer[0]);
        if(!strcmp(answer,"N"))break;

    }while(flag);
    fclose(newfile);
    printf("***************Los pokemon se cargaron exitosamente***************\n");
}
//Funcion que ingresa un pokemon y sus datos por el usuario, falta arreglar cuando es mas de un tipo
void atraparPokemon(HashMap * map, HashMap * pokedex)
{
    if(sizeMap(map)==100)
    {
        printf("Has llegado al limite de pokemon en tu almacenamiento\n");        
    }
    else
    {
        printf("Introduzca los datos del Pokemon\n");
        char cad[78] = "Id:Nombre:Tipos:PC:PS:Pre-Evolucion:Post-Evolucion:Region:Sexo:Numero-Pokedex:";
    
       char id[5];
       char nombre[50];
       char *tipos[50];
       int pc;
       int ps;
       char prEvol[50];
       char postEvol[50];
       char region[50];
       char sexo[2];
       char nPokedex[5];
       int cont=0;
       int i,j;
       int k=0;
       for (i=0 ; i<10 ; i++)
       {
           for (j=k ; j<81 ; j++)
           {
               printf("%c",cad[j]);
               if(cad[j] == ':')
               {
                   k = j+1;
                   break;
               }
           }
           if(cont == 0) scanf("%s",&id);
           if(cont == 1) scanf("%s",&nombre);
           if(cont == 2) scanf("%s",&tipos[0]);
           if(cont == 3) scanf("%d",&pc);
           if(cont == 4) scanf("%d",&ps);
           if(cont == 5) scanf("%s",&prEvol);
           if(cont == 6) scanf("%s",&postEvol);
           if(cont == 7) scanf("%s",&region);
           if(cont == 8) scanf("%s",&sexo);
           if(cont == 9) scanf("%s",&nPokedex);
           printf("\n");
           cont ++;
       }
       Storage(map,pokedex,nombre, id, tipos,  pc,  ps, sexo, prEvol, postEvol, nPokedex, region);
        
       printf("%s Fue Atrapado Correctamente\n",nombre);
       //scanf("%s",&nombre);
    }

}
int main()
{

    HashMap * map = createMap(100);
    HashMap * pokedex = createMap(100);
    int caso=2;
    while(caso!=0)
    {
        printf(" 1. Cargar Pokemon\n");
        printf(" 2. Atrapar Pokemon\n");
        printf(" 3. Evolucionar Pokemon\n");
        printf(" 4. Buscar Pokemon por tipo\n");
        printf(" 5. Buscar Pokemon por nombre\n");
        printf(" 6. Buscar Pokemon por nombre en la Pokedex\n");
        printf(" 7. Mostrar Todos los Pokemon de la Pokedex\n");
        printf(" 8. Mostrar Pokemon ordenados por PC\n");
        printf(" 9. Liberar Pokemon\n");
        printf("10. Mostrar Pokemon por Region\n");
        printf(" 0. Salir\n");
        scanf("%d", &caso);

        switch(caso)
        {
            case 1:cargarPokemon(map,pokedex);break;
            case 2:atraparPokemon(map,pokedex);break;
            case 3:printf("NO IMPLEMENTADA\n");break;
            case 4:printf("NO IMPLEMENTADA\n");break;
            case 5:printf("NO IMPLEMENTADA\n");break;
            case 6:printf("NO IMPLEMENTADA\n");break;
            case 7:printf("NO IMPLEMENTADA\n");break;
            case 8:printf("NO IMPLEMENTADA\n");break;
            case 9:printf("NO IMPLEMENTADA\n");break;
            case 10:printf("NO IMPLEMENTADA\n");break;
        }
    }
    return 0;
}