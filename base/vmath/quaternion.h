/*
Quaternion class, it is used to interpolate transformation when 
implement skeleton animation.
*/
#ifndef __QUATERNION_H__
#define __QUATERNION_H__

#include "Transform.h"

class Quaternion {
public:
    Quaternion() : v(0, 0, 0), w(1) {};
    Quaternion(const Vector3f &v, float w) : v(v), w(w) {};
    Quaternion &operator=(const Quaternion &q) {
        v = q.v;
        w = q.w;
        return *this;
    };
    Quaternion operator+(const Quaternion &q) const {
        return Quaternion(v + q.v, w + q.w);
    };
    Quaternion &operator+=(const Quaternion &q) {
        v += q.v;
        w += q.w;
        return *this;
    };
    Quaternion operator-() const {
        return Quaternion(-v, -w);
    };
    Quaternion operator-(const Quaternion &q) const {
        return Quaternion(v - q.v, w - q.w);
    };
    Quaternion operator*(float s) const {
        return Quaternion(s * v, s * w); 
    };
    Transform ToTransform() const {
        float x = v.x, y = v.y, z = v.z;
        Matrix4x4 m(
            1-2*(y*y+z*z), 2*(x*y+z*w), 2*(x*z-y*w), 0,
            2*(x*y-z*w), 1-2*(x*x+z*z), 2*(y*z+x*w), 0,
            2*(x*z+y*w), 2*(y*z-x*w), 1-2*(x*x+y*y), 0,
            0, 0, 0, 1
        );
        return Transform(Transpose(m));
    };
    Vector3f v;
    float w;
private:
};

inline float Dot(const Quaternion q1, const Quaternion &q2) {
    return Dot(q1.v, q2.v) + q1.w * q2.w; 
}


inline Quaternion Normalize(const Quaternion &q) {
    float w = 1 / std::sqrt(Dot(q, q));
    return Quaternion(w * q.v, w * q.w);
}

Quaternion slerp(float t, const Quaternion &q1, const Quaternion &q2) ;

#endif