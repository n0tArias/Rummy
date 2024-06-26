#include "Functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

int randomNumber()
{
    srand(time(NULL));
    int random = rand();
    return random;
}

void colorReset()
{
    printf(BLANCO "");
}

void ClearPlayerTurn()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void PCTurn(int i)
{
#ifdef _WIN32
    Sleep(i * 1000);
    system("cls");
#else
    sleep(i * 1000);
    system("clear");
#endif
}

void createInitialDeck(struct Fichas Baraja[4][26])
{
    srand(time(NULL));
    int i;
    for (i = 0; i < 26; i++)
    {
        Baraja[0][i].numero = (i % 13) + 1;
        Baraja[0][i].color = VERDE;
        Baraja[0][i].asignada = false;
        Baraja[1][i].numero = (i % 13) + 1;
        Baraja[1][i].color = AZUL;
        Baraja[1][i].asignada = false;
        Baraja[2][i].numero = (i % 13) + 1;
        Baraja[2][i].color = NARANJA;
        Baraja[2][i].asignada = false;
        Baraja[3][i].numero = (i % 13) + 1;
        Baraja[3][i].color = ROSA;
        Baraja[3][i].asignada = false;
    }
}

void createJokers(struct Fichas Comodin[2])
{
    int i;
    for (i = 0; i < 2; i++)
    {
        Comodin[i].numero = 99;
        Comodin[i].color = BLANCO;
        Comodin[i].asignada = false;
    }
}

void inicializarCola(struct ColaJugadores *cola)
{
    cola->frente = NULL;
    cola->trasero = NULL;
}

void inicializarColaCartas(struct colaCartas *cola)
{
    cola->cabeza = NULL;
    cola->cola = NULL;
    cola->tamanio = 0;
}

void inicializarPila(struct Pila *pila)
{
    pila->top = 0; // Inicializar el top de la pila en 0
}

void inicializarTablero(struct Tablero *tablero)
{
    tablero->cabeza = NULL;
    tablero->tamanio = 0;
}

void inicializarJugada(struct Jugada *jugada)
{
    jugada->cabeza = NULL;
    jugada->tamanio = 0;
    jugada->cerrada = false;
}

void insertarJugador(struct ColaJugadores *cola, char nombre[], bool esBot)
{
    struct Jugador *nuevoJugador = (struct Jugador *)malloc(sizeof(struct Jugador));
    if (esBot)
    {
        static int numBot = 1;
        sprintf(nuevoJugador->nombre, "Bot %d", numBot++);
    }
    else
    {
        strcpy(nuevoJugador->nombre, nombre);
    }
    nuevoJugador->esBot = esBot;
    nuevoJugador->siguiente = NULL;
    nuevoJugador->numCartas = 0;
    nuevoJugador->jugadorActivo = false;
    nuevoJugador->jugadaRealizada = false;
    if (cola->frente == NULL)
    {
        cola->frente = nuevoJugador;
    }
    else
    {
        cola->trasero->siguiente = nuevoJugador;
    }
    cola->trasero = nuevoJugador;
    cola->trasero->siguiente = cola->frente;
}

void mezclarJugadores(struct ColaJugadores *cola, int totalJugadores)
{
    if (totalJugadores <= 0)
    {
        printf("Error: totalJugadores es 0 o negativo.\n");
        return; // Si no hay jugadores, no se hace nada
    }
    if (cola->frente == NULL)
    {
        printf("Error: la cola esta vacia.\n");
        return; // Si la cola esta vacia, no hay nada que mezclar
    }
    struct Jugador *jugadores[MAX_JUGADORES];
    struct Jugador *actual = cola->frente;
    // Cargar todos los jugadores en un arreglo
    for (int i = 0; i < totalJugadores; i++)
    {
        jugadores[i] = actual;
        actual = actual->siguiente;
    }
    for (int i = totalJugadores - 1; i > 0; i--)
    {
        int j = rand() % (i + 1); // indice aleatorio
        // Intercambiar las posiciones i y j
        struct Jugador *temp = jugadores[i];
        jugadores[i] = jugadores[j];
        jugadores[j] = temp;
    }
    // Reconectar la cola para mantener la circularidad
    for (int i = 0; i < totalJugadores - 1; i++)
    {
        jugadores[i]->siguiente = jugadores[i + 1];
    }
    jugadores[totalJugadores - 1]->siguiente = jugadores[0]; // Circularidad
    cola->frente = jugadores[0];                             // Nuevo frente
    cola->trasero = jugadores[totalJugadores - 1];           // Nuevo trasero
    for (int i = 0; i < totalJugadores; i++)
    {
        printf("Jugador %d: %s\n", i + 1, jugadores[i]->nombre);
    }
}

void repartirCartasYPila(struct ColaJugadores *cola, struct Fichas baraja[4][26],
                         struct Fichas comodin[2], int totalJugadores, struct Pila *pila)
{
    struct Jugador *actual = cola->frente;
    int cartaPila = 0;
    int totalCartas = 4 * 26 + 2;    // Total de cartas en la baraja, incluyendo los comodines
    int totalManos = totalJugadores; // Total de manos que se repartiran
    // Crear un arreglo temporal para barajar las cartas
    struct Fichas tempBaraja[4 * 26 + 2];
    int idx = 0, k = 0;
    tempBaraja[idx++] = comodin[0];
    tempBaraja[idx++] = comodin[1];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 26; j++)
        {
            tempBaraja[idx++] = baraja[i][j];
        }
    }
    // Agregar los comodines al arreglo temporal
    // Barajar el arreglo de cartas
    // revolver(tempBaraja, totalCartas); // comentar para depurar
    // Repartir las cartas aleatorias a cada jugador
    for (int i = 0; i < totalManos; i++)
    {
        for (int j = 0; j < 14; j++)
        { // Cada jugador recibe 14 cartas
            // Verificar si aun quedan cartas en el arreglo barajado
            if (k < totalCartas)
            {
                actual->mano[j] = tempBaraja[k++];
            }
            else
            {
                // Si no quedan mas cartas, salir del bucle
                break;
            }
            actual->numCartas++;
        }
        // Avanzar al siguiente jugador despues de repartir todas las cartas de la mano actual
        actual = actual->siguiente;
    }
    // Llenar la pila con las cartas restantes
    for (; k < totalCartas; k++)
    {
        pila->array[cartaPila++] = tempBaraja[k];
        pila->top++; // Actualizar el valor de top
    }
}
void revolver(struct Fichas *array, int n)
{
    int i;
    for (i = 0; i < n - 1; i++)
    {
        int j = i + rand() / (RAND_MAX / (n - i) + 1);
        struct Fichas temp = array[j];
        array[j] = array[i];
        array[i] = temp;
    }
}

void imprimirJugada(struct Jugada *jugadaActual)
{
    struct Nodo *fichaActual = jugadaActual->cabeza;
    // Recorre la jugada para imprimir todas las fichas
    do
    {
        if (!isJoker(fichaActual->ficha.numero))
            printf("%s%d\t", fichaActual->ficha.color, fichaActual->ficha.numero);
        else
            printf("%sJ\t", fichaActual->ficha.color, fichaActual->ficha.numero);
        fichaActual = fichaActual->siguiente;
    } while (fichaActual != jugadaActual->cabeza);
    printf("%s\n", BLANCO);
}
void imprimirTablero(struct Tablero *tablero)
{
    if (tablero->cabeza == NULL)
    {
        printf("El tablero esta vacio.\n");
        return;
    }

    struct NodoTablero *jugadaActual = tablero->cabeza;
    int jugadaNumero = 1;
    // Recorre el tablero
    printf("-- Jugadas en el tablero --\n");
    do
    {
        printf("Jugada %d:\n", jugadaNumero);
        imprimirJugada(jugadaActual->jugada);
        jugadaActual = jugadaActual->siguiente;
        jugadaNumero++;

    } while (jugadaActual != tablero->cabeza); // Mientras no vuelva al principio del tablero
}

void imprimirModificarTablero(struct Tablero *tablero)
{
    if (tablero->cabeza == NULL)
    {
        printf("El tablero esta vacio.\n");
        return;
    }

    struct NodoTablero *jugadaActual = tablero->cabeza;
    int jugadaNumero = 1;
    // Recorre el tablero
    printf("-- Jugadas en el tablero --\n");
    printf("Selecciona la jugada a modificar\n");
    do
    {
        printf("%d:", jugadaNumero);
        imprimirJugada(jugadaActual->jugada);
        jugadaActual = jugadaActual->siguiente;
        jugadaNumero++;

    } while (jugadaActual != tablero->cabeza); // Mientras no vuelva al principio del tablero
}

void imprimirManos(struct ColaJugadores *cola, int totalJugadores)
{
    struct Jugador *actual = cola->frente;

    // Imprimir las manos de los jugadores reales
    for (int i = 1; i <= totalJugadores; i++)
    {
        colorReset();
        printf("%s: \nFichas:\t", actual->nombre);
        if (actual == cola->frente)
        {
            imprimirManoActual(actual);
        }
        else
        {
            for (int j = 0; j < actual->numCartas; j++)
            {
                printf("%s*\t", GRIS);
                if (j != 0 && j % 15 == 0)
                    printf("\n");
            }
        }
        printf("\n");
        actual = actual->siguiente;
    }
}

void imprimirManoActual(struct Jugador *actual)
{
    for (int j = 0; j < actual->numCartas; j++)
    {
        if (isJoker(actual->mano[j].numero))
            printf("%sJ\t", actual->mano[j].color);
        else
            printf("%s%d\t", actual->mano[j].color, actual->mano[j].numero);
    }
}

void imprimirIndices(int numCartas)
{
    printf("%s", BLANCO);
    for (int j = 0; j < numCartas; j++)
    {
        printf("%d\t", (j + 1));
    }
}

void intercambio(struct Fichas *a, struct Fichas *b)
{
    struct Fichas temp = *a;
    *a = *b;
    *b = temp;
}

// Función para particionar el arreglo y seleccionar un pivote
int partir(struct Fichas *arr, int low, int high)
{
    struct Fichas pivot = arr[high];
    int i = low - 1;

    for (int j = low; j <= high - 1; j++)
    {
        if (arr[j].numero < pivot.numero || (arr[j].numero == pivot.numero && strcmp(arr[j].color, pivot.color) < 0))
        {
            i++;
            intercambio(&arr[i], &arr[j]);
        }
    }
    intercambio(&arr[i + 1], &arr[high]);
    return (i + 1);
}

// Función principal del Quick Sort
void quickSort(struct Fichas *arr, int low, int high)
{
    if (low < high)
    {
        int mid = partir(arr, low, high);

        // Ordenar recursivamente las dos mitades
        quickSort(arr, low, mid - 1);
        quickSort(arr, mid + 1, high);
    }
}

// Función para ordenar la mano utilizando Quick Sort
void ordenarMano(struct Fichas *fichas, int n)
{
    if (n <= 1)
    {
        return; // No hay nada que ordenar si solo hay una carta o ninguna
    }

    // Llamar al Quick Sort para ordenar la mano
    quickSort(fichas, 0, n - 1);
}

bool isJoker(int Joker)
{
    if (Joker == 99)
        return 1;
    return 0;
}

void comer(struct ColaJugadores *cola, struct Pila *pila)
{
    struct Jugador *actual = cola->frente;
    if (pila->top > 0)
    {
        pila->top--;
        actual->mano[actual->numCartas++] = pila->array[pila->top];
    }
    finTurno(cola);
}

// Funciones para el tablero

void agregarJugada(struct Tablero *tablero, struct Jugada *nuevaJugada)
{
    struct NodoTablero *nuevo_nodo = (struct NodoTablero *)malloc(sizeof(struct NodoTablero));
    nuevo_nodo->jugada = nuevaJugada;

    if (tablero->cabeza == NULL)
    {
        tablero->cabeza = nuevo_nodo;
        nuevo_nodo->siguiente = nuevo_nodo;
        nuevo_nodo->anterior = nuevo_nodo;
    }
    else
    {
        struct NodoTablero *ultimo = tablero->cabeza->anterior;
        ultimo->siguiente = nuevo_nodo;
        nuevo_nodo->anterior = ultimo;
        nuevo_nodo->siguiente = tablero->cabeza;
        tablero->cabeza->anterior = nuevo_nodo;
    }
    tablero->tamanio++;

    // Imprimir la jugada en el tablero
    printf("Jugada %d agregada:\n", tablero->tamanio);
    struct Nodo *actual = nuevo_nodo->jugada->cabeza;
    do
    {
        if (!isJoker(actual->ficha.numero))
            printf("%s%d ", actual->ficha.color, actual->ficha.numero);
        else
            printf("%sJ ", actual->ficha.color);
        actual = actual->siguiente;
    } while (actual != nuevaJugada->cabeza);
    printf("%s\n", BLANCO); // Imprimir el color de la ficha de la jugada
}

// Agregar fichas a juego existente
void agregarFichaPorDerecha(struct Jugada *jugada, struct Fichas valor)
{
    struct Nodo *nuevo_nodo = (struct Nodo *)malloc(sizeof(struct Nodo));
    nuevo_nodo->ficha = valor;
    if (jugada->cabeza == NULL)
    {
        jugada->cabeza = nuevo_nodo;
        jugada->cabeza->siguiente = nuevo_nodo;
        jugada->cabeza->anterior = nuevo_nodo;
    }
    else
    {
        struct Nodo *cola = jugada->cabeza->anterior;
        cola->siguiente = nuevo_nodo;
        nuevo_nodo->anterior = cola;
        nuevo_nodo->siguiente = jugada->cabeza;
        jugada->cabeza->anterior = nuevo_nodo;
    }

    jugada->tamanio++;
}

void agregarFichaPorIzquierda(struct Jugada *jugada, struct Fichas valor)
{
    struct Nodo *nuevo_nodo = (struct Nodo *)malloc(sizeof(struct Nodo));
    nuevo_nodo->ficha = valor;

    if (jugada->cabeza == NULL)
    {
        jugada->cabeza = nuevo_nodo;
        jugada->cabeza->siguiente = nuevo_nodo;
        jugada->cabeza->anterior = nuevo_nodo;
    }
    else
    {
        struct Nodo *cola = jugada->cabeza->anterior;
        nuevo_nodo->siguiente = jugada->cabeza;
        nuevo_nodo->anterior = cola;
        cola->siguiente = nuevo_nodo;
        jugada->cabeza->anterior = nuevo_nodo;
        jugada->cabeza = nuevo_nodo;
    }
    jugada->tamanio++;
}
// Función para agregar ficha en un indice
void agregarFicha(struct Jugada *jugada, struct Fichas valor, int indice)
{
    struct Nodo *nuevo_nodo = (struct Nodo *)malloc(sizeof(struct Nodo));
    nuevo_nodo->ficha = valor;

    if (jugada->cabeza == NULL)
    {
        jugada->cabeza = nuevo_nodo;
        jugada->cabeza->siguiente = nuevo_nodo;
        jugada->cabeza->anterior = nuevo_nodo;
    }
    else
    {
        struct Nodo *actual = jugada->cabeza;
        for (int i = 0; i < indice && actual->siguiente != jugada->cabeza; i++)
        {
            actual = actual->siguiente;
        }
        struct Nodo *anterior = actual->anterior;

        nuevo_nodo->siguiente = actual;
        nuevo_nodo->anterior = anterior;
        anterior->siguiente = nuevo_nodo;
        actual->anterior = nuevo_nodo;

        if (indice == 0)
        {
            jugada->cabeza = nuevo_nodo;
        }
    }

    jugada->tamanio++;
}

// Robar fichas a juego existente
struct Fichas robarPorIzquierda(struct Jugada *jugada)
{
    if (jugada->cabeza == NULL)
    {
        printf("Lista vacia, no se puede robar por inicio.\n");
        return (struct Fichas){-1, "N/A", false}; // Indicador de error
    }

    struct Nodo *robar_nodo = jugada->cabeza;
    struct Fichas resultado = robar_nodo->ficha;

    if (jugada->tamanio == 1)
    {
        jugada->cabeza = NULL;
    }
    else
    {
        jugada->cabeza = robar_nodo->siguiente;
        jugada->cabeza->anterior = robar_nodo->anterior;
        robar_nodo->anterior->siguiente = jugada->cabeza;
    }

    free(robar_nodo);
    jugada->tamanio--;

    return resultado;
}

struct Fichas robarPorDerecha(struct Jugada *jugada)
{
    if (jugada->cabeza == NULL)
    {
        printf("Lista vacia, no se puede robar por final.\n");
        return (struct Fichas){-1, "N/A", false}; // Indicador de error
    }

    struct Nodo *ultimo = jugada->cabeza->anterior;
    struct Fichas resultado = ultimo->ficha;

    if (jugada->tamanio == 1)
    {
        jugada->cabeza = NULL;
    }
    else
    {
        ultimo->anterior->siguiente = jugada->cabeza;
        jugada->cabeza->anterior = ultimo->anterior;
    }

    free(ultimo);
    jugada->tamanio--;

    return resultado;
}
// Función para robar ficha en un índice
struct Fichas robarFicha(struct Jugada *jugada, int indice)
{
    if (jugada->cabeza == NULL)
    {
        printf("Lista vacía, no se puede robar ninguna ficha.\n");
        return (struct Fichas){-1, "N / A", -1}; // Indicador de error
    }

    struct Nodo *actual = jugada->cabeza;
    if (indice == 0)
        robarPorIzquierda(jugada);
    else if (indice == jugada->tamanio - 1)
        robarPorDerecha(jugada);

    for (int i = 0; i < indice && actual->siguiente != jugada->cabeza; i++)
    {
        actual = actual->siguiente;
    }
    struct Fichas resultado = actual->ficha;

    if (jugada->tamanio == 1)
    {
        jugada->cabeza = NULL;
    }
    else
    {
        actual->anterior->siguiente = actual->siguiente;
        actual->siguiente->anterior = actual->anterior;
        if (actual == jugada->cabeza)
        {
            jugada->cabeza = actual->siguiente;
        }
    }

    free(actual);
    jugada->tamanio--;

    return resultado;
}

void imprimirIndicesJugadas(int ListSize)
{
    printf("%s0", BLANCO);
    for (int j = 0; j < ListSize - 1; j++)
    {
        printf("   ");
    }
    printf("1\n");
}

// Funciones para el jugador
// Funciones jugadorActivo = 0 (Jugador no ha sumado 25 puntos)
int jugadaInicial(struct Tablero *tablero, struct ColaJugadores *cola, struct Pila *pila)
{
    int indices[MAX_COLS];
    for (int i = 0; i < MAX_COLS; i++)
        indices[i] = -1;
    struct Jugada *nuevaJugada = (struct Jugada *)malloc(sizeof(struct Jugada));
    inicializarJugada(nuevaJugada);
    struct Jugador *actual = cola->frente;
    int k = 0;
    for (int i = 0; i < MAX_ROWS; i++)
    {
        indices[i] = -1;
    }
    int sumaCartas = 0, finJugada = 0, i = 0;
    while (sumaCartas < 25)
    {
        ClearPlayerTurn();
        sumaCartas = iniciarJugada(actual, indices); // Calcular la suma de la jugada actual
        printf("La suma de tus fichas en juego es de: %d\n", sumaCartas);

        if (sumaCartas < 25)
        {
            printf("Suma insuficiente. ¿Deseas salir?\n");
            printf("1....No\n");
            printf("2....Si\n");
            scanf("%d", &finJugada);
            if (finJugada == 2)
            {
                comer(cola, pila);
                return 0;
            }
        }
    }

    // Procesar las jugadas
    for (int j = 0; indices[j] != -1; j++)
    {
        agregarFichaPorDerecha(nuevaJugada, actual->mano[indices[j]]);
        k++;
    }
    printf("Jugada correctamente agregada\n");
    // Eliminar las fichas utilizadas

    if (k == 4 && sumaCartas % 4)
        nuevaJugada->cerrada = true;
    if (k == 13)
        nuevaJugada->cerrada = true;
    while (k > 0)
    {
        int pos = indices[--k];
        for (int h = pos; h < actual->numCartas - 1; h++)
        {
            actual->mano[h] = actual->mano[h + 1];
        }
        actual->numCartas--;
    }
    PCTurn(2);
    agregarJugada(tablero, nuevaJugada);
    printf("Mano de %s actualizada:\n", actual->nombre);
    imprimirManoActual(actual);
    printf("\n");
    colorReset();
    PCTurn(2);
    actual->jugadorActivo = 1; // El jugador ya hizo su jugada inicial
    finTurno(cola);
    return 1;
}

bool detectarRepetidos(int indices[], int arrSize)
{
    // Creamos un arreglo de booleanos para marcar qué números hemos visto
    bool vistos[100] = {false}; // Suponiendo que los números están en el rango de 0 a 99

    // Iteramos sobre el arreglo
    for (int i = 0; i < arrSize; i++)
    {
        // Verificamos si el número actual ya ha sido visto
        if (vistos[indices[i]])
        {
            // Si ya lo hemos visto, hay un número repetido
            return true;
        }
        else
        {
            // Si no lo hemos visto, lo marcamos como visto
            vistos[indices[i]] = true;
        }
    }

    // Si llegamos hasta aquí, no hay números repetidos
    return false;
}

int iniciarJugada(struct Jugador *actual, int *arrIndices)
{
    int finJugada = 0, indiceSeleccionado = 0, arrSize = 0, sumaCartas = 0, jugadaValida = 0;
    struct Fichas posibleJugada[MAX_COLS]; // Solo se usa para verificar jugada

    while (finJugada != 2 && arrSize < MAX_COLS)
    {
        printf("Selecciona los indices de las cartas a jugar\n");
        imprimirManoActual(actual);
        printf("\n");
        imprimirIndices(actual->numCartas);
        printf("\n");

        do
        {
            if (scanf("%d", &indiceSeleccionado) != 1)
            {
                printf("Error en la entrada. Intenta de nuevo.\n");
                // Limpiar el buffer de entrada para evitar problemas con scanf
                while (getchar() != '\n')
                    ;
                continue;
            }
            if (indiceSeleccionado < 1 || indiceSeleccionado > actual->numCartas)
            {
                printf("Dato invalido, ingresa otro indice entre 1 y %d\n", actual->numCartas);
            }
        } while (indiceSeleccionado < 1 || indiceSeleccionado > actual->numCartas);

        arrIndices[arrSize] = indiceSeleccionado - 1;
        arrSize++;
        if (detectarRepetidos(arrIndices, arrSize))
        {
            printf("Indice repetido\n");
            arrSize--;
        }
        do
        {
            printf("Deseas agregar otro indice?\n");
            printf("1...Si\n");
            printf("2...No\n");
            if (scanf("%d", &finJugada) != 1 || (finJugada < 1 || finJugada > 2))
            {
                printf("Opcion no valida. Intenta de nuevo.\n");
                // Limpiar el buffer para evitar problemas con scanf
                while (getchar() != '\n')
                    ;
                finJugada = 0; // Para forzar a seguir en el bucle
            }
        } while (finJugada < 1 || finJugada > 2);

        if (finJugada == 2 && arrSize < 3)
        {
            printf("Faltan fichas para crear un juego valido\n");
            printf("¿Estas seguro de querer salir?\n");
            printf("1...No\n");
            printf("2...Si\n");
            if (scanf("%d", &finJugada) != 1 || (finJugada < 1 || finJugada > 2))
            {
                printf("Opcion no valida. Intenta de nuevo.\n");
                // Limpiar el buffer de entrada para evitar problemas
                while (getchar() != '\n')
                    ;
                finJugada = 0; // Para reiniciar el bucle
            }
        }

        if (finJugada == 2 && arrIndices[0] == -1)
        {
            return 0;
        }
    }
    for (int k = 0; k < arrSize; k++)
    {
        posibleJugada[k] = actual->mano[arrIndices[k]];
        if (!isJoker(posibleJugada[k].numero))
            printf("%s%d \t", posibleJugada[k].color, posibleJugada[k].numero);
        else
            printf("%sJ \t", posibleJugada[k].color);
    }
    printf(BLANCO "\n");
    int tipoJugada = revisarAgregarJugada(posibleJugada, arrSize, actual->esBot);
    int joker = -1, indiceTemporal = -1, secTempIndex = -1, valorComodin;
    if (tipoJugada == 0)
    {
        return 0; // Jugada invalida
    }
    for (int k = 0; k < arrSize; k++)
    {
        if (!isJoker(actual->mano[arrIndices[k]].numero))
        {
            sumaCartas += actual->mano[arrIndices[k]].numero;
            indiceTemporal = arrIndices[k];
            joker = -1;
        }
        else
        {
            if (tipoJugada == 1)
            { // corrida
                if (++joker == 0)
                {
                    valorComodin = obtenerValorComodin(actual->mano, arrIndices, arrSize, joker, indiceTemporal, secTempIndex);
                    sumaCartas += valorComodin;
                    secTempIndex = indiceTemporal;
                }
                else
                    sumaCartas += (valorComodin + 1);
            }
            else if (tipoJugada == 2)
            {
                if (k == 0)
                    sumaCartas += actual->mano[arrIndices[k + 1]].numero;
                indiceTemporal = arrIndices[k + 1];
            }
            else
            {
                sumaCartas += actual->mano[arrIndices[k + 2]].numero;
                indiceTemporal = arrIndices[k + 2];
            }
        }
    }

    return sumaCartas;
}

int obtenerValorComodin(struct Fichas *mano, int *arrIndices, int arrSize, int k, int tempIndex, int secTempIndex)
{
    // Buscar el valor mas alto y mas bajo de las cartas seleccionadas
    int CorridaCompleta = 0, j = -1;
    int dobleComodin = CorridaCompleta, jokerCount = 0;
    for (int i = 0; i < arrSize; i++)
    {
        if (isJoker(mano[arrIndices[0]].numero))
            break;

        if (isJoker(mano[arrIndices[i]].numero))
            continue;

        if (isJoker(mano[arrIndices[i + 1]].numero) && secTempIndex == -1)
        {
            j = i;
            break;
        }

        if (isJoker(mano[arrIndices[i + 1]].numero) && secTempIndex != -1)
        {
            secTempIndex = -1;
            continue;
        }
    }
    if (j != -1)
    {
        int valor = mano[arrIndices[j]].numero + 1;
        return valor;
    }
    if (isJoker(mano[arrIndices[0]].numero))
    {
        if (isJoker(mano[arrIndices[1]].numero))
            return mano[arrIndices[2]].numero - 2;
        else
            return mano[arrIndices[2]].numero - 1;
    }
    return -1;
}
int revisarAgregarJugada(struct Fichas fichas[MAX_COLS], int arrSize, bool esBot)
{
    if (arrSize < 3)
        return 0;
    struct Fichas manoTemporal[MAX_COLS];
    for (int i = 0; i < arrSize - 1; i++)
        manoTemporal[i] = fichas[i];
    ordenarMano(manoTemporal, arrSize);
    // Verifica "OAK" (Tres iguales)
    int OAK = 0; // Contador de cartas OAK
    if (arrSize >= 3 && arrSize <= 4)
    {
        for (int i = 0; i < arrSize - 1; i++)
        {

            if ((manoTemporal[i + 1].numero == manoTemporal[i].numero &&
                 strcmp(fichas[i + 1].color, fichas[i].color) != 0) ||
                (fichas[i + 1].numero == 99 && i == arrSize - 2))
            {
                OAK++;
            }
            else
            {
                OAK = 0; // Reiniciar contador si no hay una secuencia continua
            }
        }
    }

    // Verifica corridas
    int corrida = 0;
    int comodines = 0;
    for (int i = 0; i < arrSize - 1; i++)
    {
        if (fichas[i].numero == 99)
        {
            comodines++;
            corrida++;
        }
        else if (fichas[i + 1].numero == 99 &&
                 (fichas[i + 2].numero == (fichas[i].numero + 2) && strcmp(fichas[i + 2].color, fichas[i].color) == 0))
        {
            comodines++;
            corrida++;
        }
        else if (fichas[i + 1].numero == (fichas[i].numero + 1) && strcmp(fichas[i + 1].color, fichas[i].color) == 0)
        {
            corrida++;
        }
        else if (comodines > 0 && fichas[i + 1].numero == (fichas[i].numero + 1 + comodines) && strcmp(fichas[i + 1].color, fichas[i].color) == 0)
        {
            corrida++;
            comodines = 0; // Reset comodines después de usar
        }
        else if (fichas[i + 1].numero == 99 && i == arrSize - 2)
        {
            comodines++;
            corrida++;
        }
        else if (fichas[i + 2].numero == 99 && i == arrSize - 3 && fichas[i + 1].numero == fichas[i + 2].numero)
        {
            comodines++;
            corrida++;
        }
        else
        {
            corrida = 0; // Reset si la secuencia se rompe
            comodines = 0;
        }
    }

    if (OAK == arrSize - 1)
    {
        return 2; // OAK
    }

    if (corrida == arrSize - 1)
    {
        return 1; // Corrida
    }

    // Si no se encontraron corridas ni OAK, la jugada es invalida
    if (!esBot)
    {
        printf("La jugada que ingresaste no es valida\n");
    }
    return 0; // Jugada invalida
}

void jugadaNormal(struct Tablero *tablero, struct ColaJugadores *cola)
{
    int indices[MAX_COLS];
    for (int i = 0; i < MAX_COLS; i++)
        indices[i] = -1;
    struct Jugada *nuevaJugada = (struct Jugada *)malloc(sizeof(struct Jugada));
    inicializarJugada(nuevaJugada);
    struct Jugador *actual = cola->frente;
    int k = 0;
    for (int i = 0; i < MAX_ROWS; i++)
    {
        indices[i] = -1;
    }
    int sumaCartas = 0, finJugada = 0, i = 0;
    while (sumaCartas < 1)
    {
        ClearPlayerTurn();
        sumaCartas = iniciarJugadaNormal(actual, indices); // Calcular la suma de la jugada actual

        if (sumaCartas < 1)
        {
            printf("El juego no es valido. ¿Deseas salir?\n");
            printf("1....No\n");
            printf("2....Si\n");
            scanf("%d", &finJugada);
            if (finJugada == 2)
            {
                return;
            }
        }
    }

    // Procesar las jugadas
    for (int j = 0; indices[j] != -1; j++)
    {
        agregarFichaPorDerecha(nuevaJugada, actual->mano[indices[j]]);
        k++;
    }
    if (k == 4 && sumaCartas % 4)
        nuevaJugada->cerrada = true;
    if (k == 13)
        nuevaJugada->cerrada = true;

    // Eliminar las fichas utilizadas
    while (k > 0)
    {
        int pos = indices[--k];
        for (int h = pos; h < actual->numCartas - 1; h++)
        {
            actual->mano[h] = actual->mano[h + 1];
        }
        actual->numCartas--;
    }

    PCTurn(2);
    printf("Jugada agregada\n");
    agregarJugada(tablero, nuevaJugada);
    printf("Mano de %s actualizada:\n", actual->nombre);
    imprimirManoActual(actual);
    agregarJugada(tablero, nuevaJugada);
    imprimirJugada(nuevaJugada);
    PCTurn(2);
    actual->jugadaRealizada = 1;
    finTurno(cola);
    return;
}

int iniciarJugadaNormal(struct Jugador *actual, int *arrIndices)
{
    int finJugada = 0, indiceSeleccionado = 0, arrSize = 0, sumaCartas = 0, jugadaValida = 0;
    struct Fichas posibleJugada[MAX_COLS]; // Solo se usa para verificar jugada

    while (finJugada != 2 && arrSize < MAX_COLS)
    {
        printf("Selecciona los indices de las cartas a jugar\n");
        imprimirManoActual(actual);
        printf("\n");
        imprimirIndices(actual->numCartas);
        printf("\n");

        do
        {
            if (scanf("%d", &indiceSeleccionado) != 1)
            {
                printf("Error en la entrada. Intenta de nuevo.\n");
                // Limpiar el buffer de entrada para evitar problemas con scanf
                while (getchar() != '\n')
                    ;
                continue;
            }
            if (indiceSeleccionado < 1 || indiceSeleccionado > actual->numCartas)
            {
                printf("Dato invalido, ingresa otro indice entre 1 y %d\n", actual->numCartas);
            }
        } while (indiceSeleccionado < 1 || indiceSeleccionado > actual->numCartas);

        arrIndices[arrSize] = indiceSeleccionado - 1;
        arrSize++;
        if (detectarRepetidos(arrIndices, arrSize))
        {
            printf("Indice repetido\n");
            arrSize--;
        }
        do
        {
            printf("Deseas agregar otro indice?\n");
            printf("1...Si\n");
            printf("2...No\n");
            if (scanf("%d", &finJugada) != 1 || (finJugada < 1 || finJugada > 2))
            {
                printf("Opcion no valida. Intenta de nuevo.\n");
                // Limpiar el buffer para evitar problemas con scanf
                while (getchar() != '\n')
                    ;
                finJugada = 0; // Para forzar a seguir en el bucle
            }
        } while (finJugada < 1 || finJugada > 2);

        if (finJugada == 2 && arrSize < 3)
        {
            printf("Faltan fichas para crear un juego valido\n");
            printf("¿Estas seguro de querer salir?\n");
            printf("1...No\n");
            printf("2...Si\n");
            if (scanf("%d", &finJugada) != 1 || (finJugada < 1 || finJugada > 2))
            {
                printf("Opcion no valida. Intenta de nuevo.\n");
                // Limpiar el buffer de entrada para evitar problemas
                while (getchar() != '\n')
                    ;
                finJugada = 0; // Para reiniciar el bucle
            }
        }

        if (finJugada == 2 && arrIndices[0] == -1)
        {
            return 0;
        }
    }
    for (int k = 0; k < arrSize; k++)
    {
        posibleJugada[k] = actual->mano[arrIndices[k]];
        if (!isJoker(posibleJugada[k].numero))
            printf("%s%d \t", posibleJugada[k].color, posibleJugada[k].numero);
        else
            printf("%sJ \t", posibleJugada[k].color);
    }
    printf(BLANCO "\n");
    int tipoJugada = revisarAgregarJugada(posibleJugada, arrSize, actual->esBot);

    if (tipoJugada == 0)
    {
        return 0; // Jugada invalida
    }
    int joker = -1, indiceTemporal = -1, secTempIndex = -1, valorComodin = -1;
    for (int k = 0; k < arrSize; k++)
    {
        if (!isJoker(actual->mano[arrIndices[k]].numero))
        {
            sumaCartas += actual->mano[arrIndices[k]].numero;
            indiceTemporal = arrIndices[k];
            joker = -1;
        }
        else
        {
            if (tipoJugada == 1)
            { // corrida
                if (++joker == 0)
                {
                    valorComodin = obtenerValorComodin(actual->mano, arrIndices, arrSize, joker, indiceTemporal, secTempIndex);
                    sumaCartas += valorComodin;
                    secTempIndex = indiceTemporal;
                }
                else
                    sumaCartas += (valorComodin + 1);
            }
            else if (tipoJugada == 2)
            {
                if (k == 0)
                    sumaCartas += actual->mano[arrIndices[k + 1]].numero;
                indiceTemporal = arrIndices[k + 1];
            }
            else
            {
                sumaCartas += actual->mano[arrIndices[k + 2]].numero;
                indiceTemporal = arrIndices[k + 2];
            }
        }
    }

    return sumaCartas;
}

void liberarJugada(struct Jugada *jugada)
{
    struct Nodo *actual = jugada->cabeza;
    struct Nodo *temp;

    while (actual != NULL)
    {
        temp = actual;
        actual = actual->siguiente;
        free(temp);
        if (actual == jugada->cabeza)
        {
            break;
        }
    }

    jugada->cabeza = NULL;
    jugada->tamanio = 0;
    jugada->cerrada = false;
}

// Función para copiar una jugada de origen a destino
void copiarJugada(struct Jugada *jugadaOrigen, struct Jugada *jugadaDestino)
{
    // Liberar la memoria asignada previamente en la jugada destino
    liberarJugada(jugadaDestino);

    if (jugadaOrigen->cabeza == NULL)
    {
        jugadaDestino->cabeza = NULL;
        jugadaDestino->tamanio = 0;
        jugadaDestino->cerrada = false;
        return;
    }

    // Crear la primera ficha de la jugada destino
    struct Nodo *actualOrigen = jugadaOrigen->cabeza;
    struct Nodo *nuevoNodo = (struct Nodo *)malloc(sizeof(struct Nodo));
    nuevoNodo->ficha = actualOrigen->ficha;
    nuevoNodo->siguiente = nuevoNodo;
    nuevoNodo->anterior = nuevoNodo;

    jugadaDestino->cabeza = nuevoNodo;
    jugadaDestino->tamanio = jugadaOrigen->tamanio;
    jugadaDestino->cerrada = jugadaOrigen->cerrada;

    struct Nodo *actualDestino = nuevoNodo;
    actualOrigen = actualOrigen->siguiente;

    // Copiar el resto de las fichas de la jugada origen
    while (actualOrigen != jugadaOrigen->cabeza)
    {
        nuevoNodo = (struct Nodo *)malloc(sizeof(struct Nodo));
        nuevoNodo->ficha = actualOrigen->ficha;

        nuevoNodo->siguiente = jugadaDestino->cabeza;
        nuevoNodo->anterior = actualDestino;
        actualDestino->siguiente = nuevoNodo;
        jugadaDestino->cabeza->anterior = nuevoNodo;

        actualDestino = nuevoNodo;
        actualOrigen = actualOrigen->siguiente;
    }
}

void ordenarJugadaCompleta(struct Jugada *jugada)
{
    if (jugada == NULL || jugada->cabeza == NULL)
    {
        return;
    }

    struct Nodo *actual = jugada->cabeza->siguiente;
    while (actual != jugada->cabeza)
    {
        struct Nodo *temp = actual->anterior;
        while (temp != jugada->cabeza->anterior && temp->ficha.numero > actual->ficha.numero)
        {
            temp = temp->anterior;
        }
        if (temp != actual->anterior)
        {
            struct Nodo *anterior = actual->anterior;
            struct Nodo *siguiente = actual->siguiente;
            anterior->siguiente = siguiente;
            siguiente->anterior = anterior;
            if (temp == jugada->cabeza->anterior && temp->ficha.numero > actual->ficha.numero)
            {
                temp = temp->anterior;
            }
            actual->siguiente = temp->siguiente;
            actual->anterior = temp;
            temp->siguiente->anterior = actual;
            temp->siguiente = actual;
        }
        actual = actual->siguiente;
    }
}

int revisarJugadaExistente(struct Jugada *jugada, bool esBot)
{
    int arrSize = jugada->tamanio;
    printf("%d\n", arrSize);
    if (arrSize < 3 || arrSize > 13)
        return 0;
    struct Jugada jugadaTemporal;
    copiarJugada(jugada, &jugadaTemporal);
    struct Nodo *actual = jugadaTemporal.cabeza;
    ordenarJugadaCompleta(&jugadaTemporal);
    struct Fichas fichas[MAX_COLS];
    for (int i = 0; i < arrSize; i++)
    {
        fichas[i] = actual->ficha;
        actual = actual->siguiente;
    }
    // Verifica "OAK" (Tres iguales)
    int OAK = 0; // Contador de cartas OAK
    if (arrSize >= 3 && arrSize <= 4)
    {
        for (int i = 0; i < arrSize - 1; i++)
        {

            if ((fichas[i + 1].numero == fichas[i].numero &&
                 strcmp(fichas[i + 1].color, fichas[i].color) != 0) ||
                (fichas[i + 1].numero == 99))
            {
                OAK++;
            }
            else
            {
                OAK = 0; // Reiniciar contador si no hay una secuencia continua
            }
        }
    }
    actual = jugada->cabeza;
    for (int i = 0; i < arrSize; i++)
    {
        fichas[i] = actual->ficha;
        actual = actual->siguiente;
    }
    // Verifica corridas
    int corrida = 0;
    int comodines = 0;
    for (int i = 0; i < arrSize - 1; i++)
    {
        if (fichas[i].numero == 99)
        {
            comodines++;
            corrida++;
        }
        else if (fichas[i + 1].numero == 99 &&
                 (fichas[i + 2].numero == (fichas[i].numero + 2) && strcmp(fichas[i + 2].color, fichas[i].color) == 0))
        {
            comodines++;
            corrida++;
        }
        else if (fichas[i + 1].numero == (fichas[i].numero + 1) && strcmp(fichas[i + 1].color, fichas[i].color) == 0)
        {
            corrida++;
        }
        else if (comodines > 0 && fichas[i + 1].numero == (fichas[i].numero + 1 + comodines) && strcmp(fichas[i + 1].color, fichas[i].color) == 0)
        {
            corrida++;
            comodines = 0; // Reset comodines después de usar
        }
        else if (fichas[i + 1].numero == 99 && i == arrSize - 2)
        {
            comodines++;
            corrida++;
        }
        else if (fichas[i + 2].numero == 99 && i == arrSize - 3 && fichas[i + 1].numero == fichas[i + 2].numero)
        {
            comodines++;
            corrida++;
        }
        else
        {
            corrida = 0; // Reset si la secuencia se rompe
            comodines = 0;
        }
    }

    if (OAK == arrSize - 1)
    {
        return 2; // OAK
    }

    if (corrida == arrSize - 1)
    {
        return 1; // Corrida
    }

    // Si no se encontraron corridas ni OAK, la jugada es invalida
    printf("La jugada que ingresaste no es valida\n");
    return 0; // Jugada invalida
}

void agregarFichaAJugadaExistente(struct Tablero *tablero, struct ColaJugadores *cola)
{
    int indiceJugada, opcion, j = 0, jugadaValida = 0;
    imprimirModificarTablero(tablero);
    struct Jugador *actual = cola->frente;
    struct NodoTablero *jugadaActual = tablero->cabeza;
    printf("Ingrese el indice de la jugada a modificar: ");
    scanf("%d", &indiceJugada);

    // Buscar la jugada correspondiente
    for (int i = 0; i < indiceJugada - 1; i++)
    {
        jugadaActual = jugadaActual->siguiente;
    }

    // Verificar si la jugada esta completa
    if (jugadaActual->jugada->cerrada)
    {
        printf("La jugada ya esta completa.\n");
        return;
    }

    struct NodoTablero jugadaTemporal = *jugadaActual;

    // Mostrar la jugada a modificar
    printf("Jugada a cambiar:\n");
    imprimirJugada(jugadaTemporal.jugada);
    colorReset();
    imprimirIndicesJugadas(jugadaTemporal.jugada->tamanio);

    // Seleccionar la opcion de agregar ficha por izquierda o derecha
    printf("0...Agregar ficha por izquierda\n");
    printf("1...Agregar ficha por derecha\n");
    scanf("%d", &opcion);

    printf("Elige la carta a jugar: \n");
    imprimirManoActual(cola->frente);
    printf("\n");
    imprimirIndices(cola->frente->numCartas);
    printf("\n");
    do
    {
        scanf("%d", &indiceJugada);
        if (indiceJugada > cola->frente->numCartas || indiceJugada < 1)
            printf("Indice invalido, intentalo de nuevo");
    } while (indiceJugada > cola->frente->numCartas || indiceJugada < 1);
    // Agregar ficha por izquierda
    if (opcion == 0)
    {
        agregarFichaPorIzquierda(jugadaTemporal.jugada, cola->frente->mano[--indiceJugada]);
    }
    else if (opcion == 1)
    {
        agregarFichaPorDerecha(jugadaTemporal.jugada, cola->frente->mano[--indiceJugada]);
    }
    else
    {
        printf("Opcion invalida.\n");
        return;
    }
    struct Fichas fichaTemp;
    imprimirJugada(jugadaTemporal.jugada);
    jugadaValida = revisarJugadaExistente(jugadaTemporal.jugada, actual->esBot);
    if (jugadaValida == 0)
    {
        printf("No es una jugada valida\nDeshaciendo jugada...\n");
        if (opcion == 0)
            fichaTemp = robarPorIzquierda(jugadaTemporal.jugada);
        if (opcion == 1)
            fichaTemp = robarPorDerecha(jugadaTemporal.jugada);
        PCTurn(2);
        return;
    }
    else
        *jugadaActual = jugadaTemporal;

    if (jugadaValida == 1 && jugadaActual->jugada->tamanio == 13)
        jugadaActual->jugada->cerrada = true;
    if (jugadaValida == 2 && jugadaActual->jugada->tamanio == 4)
        jugadaActual->jugada->cerrada = true;
    // Actualizar la mano del jugador y eliminar la carta utilizada
    for (int i = indiceJugada; i < cola->frente->numCartas - 1; i++)
    {
        cola->frente->mano[i] = cola->frente->mano[i + 1];
    }
    ClearPlayerTurn();
    cola->frente->numCartas--;
    cola->frente->jugadaRealizada = 1;
}
void agregarFichaAJugadaIncompleta(struct NodoTablero *jugada, struct ColaJugadores *cola)
{
    int indiceJugada, opcion, j = 0, jugadaValida = 0;
    struct Jugador *actual = cola->frente;
    // Buscar la jugada correspondiente
    // Verificar si la jugada esta completa
    struct NodoTablero jugadaTemporal = *jugada;

    // Mostrar la jugada a modificar
    printf("Jugada a cambiar:\n");
    imprimirJugada(jugadaTemporal.jugada);
    colorReset();
    imprimirIndicesJugadas(jugadaTemporal.jugada->tamanio);

    // Seleccionar la opcion de agregar ficha por izquierda o derecha
    printf("0...Agregar ficha por izquierda\n");
    printf("1...Agregar ficha por derecha\n");
    scanf("%d", &opcion);

    printf("Elige la carta a jugar: \n");
    imprimirManoActual(cola->frente);
    printf("\n");
    imprimirIndices(cola->frente->numCartas);
    printf("\n");
    do
    {
        scanf("%d", &indiceJugada);
        if (indiceJugada > cola->frente->numCartas || indiceJugada < 1)
            printf("Indice invalido, intentalo de nuevo");
    } while (indiceJugada > cola->frente->numCartas || indiceJugada < 1);
    // Agregar ficha por izquierda
    if (opcion == 0)
    {
        agregarFichaPorIzquierda(jugadaTemporal.jugada, cola->frente->mano[--indiceJugada]);
    }
    else if (opcion == 1)
    {
        agregarFichaPorDerecha(jugadaTemporal.jugada, cola->frente->mano[--indiceJugada]);
    }
    else
    {
        printf("Opcion invalida.\n");
        return;
    }

    imprimirJugada(jugadaTemporal.jugada);
    jugadaValida = revisarJugadaExistente(jugadaTemporal.jugada, actual->esBot);
    if (jugadaValida == 0)
    {
        return;
    }
    else
        *jugada = jugadaTemporal;

    if (jugadaValida == 1 && jugada->jugada->tamanio == 13)
        jugada->jugada->cerrada = true;
    if (jugadaValida == 2 && jugada->jugada->tamanio == 4)
        jugada->jugada->cerrada = true;
    // Actualizar la mano del jugador y eliminar la carta utilizada
    for (int i = indiceJugada; i < cola->frente->numCartas - 1; i++)
    {
        cola->frente->mano[i] = cola->frente->mano[i + 1];
    }
    cola->frente->numCartas--;
}

void robarFichaAJugadaExistente(struct Tablero *tablero, struct ColaJugadores *cola)
{
    int indiceJugada, opcion, jugadaValida = 0;
    imprimirModificarTablero(tablero);
    struct NodoTablero *jugadaActual = tablero->cabeza;
    struct Jugador *actual = cola->frente;
    printf("Ingrese el indice de la jugada a modificar: ");
    scanf("%d", &indiceJugada);
    // Buscar la jugada correspondiente
    for (int i = 0; i < indiceJugada - 1; i++)
    {
        jugadaActual = jugadaActual->siguiente;
    }
    struct NodoTablero jugadaTemporal = *jugadaActual;
    jugadaValida = revisarJugadaExistente(jugadaActual->jugada, actual->esBot);
    printf("Jugada a cambiar:\n");
    imprimirJugada(jugadaActual->jugada);
    colorReset();
    imprimirIndices(jugadaActual->jugada->tamanio);
    printf("\n");
    scanf("%d", &opcion);
    struct Fichas fichaTemp;
    // Robar ficha por izquierda
    if ((opcion > 0 && opcion < actual->numCartas))
    {

        fichaTemp = robarFicha(jugadaTemporal.jugada, --opcion);
        if (isJoker(fichaTemp.numero))
        {
            printf("Intenta partir la jugada\n");
            agregarFicha(jugadaTemporal.jugada, fichaTemp, opcion);
            return;
        }
        if (jugadaTemporal.jugada->tamanio == 2)
        {
            printf("Deja una carta de tu mano para obtener la carta seleccionada.\n");
            agregarFichaAJugadaIncompleta(&jugadaTemporal, cola);
            if (jugadaTemporal.jugada->tamanio == 2)
            {
                agregarFicha(jugadaTemporal.jugada, fichaTemp, opcion);
                printf("No es una jugada valida.\n");
                return;
            }
        }
        *jugadaActual = jugadaTemporal;
        cola->frente->mano[cola->frente->numCartas++] = fichaTemp;
        return;
    }
    printf("Opcion invalida.\n");
    return;
}

void romperJugadas(struct Tablero *tablero, struct ColaJugadores *cola){
    
}

void jugadaBot(struct Tablero *tablero, struct ColaJugadores *cola, struct Pila *pila)
{
    struct colaCartas verde, azul, rosa, naranja, jokers;
    inicializarColaCartas(&verde);
    inicializarColaCartas(&azul);
    inicializarColaCartas(&rosa);
    inicializarColaCartas(&naranja);
    inicializarColaCartas(&jokers);
    struct Fichas posibleJugada[MAX_COLS];
    struct Jugador *actual = cola->frente;
    struct Jugada *nuevaJugada = malloc(sizeof(struct Jugada));
    int i = 0, sumaCartas = 0, k = 0, jugadaRealizada = 0, revision = 0;
    if (!nuevaJugada)
    {
        perror("Error al asignar memoria para nuevaJugada");
        exit(EXIT_FAILURE);
    }

    ordenarMano(actual->mano, actual->numCartas);

    // Llenar las colas y arreglos de indices
    for (int j = 0; j < actual->numCartas; j++)
    {
        if (strcmp(actual->mano[j].color, VERDE) == 0)
        {
            agregarCartaACola(&verde, actual->mano[j], j);
        }
        else if (strcmp(actual->mano[j].color, AZUL) == 0)
        {
            agregarCartaACola(&azul, actual->mano[j], j);
        }
        else if (strcmp(actual->mano[j].color, ROSA) == 0)
        {
            agregarCartaACola(&rosa, actual->mano[j], j);
        }
        else if (strcmp(actual->mano[j].color, NARANJA) == 0)
        {
            agregarCartaACola(&naranja, actual->mano[j], j);
        }
        else if (strcmp(actual->mano[j].color, BLANCO) == 0)
        {
            agregarCartaACola(&jokers, actual->mano[j], j);
        }
    }

    struct colaCartas *colas[4] = {&verde, &azul, &rosa, &naranja};
    int numColores = sizeof(colas) / sizeof(colas[0]);

    for (int c = 0; c < numColores; c++)
    {
        struct colaCartas *color = colas[c];
        int tam = color->tamanio;

        for (int j = 0; j < tam - 3 && !jugadaRealizada; j++)
        {
            sumaCartas = 0;
            for (int l = 0; l < 4; l++)
            {
                posibleJugada[l] = color->arreglo[j + l];
            }
            revision = revisarAgregarJugada(posibleJugada, 4, actual->esBot);
            if (revision != 0)
            {
                for (int m = 0; m < 4; m++)
                {
                    sumaCartas += posibleJugada[m].numero;
                    k++;
                }
                for (int m = 0; m < 4; m++)
                {
                    agregarFichaPorDerecha(nuevaJugada, actual->mano[color->indices[j + m]]);
                }
                while (k > 0)
                {
                    int pos = color->indices[--k];
                    eliminarCartaDeMano(actual, pos);
                }
                if (sumaCartas >= 25 || actual->jugadorActivo == 1)
                {
                    agregarJugada(tablero, nuevaJugada);
                    actual->jugadorActivo = 1;
                    return;
                }
            }
        }
        for (int j = 0; j < tam - 2 && !jugadaRealizada; j++)
        {
            sumaCartas = 0;
            for (int l = 0; l < 3; l++)
            {
                posibleJugada[l] = color->arreglo[j + l];
            }
            revision = revisarAgregarJugada(posibleJugada, 3, actual->esBot);
            if (revision != 0)
            {
                for (int m = 0; m < 3; m++)
                {
                    sumaCartas += posibleJugada[m].numero;
                    k++;
                }
                for (int m = 0; m < 3; m++)
                {
                    agregarFichaPorDerecha(nuevaJugada, actual->mano[color->indices[j + m]]);
                }
                while (k > 0)
                {
                    int pos = color->indices[--k];
                    eliminarCartaDeMano(actual, pos);
                }
                if (sumaCartas >= 25 || actual->jugadorActivo == 1 || (actual->jugadorActivo == 1 && k == 13))
                {
                    agregarJugada(tablero, nuevaJugada);
                    actual->jugadorActivo = 1;
                    return;
                }
            }
        }
    }
    // OAK
    for (i = 0; i < 13; i++)
    {
        k = 0, sumaCartas = 0;
        int arrIndices[4];
        for (int c = 0; c < numColores; c++)
        {
            struct colaCartas *color = colas[c];
            for (int j = 0; j < color->tamanio; j++)
            {
                if (color->arreglo[j].numero == i)
                {
                    posibleJugada[k] = color->arreglo[j];
                    arrIndices[k] = color->indices[j];
                    k++;
                }
            }
        }
        if (k > 2)
        {
            for (int l = 0; l < k; l++)
            {
                sumaCartas += posibleJugada[l].numero;
            }
            if (sumaCartas >= 25 || actual->jugadorActivo == 1 || (actual->jugadorActivo == 1 && k == 13))
            {
                for (int m = 0; m < k; m++)
                {
                    agregarFichaPorDerecha(nuevaJugada, actual->mano[arrIndices[m]]);
                }
                while (k > 0)
                {
                    int pos = arrIndices[--k];
                    eliminarCartaDeMano(actual, pos);
                }
                agregarJugada(tablero, nuevaJugada);
                actual->jugadorActivo = 1;
                return;
            }
        }
    }
    comer(cola, pila);
}

void agregarCartaACola(struct colaCartas *cola, struct Fichas carta, int indice)
{
    cola->arreglo[cola->tamanio] = carta;
    cola->indices[cola->tamanio++] = indice;
}

void eliminarCartaDeMano(struct Jugador *jugador, int indice)
{
    for (int i = indice; i < jugador->numCartas - 1; i++)
    {
        jugador->mano[i] = jugador->mano[i + 1];
    }
    jugador->numCartas--;
}

// Funciones de control de juego
void finTurno(struct ColaJugadores *cola)
{
    if (cola == NULL || cola->frente == NULL)
    {
        printf("Fin de la partida!\n");
        PCTurn(2);
        return;
    }
    // Avanzar el frente de la cola
    cola->frente = cola->frente->siguiente;
    cola->trasero = cola->trasero->siguiente;
}
void revisarSalida(struct ColaJugadores *cola, struct ColaJugadores *colaResultados, int *jugadoresActuales)
{
    if (cola == NULL || cola->frente == NULL)
    {
        printf("Error: La cola de jugadores esta vacia.\n");
        return;
    }

    struct Jugador *jugadorActual = cola->frente;

    // Verificar si el jugador no tiene fichas
    if (jugadorActual->numCartas == 0 || cola->frente == cola->trasero)
    {
        // Si el jugador no tiene fichas, sacarlo de la cola actual
        if (cola->frente == cola->trasero)
        {
            // Si era el unico jugador en la cola, se vacia la cola
            cola->frente = NULL;
            cola->trasero = NULL;
        }
        else
        {
            cola->frente = jugadorActual->siguiente; // Avanzar el frente
            cola->trasero->siguiente = cola->frente; // Mantener circularidad
        }

        // Ahora agregamos el jugador a la cola de resultados
        if (colaResultados->frente == NULL)
        {
            // Si la cola de resultados esta vacia, el jugador se convierte en el frente y el trasero
            colaResultados->frente = jugadorActual;
            colaResultados->trasero = jugadorActual;
        }
        else
        {
            // De lo contrario, agregamos al final de la cola de resultados
            colaResultados->trasero->siguiente = jugadorActual;
            colaResultados->trasero = jugadorActual; // Actualizar el trasero
        }

        jugadorActual->siguiente = NULL; // Desconectar para evitar circularidad en resultados
        if ((*jugadoresActuales) > 1)
            printf("%s ha salido del juego.\n", jugadorActual->nombre);
        else
            printf("El jugador %s ha salido porque ya no hay mas jugadores!.\n", jugadorActual->nombre);
        (*jugadoresActuales)--;
        PCTurn(2);
    }
}

// Funciones de final de juego
void Leaderboard(struct ColaJugadores *colaResultados, int totalJugadores)
{
    if (colaResultados == NULL || colaResultados->frente == NULL)
    {
        printf("No hay jugadores en la cola de resultados.\n");
        return;
    }

    struct Jugador *actual = colaResultados->frente;
    int posicion = 1; // Inicia la posicion desde 1

    printf("Clasificacion del juego:\n");

    // Recorre la cola de resultados imprimiendo los jugadores en orden
    do
    {
        printf("%d.- %s\n", posicion, actual->nombre);

        if (actual->siguiente == NULL)
        {
            break; // Si hemos llegado al final de la cola de resultados, salimos del bucle
        }

        actual = actual->siguiente; // Avanza al siguiente jugador
        posicion++;                 // Aumenta la posicion
    } while (actual != NULL && posicion <= totalJugadores);
}
