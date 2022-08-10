/*
2, 3 dimension vector class and operations on it
*/

#ifndef __VECTOR_H__
#define __VECTOR_H__ 1

#include <cmath>
#include <iostream>

template <typename T> class Vector2 {
public: 
    Vector2() : x(0), y(0) {};
    Vector2(T x, T y) : x(x), y(y) {};
    ~Vector2() {}; 
    T x, y;
    T &operator[] (int i) {
        if (i == 0) return x;
        return y;
    };
    Vector2<T> &operator= (const Vector2<T> &a) {
        x = a.x, y = a.y;
        return *this;
    };
    Vector2<T> operator+ (const Vector2<T> &a) const {
        return Vector2(x + a.x, y + a.y);
    };
    Vector2<T> &operator+= (const Vector2<T> &a) {
        x += a.x, y += a.y;
        return *this;
    };
    Vector2<T> operator* (T s) const {
        return Vector2<T>(x * s, y * s); 
    };
    Vector2<T> &operator*= (T s) {
        x *= s, y *= s;
        return *this;
    };
    Vector2<T> operator- () {
        return Vector<T>(-x, -y);
    };
    Vector2<T> operator- (const Vector2<T> &v) const {
        return Vector2<T>(x-v.x, y-v.y);
    };
    float Length() const {
        return std::sqrt(x*x + y*y);
    };
};

template <typename T> class Vector3 {
public: 
    Vector3() : x(0), y(0), z(0) {};
    Vector3(T x, T y, T z) : x(x), y(y), z(z) {};
    ~Vector3() {};
    T x, y, z;
    T &operator[] (int i) {
        if (i == 0) return x;
        else if (i == 1) return y;
        return z;
    };
    Vector3<T> &operator= (const Vector3<T> &a) {
        x = a.x, y = a.y, z = a.z;
        return *this;
    };
    Vector3<T> operator+ (const Vector3<T> &a) const {
        return Vector3(x + a.x, y + a.y, z + a.z);
    };
    Vector3<T> &operator+= (const Vector3<T> &a) {
        x += a.x, y += a.y, z += a.z;
        return *this;
    };
    Vector3<T> operator* (T s) const {
        return Vector3<T>(x * s, y * s, z * s); 
    };
    Vector3<T> &operator*= (T s) {
        x *= s, y *= s, z *= s;
        return *this;
    };
    Vector3<T> operator- () const {
        return Vector3<T>(-x, -y, -z);
    };
    Vector3<T> operator-(const Vector3<T> &v) const {
        return Vector3<T>(x-v.x, y-v.y, z-v.z);
    }
    T Length() const {
        return std::sqrt(x*x + y*y + z*z);
    };
    void print() const {
        std::cout << x << " " << y << " " << z << "\n";
    };
};

template <typename T> inline T Dot(const Vector3<T> &a, const Vector3<T> &b) {
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

template <typename T> inline Vector3<T> Cross(const Vector3<T> &a, const Vector3<T> &b) {
    double x1 = a.x, y1 = a.y, z1 = a.z;
    double x2 = b.x, y2 = b.y, z2 = b.z;
    return Vector3<T> (
        (y1 * z2) - (z1 * y2),
        (z1 * x2) - (x1 * z2),
        (x1 * y2) - (y1 * x2)
    ) ;
}

template <typename T> inline T Dot(const Vector2<T> &a, const Vector2<T> &b) {
    return (a.x * b.x) + (a.y * b.y);
}

template <typename T> inline Vector3<T> Normalize(const Vector3<T> &a) {
    float len = a.Length();
    return Vector3<T>(a.x / len, a.y / len, a.z / len);      
}

template <typename T> inline Vector2<T> Normalize(const Vector2<T> &a) {
    float len = a.Length();
    return Vector2<T>(a.x / len, a.y / len); 
}

template <typename T> inline Vector3<T> operator* (T s, const Vector3<T> &a) {
    return Vector3<T>(s * a.x, s * a.y, s * a.z);
}

template <typename T> inline Vector2<T> operator* (T s, const Vector2<T> &a) {
    return Vector2<T>(s * a.x, s * a.y);
}

template <typename T> inline Vector3<T> reflect(Vector3<T> &dir, Vector3<T> normal) {
    return Normalize((2 * Dot(dir, normal) * normal + (-dir)));
}

typedef Vector3<float> Vector3f;
typedef Vector3<int> Vector3i;
typedef Vector2<float> Vector2f;
typedef Vector2<int> Vector2i;



#endif