#include "sphere.hpp"

Sphere::Sphere(glm::vec3 pos, double r) {
    this->pos = pos;
    this->r = r;
}

Sphere::Sphere(double x, double y, double z, double r) {
    this->pos = glm::vec3(x, y, z);
    this->r = r;
}

