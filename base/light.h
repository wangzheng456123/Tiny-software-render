/*
Abstracted light class, each new implemented light sorces have
to write the get_dir and get_intensity method. The two method the
incident direction and light intensity from the light sorce to point 
pos.
*/
#include "./vmath/Point.h"

class Light {
public:
    virtual ~Light() = 0;
    virtual Vector3f get_dir(const Point3f &pos) = 0;
    virtual float get_intensity(const Point3f &pos) = 0;
};

//Directional light source
class ParLight : public Light {
public:
    ParLight(Vector3f light_dir);
    virtual Vector3f get_dir(const Point3f &pos) {return Normalize(light_dir);};
    virtual float get_intensity(const Point3f &pos) {return 1.;};
private: Vector3f light_dir;    
};

//Point light source
class PointLight : public Light {
public :
    PointLight(Point3f src);
    virtual Vector3f get_dir(const Point3f &pos) {return Normalize(pos - src);};
    virtual float get_intensity(const Point3f &pos) {return 3. / (pos - src).Length();};
private : 
    Point3f src;
};