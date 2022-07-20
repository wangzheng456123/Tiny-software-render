#include "geometry.h"
#include "./vmath/Transform.h"

#define W_PLANE 1
#define X_RIGHT 2
#define X_LEFT 3
#define Y_TOP 4
#define Y_BOTTOM 5
#define Z_NEAR 6
#define Z_FAR 7

int __clip_check(int plane_num, Point4f &p) {
    if (plane_num == W_PLANE) {
        if (p.w > 1e-5) return 1;
        else return -1;
    }
    else if (plane_num == X_RIGHT) {
        if (p.x / p.w <= 1) return 1;
        else return -1;
    }
    else if (plane_num == X_LEFT) {
        if (p.x / p.w >= -1) return 1;
        else return -1;
    }
    else if (plane_num == Y_TOP) {
        if (p.y / p.w <= 1) return 1;
        else return -1;
    }
    else if (plane_num == Y_BOTTOM) {
        if (p.y / p.w >= -1) return 1;
        else return -1;
    }
    else if (plane_num == Z_NEAR) {
        if (p.z / p.w >= -1) return 1;
        else return -1;
    }
    else {
        if (p.z / p.w <= 1) return 1;
        else return -1;
    }
}

//get the point p = p0 + t(p1 - p0)
Point4f __clip_get(int plane_num, Point4f &p0, Point4f &p1) {
    float t = 0;
    if (plane_num == W_PLANE) 
        t = (1e-5 - p0.w) / (p1.w - p0.w);
    else if (plane_num == X_RIGHT) 
        t = (p0.w - p0.x) / (p1.x - p0.x - p1.w + p0.w);
    else if (plane_num == X_LEFT) 
        t = -(p0.w + p0.x) / (p1.w - p0.w + p1.x- p0.x);
    else if (plane_num == Y_TOP)
        t = (p0.w - p0.y) / (p1.y - p0.y - p1.w + p0.w);
    else if (plane_num == Y_BOTTOM)
        t = -(p0.w + p0.y) / (p1.w - p0.w + p1.y - p0.y);
    else if (plane_num == Z_FAR)
        t = (p0.w - p0.z) / (p1.z - p0.z - p1.w + p0.w);
    else t = -(p0.w + p0.z) / (p1.w - p0.w + p1.z - p0.z);

    float x = p0.x + t * (p1.x - p0.x);
    float y = p0.y + t * (p1.y - p0.y);
    float z = p0.z + t * (p1.z - p0.z);
    float w = p0.w + t * (p1.w - p0.w);
    
    return Point4f(x, y, z, w);
}

void clip_with_plane(int plane_num, std::vector<Point4f> &in_list, std::vector<Point4f> &out_list) {
    int vert_num = in_list.size();
    for (int i = 0; i < vert_num; i++) {
        int last = (i + vert_num - 1) % vert_num;
        Point4f cvert = in_list[i], lvert = in_list[last];
        if (__clip_check(plane_num, cvert) * __clip_check(plane_num, lvert) < 0)
            out_list.push_back(__clip_get(plane_num, cvert, lvert));
        if (__clip_check(plane_num, cvert) > 0) out_list.push_back(cvert);    
    }   
}