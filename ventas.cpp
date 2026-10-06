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
 
bool buscarmozoPUP (FILE* f, int idmozo, mozo &m){
    if (idmozo <= 0) return false;
    long pup = idmozo - 1;
    fseek(f, pup * sizeof(mozo), SEEK_SET);
    int leido = fread(&m, sizeof(mozo), 1, f);
    return (leido == 1 && m.idmozo == idmozo);
}


