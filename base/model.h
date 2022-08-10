/*
a simple obj parser to process obj file.
*/

#ifndef __MODEL_H__
#define __MODEL_H__
#include <vector>
#include <string>
#include "./vmath/Point.h"
#include "tgaimage.h"

class Model {
private:
    std::vector<Point3f> verts_;
    std::vector<std::vector<Vector3i> > faces_; // attention, this Vec3i means vertex/uv/normal
    std::vector<Vector3f> norms_;
    std::vector<Vector2f> uv_;
    std::vector<Point4i> jointidx__;
    std::vector<Point4f> jointweight__; 
    TGAImage diffusemap_;
    TGAImage normalmap_;
    TGAImage specularmap_;
public:
    Model(const char *filename);
    ~Model();
    int nverts() const;
    int nfaces() const;
    Vector3f normal(int iface, int nthvert) ;
    Vector3f normal(int vertid);
    Vector3f normal(Vector2f uv) ;
    Point3f vert(int i) ;
    Point3f vert(int iface, int nthvert) ;
    int vertid(int iface, int nthvert) ; 
    Vector2f uv(int iface, int nthvert) ;
    Vector2f uv(int uvid);
    TGAColor diffuse(Vector2f uv) ;
    float specular(Vector2f uv) ;
    std::vector<int> face(int idx) ;
    Point4i jointidx(int idx); 
    Point4f jointweight(int idx) ;
    void load_texture(std::string filename, const char *suffix, const char *type) ;
    void load_obj(const char *filename) ;
    //Transform the model from right coordinate system to the right
    void flip();
};
#endif //__MODEL_H__