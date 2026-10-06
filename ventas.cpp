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
 
bool buscarMozoPUP (FILE* f, int idmozo, mozo &m){
    if (idmozo <= 0) return false;
    long pup = idmozo - 1;
    fseek(f, pup * sizeof(mozo), SEEK_SET);
    int leido = fread(&m, sizeof(mozo), 1, f);
    return (leido == 1 && m.idmozo == idmozo);
}

bool validarLogin(FILE* fmozos, int idmozo, const char* claveTipeada, mozo &m) {
    if (!buscarMozoPUP(fmozos, idmozo, m)) return false;
    char claveTransformada[20];
    aplicarCorrimiento(claveTipeada, claveTransformada, K);
    return (strcmp(claveTransformada, m.password) == 0);
}

bool buscarProductoBinaria(FILE* f, int codigo, producto &p, long &posEncontrada) {
    fseek(f, 0, SEEK_END);
    long n = ftell(f) / sizeof(producto);
    long pri = 0, ult = n - 1, pos = -1;
    while (pri <= ult && pos == -1) {
        long med = (pri + ult) / 2;
        fseek(f, med * sizeof(producto), SEEK_SET);
        fread(&p, sizeof(producto), 1, f);
        if (p.codigo == codigo) pos = med;
        else if (codigo > p.codigo) pri = med + 1;
        else ult = med - 1;
    }
 posEncontrada = pos;
 return (pos != -1);
} 

void descontarStock(FILE* f, long pos, int cantidad) {
    producto p;
    fseek(f, pos * sizeof(producto), SEEK_SET);
    fread(&p, sizeof(producto), 1, f);
    p.stockactual -= cantidad;
    fseek(f, pos * sizeof(producto), SEEK_SET);
    fwrite(&p, sizeof(producto), 1, f);
}
