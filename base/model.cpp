#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include "model.h"

Model::Model(const char *filename) : verts_(), faces_(), norms_(), uv_(), diffusemap_(), normalmap_(), specularmap_() {
    std::ifstream in;
    in.open (filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        std::string trash1, trash2;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Point3f v;
            for (int i=0;i<3;i++) iss >> v[i];
            verts_.push_back(v);
        } else if (!line.compare(0, 3, "vn ")) {
            iss >> trash >> trash;
            Vector3f n;
            for (int i=0;i<3;i++) iss >> n[i];
            norms_.push_back(n);
        } else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            Vector2f uv;
            for (int i=0;i<2;i++) iss >> uv[i];
            uv_.push_back(uv);
        }  else if (!line.compare(0, 2, "f ")) {
            std::vector<Vector3i> f;
            Vector3i tmp;
            iss >> trash;
            while (iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2]) {
                for (int i=0; i<3; i++) tmp[i]--; // in wavefront obj all indices start at 1, not zero
                f.push_back(tmp);
            }
            faces_.push_back(f);
        }  else if (!line.compare(0, 12, "# ext.joint ")) {
            iss >> trash1 >> trash2;
            Point4i tmp;
            for (int i = 0; i < 4; i++) iss >> tmp[i];
            jointidx__.push_back(tmp);
        }  else if (!line.compare(0, 13, "# ext.weight ")) {
            iss >> trash1 >> trash2;
            Point4f tmp;
            for (int i = 0; i < 4; i++) iss >> tmp[i];
            jointweight__.push_back(tmp);
        }
        
    }
    std::cerr << "# v# " << verts_.size() << " f# "  << faces_.size() << " vt# " << uv_.size() << " vn# " << norms_.size() << std::endl;
    //non flexible teture load routine, sometimes usefull. 
    //load_texture(filename, "_diffuse.tga", diffusemap_);
    //load_texture(filename, "_nm.tga",      normalmap_);
    //load_texture(filename, "_spec.tga",    specularmap_);
}

Model::~Model() {}

int Model::nverts() const {
    return (int)verts_.size();
}

int Model::nfaces() const {
    return (int)faces_.size();
}

std::vector<int> Model::face(int idx) {
    std::vector<int> face;
    for (int i=0; i<(int)faces_[idx].size(); i++) face.push_back(faces_[idx][i][0]);
    return face;
}

Point3f Model::vert(int i) {
    return verts_[i];
}

Point3f Model::vert(int iface, int nthvert) {
    return verts_[faces_[iface][nthvert][0]];
}

int Model::vertid(int iface, int nthvert) {
    return faces_[iface][nthvert][0];
}

void Model::load_texture(std::string filename, const char *suffix, const char *type) {
    TGAImage *texture = &diffusemap_;
    if (std::strcmp(type, "normal") == 0) 
        texture = &normalmap_;
    else if (std::strcmp(type, "specular") == 0)
        texture = &specularmap_;
    TGAImage &img = *texture;
    std::string texfile(filename);
    size_t dot = texfile.find_last_of(".");
    if (dot!=std::string::npos) {
        texfile = texfile.substr(0,dot) + std::string(suffix);
        std::cout << texfile << "\n";
        std::cerr << "texture file " << texfile << " loading " << (img.read_tga_file(texfile.c_str()) ? "ok" : "failed") << std::endl;
        img.flip_vertically();
    }
}

void Model::load_obj(const char *filename) {
    std::ifstream in;
    int num_vert = verts_.size();
    in.open (filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        std::string trash1, trash2;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Point3f v;
            for (int i=0;i<3;i++) iss >> v[i];
            verts_.push_back(v);
        } else if (!line.compare(0, 3, "vn ")) {
            iss >> trash >> trash;
            Vector3f n;
            for (int i=0;i<3;i++) iss >> n[i];
            norms_.push_back(n);
        } else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            Vector2f uv;
            for (int i=0;i<2;i++) iss >> uv[i];
            uv_.push_back(uv);
        }  else if (!line.compare(0, 2, "f ")) {
            std::vector<Vector3i> f;
            Vector3i tmp;
            iss >> trash;
            while (iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2]) {
                //some vertex may have already been read before
                for (int i=0; i<3; i++) tmp[i] += num_vert -  1;
                f.push_back(tmp);
            }
            faces_.push_back(f);
        }  else if (!line.compare(0, 12, "# ext.joint ")) {
            iss >> trash1 >> trash2;
            Point4i tmp;
            for (int i = 0; i < 4; i++) iss >> tmp[i];
            jointidx__.push_back(tmp);
        }  else if (!line.compare(0, 13, "# ext.weight ")) {
            iss >> trash1 >> trash2;
            Point4f tmp;
            for (int i = 0; i < 4; i++) iss >> tmp[i];
            jointweight__.push_back(tmp);
        }    
    }
}

TGAColor Model::diffuse(Vector2f uvf) {
    Vector2i uv(uvf[0]*diffusemap_.get_width(), uvf[1]*diffusemap_.get_height());
    return diffusemap_.get(uv[0], uv[1]);
}

Vector3f Model::normal(Vector2f uvf) {
    Vector2i uv(uvf[0]*normalmap_.get_width(), uvf[1]*normalmap_.get_height());
    TGAColor c = normalmap_.get(uv[0], uv[1]);
    Vector3f res;
    for (int i=0; i<3; i++)
        res[2-i] = (float)c[i]/255.f*2.f - 1.f;
    return res;
}

Vector3f Model::normal(int vertid) {
    return norms_[vertid];
}

Vector2f Model::uv(int iface, int nthvert) {
    return uv_[faces_[iface][nthvert][1]];
}

Vector2f Model::uv(int uvid) {
    return uv_[uvid];
}

float Model::specular(Vector2f uvf) {
    Vector2i uv(uvf[0]*specularmap_.get_width(), uvf[1]*specularmap_.get_height());
    return specularmap_.get(uv[0], uv[1])[0]/1.f;
}

Vector3f Model::normal(int iface, int nthvert) {
    int idx = faces_[iface][nthvert][2];
    return Normalize(norms_[idx]);
}

Point4i Model::jointidx(int idx) {
    return jointidx__[idx];
} 

Point4f Model::jointweight(int idx) {
    return jointweight__[idx];
}

void Model::flip() {
    for (auto& v : verts_)
        v.z = -v.z;
}