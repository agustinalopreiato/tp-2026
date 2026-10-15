#include <iostream>
#include <cstdio>
#include <cstring>  
using namespace std;
const int K = 4;
const int MAX_FECHAS = 60;
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
void enteroACadena (int n, char cad[]){
    char aux[12];
    int i = 0;
    if (n == 0) aux[i++] = '0';
    while (n > 0){
        aux[i++] = '0' + n % 10;
        n = n / 10;
    }
    int j = 0;
    while (i > 0) cad[j++] = aux[--i];
    cad[j] = '\0';
}
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
            enteroACadena(m.iDmozo, m.password);
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
int buscarFecha (char fechas[][11], int cantFechas, const char* fecha){
    for (int i = 0; i < cantFechas; i++)
        if (strcmp(fechas[i], fecha) == 0) return i;
    return -1;
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
bool descontarStock (FILE* inv, int codigo, int cantidad){
    fseek(inv, 0, SEEK_END);
    int n = ftell(inv) / sizeof(Producto);
    int desde = 0, hasta = n - 1;
    Producto p;
    while (desde <= hasta){
        int medio = (desde + hasta) / 2;
        fseek(inv, medio * sizeof(Producto), SEEK_SET);
        fread(&p, sizeof(Producto), 1, inv);
        if (p.codigo == codigo){
            if (p.stockActual < cantidad){
                cout << "Aviso: producto " << codigo << " sin stock suficiente, queda en 0" << endl;
                p.stockActual = 0;
            } else {
                p.stockActual = p.stockActual - cantidad;
            }
            fseek(inv, medio * sizeof(Producto), SEEK_SET);
            fwrite(&p, sizeof(Producto), 1, inv);
            return true;
        }
        if (codigo < p.codigo) hasta = medio - 1;
        else desde = medio + 1;
    }
    return false;
}
void actualizarStock (ComandaHistorica ventas [], int cantidadVentas){
    FILE* inv = fopen("inventario.dat", "rb+");
    if (inv == NULL){
        cout << "Error al abrir inventario.dat" << endl;
        return;
    }
    for (int i = 0; i < cantidadVentas; i++){
        if (!descontarStock(inv, ventas[i].CodigoProducto, ventas[i].Cantidad))
            cout << "Aviso: producto " << ventas[i].CodigoProducto << " no existe en el inventario" << endl;
    }
    fclose(inv);
}
int main() {
 FILE*archivo=fopen("comandas_historicas.dat", "rb");
    if(archivo==NULL){
    cout<<"Error al abrir el archivo"<<endl;
    return 1;
    }
    fseek(archivo, 0, SEEK_END);
    long tamanio=ftell(archivo);
    int cantidadVentas=tamanio/sizeof(ComandaHistorica);
    fseek(archivo, 0, SEEK_SET);
    ComandaHistorica* ventas=new ComandaHistorica[cantidadVentas];
    ComandaHistorica c;
    int i=0;
    while(fread(&c, sizeof(ComandaHistorica), 1, archivo)==1){
    ventas[i]=c;
    i++;
    }
    fclose(archivo);
    for (int pasada=0; pasada<cantidadVentas-1; pasada++){
        for (int j=0; j<cantidadVentas-1; j++){
            if (strcmp(ventas[j].NombreMozo, ventas[j+1].NombreMozo) > 0){
             ComandaHistorica aux;
             aux = ventas[j];
             ventas[j] = ventas[j+1];
             ventas[j+1] = aux;
            }
        }
    }
 crearMozos("mozos.dat", ventas, cantidadVentas);
 int* ids = new int[cantidadVentas];
 asignarIds(ventas, cantidadVentas, ids);
 crearPlanillasPorDia(ventas, cantidadVentas, ids);
 delete[] ids;
 actualizarStock(ventas, cantidadVentas);
 delete[] ventas;
 return 0;
}