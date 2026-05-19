#include <iostream>
#include <cmath>
#include <iomanip>
#include <GLFW/glfw3.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wbuiltin-macro-redefined"
#include <glad/glad.h>
#pragma GCC diagnostic pop
#include <GL/glu.h>
#include "Matrix3D.h"

const float PI = 3.14159265359f;

float camAngleH = 0.5f;
float camAngleV = 0.3f;
float camDist = 6.0f;
float mouseSensitivity = 0.003f;
double lastMouseX = 0, lastMouseY = 0;
bool mousePressed = false;

void printMatrix(const Matrix3D& mat) {
    std::cout << "\n=== MATRIZ RESULTANTE ===\n";
    for (int i = 0; i < 4; i++) {
        std::cout << "| ";
        for (int j = 0; j < 4; j++) {
            std::cout << std::setw(10) << std::setprecision(4) << std::fixed << mat.m[i][j] << " ";
        }
        std::cout << "|\n";
    }
    std::cout << std::endl;
}

Matrix3D getOperacion(int opcion) {
    Matrix3D mat = Matrix3D::identity();
    if (opcion == 1) {
        float sx, sy, sz;
        std::cout << "Sx Sy Sz: ";
        std::cin >> sx >> sy >> sz;
        mat = Matrix3D::scale(sx, sy, sz);
        std::cout << "\n>>> ESCALAMIENTO APLICADO\n";
        printMatrix(mat);
    }
    else if (opcion == 2) {
        float angX, angY, angZ;
        std::cout << "AngX AngY AngZ (grados): ";
        std::cin >> angX >> angY >> angZ;
        angX *= PI / 180.0f;
        angY *= PI / 180.0f;
        angZ *= PI / 180.0f;
        Matrix3D rx = Matrix3D::rotateX(angX);
        Matrix3D ry = Matrix3D::rotateY(angY);
        Matrix3D rz = Matrix3D::rotateZ(angZ);
        mat = rz * ry * rx;
        std::cout << "\n>>> ROTACION APLICADA\n";
        printMatrix(mat);
    }
    else if (opcion == 3) {
        float tx, ty, tz;
        std::cout << "Tx Ty Tz: ";
        std::cin >> tx >> ty >> tz;
        mat = Matrix3D::translate(tx, ty, tz);
        std::cout << "\n>>> TRASLACION APLICADA\n";
        printMatrix(mat);
    }
    else if (opcion == 4) {
        float shx, shy, shz;
        std::cout << "ShX ShY ShZ: ";
        std::cin >> shx >> shy >> shz;
        mat = Matrix3D::shear(shx, shy, shz);
        std::cout << "\n>>> SESGO APLICADO\n";
        printMatrix(mat);
    }
    return mat;
}

// ====== RELLENO SÓLIDO (Solid Fill) ======
void drawSolidFillCube() {
    glShadeModel(GL_FLAT);
    float r = 0.0f, g = 0.8f, b = 1.0f;

    glBegin(GL_QUADS);
    glColor3f(r, g, b);
    glVertex3f(0,0,0); glVertex3f(1,0,0); glVertex3f(1,1,0); glVertex3f(0,1,0);
    glColor3f(r*0.8f, g*0.8f, b*0.8f);
    glVertex3f(0,0,1); glVertex3f(0,1,1); glVertex3f(1,1,1); glVertex3f(1,0,1);
    glColor3f(r*0.6f, g*0.6f, b*0.6f);
    glVertex3f(0,0,0); glVertex3f(0,0,1); glVertex3f(0,1,1); glVertex3f(0,1,0);
    glColor3f(r*0.7f, g*0.7f, b*0.7f);
    glVertex3f(1,0,0); glVertex3f(1,1,0); glVertex3f(1,1,1); glVertex3f(1,0,1);
    glColor3f(r*0.9f, g*0.9f, b*0.9f);
    glVertex3f(0,1,0); glVertex3f(0,1,1); glVertex3f(1,1,1); glVertex3f(1,1,0);
    glColor3f(r*0.4f, g*0.4f, b*0.4f);
    glVertex3f(0,0,0); glVertex3f(1,0,0); glVertex3f(1,0,1); glVertex3f(0,0,1);
    glEnd();

    glColor3f(1,1,1);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    glVertex3f(0,0,0); glVertex3f(1,0,0);
    glVertex3f(1,0,0); glVertex3f(1,1,0);
    glVertex3f(1,1,0); glVertex3f(0,1,0);
    glVertex3f(0,1,0); glVertex3f(0,0,0);
    glVertex3f(0,0,1); glVertex3f(1,0,1);
    glVertex3f(1,0,1); glVertex3f(1,1,1);
    glVertex3f(1,1,1); glVertex3f(0,1,1);
    glVertex3f(0,1,1); glVertex3f(0,0,1);
    glVertex3f(0,0,0); glVertex3f(0,0,1);
    glVertex3f(1,0,0); glVertex3f(1,0,1);
    glVertex3f(1,1,0); glVertex3f(1,1,1);
    glVertex3f(0,1,0); glVertex3f(0,1,1);
    glEnd();
}

// ====== RELLENO DEGRADADO (Gradient Fill) ======
void drawGradientCube() {
    glShadeModel(GL_SMOOTH);

    glBegin(GL_QUADS);
    glColor3f(1,0,0); glVertex3f(0,0,0);
    glColor3f(0,1,0); glVertex3f(1,0,0);
    glColor3f(0,0,1); glVertex3f(1,1,0);
    glColor3f(1,1,0); glVertex3f(0,1,0);

    glColor3f(1,0,1); glVertex3f(0,0,1);
    glColor3f(0,1,1); glVertex3f(1,0,1);
    glColor3f(1,0.5f,0); glVertex3f(1,1,1);
    glColor3f(0.2f,0.8f,0.2f); glVertex3f(0,1,1);

    glColor3f(0.5f,0,0.5f); glVertex3f(0,0,0);
    glColor3f(0.5f,0.5f,0); glVertex3f(0,0,1);
    glColor3f(0,0.5f,0.5f); glVertex3f(0,1,1);
    glColor3f(0.8f,0.2f,0.2f); glVertex3f(0,1,0);

    glColor3f(0.2f,0.2f,0.8f); glVertex3f(1,0,0);
    glColor3f(0.8f,0.8f,0.2f); glVertex3f(1,0,1);
    glColor3f(0.2f,0.8f,0.8f); glVertex3f(1,1,1);
    glColor3f(0.8f,0.2f,0.8f); glVertex3f(1,1,0);

    glColor3f(0,0.6f,0); glVertex3f(0,1,0);
    glColor3f(0,0,0.6f); glVertex3f(0,1,1);
    glColor3f(0.6f,0,0); glVertex3f(1,1,1);
    glColor3f(0.6f,0.6f,0); glVertex3f(1,1,0);

    glColor3f(0.4f,0,0.4f); glVertex3f(0,0,0);
    glColor3f(0,0.4f,0.4f); glVertex3f(1,0,0);
    glColor3f(0.4f,0.4f,0); glVertex3f(1,0,1);
    glColor3f(0,0,0.4f); glVertex3f(0,0,1);
    glEnd();

    glShadeModel(GL_FLAT);

    glColor3f(1,1,1);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    glVertex3f(0,0,0); glVertex3f(1,0,0);
    glVertex3f(1,0,0); glVertex3f(1,1,0);
    glVertex3f(1,1,0); glVertex3f(0,1,0);
    glVertex3f(0,1,0); glVertex3f(0,0,0);
    glVertex3f(0,0,1); glVertex3f(1,0,1);
    glVertex3f(1,0,1); glVertex3f(1,1,1);
    glVertex3f(1,1,1); glVertex3f(0,1,1);
    glVertex3f(0,1,1); glVertex3f(0,0,1);
    glVertex3f(0,0,0); glVertex3f(0,0,1);
    glVertex3f(1,0,0); glVertex3f(1,0,1);
    glVertex3f(1,1,0); glVertex3f(1,1,1);
    glVertex3f(0,1,0); glVertex3f(0,1,1);
    glEnd();
}

// ====== RELLENO DE PATRÓN (Pattern Fill - tipo ajedrez) ======
void drawPatternCube() {
    const int DIVS = 4;
    float step = 1.0f / DIVS;

    for (int i = 0; i < DIVS; i++) {
        for (int j = 0; j < DIVS; j++) {
            float x0 = i*step, x1 = (i+1)*step;
            float y0 = j*step, y1 = (j+1)*step;
            float c = ((i + j) % 2 == 0) ? 1.0f : 0.3f;

            glBegin(GL_QUADS);
            glColor3f(1.0f*c, 0.5f*c, 0.0f);
            glVertex3f(x0,y0,0); glVertex3f(x1,y0,0);
            glVertex3f(x1,y1,0); glVertex3f(x0,y1,0);

            glColor3f(0.0f, 0.5f*c, 1.0f*c);
            glVertex3f(x0,y0,1); glVertex3f(x0,y1,1);
            glVertex3f(x1,y1,1); glVertex3f(x1,y0,1);

            glColor3f(0.5f*c, 0.0f, 1.0f*c);
            glVertex3f(0,y0,x0); glVertex3f(0,y1,x0);
            glVertex3f(0,y1,x1); glVertex3f(0,y0,x1);

            glColor3f(1.0f*c, 0.0f, 0.5f*c);
            glVertex3f(1,y0,x0); glVertex3f(1,y0,x1);
            glVertex3f(1,y1,x1); glVertex3f(1,y1,x0);

            glColor3f(0.0f, 1.0f*c, 0.5f*c);
            glVertex3f(x0,1,y0); glVertex3f(x1,1,y0);
            glVertex3f(x1,1,y1); glVertex3f(x0,1,y1);

            glColor3f(0.5f*c, 1.0f*c, 0.0f);
            glVertex3f(x0,0,y0); glVertex3f(x0,0,y1);
            glVertex3f(x1,0,y1); glVertex3f(x1,0,y0);
            glEnd();
        }
    }

    glColor3f(0.5f,0.5f,0.5f);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    glVertex3f(0,0,0); glVertex3f(1,0,0);
    glVertex3f(1,0,0); glVertex3f(1,1,0);
    glVertex3f(1,1,0); glVertex3f(0,1,0);
    glVertex3f(0,1,0); glVertex3f(0,0,0);
    glVertex3f(0,0,1); glVertex3f(1,0,1);
    glVertex3f(1,0,1); glVertex3f(1,1,1);
    glVertex3f(1,1,1); glVertex3f(0,1,1);
    glVertex3f(0,1,1); glVertex3f(0,0,1);
    glVertex3f(0,0,0); glVertex3f(0,0,1);
    glVertex3f(1,0,0); glVertex3f(1,0,1);
    glVertex3f(1,1,0); glVertex3f(1,1,1);
    glVertex3f(0,1,0); glVertex3f(0,1,1);
    glEnd();
}

// ====== SOMBRA PROYECTADA (Planar Shadow) ======
void drawShadow(float size, const Matrix3D& transform, float lightX, float lightY, float lightZ) {
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float dx = lightX, dy = lightY, dz = lightZ;
    float shadowMat[16] = {
        -dy, 0, 0, 0,
        0, -dy, 0, 0,
        0, 0, -dy, 0,
        dx, 0, dz, 0
    };

    glPushMatrix();
    glMultMatrixf(shadowMat);

    float glMat[16];
    for (int col = 0; col < 4; col++)
        for (int row = 0; row < 4; row++)
            glMat[col * 4 + row] = transform.m[row][col];
    glMultMatrixf(glMat);
    glScalef(size, size, size);

    glColor4f(0.0f, 0.0f, 0.0f, 0.3f);
    glBegin(GL_QUADS);
    glVertex3f(0,0,0); glVertex3f(1,0,0); glVertex3f(1,1,0); glVertex3f(0,1,0);
    glVertex3f(0,0,1); glVertex3f(0,1,1); glVertex3f(1,1,1); glVertex3f(1,0,1);
    glVertex3f(0,0,0); glVertex3f(0,0,1); glVertex3f(0,1,1); glVertex3f(0,1,0);
    glVertex3f(1,0,0); glVertex3f(1,1,0); glVertex3f(1,1,1); glVertex3f(1,0,1);
    glVertex3f(0,1,0); glVertex3f(0,1,1); glVertex3f(1,1,1); glVertex3f(1,1,0);
    glVertex3f(0,0,0); glVertex3f(1,0,0); glVertex3f(1,0,1); glVertex3f(0,0,1);
    glEnd();

    glPopMatrix();

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

// ====== PISO (grid con transparencia) ======
void drawFloor() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.3f, 0.3f, 0.4f, 0.5f);
    glBegin(GL_LINES);
    float size = 6.0f;
    float step = 0.5f;
    for (float i = -size; i <= size; i += step) {
        glVertex3f(i, 0, -size);
        glVertex3f(i, 0, size);
        glVertex3f(-size, 0, i);
        glVertex3f(size, 0, i);
    }
    glEnd();
    glDisable(GL_BLEND);
}

Matrix3D buildTransform(float scale, float tx, float ty, float tz, float shx, float shy, float shz, float rotX, float rotY, float rotZ) {
    Matrix3D S = Matrix3D::scale(scale, scale, scale);
    Matrix3D RX = Matrix3D::rotateX(rotX);
    Matrix3D RY = Matrix3D::rotateY(rotY);
    Matrix3D RZ = Matrix3D::rotateZ(rotZ);
    Matrix3D R = RZ * RY * RX;
    Matrix3D H = Matrix3D::shear(shx, shy, shz);
    Matrix3D T = Matrix3D::translate(tx, ty, tz);
    Matrix3D center = Matrix3D::translate(0.5f, 0.5f, 0.5f);
    Matrix3D uncenter = Matrix3D::translate(-0.5f, -0.5f, -0.5f);
    return T * center * R * S * H * uncenter;
}

void applyGLMatrix(const Matrix3D& mat) {
    float glMat[16];
    for (int col = 0; col < 4; col++)
        for (int row = 0; row < 4; row++)
            glMat[col * 4 + row] = mat.m[row][col];
    glMultMatrixf(glMat);
}

void setCameraView(float angleH, float angleV, float dist, int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float ch = cosf(angleH), sh = sinf(angleH);
    float cv = cosf(angleV), sv = sinf(angleV);
    float eyeX = dist * cv * ch;
    float eyeY = dist * sv;
    float eyeZ = dist * cv * sh;

    gluLookAt(eyeX, eyeY, eyeZ, 0, 0.2f, 0, 0, 1, 0);
}

void run() {
    if (!glfwInit()) {
        std::cerr << "Error GLFW\n";
        return;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Unidad 4: Rellenos y Sombras", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetMouseButtonCallback(window, [](GLFWwindow* w, int button, int action, int) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            mousePressed = (action == GLFW_PRESS);
            if (mousePressed) glfwGetCursorPos(w, &lastMouseX, &lastMouseY);
        }
    });
    glfwSetCursorPosCallback(window, [](GLFWwindow*, double xpos, double ypos) {
        if (!mousePressed) return;
        double dx = xpos - lastMouseX;
        double dy = ypos - lastMouseY;
        lastMouseX = xpos;
        lastMouseY = ypos;
        camAngleH -= (float)dx * mouseSensitivity;
        camAngleV += (float)dy * mouseSensitivity;
        if (camAngleV > 1.5f) camAngleV = 1.5f;
        if (camAngleV < -1.5f) camAngleV = -1.5f;
    });

    float scaleValue = 1.0f;
    float tx = 0.0f, ty = 0.0f, tz = 0.0f;
    float shx = 0.0f, shy = 0.0f, shz = 0.0f;
    float rotX = 0.0f, rotY = 0.0f, rotZ = 0.0f;

    const float scaleStep = 0.01f;
    const float translateStep = 0.02f;
    const float shearStep = 0.01f;
    const float rotateStep = 0.02f;

    std::cout << "\n========== UNIDAD 4: RELLENOS Y SOMBRAS ==========\n";
    std::cout << "Objetos en escena:\n";
    std::cout << "  Izquierda: RELLENO SOLIDO (color uniforme por cara)\n";
    std::cout << "  Centro:    RELLENO DEGRADADO (colores interpolados)\n";
    std::cout << "  Derecha:   RELLENO DE PATRON (tablero 4x4)\n";
    std::cout << "\n=== CONTROL DE CAMARA ===\n";
    std::cout << "Click izquierdo + arrastrar raton: orbitar camara\n";
    std::cout << "\n=== TRANSFORMACIONES (objeto central) ===\n";
    std::cout << "q/e: escalar   w/a/s/d: trasladar XY\n";
    std::cout << "z/x: trasladar Z   f/v: rotar X\n";
    std::cout << "g/b: rotar Y   h/n: rotar Z\n";
    std::cout << "i/k: sesgar X   j/l: sesgar Y   u/o: sesgar Z\n";
    std::cout << "r: reiniciar   ESC: salir\n";

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, 1);

        bool changed = false;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) { scaleValue += scaleStep; changed = true; }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) { scaleValue = std::max(0.1f, scaleValue - scaleStep); changed = true; }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) { ty += translateStep; changed = true; }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { ty -= translateStep; changed = true; }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) { tx -= translateStep; changed = true; }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) { tx += translateStep; changed = true; }
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) { tz += translateStep; changed = true; }
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) { tz -= translateStep; changed = true; }
        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) { shx += shearStep; changed = true; }
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) { shx -= shearStep; changed = true; }
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) { shy += shearStep; changed = true; }
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) { shy -= shearStep; changed = true; }
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) { shz += shearStep; changed = true; }
        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) { shz -= shearStep; changed = true; }
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) { rotX += rotateStep; changed = true; }
        if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) { rotX -= rotateStep; changed = true; }
        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) { rotY += rotateStep; changed = true; }
        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) { rotY -= rotateStep; changed = true; }
        if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) { rotZ += rotateStep; changed = true; }
        if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) { rotZ -= rotateStep; changed = true; }
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
            scaleValue = 1.0f;
            tx = ty = tz = 0.0f;
            shx = shy = shz = 0.0f;
            rotX = rotY = rotZ = 0.0f;
            changed = true;
        }

        if (changed) {
            std::cout << "\rEscala=" << scaleValue
                      << "  Tx=" << tx << " Ty=" << ty << " Tz=" << tz
                      << "  RotX=" << rotX << " RotY=" << rotY << " RotZ=" << rotZ
                      << "  Shx=" << shx << " Shy=" << shy << " Shz=" << shz
                      << "      ";
            std::cout.flush();
        }

        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        glViewport(0, 0, w, h);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        setCameraView(camAngleH, camAngleV, camDist, w, h);

        // Light position for shadow
        float lx = 2.0f, ly = 5.0f, lz = 3.0f;

        // -- Draw floor
        drawFloor();

        // ====== 1. RELLENO SOLIDO (izquierda) ======
        glPushMatrix();
        glTranslatef(-2.5f, -0.5f, -0.5f);
        drawSolidFillCube();
        glPopMatrix();

        // ====== 2. RELLENO DEGRADADO (centro) ======
        Matrix3D gradientTrans = buildTransform(scaleValue, tx, ty, tz, shx, shy, shz, rotX, rotY, rotZ);
        Matrix3D gradientPos = Matrix3D::translate(0.0f, 0.0f, 0.0f);
        Matrix3D gradientFinal = gradientPos * gradientTrans;

        glPushMatrix();
        glTranslatef(0.0f, -0.5f, -0.5f);
        applyGLMatrix(gradientTrans);
        drawGradientCube();
        glPopMatrix();

        // Shadow for gradient cube
        drawShadow(1.0f, gradientTrans, lx, ly, lz);

        // ====== 3. RELLENO DE PATRON (derecha) ======
        glPushMatrix();
        glTranslatef(2.5f, -0.5f, -0.5f);
        drawPatternCube();
        glPopMatrix();

        // Labels for fill types
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, w, 0, h, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glDisable(GL_DEPTH_TEST);
        glColor3f(0.8f, 0.8f, 0.8f);
        // We'd need a text rendering function - skip for now

        glEnable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}

int main() { run(); return 0; }
