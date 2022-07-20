#ifndef __POINT_H__
#define __POINT_H__ 1

#include "Vector.h"
#include <iostream>

template <typename T> class Point2 {
public: 
    Point2() : x(0), y(0) {};
    Point2(T x, T y) : x(x), y(y) {}; 
    ~Point2() {};
    T x, y;
    Point2<T> &operator= (const Point2 &a) {
        x = a.x, y = a.y;
        return *this;
    };
    T &operator[] (int i) {
        if (i == 0) return x;
        return y;
    };
    Point2<T> operator+ (const Vector2<T> &v) const {
        return Point2<T>(x + v.x, y + v.y);
    };
    Point2<T> operator+= (Vector2<T> v) {
        x += v.x, y += v.y;
        return *this;
    };
    Vector2<T> operator- (const Point2 &p) const {
        return Vector2<T>(x - p.x, y - p.y);
    };
};

template <typename T> class Point3 {
public: 
    Point3() : x(0), y(0), z(0) {};
    Point3(T x, T y, T z) : x(x), y(y), z(z) {};
    ~Point3() {};
    T x, y, z;
    Point3<T> &operator= (const Point3 &a) {
        x = a.x, y = a.y, z = a.z;
        return *this;
    };
    T &operator[] (int i) {
        if (i == 0) return x;
        else if (i == 1) return y;
        return z;
    };
    Point3<T> operator+ (const Vector3<T> &v) const {
        return Point3<T>(x + v.x, y + v.y, z + v.z);
    };
    Point3<T> operator* (T s) const {
        return Point3<T>(s * x, s * y, s * z);
    };
    Point3<T> operator+= (const Vector3<T> &v) {
        x += v.x, y += v.y, z += v.z;
        return *this;
    };
    Point3<T> operator+= (const Point3<T> &p) {
        x += p.x, y += p.y, z += p.z;
        return *this;
    };
    Vector3<T> operator- (const Point3 &p) const {
        return Vector3<T>(x - p.x, y - p.y, z - p.z);
    };
    void print() const {
        std::cout << x << " " << y << " " << z << "\n";
    };
};

//point in homogeneous cordinate
template <typename T> class Point4 {
public : 
    T x, y, z, w;
    Point4<T> () : x(0), y(0), z(0), w(1) {};
    Point4<T> (T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {};
    Point4<T> (const Point3<T> &p) : x(p.x), y(p.y), z(p.z), w(1) {};
    Point4<T> &operator=(const Point4<T> b) {
        x = b.x, y = b.y, z = b.z, w = b.w;
        return *this;
    }
    T &operator[] (int i) {
        if (i == 0) return x;
        else if (i == 1) return y;
        else if (i == 2) return z;
        else return w;
    };
    Point3<T> proj() const {
        return Point3<T>(x, y, z);
    };
    Point3<T> p3() const {
        return Point3<T>(x / w, y / w, z / w);
    };
    void print() const {
        std::cout << x << " " << y << " " << z << " " << w << "\n";
    };
};

typedef Point2<int> Point2i;
typedef Point2<float> Point2f;
typedef Point3<int> Point3i;
typedef Point3<float> Point3f;
typedef Point4<float> Point4f;
typedef Point4<int> Point4i;

template <typename T> inline T Distance(const Point3<T> &p1, const Point3<T> &p2);
template <typename T> inline T Distance(const Point2<T> &p1, const Point2<T> &p2);

//get 1 - u - v, u and v
Vector3f GetBar(const Point3f &p1, const Point3f &p2, const Point3f &p3, const Point3f &p);

#endif