#include <cmath>
#include "Transform.h"

Transform Inverse(const Transform &t) {
    return Transform(t.mInv, t.m);
}

Transform Transpose(const Transform &t) {
    return Transform(Transpose(t.m), Transpose(t.mInv));
}

Transform Translate(const Vector3f &delta) {
    Matrix4x4 m(
        1, 0, 0, delta.x,
        0, 1, 0, delta.y,
        0, 0, 1, delta.z,
        0, 0, 0, 1
    );
    Matrix4x4 mInv(
        1, 0, 0, -delta.x,
        0, 1, 0, -delta.y,
        0, 0, 1, -delta.z,
        0, 0, 0, 1
    );
    return Transform(m, mInv);
}
Transform Scale(float x, float y, float z) {
    Matrix4x4 m(
        x, 0, 0, 0,
        0, y, 0, 0,
        0, 0, z, 0,
        0, 0, 0, 1
    );
    Matrix4x4 mInv(
        1/x, 0, 0, 0, 
        0, 1/y, 0, 0,
        0, 0, 1/z, 0,
        0, 0, 0, 1
    );
    return Transform(m, mInv);
}
Transform RotateX(float theta) {
    float cosTheta = std::cos(theta);
    float sinTheta = std::sin(theta);
    Matrix4x4 m(
        1, 0, 0, 0,
        0, cosTheta, -sinTheta, 0,
        0, sinTheta, cosTheta, 0,
        0, 0, 0, 1
    );
    return Transform(m, Transpose(m));
}
Transform RotateY(float theta) {
    float cosTheta = std::cos(theta);
    float sinTheta = std::sin(theta);
    Matrix4x4 m(
        cosTheta, 0, sinTheta, 0,
        0, 1, 0, 0,
        -sinTheta, 0, cosTheta, 0,
        0, 0, 0, 1
    );
    return Transform(m, Transpose(m));
}
Transform RotateZ(float theta) {
    float cosTheta = std::cos(theta);
    float sinTheta = std::sin(theta);
    Matrix4x4 m(
        cosTheta, -sinTheta, 0, 0,
        sinTheta, cosTheta, 0, 0, 
        0, 0, 1, 0,
        0, 0, 0, 1
    );
    return Transform(m, Transpose(m));
}
Transform LookAt(const Point3f &pos, const Point3f &look, const Vector3f &up) {
    Vector3f dir = Normalize(look - pos);
    Vector3f left = Normalize(Cross(Normalize(up), dir));
    Vector3f newUp = Cross(dir, left);
    Matrix4x4 cameral2World(
        left.x, newUp.x, dir.x, pos.x,
        left.y, newUp.y, dir.y, pos.y,
        left.z, newUp.z, dir.z, pos.z,
        0, 0, 0, 1
    );
    return Transform(Inverse(cameral2World), cameral2World);
}

Transform LookAt_Right(const Point3f& pos, const Point3f& look, const Vector3f& up) {
    Vector3f dir = Normalize(pos - look);
    Vector3f left = Normalize(Cross(Normalize(dir), up));
    Vector3f newUp = Cross(dir, left);
    Matrix4x4 cameral2World(
        left.x, newUp.x, dir.x, pos.x,
        left.y, newUp.y, dir.y, pos.y,
        left.z, newUp.z, dir.z, pos.z,
        0, 0, 0, 1
    );
    return Transform(Inverse(cameral2World), cameral2World);
}

Transform Perspective(float n, float f) {
    Matrix4x4 m(
        n, 0, 0, 0,
        0, n, 0, 0,
        0, 0, n + f, -n * f,
        0, 0, 1, 0
    );
    return Transform(m);
}

Transform Transform::flip() const {
    Matrix4x4 mat;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) {
            if (i == 2 || j == 2)
                mat.mat[i][j] = -m.mat[i][j];
            else mat.mat[i][j] = m.mat[i][j];
        }
    mat.mat[2][2] = m.mat[2][2];
    return Transform(mat);
}