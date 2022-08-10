/*
Implementation of skeleton animation. 
*/

#include "./vmath/quaternion.h"
#include <string>
#include <vector>

typedef struct Skeleton Skeleton;

/*the skeletone is a tree like structure, each node in the tree
is represented as a joint.
*/ 

struct Joint {
    int idx, p;
    Transform model2joint; //transform point from model space to current joint space
    std::vector<float> transtime;
    std::vector<Vector3f> offset;
    std::vector<float> rotatetime;
    std::vector<Quaternion> quater;
    std::vector<float> scaletime;
    std::vector<Vector3f> scale;
    int tframe, qframe, sframe; 
    Transform joint2parent, joint2world; //current joint space to parrent joint space and current joint space to world space transform  
    Transform transvert, transnorm;
    void update(float ctime);
    Joint() : model2joint(), joint2parent(), joint2world(),
              tframe(0), qframe(0), sframe(0) {};
    Skeleton *skeleton; 
};

//meta data about the skeleton is maitained in the skeleton data structure.

struct Skeleton {
    Skeleton(const std::string &filename) ; 
    std::vector<Joint> joints;
    std::vector<int> *adj;
    float max_time, min_time;
    void clear();
};

// update joint state in the skeleton

void update(float ctime, int r, Skeleton &skeleton);

