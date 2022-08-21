/*
the cameral class implement the forward rendering process and 
encaplising the basic render pipeline state.
*/
#include "tgaimage.h"
#include "./vmath/Transform.h"
#include "shader.h"
#include "model.h"
#include <string>

struct framebuffer {
    int width, height, bpp;
    unsigned char *color_buffer;
    float *depth_buffer;
    framebuffer(int width, int height, int bpp);
    ~framebuffer();
    void clear() {
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++) {
                int idx = (i * width + j) * bpp;
                unsigned char *cpix = color_buffer + idx;
                for (int k = 0; k < bpp; k++)
                    cpix[k] = 0;
                depth_buffer[i * width + j] = 255;
            }
    };
} ;

class Cameral {
public:
    Cameral(
        const TGAImage& img, const Point3f& pos, const Point3f& look,
        const Vector3f& up, float _near, float _far, float bottom, float top,
        float left, float right
    );
    // Forward rederding
    void Raterize(Model &model, iShader &shader, float zbuffer[]) ;
    // Deffered rendering
    void Deffered(Model &model, Point3f *gbuffer_pos, Vector3f *gbuffer_normal, 
        TGAImage gbuffer_diffuse, float zbuffer[], const deffered_shading_phong& shader);
    void update_look(Vector2f offset);
    void update_pos(Vector2f offset, int dolly);
    void Save();
    void copy_img(unsigned char *dst) ;
    Transform World2Cameral();
    Transform Cameral2NDC();
    void clear_img() {
        img.clear();
    };
private:
    float _near, _far, bottom, top, left, right;
    Point3f look, pos;
    Vector3f up;
    TGAImage img;
};
