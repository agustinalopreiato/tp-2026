#include <iostream>
#include <cstdio>
using namespace std;

struct Mozo {
    int idMozo;
    char nombre[50];
    char password[20];
    float totalComision;
};

struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

const char* ARCHIVO_SEMANAL = "comandas_semana_sX-mm.dat";
const char* ARCHIVO_MOZOS = "mozos.dat";

int buscarMozo(const char* nombredelarchivo, int idMozo, Mozo& m) {
    FILE* f = fopen(nombredelarchivo, "rb");
    if (f == NULL) {
        cout << "No se pudo abrir " << nombredelarchivo << "." << endl;
        return 0;
    }

    const int claveInicial = 1;
    int leido = 0;
    long pos = idMozo - claveInicial;                      
    if (pos >= 0) {
        fseek(f, pos * sizeof(Mozo), SEEK_SET);           
        leido = fread(&m, sizeof(Mozo), 1, f);
    }

    fclose(f);
    return leido;                                         
}

void resumenSemanal(const char* nombredelarchivo) {
    FILE* f = fopen(nombredelarchivo, "rb");
    if (f == NULL) { cout << "No se pudo abrir " << nombredelarchivo << "." << endl; return; }

    Comanda c;
    Mozo m;
    int leido = fread(&c, sizeof(Comanda), 1, f);          // lectura anticipada

    while (leido == 1) {
        int mozoActual = c.idMozo;
        int productos = 0;
        float comision = 0;

        while (leido == 1 && c.idMozo == mozoActual) {
            productos += c.cantidad;
            comision += c.comision;
            leido = fread(&c, sizeof(Comanda), 1, f);
        }

        if (buscarMozo(ARCHIVO_MOZOS, mozoActual, m) == 1) {
            cout << m.nombre << ": " << productos
                 << " productos, comision $" << comision << endl;
        } else {
            cout << "Mozo " << mozoActual << ": " << productos
                 << " productos, comision $" << comision << endl;
        }
    }

    fclose(f);
}

int main() {
    resumenSemanal(ARCHIVO_SEMANAL);
    return 0;
}