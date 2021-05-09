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
    char sexo[10];
    char nPokedex[4];
}Pokemon;

//Estructura pokemon atrapado con datos por pokemon respecto a la pokedex
typedef struct{
    char pokemon[50];
    int existencia;
    char * tipo[50];
    char previa[50];
    char posterior[50];
    char region[20];
    char nPokedex[5];
}PokemonAtrapado;


Pokemon* createPokemon(char* nombre, char* id, int PC, int PS,char* sexo, char* nPokedex){
    Pokemon* p = (Pokemon*) malloc(sizeof(Pokemon));
    strcpy(p->nombre,nombre);
    strcpy(p->id,id);
    strcpy(p->sexo,sexo);
    strcpy(p->nPokedex,nPokedex);
    p->PC=PC;
    p->PS=PS;
    return p;
}

PokemonAtrapado* createPokemonAtrapado(char* pokemon, char** tipo, char* previa, char* posterior, char* region,char* nPokedex){
    PokemonAtrapado* pa = (PokemonAtrapado*) malloc (sizeof(PokemonAtrapado));
    strcpy(pa->pokemon,pokemon);
    strcpy(pa->previa,previa);
    strcpy(pa->posterior,posterior);
    strcpy(pa->region,region);
    strcmp(pa->nPokedex,nPokedex);
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
void Storage(HashMap * map,HashMap * Pokedexs,HashMap *REGION ,char* nombre, char * id,char** tipos, int PC, int PS, char* sexo, char* evoPrev, char* evoPos, char* nPokedex, char* region){
    Pokemon *p = createPokemon(nombre,id,PC,PS,sexo,nPokedex);
    insertMap(map,id,p);
    List *Regiones = searchMap(REGION,region);
    if(!Regiones)
    {
        PokemonAtrapado *pa = createPokemonAtrapado(nombre,tipos,evoPrev,evoPos,region,nPokedex);
        List * reg = createList();
        pushBack(reg,pa);
        insertMap(REGION,region,reg);
    }
    else
    {
        PokemonAtrapado * aux = firstList(Regiones);
        while(aux){
            if(!strcmp(aux->pokemon,nombre))break;
            aux=nextList(Regiones);
        }
        if(!aux){
            PokemonAtrapado * pa = createPokemonAtrapado(nombre,tipos,evoPrev,evoPos,region,nPokedex);
            pushBack(Regiones,pa);
        }
    }
    List * pokedex = searchMap(Pokedexs,nPokedex);
    if(!pokedex){
        PokemonAtrapado * pa = createPokemonAtrapado(nombre,tipos,evoPrev,evoPos,region,nPokedex);
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
            PokemonAtrapado * pa = createPokemonAtrapado(nombre,tipos,evoPrev,evoPos,region,nPokedex);
            pushBack(pokedex,pa);
        }
        else{
            aux->existencia+=1;
        }
    }
}


//Cargamos pokemon a partir de uno o mas archivos .csv ingresados por el usuario
void cargarPokemon(HashMap * map, HashMap * pokedex, HashMap * REGION){
    printf("-----------------------------------------------------------------------\n");
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
            printf("-----------------------------------------------------------------------\n");
        }while(!file);

        // Cadena para guardar la linea completa del archivo csv
        char linea[1024];
        //Obtenemos la primera linea del archivo sin datos y copiamos en el nuevo 
        fgets (linea, 1023, file);

        newfile=fopen("storage.csv","a");
        fputs(linea,newfile);

        while (fgets (linea, 1023, file) != NULL) { 
        // Se lee la linea y se copia en el nuevo archivo
            fputs(linea,newfile);
            char *nombre,*sexo,*ePrev,*ePos,*region,*id,*tipos[4],*nPok;
            int i,pc=0,ps=0,k=0;
            for(int i=0;i<10;i++){
                char *aux = get_csv_field(linea, i); 
                if(i==0)id=aux;
                if(i==1)nombre=aux;
                if(i==2){
                    //Separamos los tipos y los almacenamos en un arreglo bidimensional
                    int cont=0;
                    aux[strlen(aux)]='a';
                    for(int w=0;w<4;w++){
                        char * q = get_csv_field(aux, w);
                        if(q){
                            tipos[cont]=q;
                            cont++;
                        }
                        else{
                            tipos[cont]=NULL;
                            cont++;
                        }
                    }
                }
                if(i==3)pc=conversorInt(aux);
                if(i==4)ps=conversorInt(aux);
                if(i==5)sexo=aux;
                if(i==6)ePrev=aux;
                if(i==7)ePos=aux;
                if(i==8)nPok=aux;
                if(i==9)region=aux;
            }
            Storage(map,pokedex,REGION,nombre,id,tipos,pc,ps,sexo,ePrev,ePos,nPok,region);
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
    printf("-----------------------------------------------------------------------\n");
    printf("***************  Los pokemon se cargaron exitosamente  ***************\n");
    printf("-----------------------------------------------------------------------\n");
    printf("\n");
}
void cargarTipos(char** pokemon, int i)
{
    int f;
    for(f=0;f<i;f++)
    {
        printf("\nTipos:");
        scanf("%s",pokemon[f]);
    }
}
//Funcion que ingresa un pokemon y sus datos por el usuario
void atraparPokemon(HashMap * map, HashMap * pokedex, HashMap *REGION){
 printf("-----------------------------------------------------------------------\n");
    if(sizeMap(map)==100)
    {
        printf("Has llegado al limite de pokemon en tu almacenamiento\n");   
    }
    else
    {
        printf("Introduzca los datos del Pokemon\n");
        char cad[72] = "Id:Nombre:PC:PS:Pre-Evolucion:Post-Evolucion:Region:Sexo:Numero-Pokedex:";
        printf("-----------------------------------------------------------------------\n");
       char id[5];
       char nombre[50];
       //char **tipos;  
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
       //int z=0;
       for (i=0 ; i<9 ; i++)
       {
           for (j=k ; j<75 ; j++)
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
           if(cont == 2) scanf("%d",&pc);
           if(cont == 3) scanf("%d",&ps);
           if(cont == 4) scanf("%s",&prEvol);
           if(cont == 5) scanf("%s",&postEvol);
           if(cont == 6) scanf("%s",&region);
           if(cont == 7) scanf("%s",&sexo);
           if(cont == 8) scanf("%s",&nPokedex);
           printf("\n");
           cont ++;
       }
       printf("Escriba cuantos Tipos quiere ingresar: ");
       scanf("%d",&j);
       char** Tipos =(char**) malloc (sizeof(char*)*j);
       for (i=0;i< j; i++)
       {
           Tipos[i] = (char*)malloc(sizeof(char)*32);
       }
       cargarTipos(Tipos,j);
       Storage(map,pokedex,REGION,nombre, id, Tipos,  pc,  ps, sexo, prEvol, postEvol, nPokedex, region);
       printf("%s Fue Atrapado Correctamente\n",nombre);
    }
    printf("-----------------------------------------------------------------------\n");
}

//Le da formato a la cadena ingresada por si el usuario no inicio o escribio toda la palabra con mayuscula
char * formatoCadena(char * cadena){
    for(int i=0;i<strlen(cadena);i++){
        if(i==0)cadena[0]=toupper(cadena[0]);
        else cadena[i]=tolower(cadena[i]);
    }
    return cadena;
}

void buscarPorNombre(HashMap * map){
    if(map){
        printf("-----------------------------------------------------------------------\n");
        char nombre[50];
        bool flag=false;
        Pokemon * pokemon = firstMap(map);
        printf("Ingrese el nombre del pokemon a buscar: ");
        scanf("%s",&nombre);
        printf("-----------------------------------------------------------------------\n");
        strcpy(nombre,formatoCadena(nombre));
        while(pokemon){
            if(is_equal(pokemon->nombre,nombre)){
                if(!flag){
                    printf("***************** Resultados de %s *****************\n",nombre);
                    printf("----------------------------------\n");
                    printf("ID       PC       PS       SEXO\n");
                    printf("----------------------------------\n");
                    flag=true;
                }
                printf("%-2s",pokemon->id);
                printf("       ");
                printf("%-2d",pokemon->PC);
                printf("       ");
                printf("%-2d",pokemon->PS);
                printf("       ");
                printf("%s\n",pokemon->sexo);
            }
            pokemon=nextMap(map);
        }
        if(!flag)printf("No se encontraron resultados para %s\n",nombre);
        printf("----------------------------------\n");
        printf("\n");
    }
    else printf("No tiene ningun pokemon en su almacenamiento\n");
}

void evolucionar(HashMap * map, HashMap * pokedexs){
    if(map){
        printf("-----------------------------------------------------------------------\n");
        char id[3];
        bool flag=false;
        printf("Ingrese la id pokemon a evolucionar: ");
        scanf("%s",&id);
        Pokemon * p = searchMap(map,id);;
        if(p){
            List *pokedex = searchMap(pokedexs,p->nPokedex);
            if(pokedex){
                PokemonAtrapado * pa =firstList(pokedex);
                while(pa){
                    if(is_equal(pa->pokemon,p->nombre)){
                        if(is_equal(pa->pokemon,pa->posterior) || is_equal(pa->posterior,"No tiene")){
                            printf("El pokemon %s ha alcanzado su maxima evolucion\n",p->nombre);
                            flag=true;
                            break;
                        }
                        else{
                            printf("El pokemon %s evoluciono a",p->nombre);
                            strcpy(p->nombre,pa->posterior);
                            strcpy(pa->pokemon,pa->posterior);
                            p->PC=p->PC*1,5;
                            p->PS=p->PS*1,25;
                            flag=true;
                            printf(" %s exitosamente\n",p->nombre);
                            break;
                        }
                    }
                    else{
                        pa=nextList(pokedex);
                    }
                }
            }
        }
        if(!flag)printf("No se encontro ningun pokemon con la id asociada\n");
        printf("-----------------------------------------------------------------------\n");
    }
    else printf("No tiene ningun pokemon en su almacenamiento\n");
}
void imprimirTipos(char **pokemon, int i)
{
    int f;
    printf("Tipos: ");
    for(f=0;f<i;f++)
    {
        printf("%s",pokemon[f]);
        if(pokemon[f+1]==NULL)break;
        printf(", ");
    }
    printf("\n");
}
void MostrarXregion(HashMap *map, HashMap *pokedex, HashMap *REGION)
{
    printf("Escriba la region que quiere buscar:");
    char nregion[30];
    scanf("%s",&nregion);
    int i=0;
    printf("Usted se encuentra en a region %s\n",nregion);
    List *x = searchMap(REGION,nregion);
    PokemonAtrapado *aux = firstList(x);
    while(aux)
    {

        if(is_equal(aux->region,nregion)==1)
        {
            printf("Numero en la Pokedex: %s\n",aux->nPokedex);
            printf("Nombre: %s\n",aux->pokemon);
            printf("Pre-Evolucion: %s\n",aux->previa);
            printf("Post-Evolucion: %s\n",aux->posterior);
            printf("Existencia: %d\n",aux->existencia);
            printf("Region: %s\n",aux->region);
            printf("Tipo: %s\n",aux->tipo[0]);
            imprimirTipos(aux->tipo,10);
            i++;
        }
        printf("\n");
        aux = nextList(x);
    }
    printf("-----------------------------------------------------------------------\n");
    printf("************ Existen %d Pokemones en la region %s *****************\n",i,nregion);
    printf("-----------------------------------------------------------------------\n");
}
//Falta que sea probada en la Pokedex
void EliminarPokemon(HashMap * map, HashMap *Pokedex)
{
    printf("Ingrese el ID del pokemon que desea eliminar:");
    char id[5];
    char nombre[30];
    scanf("%s",&id);
    Pokemon * aux = searchMap(map,id);
    List *aux2 = searchMap(Pokedex,aux->nPokedex);
    PokemonAtrapado *P = firstList(aux2);
    
    
        if(is_equal(aux->id,id)==1)
        {
            strcpy(nombre,aux->nombre);
            eraseMap(map,id);
        }
        
        //aux = nextMap(map);
    
    while(aux2)
    {
        if(is_equal(P->nPokedex,aux->nPokedex)==1)
        {
            popCurrent(aux2);
            if(P->existencia>1)
            {
                P->existencia--;
                //Pokedex = aux2;
            }
            
        }
        aux2 = nextList(aux2);
    }
    printf("-----------------------------------------------------------------------\n");
    printf("************** %s fue eliminado\n****************",nombre);
    printf("-----------------------------------------------------------------------\n");
}
int main()
{

    HashMap * map = createMap(100);
    HashMap * pokedex = createMap(100);
    HashMap *REGION = createMap(100);
    int caso=2;
    printf("-----------------------------------------------------------------------\n");
    printf("                        ALMACENAMIENTO POKEMON                        \n");
    printf("-----------------------------------------------------------------------\n");
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
            case 1:cargarPokemon(map,pokedex,REGION);break;
            case 2:atraparPokemon(map,pokedex,REGION);break;
            case 3:evolucionar(map,pokedex);break;
            case 4:printf("NO IMPLEMENTADA\n");break;
            case 5:buscarPorNombre(map);break;
            case 6:printf("NO IMPLEMENTADA\n");break;
            case 7:printf("NO IMPLEMENTADA\n");break;
            case 8:printf("NO IMPLEMENTADA\n");break;
            case 9:EliminarPokemon(map,pokedex);break;
            case 10:MostrarXregion(map,pokedex,REGION);break;
        }
    }
    return 0;
}