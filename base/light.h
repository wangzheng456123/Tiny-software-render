#include "./vmath/Point.h"

class Light {
public:
    virtual Vector3f get_dir(const Point3f &pos) = 0;
    virtual float get_intensity(const Point3f &pos) = 0;
};

class ParLight : public Light {
public:
    ParLight(Vector3f light_dir) : light_dir(light_dir) {};
    virtual Vector3f get_dir(const Point3f &pos) {return Normalize(light_dir);};
    virtual float get_intensity(const Point3f &pos) {return 1.;};
private: Vector3f light_dir;    
};

class PointLight : public Light {
public :
    PointLight(Point3f src) : src(src) {};
    virtual Vector3f get_dir(const Point3f &pos) {return Normalize(pos - src);};
    virtual float get_intensity(const Point3f &pos) {return 3. / (pos - src).Length();};
private : 
    Point3f src;
};