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