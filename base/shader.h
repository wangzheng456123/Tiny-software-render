/*
The shader class hold the contant data in one pass.
It also supply the vertex and fragment method to make the
render pipelien programmable in some extent. 
*/
#ifndef __SHADER_H__
#define __SHADER_H__ 1

#include "animation.h"
#include "model.h"
#include "./vmath/Point.h"
#include "./vmath/Transform.h"
#include "tgaimage.h"
#include "light.h"

//intrtpolate between uv0, uv1, uv2 using bar
Vector2f __uv(Vector3f bar, Vector2f uv0, Vector2f uv1, Vector2f uv2);
Point3f __interp_pos(Vector3f bar, Point3f p1, Point3f p2, Point3f p3);
Vector3f __interp_norms(Vector3f bar, Vector3f n0, Vector3f n1, Vector3f n2);

struct iShader {
    virtual ~iShader() = 0;
    virtual Point4f vertex(Model& model, int nface, int nthvert) = 0;
    virtual void geometry(Model& model, Vector3f &bar, int nthvert) = 0;
    virtual int fragment(Model &model, Vector3f &bar, TGAColor &color) = 0;  
};

struct Phong : public iShader {
    Phong(Light *light, Point3f view) : light(light), view(view) {}; 
    Vector2f uniform_uv[3], vary_uv[3];
    Point3f uniform_point[3], vary_point[3], view;
    Light *light;
    virtual Point4f vertex(Model& model, int nface, int nthvert) {
        uniform_point[nthvert] = model.vert(nface, nthvert);
        uniform_uv[nthvert] = model.uv(nface, nthvert);
        return Point4f(model.vert(nface, nthvert));
    };
    virtual void geometry(Model& model, Vector3f &bar, int nthvert) {
        Point3f &v0 = uniform_point[0], &v1 = uniform_point[1], &v2 = uniform_point[2];
        Vector2f &uv0 = uniform_uv[0], &uv1 = uniform_uv[1], &uv2 = uniform_uv[2];
        vary_point[nthvert] = __interp_pos(bar, v0, v1, v2);
        vary_uv[nthvert] = __uv(bar, uv0, uv1, uv2);
    };
    virtual int fragment(Model &model, Vector3f &bar, TGAColor &color) {
        Point3f &p0 = vary_point[0], &p1 = vary_point[1], &p2 = vary_point[2];
        Vector2f &uv0 = vary_uv[0], &uv1 = vary_uv[1], &uv2 = vary_uv[2];
        Vector2f uv = __uv(bar, uv0, uv1, uv2);
        Point3f pos = __interp_pos(bar, p0, p1, p2);
        Vector3f light_dir = light->get_dir(pos);
        float intensity = light->get_intensity(pos);
        Vector3f normal = Normalize(model.normal(uv));
        Vector3f bisect = Normalize((view - pos) - light_dir);
        float spec = (std::max)(Dot(bisect, normal), 0.f);
        spec = (std::pow)(spec, 3);
        float dif = (std::max)(Dot(normal, light_dir), 0.f);
        TGAColor c = model.diffuse(uv);
        for (int i = 0; i < 3; i++) color[i] = (std::min)(255.f, (float)(c[i] * (2 + 1.6 * spec)) * dif);
        return 1;
    };
};

struct Animation : public iShader {
    Animation(Light *light, Skeleton *skeleton) : light(light), skeleton(skeleton) {};
    Skeleton* skeleton;
    Light* light;
    Vector3f uniform_normal[3], vary_normal[3];
    Vector2f uniform_uv[3], vary_uv[3];
    Point3f uniform_point[3], vary_point[3];
    virtual Point4f vertex(Model& model, int nface, int nthvert) {
        int vertid = model.vertid(nface, nthvert);
        Point3f vert, p = model.vert(vertid);
        Vector3f normal, n = model.normal(vertid);
        for (int j = 0; j < 4; j++) {
            int jid = model.jointidx(vertid)[j];
            Joint& joint = skeleton->joints[jid];
            float w = model.jointweight(vertid)[j];
            vert += joint.transvert(p) * w;
            normal += joint.transnorm(n) * w;
        }
        uniform_normal[nthvert] = normal;
        uniform_uv[nthvert] = model.uv(nface, nthvert);
        uniform_point[nthvert] = vert;
        return Point4f(vert);
    };
    virtual void geometry(Model &model, Vector3f &bar, int nthvert) {
        Vector3f &n0 = uniform_normal[0], &n1 = uniform_normal[1], &n2 = uniform_normal[2];
        Point3f &v0 = uniform_point[0], &v1 = uniform_point[1], &v2 = uniform_point[2];
        Vector2f &uv0 = uniform_uv[0], &uv1 = uniform_uv[1], &uv2 = uniform_uv[2];
        vary_normal[nthvert] = __interp_norms(bar, n0, n1, n2);
        vary_point[nthvert] = __interp_pos(bar, v0, v1, v2);
        vary_uv[nthvert] = __uv(bar, uv0, uv1, uv2);
    };
    virtual int fragment(Model& model, Vector3f& bar, TGAColor& color) {
        Vector2f uv = __uv(bar, vary_uv[0], vary_uv[1], vary_uv[2]);
        color = model.diffuse(uv);
        return 1;
    };
};

struct Depth : public iShader {
    Point3f uniform_point[3], vary_point[3];
    float uniform_dep[3], vary_dep[3];
    virtual Point4f vertex(Model& model, int nface, int nthvert) {
        uniform_point[nthvert] = model.vert(nface, nthvert);
        uniform_dep[nthvert] = model.vert(nface, nthvert).z;
        return Point4f(model.vert(nface, nthvert));
    };
    virtual void geometry(Model& model, Vector3f& bar, int nthvert) {
        Point3f& v0 = uniform_point[0], & v1 = uniform_point[1], & v2 = uniform_point[2];
        float d0 = uniform_dep[0], d1 = uniform_dep[1], d2 = uniform_dep[2];
        vary_dep[nthvert] = bar[0] * d0 + bar[1] * d1 + bar[2] * d2;
        vary_point[nthvert] = __interp_pos(bar, v0, v1, v2);
    };
    virtual int fragment(Model& model, Vector3f& bar, TGAColor& color) {
        float d0 = vary_dep[0], d1 = vary_dep[1], d2 = vary_dep[2];
        float dep = d0 * bar[0] + d1 * bar[1] + d2 * bar[2];
        color = TGAColor(255, 255, 255) * (dep / 2);
        return 1;
    };
};

struct Shadow : public iShader {
    Shadow(Light* light, float* depth_buffer, int buff_width, int buff_height, const Point3f &view, const Transform &model2screen) :
        light(light), depth_buffer(depth_buffer), buff_width(buff_width), buff_height(buff_height), view(view), model2screen(model2screen) {};
    Point3f uniform_point[3], vary_point[3], view;
    Vector2f uniform_uv[3], vary_uv[3];
    float* depth_buffer;
    Light* light;
    int buff_width, buff_height;
    Transform model2screen;
    virtual Point4f vertex(Model& model, int nface, int nthvert) {
        uniform_point[nthvert] = model.vert(nface, nthvert);
        uniform_uv[nthvert] = model.uv(nface, nthvert);
        return Point4f(model.vert(nface, nthvert));
    };
    virtual void geometry(Model& model, Vector3f& bar, int nthvert) {
        Point3f& v0 = uniform_point[0], & v1 = uniform_point[1], & v2 = uniform_point[2];
        Vector2f& uv0 = uniform_uv[0], & uv1 = uniform_uv[1], & uv2 = uniform_uv[2];
        vary_point[nthvert] = __interp_pos(bar, v0, v1, v2);
        vary_uv[nthvert] = __uv(bar, uv0, uv1, uv2);
    };
    virtual int fragment(Model& model, Vector3f& bar, TGAColor& color) {
        int w = buff_width, h = buff_height;
        Point3f &v0 = vary_point[0], &v1 = vary_point[1], &v2 = vary_point[2];
        Point3f pos = __interp_pos(bar, v0, v1, v2);
        Point3f screen_cord = model2screen(pos);
        float shadow;
        int x = screen_cord.x, y = screen_cord.y;
        if (x < 0 || x >= w || y < 0 || y >= h) shadow = 0.3;
        else shadow = (depth_buffer[y * w + x] < pos.z) ? (0.3) : (1.);
        Vector2f &uv0 = vary_uv[0], &uv1 = vary_uv[1], &uv2 = vary_uv[2];
        Vector2f uv = __uv(bar, uv0, uv1, uv2);
        Vector3f light_dir = light->get_dir(pos);
        float intensity = light->get_intensity(pos);
        Vector3f normal = Normalize(model.normal(uv));
        Vector3f bisect = Normalize((view - pos) - light_dir);
        float spec = (std::max)(Dot(bisect, normal), 0.f);
        spec = (std::pow)(spec, 3);
        float dif = (std::max)(Dot(normal, light_dir), 0.f);
        TGAColor c = model.diffuse(uv);
        for (int i = 0; i < 3; i++) color[i] = (std::min)(255.f, shadow * (float)(c[i] * (2 + 1.6 * spec)) * dif);
        return 1;
    };
};

struct deffered_shading_phong {
    deffered_shading_phong(const std::vector<Light*> &light) : light(light) {};
    std::vector<Light*> light;
    TGAColor shading(const TGAColor& Diffuse, const Vector3f& Normal, const Point3f& pos, const Point3f& view) const {
        TGAColor c = TGAColor(0, 0, 0);
        for (int i = 0; i < light.size(); i++) {
            Light *l = light[i];
            Vector3f light_dir = l->get_dir(pos);
            float intensity = l->get_intensity(pos);
            Vector3f normal = Normalize(Normal);
            Vector3f bisect = Normalize((view - pos) - light_dir);
            float spec = (std::max)(Dot(bisect, normal), 0.f);
            spec = (std::pow)(spec, 3);
            float dif = (std::max)(Dot(normal, light_dir), 0.f);
            for (int j = 0; j < 3; j++) c[j] = (std::min)(255.f, c[j] + (float)(Diffuse.bgra[j] * (2 + 1.6 * spec)) * dif);
        }
        return c;
    }
};

#endif