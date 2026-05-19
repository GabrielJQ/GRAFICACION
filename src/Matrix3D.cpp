#include "Matrix3D.h"
#include <cmath>
#include <cstring>

Matrix3D::Matrix3D() {
    // Inicializar todos los elementos a 0
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m[i][j] = 0.0f;
        }
    }
}

Matrix3D::Matrix3D(bool identity) {
    // Inicializar todos los elementos a 0 primero
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m[i][j] = 0.0f;
        }
    }
    
    // Si se pide una matriz de identidad, asignar 1s en la diagonal
    if (identity) {
        for (int i = 0; i < 4; i++) {
            m[i][i] = 1.0f;
        }
    }
}

Matrix3D Matrix3D::identity() {
    return Matrix3D(true);
}

Matrix3D Matrix3D::operator*(const Matrix3D& other) const {
    Matrix3D result;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.m[i][j] = 0;
            for (int k = 0; k < 4; k++) {
                result.m[i][j] += this->m[i][k] * other.m[k][j];
            }
        }
    }
    return result;
}

Matrix3D Matrix3D::scale(float sx, float sy, float sz) {
    Matrix3D mat(true);
    mat.m[0][0] = sx;
    mat.m[1][1] = sy;
    mat.m[2][2] = sz;
    return mat;
}

Matrix3D Matrix3D::translate(float tx, float ty, float tz) {
    Matrix3D mat(true);
    mat.m[0][3] = tx;
    mat.m[1][3] = ty;
    mat.m[2][3] = tz;
    return mat;
}

Matrix3D Matrix3D::rotateX(float angle) {
    Matrix3D mat(true);
    float c = std::cos(angle);
    float s = std::sin(angle);
    mat.m[1][1] = c;
    mat.m[1][2] = -s;
    mat.m[2][1] = s;
    mat.m[2][2] = c;
    return mat;
}

Matrix3D Matrix3D::rotateY(float angle) {
    Matrix3D mat(true);
    float c = std::cos(angle);
    float s = std::sin(angle);
    mat.m[0][0] = c;
    mat.m[0][2] = s;
    mat.m[2][0] = -s;
    mat.m[2][2] = c;
    return mat;
}

Matrix3D Matrix3D::rotateZ(float angle) {
    Matrix3D mat(true);
    float c = std::cos(angle);
    float s = std::sin(angle);
    mat.m[0][0] = c;
    mat.m[0][1] = -s;
    mat.m[1][0] = s;
    mat.m[1][1] = c;
    return mat;
}

Matrix3D Matrix3D::shear(float shx, float shy, float shz) {
    Matrix3D mat(true);
    mat.m[0][1] = shx;
    mat.m[1][0] = shy;
    mat.m[2][0] = shz;
    return mat;
}

Matrix3D Matrix3D::scaleThenTranslate(float sx, float sy, float sz, float tx, float ty, float tz) {
    Matrix3D S = scale(sx, sy, sz);
    Matrix3D T = translate(tx, ty, tz);
    return T * S;
}

Matrix3D Matrix3D::translateThenScale(float tx, float ty, float tz, float sx, float sy, float sz) {
    Matrix3D T = translate(tx, ty, tz);
    Matrix3D S = scale(sx, sy, sz);
    return S * T;
}