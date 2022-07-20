#include "Point.h"
#include <cmath>

template <typename T> inline T Distance(const Point3<T> &p1, const Point3<T> &p2) {
    return std::sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) + (p1.z - p2.z) * (p1.z - p2.z));
}
template <typename T> inline T Distance(const Point2<T> &p1, const Point2<T> &p2) {
    return std::sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

Vector3f GetBar(const Point3f &p1, const Point3f &p2, const Point3f &p3, const Point3f &p) {
    float p1x = p1.x, p1y = p1.y, p2x = p2.x, p2y = p2.y, p3x = p3.x, p3y = p3.y, px = p.x, py = p.y;
    float v = ((p1y - p2y) * px + (p2x - p1x) * py + p1x * p2y - p2x * p1y) /
              ((p1y - p2y) * p3x + (p2x - p1x) * p3y + p1x * p2y - p2x * p1y);
    float u = ((p1y - p3y) * px + (p3x - p1x) * py + p1x * p3y - p3x * p1y) /
              ((p1y - p3y) * p2x + (p3x - p1x) * p2y + p1x * p3y - p3x * p1y);
    return Vector3f(1 - u - v, u, v); 
}