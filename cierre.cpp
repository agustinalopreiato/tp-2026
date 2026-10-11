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

    const int MAX_COMANDAS = 1000;
    comanda vectorComandas[MAX_COMANDAS];
    int totalComandas = 0;

    comanda reg;
    for (int i = 0; i < cantDias; i++) {
        char nombre[40];
        sprintf(nombre, "comandas_%s.dat", fechas[i]);
        FILE *archDia = fopen(nombre, "rb");
        if (archDia != NULL) {
            while (fread(&reg, sizeof(comanda), 1, archDia) == 1 && totalComandas < MAX_COMANDAS)
            {
                vectorComandas[totalComandas] = reg;
                totalComandas++;
            }
            fclose(archDia);
            cout << "Procesado y unificado: " << nombre << endl;
        }
        else {
            cout << "Aviso: el archivo " << nombre << " no existe o no se abrió." << endl;
        }
    }

    for (int i = 0; i < totalComandas - 1; i++) {
        for (int j = 0; j < totalComandas - i - 1; j++) {
            if (vectorComandas[j].idmozo > vectorComandas[j + 1].idmozo)
            {
                comanda aux = vectorComandas[j];
                vectorComandas[j] = vectorComandas[j + 1];
                vectorComandas[j + 1] = aux;
            }
        }
    }

    char salida[60];
    sprintf(salida, "comandas_semana_%s.dat", sufijo);
    FILE *archSemanal = fopen(salida, "wb");
    if (archSemanal == NULL) {
        cout << "Error: no se pudo crear o abrir el archivo semanal." << endl;
        return 1;
    }

    fwrite(vectorComandas, sizeof(comanda), totalComandas, archSemanal);
    fclose(archSemanal);
    cout << "Cierre de semana generado y ordenado por mozo exitosamente" << endl;
    return 0;
}