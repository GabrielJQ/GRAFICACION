#ifndef PUNTO3D_H
#define PUNTO3D_H

#include "Matrix3D.h"

class Punto3D {
public:
    float x, y, z;

    Punto3D();
    Punto3D(float x, float y, float z);

    Punto3D operator*(const Matrix3D& mat) const;
    Punto3D operator+(const Punto3D& other) const;
    Punto3D operator-(const Punto3D& other) const;
    Punto3D operator*(float scalar) const;

    void print() const;
};

#endif