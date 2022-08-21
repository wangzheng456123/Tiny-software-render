#include "light.h"

Light::~Light() {}
ParLight::ParLight(Vector3f light_dir) : light_dir(light_dir) {}
PointLight::PointLight(Point3f src) : src(src) {}