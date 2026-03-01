# Geometry Patterns Generator

Generador de patrones geométricos en **G-code** a partir de 3 polígonos base:

- Rectángulo 1
- Rectángulo 2
- Triángulo

El programa calcula intersecciones/diferencias entre regiones y aplica un patrón distinto por zona.

---

## Vista previa

> Coloca tu imagen en `docs/output_patterns.png` (o cambia la ruta).

![Vista de patrones](docs/output_patterns.png)

---

## Arquitectura del proyecto

### Componentes principales

- **`GeometryProcessor`**
  - Operaciones booleanas entre polígonos (intersección, diferencia, triple intersección).
  - Basado en Clipper2.

- **`IPattern`** (interfaz)
  - Contrato común: `generate(const Polygon&)` y `name()`.

- Patrones concretos:
  - **`RectilinearPattern`** (rotación configurable, p.ej. 0°, 90°, 120°)
  - **`HoneycombPattern`**
  - **`SpiralPattern`**
  - **`ConcentricPattern`**

- **`GCodeGenerator`**
  - Construye salida G-code por regiones.
  - Soporta segmentos normales y de viaje (`is_travel`).

- **`main.cpp`**
  - Define geometrías de entrada.
  - Calcula regiones:
    - zonas exclusivas (`r1Only`, `r2Only`, `tOnly`)
    - intersecciones dobles sin triple (`intR1R2Only`, `intR1TOnly`, `intR2TOnly`)
    - triple intersección (`intAll`)
  - Asigna patrón a cada región y exporta `output_patterns.gcode`.

---

## Asignación de patrones actual

- `Rectangle 1` → Rectilinear 0°
- `Rectangle 2` → Honeycomb
- `Triangle` → Spiral
- `Rect1 ∩ Rect2` → Rectilinear 90°
- `Rect1 ∩ Triangle` → Rectilinear 120°
- `Triple Intersection` → Concentric

---

## Requisitos

- Linux
- `g++` con C++17
- `cmake` (>= 3.15 recomendado)
- Clipper2 incluido en:
  - `external/Clipper2/CPP`

Instalación básica en Ubuntu/Debian:

```bash
sudo apt update
sudo apt install -y build-essential cmake
```

---

## Compilación

Desde la raíz del proyecto:

```bash
mkdir -p build
cd build
cmake ..
make -j$(nproc)
```

---

## Ejecución

```bash
cd build
./geometry_patterns
```

Salida esperada:
- Archivo G-code: `output_patterns.gcode` (en raíz del proyecto o en cwd según implementación)
- Mensaje: `✓ Generation complete!`

---

## Estructura de carpetas (resumen)

```text
include/
  core/Types.hpp
  GeometryProcessor.hpp
  GCodeGenerator.hpp
  patterns/
    IPattern.hpp
    RectilinearPattern.hpp
    HoneycombPattern.hpp
    SpiralPattern.hpp
    ConcentricPattern.hpp

src/
  main.cpp
  GeometryProcessor.cpp
  GCodeGenerator.cpp
  patterns/
    IPattern.cpp
    RectilinearPattern.cpp
    HoneycombPattern.cpp
    SpiralPattern.cpp
    ConcentricPattern.cpp
```

---

## Notas técnicas

- Para evitar rellenos duplicados en zonas solapadas, usa `FillRule::NonZero` y unifica recortes antes de `Difference`.
- `std::vector` ya reserva datos en heap internamente; `unique_ptr` se usa para ciclo de vida seguro de objetos polimórficos.
- Si hay patrones muy densos, considera:
  - subir `line_spacing`
  - reducir resolución angular (spiral/concentric)
  - simplificar polígonos antes de generar trayectorias.

---

## Añadir o cambiar la imagen PNG del README

1. Crea carpeta:
   ```bash
   mkdir -p docs
   ```
2. Copia tu imagen:
   ```bash
   cp ruta/de/tu_imagen.png docs/output_patterns.png
   ```
3. El README ya la mostrará automáticamente con:
   ```md
   ![Vista de patrones](docs/output_patterns.png)
   ```
