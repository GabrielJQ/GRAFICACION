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

void drawCube(float r, float g, float b) {
    glColor3f(r*0.3f, g*0.3f, b*0.3f);
    glBegin(GL_QUADS);
    glVertex3f(0,0,0); glVertex3f(1,0,0); glVertex3f(1,1,0); glVertex3f(0,1,0);
    glVertex3f(0,0,1); glVertex3f(1,0,1); glVertex3f(1,1,1); glVertex3f(0,1,1);
    glVertex3f(0,0,0); glVertex3f(0,1,0); glVertex3f(0,1,1); glVertex3f(0,0,1);
    glVertex3f(1,0,0); glVertex3f(1,1,0); glVertex3f(1,1,1); glVertex3f(1,0,1);
    glVertex3f(0,1,0); glVertex3f(1,1,0); glVertex3f(1,1,1); glVertex3f(0,1,1);
    glVertex3f(0,0,0); glVertex3f(1,0,0); glVertex3f(1,0,1); glVertex3f(0,0,1);
    glEnd();

    glColor3f(r, g, b);
    glLineWidth(2.0f);
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

void run() {
    if (!glfwInit()) {
        std::cerr << "Error GLFW\n";
        return;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "3D", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);

    float camAngle = 0.5f;
    float camDist = 6.0f;

    float scaleValue = 1.0f;
    float tx = 0.0f, ty = 0.0f, tz = 0.0f;
    float shx = 0.0f, shy = 0.0f, shz = 0.0f;
    float rotX = 0.0f, rotY = 0.0f, rotZ = 0.0f;

    const float scaleStep = 0.01f;
    const float translateStep = 0.02f;
    const float shearStep = 0.01f;
    const float rotateStep = 0.02f;

    std::cout << "\n=== CONTROL POR TECLAS ===\n";
    std::cout << "q/e: escalar (+/-)\n";
    std::cout << "w/a/s/d: trasladar (arriba/izquierda/abajo/derecha)\n";
    std::cout << "z/x: trasladar en z (adelante/atras)\n";
    std::cout << "f/v: rotar en X (+/-)\n";
    std::cout << "g/b: rotar en Y (+/-)\n";
    std::cout << "h/n: rotar en Z (+/-)\n";
    std::cout << "i/k: sesgar en X (+/-)\n";
    std::cout << "j/l: sesgar en Y (+/-)\n";
    std::cout << "u/o: sesgar en Z (+/-)\n";
    std::cout << "r: reiniciar transformacion\n";
    std::cout << "ESC: salir\n";

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, 1);

        bool changed = false;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            scaleValue += scaleStep;
            changed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            scaleValue = std::max(0.1f, scaleValue - scaleStep);
            changed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            ty += translateStep;
            changed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            ty -= translateStep;
            changed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            tx -= translateStep;
            changed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            tx += translateStep;
            changed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
            tz += translateStep;
            changed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
            tz -= translateStep;
            changed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
            shx += shearStep;
            changed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
            shx -= shearStep;
            changed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
            shy += shearStep;
            changed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
            shy -= shearStep;
            changed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
            shz += shearStep;
            changed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
            shz -= shearStep;
            changed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
            rotX += rotateStep;
            changed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
            rotX -= rotateStep;
            changed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
            rotY += rotateStep;
            changed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
            rotY -= rotateStep;
            changed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
            rotZ += rotateStep;
            changed = true;
        }
        if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
            rotZ -= rotateStep;
            changed = true;
        }
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

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0, (float)w / h, 0.1, 100.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(camDist * cosf(camAngle), 0, camDist * sinf(camAngle),
                  0, 0, 0,
                  0, 1, 0);

        glPushMatrix();
        glTranslatef(-1.2f, -0.5f, -0.5f);
        drawCube(0.0f, 0.8f, 1.0f);
        glPopMatrix();

        glPushMatrix();
        Matrix3D transformacion = buildTransform(scaleValue, tx, ty, tz, shx, shy, shz, rotX, rotY, rotZ);
        Matrix3D posicion = Matrix3D::translate(1.2f, 0.0f, 0.0f);
        Matrix3D matrizFinal = posicion * transformacion;
        
        float glMat[16];
        for (int col = 0; col < 4; col++) {
            for (int row = 0; row < 4; row++) {
                glMat[col * 4 + row] = matrizFinal.m[row][col];
            }
        }
        glMultMatrixf(glMat);
        drawCube(1.0f, 0.4f, 0.2f);
        glPopMatrix();

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}

int main() { run(); return 0; }
