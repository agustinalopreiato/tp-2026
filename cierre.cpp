#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};

int main() {
FILE *archSemanal = fopen("comandas_semana_s1-10.dat", "wb");
    if (archSemanal == NULL) {
        cout << "Error: No se pudo crear o abrir el archivo semanal." << endl;
        return 1;
    }

    cout << "El archivo semanal fue abierto con éxito." << endl;
    fclose (archSemanal);


   }