/*
Transformations on different math objets(vector, point, etc.).
*/
#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__ 1

#include "Matrix.h"
#include "Vector.h"
#include "Point.h"
#include <iostream>

class Transform {
    friend Transform Inverse(const Transform &t);
    friend Transform Transpose(const Transform &t);
public:
    Transform() : m(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    ), mInv(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    ) {};
    Transform (const Matrix4x4 &m) 
        :m(m), mInv(Inverse(m)) {};
    Transform (const Matrix4x4 &m, const Matrix4x4 &mInv)
        :m(m), mInv(mInv) {};
    Transform (const float mat[4][4]) {
        m = Matrix4x4(mat[0][0], mat[0][1], mat[0][2], mat[0][3],
                      mat[1][0], mat[1][1], mat[1][2], mat[1][3],
                      mat[2][0], mat[2][1], mat[2][2], mat[2][3],
                      mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
        mInv = Inverse(m);
    };
    ~Transform() {};
    template <typename T> inline Point3<T> operator()(const Point3<T> &v) const {
        T x = v.x, y = v.y, z = v.z;
        T px = x * m.mat[0][0] + y * m.mat[0][1] + z * m.mat[0][2] + m.mat[0][3];
        T py = x * m.mat[1][0] + y * m.mat[1][1] + z * m.mat[1][2] + m.mat[1][3];
        T pz = x * m.mat[2][0] + y * m.mat[2][1] + z * m.mat[2][2] + m.mat[2][3];
        T pw = x * m.mat[3][0] + y * m.mat[3][1] + z * m.mat[3][2] + m.mat[3][3];
        if (pw == 1) return Point3<T>(px, py, pz);
        else return Point3<T>(px / pw, py / pw, pz / pw);   
    }; 
    template <typename T> inline Vector3<T> operator()(const Vector3<T> &v) const {
        return Vector3<T>(
            v.x * m.mat[0][0] + v.y * m.mat[0][1] + v.z * m.mat[0][2],
            v.x * m.mat[1][0] + v.y * m.mat[1][1] + v.z * m.mat[1][2],
            v.x * m.mat[2][0] + v.y * m.mat[2][1] + v.z * m.mat[2][2]);
    }; 
    template <typename T> inline Point4<T> operator()(const Point4<T> &v) const {
        return Point4<T>(
            v.x * m.mat[0][0] + v.y * m.mat[0][1] + v.z * m.mat[0][2] + v.w * m.mat[0][3],
            v.x * m.mat[1][0] + v.y * m.mat[1][1] + v.z * m.mat[1][2] + v.w * m.mat[1][3],
            v.x * m.mat[2][0] + v.y * m.mat[2][1] + v.z * m.mat[2][2] + v.w * m.mat[2][3],
            v.x * m.mat[3][0] + v.y * m.mat[3][1] + v.z * m.mat[3][2] + v.w * m.mat[3][3]);
    }; 
    Transform &operator= (const Transform &t2) {
        m = t2.m, mInv = t2.mInv;
        return *this;
    };
    Transform operator*(const Transform &t2) const {
        return Transform(Mul(m, t2.m), Mul(mInv, t2.mInv));
    };
    void inline print() const {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) 
                std::cout << m.mat[i][j] << " ";
            std::cout << "\n"; 
        }
    };
    Transform flip() const;
private:
    Matrix4x4 m, mInv;
};

Transform Translate(const Vector3f &delta);
Transform Scale(float x, float y, float z);
Transform RotateX(float theta);
Transform RotateY(float theta);
Transform RotateZ(float theta);
Transform LookAt(const Point3f &pos, const Point3f &look, const Vector3f &up);
Transform Perspective(float n, float f);

#endif