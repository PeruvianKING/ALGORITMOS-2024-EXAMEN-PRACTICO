#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>
#include <math.h>

void auxOrd(int *v, int izq, int dcha);
void ordenar(int *v, int n);
void intercambiar(int *v, int pos1, int pos2);
void test(void (*algoritmo)(int *, int));
double microsegundos(); /* obtiene la hora del sistema en microsegundos */
void inicializar_semilla();
void aleatorio(int *v, int n);
void printVector(int v[], int n);
void vectorDescendente(int *v, int n);
void vectorAscendente(int *v, int n);
double tiempoEjecucion(int k, void (*algoritmo)(int *, int), void (*modo)(int[], int));
void mostrarTabla(void (*algoritmo)(int *, int), void (*modo)(int *, int), float subEstimada, float ajustada, float sobreEstimada);

int main()
{
    inicializar_semilla();
    test(ordenar);
    mostrarTabla(ordenar, vectorAscendente, 1.7, 1.95, 2.3);
    mostrarTabla(ordenar, vectorDescendente, 1.7, 2, 2.3);
    mostrarTabla(ordenar, aleatorio, 1, 1.13, 1.3);
}

void vectorDescendente(int *v, int n)
{
    int aux = n - 1, i;

    for (i = 0; i < n; i++)
    {
        v[aux - i] = i;
    }
}

void vectorAscendente(int *v, int n)
{
    int i;

    for (i = 0; i < n; i++)
    {
        v[i] = i;
    }
}

void printVector(int v[], int n)
{
    int i;

    printf("%3d", v[0]);
    for (i = 1; i < n; i++)
    {
        printf(", %3d", v[i]);
    }
}

void inicializar_semilla()
{
    srand(time(NULL));
}

void aleatorio(int *v, int n)
{
    int i, m = 2 * n + 1;

    for (i = 0; i < n; i++)
        v[i] = (rand() % m) - n;
}

double microsegundos() /* obtiene la hora del sistema en microsegundos */
{
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0)
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

void intercambiar(int *v, int pos1, int pos2)
{
    int aux;
    aux = v[pos1];
    v[pos1] = v[pos2];
    v[pos2] = aux;
}

void auxOrd(int *v, int izq, int dcha)
{
    int piv, i, j;

    if (izq < dcha)
    {
        piv = v[dcha - 1];
        intercambiar(v, izq, dcha - 1);
        i = izq + 1;
        j = dcha;

        while (i <= j)
        {
            while (v[j] > piv)
                j -= 1;
            while (i <= dcha && v[i] < piv)
                i = i + 1;
            if (i <= j)
            {
                intercambiar(v, i, j);
                i += 1;
                j -= 1;
            }
        }
        intercambiar(v, izq, j);
        auxOrd(v, izq, j - 1);
        auxOrd(v, j + 1, dcha);
    }
}

void ordenar(int *v, int n)
{
    auxOrd(v, 0, n - 1);
}

void test(void (*algoritmo)(int *, int))
{
    int n = 10, v[n];

    printf("*** Test ***\n");

    printf("Inicializacion aleatoria\n");
    aleatorio(v, n);
    printVector(v, n);
    printf("Ordenacion\n");
    algoritmo(v, n);
    printVector(v, n);

    printf("\nInicializacion descendente\n");
    vectorDescendente(v, n);
    printVector(v, n);
    algoritmo(v, n);
    printf("Ordenacion\n");
    printVector(v, n);
    printf("\n\n\n\n");
}

double tiempoEjecucion(int k, void (*algoritmo)(int *, int), 
void (*modo)(int[], int))
{
    int v[32000], i, n = 1000;
    double tInicial, tFinal, tiempoEj1, tiempoEj2, tiempoEj;

    modo(v, k);
    tInicial = microsegundos();
    algoritmo(v, k);
    tFinal = microsegundos();
    tiempoEj = tFinal - tInicial;

    if (tiempoEj < 500)
    {
        tInicial = microsegundos();
        for (i = 0; i < n; i++)
        {
            modo(v, k);
            algoritmo(v, k);
        }
        tFinal = microsegundos();
        tiempoEj1 = tFinal - tInicial;

        tInicial = microsegundos();
        for (i = 0; i < n; i++)
        {
            modo(v, k);
        }
        tFinal = microsegundos();
        tiempoEj2 = tFinal - tInicial;

        tiempoEj = (tiempoEj1 - tiempoEj2) / n;
        printf("(*)"); // si el tiempo es menor a 500, lo despreciamos y repetimos la medición, por lo tanto lo indicamos con (*)
    }
    else
        printf("   ");
    return tiempoEj;
}

void mostrarTabla(void (*algoritmo)(int *, int), void (*modo)(int *, int), 
float subEstimada, float ajustada, float sobreEstimada)
{
    int k;
    double tiempo;

    printf("Ordenación con inicialización %s\n",
           (modo == vectorAscendente) ? "ascendente" : 
           ((modo == vectorDescendente) ? "descendente" : "aleatoria"));

    printf("%12s%16s%16s%.2f%14s%.2f%14s%.2f\n",
           "n", "t(n)", "t(n)/n^", subEstimada, "t(n)/n^", ajustada,
           "t(n)/n^", sobreEstimada);

    for (k = 500; k <= 32000; k = k * 2)
    {
        tiempo = tiempoEjecucion(k, algoritmo, modo);

        printf("%10d%19.8f%16.8f%18.8f%18.8f\n",
               k, tiempo, tiempo / pow(k, subEstimada),
               tiempo / pow(k, ajustada), tiempo / pow(k, sobreEstimada));
    }
    printf("\n\n");
}
