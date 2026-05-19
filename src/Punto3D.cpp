#include "Punto3D.h"
#include <iostream>

Punto3D::Punto3D() : x(0), y(0), z(0) {}

Punto3D::Punto3D(float x, float y, float z) : x(x), y(y), z(z) {}

Punto3D Punto3D::operator*(const Matrix3D& mat) const {
    float wx = mat.m[0][0] * x + mat.m[0][1] * y + mat.m[0][2] * z + mat.m[0][3];
    float wy = mat.m[1][0] * x + mat.m[1][1] * y + mat.m[1][2] * z + mat.m[1][3];
    float wz = mat.m[2][0] * x + mat.m[2][1] * y + mat.m[2][2] * z + mat.m[2][3];
    return Punto3D(wx, wy, wz);
}

Punto3D Punto3D::operator+(const Punto3D& other) const {
    return Punto3D(x + other.x, y + other.y, z + other.z);
}

Punto3D Punto3D::operator-(const Punto3D& other) const {
    return Punto3D(x - other.x, y - other.y, z - other.z);
}

Punto3D Punto3D::operator*(float scalar) const {
    return Punto3D(x * scalar, y * scalar, z * scalar);
}

void Punto3D::print() const {
    std::cout << "(" << x << ", " << y << ", " << z << ")";
}