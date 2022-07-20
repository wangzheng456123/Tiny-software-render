#include <cmath>
#include "quaternion.h"

Quaternion slerp(float t, const Quaternion &q1, const Quaternion &q2) {
    float cosTheta = Dot(q1, q2);
    if (cosTheta > 0.9995f)
        return Normalize(q1 * (1 - t) + q2 * t);
    else {
        cosTheta = cosTheta < -1 ? (-1) : (std::min(cosTheta, 1.f));
        float theta = std::acos(cosTheta);
        float ttheta = t * theta;
        Quaternion qperp = Normalize(q2 - q1 * cosTheta);
        return q1 * std::cos(ttheta) + qperp * std::sin(ttheta);  
    }
}