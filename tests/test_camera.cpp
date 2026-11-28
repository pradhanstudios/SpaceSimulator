#include "test.hpp"
#include "camera.hpp"

namespace {

void checkOrthonormal(Camera& c) {
    glm::vec3 f = c.getFront(), r = c.getRightAxis();
    glm::vec3 u = glm::cross(r, f);
    CHECK_NEAR(glm::length(f), 1.f, 1e-5);
    CHECK_NEAR(glm::length(r), 1.f, 1e-5);
    CHECK_NEAR(glm::dot(f, r), 0.f, 1e-5);
    CHECK_NEAR(glm::dot(u, f), 0.f, 1e-5);
    CHECK_NEAR(r.y, 0.f, 1e-5);
}

}

TEST("camera: default orientation looks down -z") {
    Camera c(glm::vec3(0, 0, 10));
    CHECK_EQ(c.getYaw(), cameraDefaultYaw);
    CHECK_EQ(c.getPitch(), cameraDefaultPitch);
    CHECK_VEC_NEAR(c.getFront(), glm::vec3(0, 0, -1), 1e-5f);
    CHECK_VEC_NEAR(c.getRightAxis(), glm::vec3(1, 0, 0), 1e-5f);
}

TEST("camera: axes stay orthonormal after mouse movement") {
    Camera c(glm::vec3(0.f));
    checkOrthonormal(c);
    c.processMouse(3.7, 1.2);
    checkOrthonormal(c);
    c.processMouse(-20.0, -9.0);
    checkOrthonormal(c);
}

TEST("camera: pitch clamps at +/-89 degrees") {
    Camera c(glm::vec3(0.f));
    c.processMouse(0.0, 1000.0);
    CHECK_EQ(c.getPitch(), 89.f);
    CHECK(c.getFront().y > 0.99f);
    checkOrthonormal(c);
    c.processMouse(0.0, -5000.0);
    CHECK_EQ(c.getPitch(), -89.f);
    CHECK(c.getFront().y < -0.99f);
    checkOrthonormal(c);
}

TEST("camera: mouse offsets scale by sensitivity") {
    Camera c(glm::vec3(0.f), cameraDefaultFOV, 0.f, 0.f, 2.f);
    c.processMouse(3.0, 4.0);
    CHECK_NEAR(c.getYaw(), -6.f, 1e-5);
    CHECK_NEAR(c.getPitch(), 8.f, 1e-5);
}

TEST("camera: view matrix maps camera position to origin, looking down -z") {
    Camera c(glm::vec3(1, 2, 3));
    c.processMouse(5.0, 2.0);
    c.updateView();
    glm::mat4 view = c.getView();
    glm::vec4 eye = view * glm::vec4(c.getPosition(), 1.f);
    glm::vec4 ahead = view * glm::vec4(c.getPosition() + c.getFront(), 1.f);
    CHECK_VEC_NEAR(glm::vec3(eye), glm::vec3(0.f), 1e-4f);
    CHECK_VEC_NEAR(glm::vec3(ahead), glm::vec3(0, 0, -1), 1e-4f);
}

TEST("camera: getViewForPosition places origin-built meshes at the body position") {
    Camera c(glm::vec3(4, -2, 9));
    c.processMouse(1.5, -0.7);
    c.updateView();
    glm::vec3 bodyPos(3, 1, -5);
    glm::mat4 bodyView = c.getViewForPosition(bodyPos);
    for (glm::vec3 local : {glm::vec3(0.f), glm::vec3(1, 0, 0), glm::vec3(0, -2, 0.5f)}) {
        glm::vec4 viaBody = bodyView * glm::vec4(local, 1.f);
        glm::vec4 viaWorld = c.getView() * glm::vec4(local + bodyPos, 1.f);
        CHECK_VEC_NEAR(glm::vec3(viaBody), glm::vec3(viaWorld), 1e-4f);
    }
}

TEST("camera: projection uses default aspect ratio and FOV") {
    Camera c(glm::vec3(0.f));
    glm::mat4 p = c.getProjection();
    CHECK_NEAR(p[1][1] / p[0][0], float(defaultWidth) / float(defaultHeight), 1e-4);
    CHECK_NEAR(p[1][1], 1.f / std::tan(glm::radians(cameraDefaultFOV) / 2.f), 1e-4);
}

TEST("camera: position and zoom setters round-trip") {
    Camera c(glm::vec3(0.f));
    c.setPosition(glm::vec3(7, 8, 9));
    c.setZoom(12.f);
    CHECK_VEC_NEAR(c.getPosition(), glm::vec3(7, 8, 9), 0.f);
    CHECK_EQ(c.getZoom(), 12.f);
    CHECK_EQ(c.getFOV(), cameraDefaultFOV);
}
