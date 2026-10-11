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

int main() {
    return 0;
}