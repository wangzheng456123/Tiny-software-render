#include "animation.h"
#include <sstream>
#include <fstream>
/*
std::vector<Joint> joints;
std::vector<int> *adj;
float max_time, min_time;
void clear();
*/

Joint::Joint() : model2joint(), joint2parent(), joint2world(), transvert(), transnorm(),
tframe(0), qframe(0), sframe(0), idx(0), p(0), skeleton(nullptr) {}

Skeleton::Skeleton(const std::string &filename) : max_time(0), min_time(0) {
    std::ifstream in;
    in.open (filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line);
        std::string tag;
        iss >> tag;
        char trash;
        std::string strash;
        if (tag == "joint-size:") { 
            int n; iss >> n;
            adj = new std::vector<int>[n];
        }
        else if (tag == "time-range:") 
            iss >> trash >> min_time >> trash >> max_time;
        else if (tag == "joint") {
            Joint joint = Joint();
            joint.skeleton = this;
            iss >> joint.idx;
            joints.push_back(joint);
        }
        else if (tag == "parent-index:") {
            int idx = joints.size() - 1;
            Joint &joint = joints[idx];
            iss >> joint.p;
            if (joint.p >= 0)
                 adj[joint.p].push_back(idx); 
        }
        else if (tag == "inverse-bind:") {
            Joint &joint  = joints[joints.size() - 1];
            Matrix4x4 m;
            for (int i = 0; i < 4; i++) {
                std::getline(in, line);
                std::istringstream tss(line);
                for (int j = 0; j < 4; j++) 
                    tss >> m.mat[i][j];
            }
            joint.model2joint = Transform(m);
        }
        else if (tag == "translations") {
            int ntrans; iss >> ntrans;
            Joint &joint = joints[joints.size() - 1];
            for (int i = 0; i < ntrans; i++) {
                std::getline(in, line);
                std::istringstream tss(line);
                float t;
                tss >> strash;
                tss >> t >> strash;
                joint.transtime.push_back(t);
                tss >> strash >> trash;
                Vector3f offset;
                for (int j = 0; j < 3; j++) 
                    tss >> offset[j] >> strash;
                joint.offset.push_back(offset);
            }
        }
        else if (tag == "rotations") {
            int ntrans; iss >> ntrans;
            Joint &joint = joints[joints.size() - 1];
            for (int i = 0; i < ntrans; i++) {
                std::getline(in, line);
                std::istringstream tss(line);
                float t;
                tss >> strash;
                tss >> t >> strash;
                joint.rotatetime.push_back(t);
                tss >> strash >> trash;
                Vector3f v;
                for (int j = 0; j < 3; j++) 
                    tss >> v[j] >> trash;
                float w; tss >> w;
                joint.quater.push_back(Quaternion(v, w));
            }
        }
        else if (tag == "scales") {
            int nscales; iss >> nscales;
            Joint &joint = joints[joints.size() - 1];
            for (int i = 0; i < nscales; i++) {
                std::getline(in, line);
                std::istringstream tss(line);
                float t;
                tss >> strash;
                tss >> t >> strash;
                joint.scaletime.push_back(t);
                tss >> strash >> trash;
                Vector3f s;
                for (int j = 0; j < 3; j++) 
                    tss >> s[j] >> trash;
                joint.scale.push_back(s);
            }
        }
    }
}

void Skeleton::clear() {
    for (auto &joint : joints) {
        joint.qframe = 0;
        joint.tframe = 0;
        joint.sframe = 0;
    }
}

//top-down method to caculate current joint to world transform of the joints in the skeleton,
//see my article for a brief proof for this method.
void Joint::update(float ctime) {
    Transform ctrans, crotate, cscale;
    while (tframe + 1 < transtime.size() && ctime > transtime[tframe + 1]) tframe++;
    if (tframe + 1 < transtime.size()) {
        float tt = (transtime[tframe + 1] - ctime) / (transtime[tframe + 1] - transtime[tframe]);
        Vector3f &coffset = offset[tframe], &noffset = offset[tframe + 1];
        ctrans = Translate(tt * coffset + (1 - tt) * noffset);
    }
    while (qframe + 1 < rotatetime.size() && ctime > rotatetime[qframe + 1]) qframe++;
    if (qframe + 1 < rotatetime.size()) {
        float tr = (rotatetime[qframe + 1] - ctime) / (rotatetime[qframe + 1] - rotatetime[qframe]);
        Quaternion cq = quater[qframe], nq = quater[qframe + 1];
        if (Dot(cq, nq) < 0) cq = -cq;
        crotate = slerp(tr, nq, cq).ToTransform();
    }
    while (sframe + 1 < scaletime.size() && ctime > scaletime[sframe + 1]) sframe++;
    if (sframe + 1 < scaletime.size()) {
        float ts = (scaletime[sframe + 1] - ctime) / (scaletime[sframe + 1] - scaletime[sframe]);
        Vector3f &csca = scale[sframe], &ns = scale[sframe + 1];
        Vector3f cs = ts * csca + (1 - ts) * ns;
        cscale = Scale(cs[0], cs[1], cs[2]);
    }
    joint2parent = ctrans * crotate * cscale;
    if (p >= 0) 
        joint2world = skeleton->joints[p].joint2world * joint2parent;
    transvert = joint2world * model2joint;
    transnorm = Inverse(transvert);
}

void update(float ctime, int r, Skeleton &skeleton) {
    Joint &joint = skeleton.joints[r];
    joint.update(ctime);
    int n = skeleton.adj[r].size();
    for (int i = 0; i < n; i++)
        update(ctime, skeleton.adj[r][i], skeleton); 
}