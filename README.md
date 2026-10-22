# Práctica 2 — Simulador de una Máquina de Turing determinista

Complejidad Computacional, curso 2026/27.

Simulador en C++ de una Máquina de Turing **multicinta determinista**
M = (Q, Σ, Γ, s, b, F, δ). La ejecución termina únicamente cuando no hay
ninguna transición aplicable.

## Variantes elegidas

| Variación | Opción elegida |
|---|---|
| Escritura y movimiento | **Simultáneos**: cada transición escribe un símbolo y mueve la cabeza en cada cinta |
| Movimientos | **L, R y S**: izquierda, derecha y sin movimiento |
| Cinta | **Infinita en ambas direcciones** |

Además, el simulador admite **k ≥ 1 cintas**. La entrada se coloca solo en la
primera cinta, y el resultado también queda en la primera.

## Estructura del proyecto

```
.
├── CMakeLists.txt
├── README.md
├── include/            Cabeceras (.h)
│   ├── options.h         Opciones de la línea de comandos
│   ├── tape.h            Cinta y cabeza de L/E
│   ├── tm_parser.h       Lectura del fichero de configuración
│   ├── transition.h      Transición de δ
│   └── turing_machine.h  Definición y ejecución de la MT
├── src/                Implementación (.cc) y main.cc
├── data/               Ficheros de configuración de MT y cadenas de entrada
└── img/                Grafos de las MT diseñadas (.dot y .png)
```

## Compilación

Hace falta CMake 3.16 o superior y un compilador de C++17 (GCC, Clang o MSVC).

**Linux / macOS**

```bash
cmake -S . -B build
cmake --build build
```

**Windows (MinGW + Ninja)**

```powershell
cmake -S . -B build -G Ninja
cmake --build build
```

El ejecutable se genera en `build/tm` (o en `build\tm.exe` en Windows).

## Ejecución

```
tm -config <f> [-in <f>]
```

- `-config <f>`: fichero con la definición de la MT (obligatorio).
- `-in <f>`: fichero con las cadenas de entrada, una por línea (opcional). Si
  se omite, las cadenas se leen por teclado. Para terminar, pulsa Ctrl+D en
  Linux o Ctrl+Z y Enter en Windows. Una línea vacía representa la cadena
  vacía.

Ejemplos:

```bash
./build/tm -config data/MT-anbm.txt -in data/cadenas-1.txt
./build/tm -config data/MT-contar.txt -in data/cadenas-2.txt
./build/tm -config data/MT-anbm.txt          # entrada por teclado
```

```powershell
build\tm.exe -config data\MT-anbm.txt -in data\cadenas-1.txt
build\tm.exe -config data\MT-contar.txt -in data\cadenas-2.txt
```

Para cada cadena, el programa indica si la MT se ha parado en un estado de
aceptación, en qué estado lo ha hecho y tras cuántos pasos. También muestra el
contenido de cada cinta, con la posición de la cabeza entre corchetes:

```
'abbabaabb': aceptada (parada en q9 tras 43 pasos)
  Cinta 1: [1]1111.1111
  Cinta 2: [.]
  Cinta 3: [.]
```

## Formato del fichero de configuración

```
# Comentarios (desde '#' hasta el final de la línea)
q1 q2 q3 ...     # conjunto Q
a1 a2 a3 ...     # conjunto Σ
A1 A2 A3 ...     # conjunto Γ (símbolos de un único carácter)
q1               # estado inicial
b                # símbolo blanco
q2 q3            # conjunto F
k                # número de cintas
q1 r1 ... rk q2 w1 m1 ... wk mk   # una transición por línea
```

Una transición δ(q1, r1…rk) = (q2, w1 m1, …, wk mk) indica lo siguiente: en el
estado `q1`, leyendo `ri` en la cinta i, la máquina pasa al estado `q2` y, en
cada cinta i, escribe `wi` y mueve su cabeza según `mi` (`L`, `R` o `S`).
Con una sola cinta el formato queda `q1 a1 q2 a2 m`, igual que en el
enunciado.

Al cargar el fichero se comprueba lo siguiente:

- s ∈ Q, F ⊆ Q, Σ ⊆ Γ, b ∈ Γ y b ∉ Σ.
- Los estados y símbolos de cada transición pertenecen a Q y Γ.
- Cada transición tiene el número de elementos correcto.
- La máquina es determinista: no hay dos transiciones para el mismo par
  (estado, símbolos leídos).

Las cadenas de entrada deben estar formadas por símbolos de Σ.

## Máquinas de Turing diseñadas

### 1. L = { aⁿbᵐ | m ≥ n, n > 0 }

| | Ruta |
|---|---|
| Configuración | `data/MT-anbm.txt` |
| Grafo (imagen) | `img/MT-anbm.png` |
| Grafo (fuente Graphviz) | `img/MT-anbm.dot` |
| Cadenas de prueba | `data/cadenas-1.txt` |

Usa una cinta. En cada pasada marca con `X` la primera `a` sin marcar y con
`Y` la primera `b` sin marcar. Si una `a` no tiene `b` con la que emparejarse,
la máquina se para sin aceptar en `q1`. Cuando ya no quedan `a`, `q4`
comprueba que el resto de la cadena solo contiene `Y` y `b`, y acepta en `q5`
al llegar al blanco. Las cadenas que no empiezan por `a`, incluida la cadena
vacía, se rechazan en `q0`.

### 2. Número de `b` y número de `a` en unario

| | Ruta |
|---|---|
| Configuración | `data/MT-contar.txt` |
| Grafo (imagen) | `img/MT-contar.png` |
| Grafo (fuente Graphviz) | `img/MT-contar.dot` |
| Cadenas de prueba | `data/cadenas-2.txt` |

Usa 3 cintas.

1. **`q0`** recorre la entrada y la borra. Por cada `b` escribe un `1` en la
   cinta 2 y por cada `a` escribe un `1` en la cinta 3.
2. **`q1` y `q2`** devuelven las cabezas de las cintas 2 y 3 al principio de
   sus contadores.
3. **`q3` a `q6`** copian los contadores en la cinta 1, primero el de `b` y
   luego el de `a`, separados por un blanco. Las cintas 2 y 3 se borran al
   copiarlas. Si un contador está vacío, se escribe `0`.
4. **`q7` y `q8`** llevan la cabeza de la cinta 1 al principio del resultado,
   y la máquina acepta en `q9`.

| Entrada | Cinta 1 final |
|---|---|
| `abbabaabb` | `[1]1111.1111` |
| `aa` | `[0].11` |
| `bb` | `[1]1.0` |

En el grafo, `y` representa cualquiera de los símbolos `1` o `.`. Cada arista
etiquetada con `y` corresponde a dos transiciones del fichero.

### Máquinas de ejemplo

`data/MT-1.txt` (número impar de ceros) y `data/MT-2.txt` (doble de un número
en unario) son los ejemplos de partida y no forman parte de los problemas
propuestos.

## Regenerar los grafos

Los PNG se generan a partir de los `.dot` con [Graphviz](https://graphviz.org):

```bash
dot -Tpng -Gdpi=150 img/MT-anbm.dot -o img/MT-anbm.png
dot -Tpng -Gdpi=130 img/MT-contar.dot -o img/MT-contar.png
```
