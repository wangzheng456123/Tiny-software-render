/*
this function caculate the intersection of the optic cone with 
a triangle mesh, it is used to process the triangle outside the 
optic cone correctly.
*/
#include <vector>
#include "./vmath/Point.h"

void clip_with_plane(int plane_num, std::vector<Point4f> &in_list, std::vector<Point4f> &out_list);