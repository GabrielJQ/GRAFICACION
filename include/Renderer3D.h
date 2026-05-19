#ifndef RENDERER3D_H
#define RENDERER3D_H

#include "Matrix3D.h"
#include <vector>
#include <GLFW/glfw3.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wbuiltin-macro-redefined"
#include <glad/glad.h>
#pragma GCC diagnostic pop

class Renderer3D {
public:
    GLFWwindow* window;
    int width, height;

    Renderer3D(int w, int h, const char* title);
    ~Renderer3D();

    bool shouldClose() const;
    void swapBuffers();
    void pollEvents();

    void clear();
    void setProjection(float fov, float aspect, float zNear, float zFar);
    void setView(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ);

    void drawCube(float size, const Matrix3D& transform, float r, float g, float b);
    void drawAxis(float length);
    void drawGrid(float size, float divisions);
};

#endif