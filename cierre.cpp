#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

struct comanda {
    int idmozo;
    int codigoproducto;
    int cantidad;
    float comision;
};

long apareo(const char *nomA, const char *nomB, const char *nomC) {
    FILE *a = fopen(nomA, "rb");
    if (a == NULL) return -1;
    FILE *b = fopen(nomB, "rb");
    if (b == NULL) { fclose(a); return -1; }
    FILE *c = fopen(nomC, "wb");
    if (c == NULL) { fclose(a); fclose(b); return -1; }

    comanda ra, rb;
    long grabados = 0;
    int la = fread(&ra, sizeof(comanda), 1, a);   
    int lb = fread(&rb, sizeof(comanda), 1, b);

    while (la == 1 && lb == 1) {                 
        if (ra.idmozo <= rb.idmozo) {
            fwrite(&ra, sizeof(comanda), 1, c);
            la = fread(&ra, sizeof(comanda), 1, a);
        } else {
            fwrite(&rb, sizeof(comanda), 1, c);
            lb = fread(&rb, sizeof(comanda), 1, b);
        }
        grabados++;
    }
    while (la == 1) {                            
        fwrite(&ra, sizeof(comanda), 1, c);
        la = fread(&ra, sizeof(comanda), 1, a);
        grabados++;
    }
    while (lb == 1) {                            
        fwrite(&rb, sizeof(comanda), 1, c);
        lb = fread(&rb, sizeof(comanda), 1, b);
        grabados++;
    }

    fclose(a); fclose(b); fclose(c);
    return grabados;
}

int main() {
    const int MAX_DIAS = 7;
    char fechas[MAX_DIAS][11];
    int cantDias = 0;
    cout << "Fechas de la semana (DD-MM-AAAA), 0 para terminar:" << endl;
    cout << "Fecha: ";
    cin >> fechas[cantDias];
    while (strcmp(fechas[cantDias], "0") != 0 && cantDias < MAX_DIAS - 1) {
        cantDias++;
        cout << "Fecha: ";
        cin >> fechas[cantDias];
    }
    if (strcmp(fechas[cantDias], "0") != 0) cantDias++;

    char sufijo[20];
    cout << "Identificador de la semana (ej: s1-06): ";
    cin >> sufijo;

    const char *tmpA = "tmp_semana_a.dat";
    const char *tmpB = "tmp_semana_b.dat";
    const char *actual = tmpA;
    const char *otro = tmpB;

    FILE *vacio = fopen(actual, "wb");
    if (vacio == NULL) {
        cout << "Error: no se pudo crear el archivo auxiliar." << endl;
        return 1;
    }
    fclose(vacio);

    int diasUnidos = 0;
    long total = 0;
    for (int i = 0; i < cantDias; i++) {
        char nombre[40];
        sprintf(nombre, "comandas_%s.dat", fechas[i]);

        long n = apareo(actual, nombre, otro);
        if (n == -1) {
            cout << "Aviso: el archivo " << nombre << " no existe o no se abrio." << endl;
            remove(otro);
        } else {
            cout << "Procesado y unificado: " << nombre << endl;
            remove(actual);
            const char *aux = actual;             
            actual = otro;
            otro = aux;
            total = n;
            diasUnidos++;
        }
    }

    if (diasUnidos == 0) {
        cout << "No se unio ningun dia: no se genera la semanal." << endl;
        remove(actual);
        return 1;
    }

    char salida[60];
    sprintf(salida, "comandas_semana_%s.dat", sufijo);
    remove(salida);
    if (rename(actual, salida) != 0) {
        cout << "Error: no se pudo crear o abrir el archivo semanal." << endl;
        return 1;
    }

    cout << "Cierre de semana generado y ordenado por mozo exitosamente (" << total << " comandas)." << endl;
    return 0;
}