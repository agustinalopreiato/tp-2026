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

- normalizacion.cpp (mozos): Nahuel Mengo, Santino Bollati
- ventas.cpp: Sofia Matilde Lucieri
- cierre.cpp: Agustina Ayelen Lo Preiato
- resumen.cpp: Azul Katerina Basovsky Paez


La consigna completa está en **`enunciado.pdf`**. Leela antes de arrancar: el
cliente (Alberto) cuenta su problema a su manera y ustedes tienen que descubrir
cómo resolverlo con lo que vimos de archivos.

## normalizacion.cpp
Funcionamiento

Este programa permite procesar un archivo de comandas históricas de un restaurante para generar archivos con información de los mozos, organizar las comandas por día y actualizar el stock de los productos vendidos.
El sistema trabaja con archivos binarios para almacenar y modificar la información.

Requisitos

-Un compilador compatible con C++, como g++.
-El archivo comandas_historicas.dat con registros del tipo ComandaHistorica.
-El archivo inventario.dat con registros del tipo Producto, ordenados por código.
-Los archivos binarios deben haberse generado utilizando estructuras compatibles con las definidas en el programa.

Funciones principales

-Ordenamiento de ventas: ordena las comandas históricas alfabéticamente por el nombre del mozo.
-Generación de mozos: crea el archivo mozos.dat, almacenando el identificador, nombre, contraseña y comisión total de cada mozo.
-Generación de identificadores: asigna un ID a cada mozo para relacionarlo con sus comandas.
-Separación de comandas por fecha: genera un archivo independiente para cada día, con las ventas correspondientes a esa fecha.
-Actualización del inventario: descuenta del stock las cantidades vendidas de cada producto.
-Control de errores: informa si no se puede abrir un archivo, si un producto no existe en el inventario o si no hay stock suficiente

Objetivo final

Automatizar el procesamiento de las ventas de un restaurante, centralizando la información de los mozos, organizando las comandas por fecha y manteniendo actualizado el inventario de productos.

## Ventas.cpp
Pide la fecha del dia una sola vez (dd-mm-aaaa) y abre la planilla comandas_dd-mm-aaaa.dat con ab, si no existe la crea y si ya existe agrega al final sin tocar lo anterior. Despues por cada venta pide:
- Id de mozo y su clave, y los valida
- Codigo del producto y lo busca en el inventario
- Cantidad vendida y verifica el stock
Luego calcula la comision y descuenta lo vendido del stock. Por ultimo, registra la venta y ordena la planilla del dia por mozo.
Tecnicas elegidas:
- Corrimiento de la clave:Para que las claves de los mozos no queden legibles en mozos.dat, las guardamos transformadas, la función recorre la clave carácter por carácter y le suma un número fijo (K = 4) al código ASCII de cada uno. A la clave que tipea el mozo le aplicamos la misma cuenta y la comparamos con strcmp contra lo guardado en el archivo.
- Buscar mozo con PUP: porque normalizacion.cpp numera a los mozos a partir del 1 y sin huecos, por lo   que el mozo "idmozo" esta en la posicion idmozo - 1. Con el `fseek` y el `fread` lo leemos.
- Buscar producto con busqueda binaria: inventario.dat está ordenado por código pero con huecos, así que PUP no sirve. En cada paso se descarta la mitad, con costo
log₂(N) accesos a disco y devuelve la posicion del producto. 
- Descontar stock: editar un registro in situ, para poder modificar el archivo sin reescribirlo todo. Como ya sabemos la posición del producto, nos paramos ahí con `fseek`, leemos, restamos la cantidad, volvemos a apuntar al mismo lugar y grabamos con `fwrite`. ´
- Ordenar planilla por mozo: Ordenar adentro del archivo sería ineficiente por el costo de disco, por eso lo llevamos a memoria (a un vector). Se ordena por idmozo con burbuja (en cada pasada compara pares adyacentes e intercambia si están en orden incorrecto) y se reescribe el archivo con `wb`.

## cierre.cpp
Junta las planillas de todos los días de la semana en una sola planilla semanal, ordenada por mozo. Pide por teclado: las fechas de los días (hasta 7 pero se puede terminar antes con `0`), el identificador de la semana (por ejemplo `s1-06`), y genera `comandas_semana_s1-06.dat`.

Partes clave:
- Apareo: cada planilla diaria ya queda ordenada por id de mozo, así que se intercalan de a dos archivos: se compara el registro actual de cada uno, se graba el de menor id de mozo y se avanza solo ese archivo. Cuando uno se agota, se vuelca lo que queda del otro. Es mejor que cargar todo en un arreglo y ordenar con burbuja porque recorre cada archivo una vez, usa memoria constante y no tiene tope de comandas.
- Cuando tengo más de dos días: se parte de un acumulado vacío y se lo aparea con cada día, alternando dos archivos temporales (`tmp_semana_a.dat` y `tmp_semana_b.dat`), porque no se puede leer y escribir el mismo archivo a la vez. Y al terminar, el acumulado se renombra a la planilla semanal.
- Orden: si un mismo mozo está en los dos archivos, sale primero el del acumulado, así se conserva el orden de los días.
- Inexistencia: si la planilla de un día no existe, se avisa y se omite; si no existe ninguno, no se genera la semanal.

## Resumen.cpp

Pide por teclado el identificador de la semana (por ejemplo s1-06), arma el nombre comandas_semana_s1-06.dat y abre esa planilla semanal en modo lectura. Recorre las ventas de la semana, que vienen ordenadas por mozo, y por cada mozo muestra su nombre, cuántos productos vendió y cuánta comisión se le debe pagar. Al final muestra el total de productos que vendió el buffet en toda la semana.

Tecnicas elegidas:
- Corte de control por mozo: como la planilla semanal está ordenada por idMozo, todas las ventas de un mismo mozo quedan juntas. Leemos el primer registro antes del ciclo (lectura anticipada) y, por cada mozo, un ciclo interno acumula la cantidad de productos y la comisión mientras el id no cambie. Cuando cambia, se cierra el grupo, se muestra la línea del mozo y se pasa al siguiente. El último mozo se muestra solo: cuando fread devuelve 0 el ciclo interno termina, se muestra el grupo y recién ahí corta el ciclo externo.
- Buscar mozo con PUP: la planilla solo tiene el número de mozo y el resumen tiene que mostrar el nombre. Como normalizacion.cpp numera a los mozos desde 1 y sin huecos, el mozo idMozo está en la posición idMozo - 1 de mozos.dat. Con fseek nos paramos ahí y con fread leemos un solo registro, sin recorrer el archivo. La función devuelve 1 si el mozo existe y 0 si no.
- Total general: antes del ciclo inicializamos un acumulador y, cada vez que se cierra el grupo de un mozo, le sumamos sus productos. Se muestra al final, debajo del resumen por mozo.
- Casos raros: si la planilla no existe se avisa y el programa termina; si está vacía solo muestra el total en 0; y si una venta trae un idMozo que no está en mozos.dat, el programa no se rompe: muestra igual los totales de ese mozo aclarando que no se encontró.

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
