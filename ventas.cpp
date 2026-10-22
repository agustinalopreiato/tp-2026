#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;

struct mozo {
    int idmozo;
    char nombre[50];
    char password[20];
    float totalcomision;
};
struct producto{
    int codigo;
    char descripcion[50];
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

void ordenarPlanillaPorMozo (const char* nombreArchivo){
    FILE* f = fopen(nombreArchivo, "rb+");
    if (f == NULL)return;
    fseek(f, 0, SEEK_END);
    long n = ftell(f) / sizeof(comanda);
    comanda a, b;
    bool hubocambio=true;
    for (long i = 0; i < n - 1; i++) {
        hubocambio=false;
        for (long j = 0; j < n - i - 1; j++) {
            fseek(f, j * sizeof(comanda), SEEK_SET);
            fread(&a, sizeof(comanda), 1, f);
            fread(&b, sizeof(comanda), 1, f);
            if (a.idmozo > b.idmozo) {
                fseek(f, j * sizeof(comanda), SEEK_SET);
                fwrite(&b, sizeof(comanda), 1, f);
                fwrite(&a, sizeof(comanda), 1, f);
                hubocambio=true;
            }
        }
        if (!hubocambio) break;
    }
}

int main() {
    char fecha[11];
    cout << "Ingrese la fecha de la venta (dd-mm-aaaa): ";
    cin >> fecha;

    char nombreArchivo[50];
    sprintf(nombreArchivo, "comandas_%s.dat", fecha);

    FILE* fmozos = fopen("mozos.dat", "rb");
    if (fmozos == NULL) {
        cout << "Error al abrir el archivo de mozos.dat" << endl;
        return 1;
    }

     FILE* finventario = fopen("inventario.dat", "rb+");
    if (finventario == NULL) {
        cout << "Error al abrir el archivo de inventario.dat" << endl;
        fclose(fmozos);
        return 1;
    }
    FILE* fdia = fopen(nombreArchivo, "ab");
    if (fdia == NULL) {
        cout << "Error al crear el archivo de ventas del dia" << endl;
        fclose(fmozos);
        fclose(finventario);
        return 1;
    }
    int idmozo;
    do {
        cout << "Ingrese el ID del mozo (0 para finalizar): ";
        cin >> idmozo;
        if (idmozo == 0) {
            cout << "Id no valido." << endl;
            break;
        }
        char clave[20];
        cout << "Ingrese la clave del mozo: ";
        cin >> clave;
        if (strcmp(clave, "0") == 0) break;
        mozo m;
        if (!validarLogin(fmozos, idmozo, clave, m)) {
            cout << "ID de mozo o clave incorrectos." << endl;
            continue;
        }
        int codigoproducto, cantidad;
        cout << "Ingrese el codigo del producto (0 para finalizar): ";
        cin >> codigoproducto;
        if (codigoproducto == 0) break;
        producto p;
        long posEncontrada;
        if (!buscarProductoBinaria(finventario, codigoproducto, p, posEncontrada)) {
            cout << "Producto no encontrado." << endl;
          continue;
        }
        cout << "Ingrese la cantidad vendida: ";
        cin >> cantidad;
        if (cantidad == 0) break;
        if (cantidad < 0){
            cout << "Cantidad invalida." << endl;
            continue;
        }
        if (cantidad > p.stockactual) {
            cout << "Stock insuficiente. Stock actual: " << p.stockactual << endl;
            continue;
        }
        float comision = cantidad * p.precio * TASA_COMISION;
        
        comanda c;
        c.idmozo = idmozo;
        c.codigoproducto = codigoproducto;
        c.cantidad = cantidad;
        c.comision = comision;
        fwrite(&c, sizeof(comanda), 1, fdia);
        descontarStock(finventario, posEncontrada, cantidad);

        cout << "Venta registrada. Comision del mozo: " << comision << endl;
    } while (true);
    fclose(fmozos);
    fclose(finventario);
    fclose(fdia);
    ordenarPlanillaPorMozo(nombreArchivo);

    cout << "Archivo de ventas del dia " << nombreArchivo << " generado y ordenado por mozo." << endl;
    return 0;
}