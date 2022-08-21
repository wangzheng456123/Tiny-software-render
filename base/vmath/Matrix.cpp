/*
4x4 matrix class and correponding operations. In computer graphic,
almost all the matrix we use is 4x4 matrix, so implement only 4x4 matrix 
opertaion is enough.
*/

#include <cmath>
#include "Matrix.h"

Matrix4x4::Matrix4x4() : mat() {}

Matrix4x4::Matrix4x4(float mat[4][4]) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            this->mat[i][j] = mat[i][j];
}

Matrix4x4::Matrix4x4(float m00, float m01, float m02, float m03,
                     float m10, float m11, float m12, float m13,
                     float m20, float m21, float m22, float m23,
                     float m30, float m31, float m32, float m33) {
    this->mat[0][0] = m00, this->mat[0][1] = m01, this->mat[0][2] = m02, this->mat[0][3] = m03;
    this->mat[1][0] = m10, this->mat[1][1] = m11, this->mat[1][2] = m12, this->mat[1][3] = m13;
    this->mat[2][0] = m20, this->mat[2][1] = m21, this->mat[2][2] = m22, this->mat[2][3] = m23;
    this->mat[3][0] = m30, this->mat[3][1] = m31, this->mat[3][2] = m32, this->mat[3][3] = m33;
}

bool operator== (const Matrix4x4 &a, const Matrix4x4 &b) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (std::fabs(a.mat[i][j] - b.mat[i][j]) > 1e-6) return false;
    return true; 
} 

Matrix4x4 Transpose(const Matrix4x4 &a) {
    float b[4][4];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) 
            b[j][i] = a.mat[i][j];
    return Matrix4x4(b);
}

Matrix4x4 Mul(const Matrix4x4 &a, const Matrix4x4 &b) {
    Matrix4x4 c;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) {
            c.mat[i][j] = 0;
            for (int k = 0; k < 4; k++)
                c.mat[i][j] += a.mat[i][k] * b.mat[k][j];
        }
    return c;
}

//Gauss-Jordan method to caculate the inverse of a 4x4 matrix.

Matrix4x4 Inverse(const Matrix4x4 &a) {
    double b[4][4];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            b[i][j] = a.mat[i][j];
    double c[4][4] = {1., 0, 0, 0,
                     0, 1., 0, 0,
                     0, 0, 1., 0,
                     0, 0, 0, 1.
                    };
    for (int i = 0; i < 4; i++) {
        double maxi = 0.;
        int idx = 0;
        for (int j = i; j < 4; j++) {
            if (std::fabs(b[j][i]) > maxi)
                maxi = fabs(b[j][i]), idx = j;
        }
        for (int j = 0; j < 4; j++) {
            double tmp = b[i][j];
            b[i][j] = b[idx][j];
            b[idx][j] = tmp;
        }
        for (int j = 0; j < 4; j++) {
            double tmp = c[i][j];
            c[i][j] = c[idx][j];
            c[idx][j] = tmp;
        }
        for (int j = 0; j < 4; j++) {
            if (i == j) continue;
            double ratio = b[j][i] / b[i][i];
            for (int k = i; k < 4; k++)
                b[j][k] -= ratio * b[i][k];
            for (int k = 0; k < 4; k++)
                c[j][k] -= ratio * c[i][k];
        }
    }
    float res[4][4];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) {
            c[i][j] /= b[i][i];
            res[i][j] = c[i][j];
        }
    return Matrix4x4(res);
}