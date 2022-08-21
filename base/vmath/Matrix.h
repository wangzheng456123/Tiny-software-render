#include <iostream>
#include <array>

struct Matrix4x4 {
    Matrix4x4() ;
    Matrix4x4(float mat[4][4]) ;
    Matrix4x4(float m00, float m01, float m02, float m03,
              float m10, float m11, float m12, float m13,
              float m20, float m21, float m22, float m23,
              float m30, float m31, float m32, float m33) ;
    std::array<std::array<float, 4>, 4> mat;
    void print() const {
       for (int i = 0; i < 4; i++) {
           for (int j = 0; j < 4; j++)
               std::cout << mat[i][j] << " ";
            std::cout << "\n";
       }
    };
};

Matrix4x4 Mul(const Matrix4x4 &a, const Matrix4x4 &b);
Matrix4x4 Inverse(const Matrix4x4 &a);
Matrix4x4 Transpose(const Matrix4x4 &a);

bool operator== (const Matrix4x4 &a, const Matrix4x4 &b);

