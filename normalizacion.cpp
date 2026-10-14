#include <iostream>
#include <cstdio>
#include <cstring>  
#include <string>
using namespace std;
const int K = 4;
struct ComandaHistorica{
    char fecha[11];
    char NombreMozo[50];
    int CodigoProducto;
    int Cantidad;
    float Comision;
};
struct Comanda{
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};
struct Producto{
    int codigo;
    char descripcion[50];
    float precio;
    int stockActual;
};
struct mozo{
    int iDmozo;
    char nombre[50];
    char password[20];
    float totalComision;    
};
void crearMozos (const char* nombre, ComandaHistorica ventas [], int cantidadVentas){
    FILE* f = fopen(nombre, "wb");
    if (f == NULL) {
        cout << "Error al abrir el archivo" << endl;
        return;
    }
    float suma = 0;
    int iDmozo = 1;
    for (int i = 0; i < cantidadVentas; i++){
        suma += ventas[i].Comision;
        if (i == cantidadVentas - 1 || strcmp(ventas[i].NombreMozo, ventas[i + 1].NombreMozo) != 0) {
            mozo m;
            m.iDmozo = iDmozo++;
            strcpy(m.nombre, ventas[i].NombreMozo);
            m.totalComision = suma;
            strcpy(m.password, to_string(m.iDmozo).c_str());
            for (int k = 0; m.password[k] != '\0'; k++) {
                m.password[k] = m.password[k] + K;
            }
            fwrite(&m, sizeof(mozo), 1, f);
            suma = 0;
        }
    }
    fclose(f);
}
void asignarIds (ComandaHistorica ventas [], int cantidadVentas, int ids []){
    int iDmozo = 1;
    for (int i = 0; i < cantidadVentas; i++){
        ids[i] = iDmozo;
        if (i < cantidadVentas - 1 && strcmp(ventas[i].NombreMozo, ventas[i + 1].NombreMozo) != 0)
            iDmozo++;
    }

