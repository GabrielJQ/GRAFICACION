#ifndef MATRIX3D_H
#define MATRIX3D_H

#include <array>

class Matrix3D {
public:
    std::array<std::array<float, 4>, 4> m;

    Matrix3D();
    Matrix3D(bool identity);

    static Matrix3D identity();

    Matrix3D operator*(const Matrix3D& other) const;

    static Matrix3D scale(float sx, float sy, float sz);
    static Matrix3D translate(float tx, float ty, float tz);
    static Matrix3D rotateX(float angle);
    static Matrix3D rotateY(float angle);
    static Matrix3D rotateZ(float angle);
    static Matrix3D shear(float shx, float shy, float shz);

    static Matrix3D scaleThenTranslate(float sx, float sy, float sz, float tx, float ty, float tz);
    static Matrix3D translateThenScale(float tx, float ty, float tz, float sx, float sy, float sz);
};

#endif