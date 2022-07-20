#include "./vmath/quaternion.h"
#include <string>
#include <vector>

typedef struct Skeleton Skeleton;

struct Joint {
    int idx, p;
    Transform model2joint;
    std::vector<float> transtime;
    std::vector<Vector3f> offset;
    std::vector<float> rotatetime;
    std::vector<Quaternion> quater;
    std::vector<float> scaletime;
    std::vector<Vector3f> scale;
    int tframe, qframe, sframe; 
    Transform joint2parent, joint2world;
    Transform transvert, transnorm;
    void update(float ctime);
    Joint() : model2joint(), joint2parent(), joint2world(),
              tframe(0), qframe(0), sframe(0) {};
    Skeleton *skeleton; 
};

struct Skeleton {
    Skeleton(const std::string &filename) ; 
    std::vector<Joint> joints;
    std::vector<int> *adj;
    float max_time, min_time;
    void clear();
};

void update(float ctime, int r, Skeleton &skeleton);

