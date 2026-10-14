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
}
void crearPlanillasPorDia (ComandaHistorica ventas [], int cantidadVentas, int ids []){
    char fechas[MAX_FECHAS][11];
    int cantFechas = 0;
    for (int i = 0; i < cantidadVentas; i++){
        if (buscarFecha(fechas, cantFechas, ventas[i].fecha) == -1 && cantFechas < MAX_FECHAS){
            strcpy(fechas[cantFechas], ventas[i].fecha);
            cantFechas++;
        }
    }
    for (int d = 0; d < cantFechas; d++){
        char nombreArchivo[40];
        strcpy(nombreArchivo, "comandas_");
        strcat(nombreArchivo, fechas[d]);
        strcat(nombreArchivo, ".dat");

        FILE* f = fopen(nombreArchivo, "wb");
        if (f == NULL){
            cout << "Error al crear " << nombreArchivo << endl;
            continue;
        }
        int cant = 0;
        for (int i = 0; i < cantidadVentas; i++){
            if (strcmp(ventas[i].fecha, fechas[d]) == 0){
                Comanda c;
                c.idMozo = ids[i];
                c.codigoProducto = ventas[i].CodigoProducto;
                c.cantidad = ventas[i].Cantidad;
                c.comision = ventas[i].Comision;
                fwrite(&c, sizeof(Comanda), 1, f);
                cant++;
            }
        }
        fclose(f);
        cout << nombreArchivo << " -> " << cant << " ventas" << endl;
    }
}
