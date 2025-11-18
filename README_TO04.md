# TO04 - Dibujador de Figuras Geométricas

## Descripción

Programa que dibuja figuras geométricas regulares (cuadrados, pentágonos, heptágonos y decágonos) en el framebuffer de Linux usando mapeo de memoria.

## Características

- Dibuja 4 tipos de figuras geométricas regulares:
  - Cuadrados (4 lados)
  - Pentágonos (5 lados)
  - Heptágonos (7 lados)
  - Decágonos (10 lados)
- Por cada tipo seleccionado, dibuja aleatoriamente entre 2 y 5 figuras
- Posiciones, tamaños y colores aleatorios
- Interfaz de menú simple y clara

## Requisitos

- Sistema Linux con acceso a `/dev/fb0` (framebuffer)
- Compilador C++ (g++)
- Permisos de superusuario para acceder al framebuffer

## Compilación

```bash
g++ -o to04_polygons to04_polygons.cpp -lm
```

El flag `-lm` enlaza la librería matemática necesaria para las funciones trigonométricas.

## Ejecución

```bash
sudo ./to04_polygons
```

Se requiere `sudo` porque el acceso al framebuffer (`/dev/fb0`) normalmente requiere permisos de root.

## Uso

1. Al iniciar el programa, se muestra un menú con 5 opciones:

   ```
   1. Cuadrados (4 lados)
   2. Pentágonos (5 lados)
   3. Heptágonos (7 lados)
   4. Decágonos (10 lados)
   5. Salir
   ```

2. Presiona el número correspondiente a la figura que deseas dibujar

3. El programa dibujará entre 2 y 5 figuras del tipo seleccionado en posiciones, tamaños y colores aleatorios

4. Presiona cualquier tecla para volver al menú

5. Selecciona opción 5 para salir

## Estructura del Código

### Funciones Principales

- **`drawPixel(x, y, color)`**: Dibuja un píxel en las coordenadas especificadas
- **`drawLine(x1, y1, x2, y2, color)`**: Dibuja una línea usando el algoritmo de Bresenham
- **`drawPolygon(centerX, centerY, size, sides, color)`**: Función genérica para dibujar polígonos regulares
- **`drawSquare/Pentagon/Heptagon/Decagon(...)`**: Funciones específicas para cada figura
- **`drawRandomFigures(figureType)`**: Genera y dibuja 2-5 figuras aleatorias del tipo seleccionado
- **`showMenu()`**: Muestra el menú de selección

### Variables Globales

- **`fbmap`**: Puntero al framebuffer mapeado en memoria
- **`vinfo`**: Información variable de la pantalla (resolución, bits por píxel)
- **`finfo`**: Información fija de la pantalla
- **`screensize`**: Tamaño total del framebuffer en bytes

## Lógica de Dibujo

El programa usa matemáticas simples para dibujar polígonos regulares:

1. Calcula los vértices del polígono usando coordenadas polares:

   - Ángulo de cada vértice = `(2π × i) / número_de_lados`
   - Posición X = `centro_X + radio × cos(ángulo)`
   - Posición Y = `centro_Y + radio × sin(ángulo)`

2. Conecta los vértices consecutivos con líneas usando el algoritmo de Bresenham

3. Los colores están predefinidos en formato RGB565 (16 bits)

## Formato de Color

El programa usa el formato RGB565 típico de framebuffers de 16 bits:

- Rojo: 5 bits (bits 11-15)
- Verde: 6 bits (bits 5-10)
- Azul: 5 bits (bits 0-4)

Ejemplos de colores usados:

- `0xF800`: Rojo
- `0x07E0`: Verde
- `0x001F`: Azul
- `0xFFE0`: Amarillo
- `0xF81F`: Magenta
- `0x07FF`: Cian
- `0xFFFF`: Blanco

## Notas Técnicas

- El programa funciona en Raspberry Pi con Raspbian Bookworm
- Compatible con arquitecturas ARM y x86
- Requiere que el framebuffer esté habilitado en el sistema
- Si tu sistema usa Wayland o X11, puede que necesites cambiar a una consola virtual (Ctrl+Alt+F1 a F6)

## Créditos

Desarrollado para la tarea TO04 del curso de Arquitectura de Computadoras.
Basado en ejemplos de programación de framebuffer en Linux.
