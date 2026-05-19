#include "Renderer3D.h"
#include <GLFW/glfw3.h>
#include <cmath>

Renderer3D::Renderer3D(int w, int h, const char* title) : width(w), height(h) {
    if (!glfwInit()) {
        window = nullptr;
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        window = nullptr;
        return;
    }

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glEnable(GL_DEPTH_TEST);
}

Renderer3D::~Renderer3D() {
    if (window) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}

bool Renderer3D::shouldClose() const {
    return window && glfwWindowShouldClose(window);
}

void Renderer3D::swapBuffers() {
    if (window) glfwSwapBuffers(window);
}

void Renderer3D::pollEvents() {
    if (window) glfwPollEvents();
}

void Renderer3D::clear() {
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer3D::setProjection(float fov, float aspect, float zNear, float zFar) {
    float tanHalfFov = tanf(fov * 0.5f);
    float yScale = 1.0f / tanHalfFov;
    float xScale = yScale / aspect;
    float zRange = zFar - zNear;

    float projMatrix[16] = {
        xScale, 0.0f, 0.0f, 0.0f,
        0.0f, yScale, 0.0f, 0.0f,
        0.0f, 0.0f, -(zFar + zNear) / zRange, -1.0f,
        0.0f, 0.0f, -2.0f * zFar * zNear / zRange, 0.0f
    };
    glMultMatrixf(projMatrix);
}

void Renderer3D::setView(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ) {
    float fx = eyeX - centerX;
    float fy = eyeY - centerY;
    float fz = eyeZ - centerZ;
    float fLen = sqrtf(fx*fx + fy*fy + fz*fz);
    float fnx = fx/fLen, fny = fy/fLen, fnz = fz/fLen;

    float rx = fny*0.0f - fnz*0.0f;
    float ry = fnz*0.0f - fnx*0.0f;
    float rz = fnx*0.0f - fny*1.0f;
    float rLen = sqrtf(rx*rx + ry*ry + rz*rz);
    float rnx = rx/rLen, rny = ry/rLen, rnz = rz/rLen;

    float ux = rny*fnz - rnz*fny;
    float uy = rnz*fnx - rnx*fnz;
    float uz = rnx*fny - rny*fnx;

    float viewMatrix[16] = {
        rnx, ux, -fnx, 0.0f,
        rny, uy, -fny, 0.0f,
        rnz, uz, -fnz, 0.0f,
        -(rnx*eyeX + rny*eyeY + rnz*eyeZ),
        -(ux*eyeX + uy*eyeY + uz*eyeZ),
        (fnx*eyeX + fny*eyeY + fnz*eyeZ),
        1.0f
    };
    glMultMatrixf(viewMatrix);
}

static void applyMatrix(const Matrix3D& mat) {
    float m[16] = {
        mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
        mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1],
        mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
        mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3]
    };
    glMultMatrixf(m);
}

static void drawUnitCube(float r, float g, float b) {
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0f, 1.0f);
    glColor3f(r*0.4f, g*0.4f, b*0.4f);
    
    glBegin(GL_QUADS);
    
    glVertex3f(0, 0, 0); glVertex3f(0, 1, 0); glVertex3f(1, 1, 0); glVertex3f(1, 0, 0);
    glVertex3f(0, 0, 1); glVertex3f(1, 0, 1); glVertex3f(1, 1, 1); glVertex3f(0, 1, 1);
    
    glVertex3f(0, 1, 0); glVertex3f(0, 1, 1); glVertex3f(1, 1, 1); glVertex3f(1, 1, 0);
    glVertex3f(0, 0, 0); glVertex3f(1, 0, 0); glVertex3f(1, 0, 1); glVertex3f(0, 0, 1);
    
    glVertex3f(0, 0, 0); glVertex3f(0, 0, 1); glVertex3f(0, 1, 1); glVertex3f(0, 1, 0);
    glVertex3f(1, 0, 0); glVertex3f(1, 1, 0); glVertex3f(1, 1, 1); glVertex3f(1, 0, 1);
    
    glEnd();

    glDisable(GL_POLYGON_OFFSET_FILL);

    glColor3f(r, g, b);
    glLineWidth(2.0f);
    glBegin(GL_LINES);

    glVertex3f(0, 0, 0); glVertex3f(1, 0, 0);
    glVertex3f(1, 0, 0); glVertex3f(1, 1, 0);
    glVertex3f(1, 1, 0); glVertex3f(0, 1, 0);
    glVertex3f(0, 1, 0); glVertex3f(0, 0, 0);

    glVertex3f(0, 0, 1); glVertex3f(1, 0, 1);
    glVertex3f(1, 0, 1); glVertex3f(1, 1, 1);
    glVertex3f(1, 1, 1); glVertex3f(0, 1, 1);
    glVertex3f(0, 1, 1); glVertex3f(0, 0, 1);

    glVertex3f(0, 0, 0); glVertex3f(0, 0, 1);
    glVertex3f(1, 0, 0); glVertex3f(1, 0, 1);
    glVertex3f(1, 1, 0); glVertex3f(1, 1, 1);
    glVertex3f(0, 1, 0); glVertex3f(0, 1, 1);

    glEnd();

    glPointSize(6.0f);
    glBegin(GL_POINTS);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);
    glVertex3f(1, 1, 0);
    glVertex3f(0, 1, 0);
    glVertex3f(0, 0, 1);
    glVertex3f(1, 0, 1);
    glVertex3f(1, 1, 1);
    glVertex3f(0, 1, 1);
    glEnd();
}

void Renderer3D::drawCube(float size, const Matrix3D& transform, float r, float g, float b) {
    glColor3f(r, g, b);
    glPushMatrix();
    applyMatrix(transform);
    glScalef(size, size, size);
    drawUnitCube(r, g, b);
    glPopMatrix();
}

void Renderer3D::drawAxis(float length) {
    glBegin(GL_LINES);

    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(length, 0, 0);

    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, length, 0);

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, length);

    glEnd();
}

void Renderer3D::drawGrid(float size, float divisions) {
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_LINES);

    float step = size / divisions;
    for (float i = 0; i <= divisions; i++) {
        float pos = -size * 0.5f + i * step;
        glVertex3f(pos, 0, -size * 0.5f);
        glVertex3f(pos, 0, size * 0.5f);
        glVertex3f(-size * 0.5f, 0, pos);
        glVertex3f(size * 0.5f, 0, pos);
    }

    glEnd();
}