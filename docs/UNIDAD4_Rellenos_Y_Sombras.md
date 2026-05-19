# UNIDAD 4: RELLENOS Y SOMBRAS

## ¿Qué es un relleno?

Un relleno (fill) en gráficos computacionales es el proceso de colorear el interior de una figura geométrica delimitada por un contorno. Mientras que el contorno define los bordes de la forma, el relleno determina cómo se pinta el área interior, pudiendo ser un color sólido, un degradado, un patrón, o una textura.

## ¿Cómo hago un relleno?

En OpenGL, un relleno se logra dibujando primitivas como `GL_QUADS`, `GL_TRIANGLES`, o `GL_POLYGON` con colores asignados. El tipo de sombreado (shading) controla cómo se aplica el color:

- **GL_FLAT**: Un solo color por primitiva (sin interpolación)
- **GL_SMOOTH**: Interpolación de colores entre vértices (gradiente)

## Tipo de relleno

### 1. Relleno Sólido (Solid Fill)
Un solo color uniforme aplicado a toda la superficie de la figura. Es el relleno más básico y común.

```cpp
glShadeModel(GL_FLAT);
glColor3f(0.0f, 0.8f, 1.0f);
glBegin(GL_QUADS);
glVertex3f(0,0,0); glVertex3f(1,0,0);
glVertex3f(1,1,0); glVertex3f(0,1,0);
glEnd();
```

### 2. Relleno Degradado (Gradient Fill)
Interpolación suave de colores entre los vértices de una primitiva. OpenGL calcula automáticamente la mezcla de colores en cada píxel.

```cpp
glShadeModel(GL_SMOOTH);
glBegin(GL_QUADS);
glColor3f(1,0,0); glVertex3f(0,0,0);
glColor3f(0,1,0); glVertex3f(1,0,0);
glColor3f(0,0,1); glVertex3f(1,1,0);
glColor3f(1,1,0); glVertex3f(0,1,0);
glEnd();
```

### 3. Relleno de Patrón (Pattern Fill)
Relleno basado en una repetición de un patrón o diseño, como un tablero de ajedrez o una textura procedural.

```cpp
int divs = 4; // Dividir cada cara en 4x4
for (int i = 0; i < divs; i++) {
    for (int j = 0; j < divs; j++) {
        if ((i + j) % 2 == 0)
            glColor3f(1.0f, 0.5f, 0.0f); // Naranja
        else
            glColor3f(0.3f, 0.3f, 0.8f); // Azul
        // Dibujar quad pequeño
    }
}
```

---

## ¿Qué es una sombra?

Una sombra en gráficos 3D es la representación visual de la ausencia de luz en un área debido a que un objeto bloquea la fuente de luz. Las sombras proporcionan profundidad y realismo a las escenas 3D.

En OpenGL clásico, las sombras se pueden implementar mediante:
- **Shadow mapping**: Usar el buffer de profundidad desde la perspectiva de la luz
- **Planar shadows**: Proyectar la geometría del objeto sobre un plano
- **Volumen de sombra**: Usar stencil buffer para calcular áreas en sombra

## ¿Cómo hago una sombra?

Para este proyecto, implementamos sombras planares proyectando la geometría del cubo sobre el plano XZ (suelo) usando una matriz de proyección de sombra.

## Tipos de sombras

1. **Sombras planares**: Proyección de la geometría sobre un plano
2. **Shadow mapping**: Mapeo de profundidad desde la luz (técnica más avanzada)
3. **Sombra suave (Soft shadow)**: Bordes difuminados de la sombra

---

## Implementación en el proyecto

Los ejemplos de relleno y sombras se han implementado en el programa principal (`main.cpp`):

- **Relleno Sólido**: Cubo izquierdo (cyan)
- **Relleno Degradado**: Cubo central (colores interpolados por vértice)
- **Relleno de Patrón**: Cubo derecho (patrón de ajedrez 4x4)
- **Sombra**: Sombra proyectada en el suelo usando matriz de proyección

### Controles de cámara:
- `1`: Vista perspectiva (orbitar con flechas)
- `2`: Vista superior (ortográfica)
- `3`: Vista inferior (ortográfica)
- `4`: Vista frontal (ortográfica)
- `5`: Vista trasera (ortográfica)
- `6`: Vista izquierda (ortográfica)
- `7`: Vista derecha (ortográfica)
- `←/→/↑/↓`: Orbitar cámara en modo perspectiva
