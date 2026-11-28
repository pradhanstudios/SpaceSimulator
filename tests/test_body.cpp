#include "test.hpp"
#include "body.hpp"
#include "deltaTime.hpp"
#include "physics.hpp"

TEST("body: constructor stores state and starts with no force") {
    Body b(2.f, glm::vec3(1, 2, 3), glm::vec3(4, 5, 6), 7.f, false);
    CHECK_EQ(b.getRadius(), 2.f);
    CHECK_VEC_NEAR(b.getPos(), glm::vec3(1, 2, 3), 0.f);
    CHECK_VEC_NEAR(b.getVelocity(), glm::vec3(4, 5, 6), 0.f);
    CHECK_EQ(b.getMass(), 7.f);
    CHECK_VEC_NEAR(b.getForce(), glm::vec3(0.f), 0.f);
    CHECK_VEC_NEAR(b.getAcceleration(), glm::vec3(0.f), 0.f);
    CHECK(b.getMesh() == nullptr);
}

TEST("body: setters round-trip") {
    Body b(1.f, glm::vec3(0.f), glm::vec3(0.f), 1.f, false);
    b.setRadius(3.f);
    b.setMass(4.f);
    b.setPos(glm::vec3(1, 1, 1));
    b.setVelocity(glm::vec3(2, 2, 2));
    b.setAcceleration(glm::vec3(3, 3, 3));
    b.setForce(glm::vec3(4, 4, 4));
    b.setMomentum(glm::vec3(5, 5, 5));
    CHECK_EQ(b.getRadius(), 3.f);
    CHECK_EQ(b.getMass(), 4.f);
    CHECK_VEC_NEAR(b.getPos(), glm::vec3(1.f), 0.f);
    CHECK_VEC_NEAR(b.getVelocity(), glm::vec3(2.f), 0.f);
    CHECK_VEC_NEAR(b.getAcceleration(), glm::vec3(3.f), 0.f);
    CHECK_VEC_NEAR(b.getForce(), glm::vec3(4.f), 0.f);
    CHECK_VEC_NEAR(b.getMomentum(), glm::vec3(5.f), 0.f);
}

TEST("body: acceleration is force over mass") {
    Body b(1.f, glm::vec3(0.f), glm::vec3(0.f), 4.f, false);
    b.setForce(glm::vec3(8, -4, 2));
    b.updateAcc();
    CHECK_VEC_NEAR(b.getAcceleration(), glm::vec3(2, -1, 0.5f), 1e-6f);
}

TEST("body: velocity and position advance by physicsDeltaTime") {
    Body b(1.f, glm::vec3(0.f), glm::vec3(1, 0, 0), 1.f, false);
    physicsDeltaTime = 0.5f;
    b.setAcceleration(glm::vec3(0, 2, 0));
    b.updateVel();
    CHECK_VEC_NEAR(b.getVelocity(), glm::vec3(1, 1, 0), 1e-6f);
    b.updatePos();
    CHECK_VEC_NEAR(b.getPos(), glm::vec3(0.5f, 0.5f, 0), 1e-6f);
}

TEST("body: zero time step leaves the body unchanged") {
    Body b(1.f, glm::vec3(3, 3, 3), glm::vec3(1, 2, 3), 1.f, false);
    physicsDeltaTime = 0.f;
    b.setAcceleration(glm::vec3(5.f));
    b.updateVel();
    b.updatePos();
    CHECK_VEC_NEAR(b.getVelocity(), glm::vec3(1, 2, 3), 0.f);
    CHECK_VEC_NEAR(b.getPos(), glm::vec3(3.f), 0.f);
}

TEST("body: calcGravForceVec matches calcGravityForceVectors") {
    Body a(1.f, glm::vec3(0.f), glm::vec3(0.f), 2.f, false);
    Body b(1.f, glm::vec3(3, 4, 0), glm::vec3(0.f), 5.f, false);
    glm::vec3 f1, f2, e1, e2;
    a.calcGravForceVec(&b, f1, f2);
    calcGravityForceVectors(2.f, 5.f, a.getPos(), b.getPos(), e1, e2);
    CHECK_VEC_NEAR(f1, e1, 1e-6f);
    CHECK_VEC_NEAR(f2, e2, 1e-6f);
}
