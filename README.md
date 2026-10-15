# TP Integrador — Buffet Albert Einstein

Trabajo Práctico grupal de **Algoritmos y Estructuras de Datos** (UTN FRBA) —
unidad de **archivos binarios en C/C++**.

> **Grupo:** _K_
> **Integrantes:** Azul Katerina Basovsky Paez — azul310-basovsky 
Santino Bollati — santinobollati-wq
Agustina Ayelen Lo Preiato — agustinalopreiato
Sofia Matilde Lucieri — sofilucieri
Nahuel Mengo — nmengo2000 

## Quién hizo cada parte

- normalizacion.cpp (mozos): <Nahuel Mengo, Santino Bollati>
- ventas.cpp: <Sofia Matilde Lucieri>
- cierre.cpp: <Agustina Ayelen Lo Preiato>
- resumen.cpp: <Azul Katerina Basovsky Paez>

La consigna completa está en **`enunciado.pdf`**. Leela antes de arrancar: el
cliente (Alberto) cuenta su problema a su manera y ustedes tienen que descubrir
cómo resolverlo con lo que vimos de archivos.

## Ventas.cpp
Pide la fecha del dia una sola vez (dd-mm-aaaa) y abre la planilla comandas_dd-mm-aaaa.dat con ab, si no existe la crea y si ya existe agrega al final sin tocar lo anterior. Despues por cada venta pide:
- Id de mozo y su clave, y los valida
- Codigo del producto y lo busca en el inventario
- Cantidad vendida y verifica el stock
Luego calcula la comision y descuenta lo vendido del stock. Por ultimo, registra la venta y ordena la planilla del dia por mozo.
Tecnicas elegidas:
- Buscar mozo con PUP: porque normalizacion.cpp numera a los mozos a partir del 1 y sin huecos, por lo   que el mozo "idmozo" esta en la posicion idmozo - 1. Con el `fseek` y el `fread` lo leemos.
- Buscar producto con busqueda binaria: inventario.dat está ordenado por código pero con huecos, así que PUP no sirve. En cada paso se descarta la mitad, con costo
log₂(N) accesos a disco y devuelve la posicion del producto. 
- Descontar stock: editar un registro in situ, para poder modificar el archivo sin reescribirlo todo. Como ya sabemos la posición del producto, nos paramos ahí con `fseek`, leemos, restamos la cantidad, volvemos a apuntar al mismo lugar y grabamos con `fwrite`. ´
- Ordenar planilla por mozo: Ordenar adentro del archivo sería ineficiente por el costo de disco, por eso lo llevamos a memoria (a un vector). Se ordena por idmozo con burbuja (en cada pasada compara pares adyacentes e intercambia si están en orden incorrecto) y se reescribe el archivo con `wb`.



## Estructura del repo

```
.
├── enunciado.pdf              # la consigna
├── datos/                     # archivos PROVISTOS por la cátedra (no se tocan)
│   ├── comandas_historicas.dat
│   ├── inventario.dat
│   └── dump_datos_de_prueba.txt   # los dos .dat en texto, para verificar la lectura
├── .gitignore
│
│   # --- estos los crean USTEDES (no vienen en el repo base) ---
├── normalizacion.cpp          # cada uno con su propio main; NO hay main.cpp
├── ventas.cpp
├── cierre.cpp
└── resumen.cpp
```

Los 4 programas los escriben ustedes desde cero: el repo base no trae código.
Créenlos en la raíz del repo (como en el diagrama del enunciado).

## Cómo trabajar

Los programas leen y escriben archivos `.dat` en la carpeta donde se ejecutan.
Los archivos de `datos/` son los **originales** y no se modifican: copiálos a la
carpeta donde vas a compilar y correr, y trabajá sobre esas copias.

```bash
cp datos/comandas_historicas.dat datos/inventario.dat .
```

Compilar y correr cada programa (cada uno tiene su propio `main`, no hay `main.cpp`):

```bash
g++ -O2 -o normalizacion normalizacion.cpp
g++ -O2 -o ventas        ventas.cpp
g++ -O2 -o cierre        cierre.cpp
g++ -O2 -o resumen       resumen.cpp
```

Orden de ejecución (cada programa deja los archivos que usa el siguiente):

```
./normalizacion     # historicas + inventario  ->  mozos.dat, comandas_dd-mm-aaaa.dat (varios), inventario actualizado
./ventas            # carga interactiva de la semana  ->  agrega a las planillas del día
./cierre            # junta los días de la semana      ->  comandas_semana_sX-mm.dat
./resumen           # imprime el resumen por mozo + total del buffet
```

## Verificar que leen bien el binario

`datos/dump_datos_de_prueba.txt` tiene el contenido de los dos archivos provistos
en texto legible. Si tu programa imprime otra cosa al leerlos, revisá los `struct`
(orden de campos / tamaños): tienen que dar `sizeof(ComandaHistorica)=76` y
`sizeof(Producto)=64`.

## Qué se entrega

- Este mismo repo (uno por grupo), con los **4 programas** `.cpp` en la raíz
  (cada uno con su propio `main`, no hay `main.cpp`).
- El **README** con el grupo, los integrantes (nombre + usuario de GitHub) y, si
  hace falta, cualquier aclaración de cómo correrlo.
- Los `.dat` de `datos/` son los **provistos por la cátedra** y ya vienen en el
  repo: no los borren ni agreguen otros. Cualquier `.dat` que ustedes copien a la
  raíz o generen al correr **no** se versiona (el `.gitignore` ya los deja afuera).
  La cátedra corre sus programas sobre su propio dataset.
- **Commits repartidos:** cada integrante tiene que tener commits propios a lo
  largo del trabajo. El historial es parte de lo que se mira para la defensa
  individual (no vale un único commit final ni que suba todo una sola persona).
