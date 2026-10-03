#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

struct comanda {
int idmozo;
int codigoproducto;
int cantidad;
float comision;
};

int main() {
FILE *archSemanal = fopen("comandas_semana_s1-06.dat", "wb");
if (archSemanal == NULL) {
cout << "Error: no se pudo crear o abrir el archivo semanal." << endl;
return 1;
}
cout << "El archivo semanal fue abierto con éxito." << endl;

const char *archivosDiarios[] = {
"comandas_01-06-2025.dat",
"comandas_02-06-2025.dat"
};
int cantDias = 2;

comanda reg;

for (int i = 0; i < cantDias; i++) {
FILE *archDia = fopen(archivosDiarios[i], "rb");
        
if (archDia != NULL) {
while (fread(&reg, sizeof(comanda), 1, archDia) == 1) {
fwrite(&reg, sizeof(comanda), 1, archSemanal);
}
fclose(archDia);
cout << "Procesado y unificado: " << archivosDiarios[i] << endl;
} 
else {
cout << "Aviso: el archivo " << archivosDiarios[i] << " no existe o no se abrió." << endl;
} 
}
fclose(archSemanal);

cout << "Cierre de semana generado exitosamente" << endl;

return 0;
}

