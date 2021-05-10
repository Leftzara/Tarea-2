# Tarea-2

Instrucciones para compilar
ingresar   gcc main.c list.c hashmap.c -o Tarea-2 luego executar Tarea-2

**Si el programa se cae depurar con   gcc -g  main.c list.c hashmap.c -o Tarea-2
                                    gdb --args ./Tarea-2    
                                    run

Las estructuras creadas fueron 2 mapas 
- map = Contiene todos los pokemons del storage con key su id y value la estructura pokemon
- pokedex = Contiene todas las pokedex en forma de lista con key el numero de pokedex y value la lista con los pokemons atrapados asociados a esta pokedex

Repositorio en GitHub https://github.com/Leftzara/Tarea-2

ASPECTOS TECNICOS
    * Almacenamos todos los pokemon en un mapa implementado con tabla hash, con key la id del pokemon y el valor una estructura pokemon con todos sus datos de combate
    * Creamos un mapa pokedex implementado con tabla hash tambien donde la key es el numero de la pokedex y su valor una lista con todos los pokemons pertenecientes a esa pokedex
MEJORAS:
    * La funcion mostrar todos los pokemon en la pokedex no lo hace en orden del numero de la pokedex
    * La funcion Atrapar Pokemon funciona, al momento de escribir pre o post evolucion, si se pone No tiene, haga el favor de escribir No-tiene ,La funcion hace lo suyo pero al momento de llamar a Mostrar todos los pokemon en la pokedex se cae   

INTEGRANTES:
* Nicolas Eduardo Zarate Miethe
    Aspectos positivos:
    Mejorar:Avisar dificultades para trabajar, trabajar con mayor ventana de tiempo
    Puntaje:-1

* Ignacio Benjamin Cabrera Salazar
    Aspectos positivos:Ayuda en lo mas posible a solucionar problemas y preguntas a compañeros.
    Mejorar: Poco manejo de las tablas hash y mal manejo del tiempo para realizar la tarea.
    Puntaje: 0

* Beatriz Macarena Segura Pasten
    Aspectos positivos: Trabajo con tiempo en la tarea, avanza rapido y ayuda en soluciones de errores en el codigo
    Mejorar:Adelantar trabajo con anticipacion si alguno de los integrantes no ha realizado su parte, y mejorar comunicacion con el grupo si alguno no trabaja para evitar inconvenientes de ultima hora
    Puntaje:1

* Jose Ignacio Pailamilla Montecinos
    Aspectos positivos: Trabajo con tiempo,ayuda en soluciones de codigo
    Mejorar: Trabajar mas rapido, Detectar errores rapidamente
    Puntaje:
 