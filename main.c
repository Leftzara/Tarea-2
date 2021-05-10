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
    char * tipo[6];
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
    strcpy(pa->nPokedex,nPokedex);
    for(int i=0; i<sizeof(tipo);i++){
        if(tipo[i])pa->tipo[i]=tipo[i];
    }
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
            char *nombre,*sexo,*ePrev,*ePos,*region,*id,*tipos[6],*nPok;
            int i,pc=0,ps=0,k=0;
            for(int i=0;i<10;i++){
                char *aux = get_csv_field(linea, i); 
                if(i==0)id=aux;
                if(i==1)nombre=aux;
                if(i==2){
                    //Separamos los tipos y los almacenamos en un arreglo bidimensional
                    int cont=0;
                    aux[strlen(aux)]='a';
                    for(int w=0;w<6;w++){
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
            if(sizeMap(map)>=capacity(map)*0.7)enlarge(map);
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
        printf("-----------------------------------------------------------------------\n");
        printf("***************** Introduzca los datos del Pokemon *****************\n");
        char cad[72] = "Id:Nombre:PC:PS:Pre-Evolucion:Post-Evolucion:Region:Sexo:Numero-Pokedex:";
        printf("-----------------------------------------------------------------------\n");
       char id[3];
       char nombre[50];
       //char **tipos;  
       int pc;
       int ps;
       char prEvol[50];
       char postEvol[50];
       char region[20];
       char sexo[10];
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


void buscarPorNombre(HashMap * map){
    if(map){
        printf("-----------------------------------------------------------------------\n");
        char nombre[50];
        bool flag=false;
        Pokemon * pokemon = firstMap(map);
        printf("Ingrese el nombre del pokemon a buscar: ");
        scanf("%s",&nombre);
        printf("-----------------------------------------------------------------------\n");
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
void imprimirTipos(char **pokemon, int i){
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

void MostrarXtipo(HashMap *map,HashMap *pokedex){
    PokemonAtrapado* dexaux; 
    Pokemon* paux=firstMap(map);
    int f,i,x=0;//contador
    char ndexaux[5];
    char auxtipo[10];//Auxuliar para comparar tipo
    char tablat[18][10];//tabla con los tipos existentes
    //LLenamos la tabla
    strcpy(tablat[0],"Fuego");
    strcpy(tablat[1],"Agua");
    strcpy(tablat[2],"Planta");
    strcpy(tablat[3],"Fantasma");
    strcpy(tablat[4],"Electrico");
    strcpy(tablat[5],"Psiquico");
    strcpy(tablat[6],"Normal");
    strcpy(tablat[7],"Siniestro");
    strcpy(tablat[8],"Roca");
    strcpy(tablat[9],"Lucha");
    strcpy(tablat[10],"Volador");
    strcpy(tablat[11],"Dragon");
    strcpy(tablat[12],"Hada");
    strcpy(tablat[13],"Acero");
    strcpy(tablat[14],"Bicho");
    strcpy(tablat[15],"Hielo");
    strcpy(tablat[16],"Tierra");
    strcpy(tablat[17],"Veneno");

    printf("Favor seleccione el tipo de pokemon que desea buscar\n");
    for(f=0;f<17;f++){
        printf("opcion %d: %s\n",f+1,tablat[f]);
    }
    
    scanf("%d",&f);
    if(f>18 || f<0){
    
        while(f>18 || f<0){
            printf("el valor otorgado no esta dentro de los pedidos favor ingreselo denuevo\n");
            scanf("%d",&f);
        }
    }
    switch(f){
        case 1:strcpy(auxtipo,tablat[f-1]);break;
        case 2:strcpy(auxtipo,tablat[f-1]);break;
        case 3:strcpy(auxtipo,tablat[f-1]);break;
        case 4:strcpy(auxtipo,tablat[f-1]);break;
        case 5:strcpy(auxtipo,tablat[f-1]);break;
        case 6:strcpy(auxtipo,tablat[f-1]);break;
        case 7:strcpy(auxtipo,tablat[f-1]);break;
        case 8:strcpy(auxtipo,tablat[f-1]);break;
        case 9:strcpy(auxtipo,tablat[f-1]);break;
        case 10:strcpy(auxtipo,tablat[f-1]);break;
        case 11:strcpy(auxtipo,tablat[f-1]);break;
        case 12:strcpy(auxtipo,tablat[f-1]);break;
        case 13:strcpy(auxtipo,tablat[f-1]);break;
        case 14:strcpy(auxtipo,tablat[f-1]);break;
        case 15:strcpy(auxtipo,tablat[f-1]);break;
        case 16:strcpy(auxtipo,tablat[f-1]);break;
        case 17:strcpy(auxtipo,tablat[f-1]);break;
        case 18:strcpy(auxtipo,tablat[f-1]);break;
    }
    i=sizeMap(map);
    for(f=1;f<=i;f++){
        strcpy(ndexaux,paux->nPokedex);
        dexaux=searchMap(pokedex,ndexaux);
        printf("%s, %s",dexaux->tipo[0],dexaux->tipo[1]);
        if(strcmp(dexaux->tipo[0],auxtipo) == 0 || strcmp(dexaux->tipo[1],auxtipo) == 0 ){
            x+=1;
            printf("ID: %s Nombre: %s PC: %d PS: %d Region: %s Sexo: %s\n");
        }
        paux=nextMap(map);
    }
    if(x == 0){
        printf("No se ha encontrado ningun pokemon de ese tipo.");
    }

}

void mostrarPorPokedex(HashMap * pokedex){
    List * l = firstMap(pokedex);
    printf("-----------------------------------------------------------------------\n");
    while(l){
        PokemonAtrapado * pa = firstList(l);
        printf("***************  POKEDEX ID: %s  ***************\n",pa->nPokedex);
        printf("-----------------------------------------------------------------------\n");
        while(pa){
            printf("Nombre: %s\n",pa->pokemon);
            printf("Existencia: %d\n",pa->existencia);
            printf("Tipos: ");
            for(int i=0; i<6;i++){
                if(pa->tipo[i])printf("%s  ",pa->tipo[i]);
            }
            printf("\n");
            printf("Evolucion previa: %s\n",pa->previa);
            printf("Evolucion posterior: %s\n",pa->posterior);
            printf("Region: %s\n",pa->region);
            printf("-----------------------------------------------------------------------\n");
            pa=nextList(l);
        }
        l=nextMap(pokedex);
    }

}
void buscarNombrePokedex(HashMap * map,HashMap *pokedex){
    char nombre[50];
    printf("Ingrese el nombre del pokemon a buscar:");
    scanf("%s",&nombre);
    Pokemon * pokemon = firstMap(map);
    printf("-----------------------------------------------------------------------\n");
    while(pokemon){
        if(is_equal(pokemon->nombre,nombre)){
            List * l=searchMap(pokedex,pokemon->nPokedex);
            PokemonAtrapado * pa = firstList(l);
            printf("-----------------------------------------------------------------------\n");
            while(pa){
                if(is_equal(pa->pokemon,nombre)){
                    printf("Numero de pokedex en que se encuentra: %s\n",pokemon->nPokedex);
                    printf("Existencia: %d\n",pa->existencia);
                    printf("Tipos: ");
                    for(int i=0; i<6;i++){
                        if(pa->tipo[i])printf("%s  ",pa->tipo[i]);
                    }
                    printf("\n");
                    printf("Evolucion previa: %s\n",pa->previa);
                    printf("Evolucion posterior: %s\n",pa->posterior);
                    printf("Region: %s\n",pa->region);
                    printf("-----------------------------------------------------------------------\n");
                    break;
                }
                pa=nextList(l);
            }
        }
        pokemon=nextMap(map);
    }
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
            case 4:MostrarXtipo(map,pokedex);break;
            case 5:buscarPorNombre(map);break;
            case 6:buscarNombrePokedex(map,pokedex);break;
            case 7:mostrarPorPokedex(pokedex);break;
            case 8:printf("NO IMPLEMENTADA\n");break;
            case 9:EliminarPokemon(map,pokedex);break;
            case 10:MostrarXregion(map,pokedex,REGION);break;
        }
    }
    return 0;
}