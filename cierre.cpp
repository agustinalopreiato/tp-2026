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
    const char *archivosDiarios[] = {
        "comandas_01-06-2025.dat",
        "comandas_02-06-2025.dat"
    };
    int cantDias = 2;

    const int MAX_COMANDAS = 1000;
    comanda vectorComandas[MAX_COMANDAS];
    int totalComandas = 0;

    comanda reg;
    for (int i = 0; i < cantDias; i++) {
        FILE *archDia = fopen(archivosDiarios[i], "rb");
        if (archDia != NULL) {
            while (fread(&reg, sizeof(comanda), 1, archDia) == 1 && totalComandas < MAX_COMANDAS) 
            {
            vectorComandas[totalComandas] = reg; 
            totalComandas++;
            }
            fclose(archDia);
            cout << "Procesado y unificado: " << archivosDiarios[i] << endl;
        } 
        else {
            cout << "Aviso: el archivo " << archivosDiarios[i] << " no existe o no se abrió." << endl;
        } 
    }
    for (int i = 0; i < totalComandas - 1; i++) {
        for (int j = 0; j < totalComandas - i - 1; j++) {
            if (vectorComandas[j].idmozo > vectorComandas[j + 1].idmozo) 
            {
                comanda aux = vectorComandas[j];
                vectorComandas[j] = vectorComandas[j + 1];
                vectorComandas[j + 1] = aux; }}
            }
            FILE *archSemanal = fopen("comandas_semana_s1-06.dat", "wb");
            if (archSemanal == NULL) {
                cout << "Error: no se pudo crear o abrir el archivo semanal." << endl;
                return 1;
    }

    fwrite(vectorComandas, sizeof(comanda), totalComandas, archSemanal);
    fclose(archSemanal);
        cout << "Cierre de semana generado y ordenado por mozo exitosamente" << endl;
        return 0;
}