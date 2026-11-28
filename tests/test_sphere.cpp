#include "test.hpp"
#include "sphere.hpp"

namespace {

constexpr int FLOATS_PER_VERTEX = 11;
constexpr int VERTICES_PER_BAND = 2 * (SPHERE_RESOLUTION + 1);
constexpr int DRAWN_VERTICES = SPHERE_RESOLUTION * VERTICES_PER_BAND;

struct Vertex {
    glm::vec3 pos, normal, tangent;
    float u, v;
};

Vertex vertexAt(const std::vector<float>& buf, size_t index) {
    const float* f = &buf[index * FLOATS_PER_VERTEX];
    return {{f[0], f[1], f[2]}, {f[3], f[4], f[5]}, {f[8], f[9], f[10]}, f[6], f[7]};
}

size_t indexOf(int band, int column, int slot) {
    return size_t(band) * VERTICES_PER_BAND + size_t(column) * 2 + size_t(slot);
}

}

TEST("sphere: buffer holds at least every vertex the renderer draws") {
    std::vector<float> buf = generateSphereVertices(1.f);
    CHECK_EQ(buf.size() % FLOATS_PER_VERTEX, size_t(0));
    CHECK(buf.size() / FLOATS_PER_VERTEX >= size_t(DRAWN_VERTICES));
}

TEST("sphere: every vertex lies on the sphere surface") {
    for (float r : {0.5f, 1.f, 7.f}) {
        std::vector<float> buf = generateSphereVertices(r);
        int bad = 0;
        for (int i = 0; i < DRAWN_VERTICES; i++)
            if (std::fabs(glm::length(vertexAt(buf, i).pos) - r) > 1e-4f * r) bad++;
        CHECK_EQ(bad, 0);
    }
}

TEST("sphere: normals are unit length and point outward") {
    float r = 3.f;
    std::vector<float> buf = generateSphereVertices(r);
    int bad = 0;
    for (int i = 0; i < DRAWN_VERTICES; i++) {
        Vertex v = vertexAt(buf, i);
        if (std::fabs(glm::length(v.normal) - 1.f) > 1e-4f || glm::length(v.normal - v.pos / r) > 1e-4f) bad++;
    }
    CHECK_EQ(bad, 0);
}

TEST("sphere: texture coordinates are within [0, 1]") {
    std::vector<float> buf = generateSphereVertices(1.f);
    int bad = 0;
    for (int i = 0; i < DRAWN_VERTICES; i++) {
        Vertex v = vertexAt(buf, i);
        if (v.u < 0.f || v.u > 1.f || v.v < 0.f || v.v > 1.f) bad++;
    }
    CHECK_EQ(bad, 0);
}

TEST("sphere: tangents are unit length and perpendicular to normals") {
    std::vector<float> buf = generateSphereVertices(1.f);
    int bad = 0;
    for (int i = 0; i < DRAWN_VERTICES; i++) {
        Vertex v = vertexAt(buf, i);
        bool finite = std::isfinite(v.tangent.x) && std::isfinite(v.tangent.y) && std::isfinite(v.tangent.z);
        if (!finite || std::fabs(glm::length(v.tangent) - 1.f) > 1e-3f || std::fabs(glm::dot(v.tangent, v.normal)) > 1e-3f) bad++;
    }
    CHECK_EQ(bad, 0);
}

TEST("sphere: adjacent bands share their boundary ring") {
    std::vector<float> buf = generateSphereVertices(2.f);
    int bad = 0;
    for (int band = 0; band + 1 < SPHERE_RESOLUTION; band++)
        for (int col = 0; col <= SPHERE_RESOLUTION; col++)
            if (glm::length(vertexAt(buf, indexOf(band, col, 0)).pos - vertexAt(buf, indexOf(band + 1, col, 1)).pos) > 1e-5f) bad++;
    CHECK_EQ(bad, 0);
}

TEST("sphere: first and last columns of each band meet at the longitude seam") {
    std::vector<float> buf = generateSphereVertices(2.f);
    int bad = 0;
    for (int band = 0; band < SPHERE_RESOLUTION; band++)
        for (int slot = 0; slot < 2; slot++)
            if (glm::length(vertexAt(buf, indexOf(band, 0, slot)).pos - vertexAt(buf, indexOf(band, SPHERE_RESOLUTION, slot)).pos) > 1e-4f) bad++;
    CHECK_EQ(bad, 0);
}

TEST("sphere: bands run from the -z pole to the +z pole") {
    float r = 4.f;
    std::vector<float> buf = generateSphereVertices(r);
    CHECK_VEC_NEAR(vertexAt(buf, indexOf(0, 0, 1)).pos, glm::vec3(0, 0, -r), 1e-4f);
    CHECK_VEC_NEAR(vertexAt(buf, indexOf(SPHERE_RESOLUTION - 1, 0, 0)).pos, glm::vec3(0, 0, r), 1e-4f);
}
