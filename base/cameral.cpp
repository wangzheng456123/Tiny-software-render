#include "cameral.h"
#include "geometry.h"
#include <vector>
#include <iostream>
#include <cmath>

void Cameral::Raterize(Model &model, iShader &shader, float zbuffer[]) {
    int w = img.get_width(), h = img.get_height();
    Transform world2NDC = (this->Cameral2NDC() * this->World2Cameral());
    Transform world2cameral = this->World2Cameral();
    Transform viewport = Scale(w / 2, h / 2, 1) * Translate(Vector3f(1, 1, 1));
    int num_face = model.nfaces();
    //scroll array trick to optimize the process of clipping 
    std::vector<Point4f> in_list[2];
    std::vector<Point4f> &out_list = in_list[1];
    std::vector<Vector2f> vert_uv;
    Vector2f tri_uv[3];
    Point4f tri_vert[3], NDCvert[3];
    float z[3], vert[3];
    for (int i = 0; i < num_face; i++) {
        in_list[0].clear();
        for (int j = 0; j < 3; j++) {
            int id = model.vertid(i, j);
            vert[j] = world2cameral(model.vert(i, j)).z;
            NDCvert[j] = world2NDC(shader.vertex(model, i, j));
            //NDCvert[j].print();
            in_list[0].push_back(NDCvert[j]);
        }
        for (int j = 0; j < 7; j++) {
            in_list[j + 1 & 1].clear();
            clip_with_plane(j + 1, in_list[j & 1], in_list[j + 1 & 1]);
        }
        int num_vert = out_list.size();
        for (int j = 1; j < num_vert - 1; j++) {
            int idx[3];
            idx[0] = 0, idx[1] = j, idx[2] = j + 1;
            for (int k = 0; k < 3; k++) {
                //the uv coordinate of the clipped vertex have to be caculated in homogenous space, see my artilce for the 
                //detail of the proof: 
                Vector3f bar = GetBar(NDCvert[0].proj(), NDCvert[1].proj(), NDCvert[2].proj(), out_list[idx[k]].proj());
                z[k] = bar[0] * vert[0] + bar[1] * vert[1] + bar[2] * vert[2];
                //it is not the geometry shader you think, see my article for the detail:
                shader.geometry(model, bar, k);
                tri_vert[k] = viewport(out_list[idx[k]]);
            }
            //draw the triangle consisted by trivert
            int x0 = w, x1 = 0, y0 = h, y1 = 0;
            for (int k = 0; k < 3; k++) {
                Point4f& p = tri_vert[k];
                x0 = (std::min)(x0, (int)std::floor(p.x / p.w));
                x1 = (std::max)(x1, (int)std::ceil(p.x / p.w));
                y0 = (std::min)(y0, (int)std::floor(p.y / p.w));
                y1 = (std::max)(y1, (int)std::ceil(p.y / p.w));
            }
            x0 = std::max(0, x0), x1 = std::min(w - 1, x1);
            y0 = std::max(0, y0), y1 = std::min(h - 1, y1);
            //trivial clip, simply drop all the faces not in the screen space.
            //if (x0 < 0 || x1 >= w || y0 < 0 || y1 >= h) continue;
            for (int y = y0; y <= y1; y++) {
               for (int x = x0; x <= x1; x++) {
                    //uv cordinate caculted in screeen space is not right, 
                    Vector3f bar = GetBar(tri_vert[0].p3(), tri_vert[1].p3(), tri_vert[2].p3(), Point3f(x, y, 1));
                    if (bar.x < 0 || bar.y < 0 || bar.z < 0) continue;
                    float zn = 1 / (bar[0] / z[0] + bar[1] / z[1] + bar[2] / z[2]);
                    //bar[0] *= zn / z[0], bar[1] *= zn / z[1], bar[2] *= zn / z[2];
                    float cz = 0;
                    for (int k = 0; k < 3; k++) cz += bar[k] * (tri_vert[k].z / tri_vert[k].w);
                    if (cz >= zbuffer[y * w + x]) continue;
                    zbuffer[y * w + x] = cz;
                    TGAColor color;
                    shader.fragment(model, bar, color);
                    img.set(y, x, color);
                }
            }
        }
    }
}

void Cameral::Save() {
    img.write_tga_file("./res.tga");
}

void Cameral::update_look(Vector2f offset) {
    look.y -= offset.y, look.x -= offset.x;
}

void Cameral::update_pos(Vector2f offset, int dolly) {
    Vector3f dir = pos - look;
    float r = dir.Length();
    float pi = M_PI;
    float theta = (float)std::atan2(dir.x, dir.z);
    float phi = acos(dir.y / r);

    theta += offset.x;
    phi -= offset.y;

    phi = phi < 0.1 ? 0.1 : (std::min(phi, pi / 2));

    r *= std::pow(0.95, dolly);

    Vector3f delta(0, 0, 0);

    delta.x = r * (float)std::sin(phi) * (float)std::sin(theta);
    delta.y = r * (float)std::cos(phi);
    delta.z = r * (float)std::sin(phi) * (float)std::cos(theta);
    pos = look + delta;
}

void Cameral::copy_img(unsigned char *buffer) {
    int w = img.get_width(), h = img.get_height();
    for (int i = 0; i < h; i ++) {
        for (int j = 0; j < w; j++) {
            int idx = (i * w + j) * 4;
            unsigned char* dst = buffer + idx;
            unsigned char* cur_pix = img.get(i, j).bgra;
            dst[0] = cur_pix[0];
            dst[1] = cur_pix[1];
            dst[2] = cur_pix[2];
        }  
    }
}

Transform Cameral::World2Cameral() {
    return LookAt(pos, look, up);
}

Transform Cameral::Cameral2NDC() {
    return Scale(2/(right-left), 2/(top-bottom), 2/(_far-_near)) * 
           Translate(Vector3f(-(right+left)/2, -(bottom+top)/2, -(_far+_near)/2)) 
           * Perspective(_near, _far)
           ;
}