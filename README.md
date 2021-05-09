# Tarea-2

Instrucciones ara compilar
ingresar   gcc main.c list.c hashmap.c -o Tarea-2 luego executar Tarea-2
si el programa se cae depurar con   gcc -g  main.c list.c hashmap.c -o Tarea-2
                                    gdb --args ./Tarea-2    
                                    run
Las estructuras creadas fueron 2 mapas 
- map = Contiene todos los pokemons del storage con key su id y value la estructura pokemon
- pokedex = Contiene todas las pokedex en forma de lista con key el numero de pokedex y value la lista con los pokemons atrapados asociados a esta pokedex