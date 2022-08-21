#include "shader.h"

Vector2f __uv(Vector3f bar, Vector2f uv0, Vector2f uv1, Vector2f uv2) {
    Vector3f uvx(uv0.x, uv1.x, uv2.x), uvy(uv0.y, uv1.y, uv2.y);
    return Vector2f(Dot(bar, uvx), Dot(bar, uvy));
}

Point3f __interp_pos(Vector3f bar, Point3f p1, Point3f p2, Point3f p3) {
    Vector3f x(p1.x, p2.x, p3.x), y(p1.y, p2.y, p3.y), z(p1.x, p2.x, p3.x);
    return Point3f(Dot(x, bar), Dot(y, bar), Dot(z, bar));
}

Vector3f __interp_norms(Vector3f bar, Vector3f n0, Vector3f n1, Vector3f n2) {
    Vector3f nx(n0.x, n1.x, n2.x), ny(n0.y, n1.y, n2.y), nz(n0.z, n1.z, n2.z);
    return Vector3f(Dot(bar, nx), Dot(bar, ny), Dot(bar, nz));
}

iShader::~iShader() {}