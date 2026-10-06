#include <iostream>
#include <cstdio>
#include <cstring> 
using namespace std;

struct mozo {
    int idmozo;
    char nombre[50];
    char password[20];
    float totalcomision;
};
struct producto{
    int codigo;
    char descripcion;
    float precio;
    int stockactual;
};
struct comanda {
    int idmozo;
    int codigoproducto;
    int cantidad;
    float comision;
};

const float TASA_COMISION = 0.10f ; 
const int K = 4;  
const char* ARCH_MOZOS = "mozos.dat";
const char* ARCH_INVENTARIO = "inventario.dat";

void aplicarCorrimiento(const char* origen, char* destino, int k) {
    int i = 0;
    while (origen[i] != '\0') {
        destino[i] = (char)(origen[i] + k);
        i++;
    }
    destino[i] = '\0';
}
 

