#include "test.hpp"
#include "physics.hpp"

TEST("physics: magnitude follows G*m1*m2/d^2") {
    CHECK_NEAR(calcGravityForceMagnitude(2.f, 3.f, 1.f), G * 6.f, 1e-5);
    CHECK_NEAR(calcGravityForceMagnitude(10.f, 5.f, 5.f), G * 50.f / 25.f, 1e-5);
}

TEST("physics: magnitude is symmetric in the masses") {
    CHECK_NEAR(calcGravityForceMagnitude(7.f, 2.f, 3.f), calcGravityForceMagnitude(2.f, 7.f, 3.f), 1e-6);
}

TEST("physics: magnitude falls off with inverse square of distance") {
    float near = calcGravityForceMagnitude(1.f, 1.f, 2.f);
    float far = calcGravityForceMagnitude(1.f, 1.f, 4.f);
    CHECK_NEAR(near / far, 4.f, 1e-5);
}

TEST("physics: magnitude is zero below the minimum distance") {
    CHECK_EQ(calcGravityForceMagnitude(1.f, 1.f, 0.f), 0.f);
    CHECK_EQ(calcGravityForceMagnitude(1.f, 1.f, 0.00005f), 0.f);
    CHECK(calcGravityForceMagnitude(1.f, 1.f, 0.001f) > 0.f);
}

TEST("physics: force vectors are equal, opposite and attractive") {
    glm::vec3 pos1(1.f, 2.f, 3.f), pos2(4.f, 6.f, 3.f);
    glm::vec3 f1, f2;
    calcGravityForceVectors(2.f, 3.f, pos1, pos2, f1, f2);

    CHECK_VEC_NEAR(f1 + f2, glm::vec3(0.f), 1e-6f);
    CHECK_NEAR(glm::length(f1), calcGravityForceMagnitude(2.f, 3.f, 5.f), 1e-5);
    CHECK(glm::dot(f1, pos2 - pos1) > 0.f);
    CHECK(glm::dot(f2, pos1 - pos2) > 0.f);
}

TEST("physics: coincident bodies produce finite forces") {
    glm::vec3 p(1.f, 1.f, 1.f);
    glm::vec3 f1, f2;
    calcGravityForceVectors(1.f, 1.f, p, p, f1, f2);
    CHECK(std::isfinite(f1.x) && std::isfinite(f1.y) && std::isfinite(f1.z));
    CHECK(std::isfinite(f2.x) && std::isfinite(f2.y) && std::isfinite(f2.z));
}

TEST("physics: substepsFor rounds speed up to whole steps, minimum one") {
    CHECK_EQ(substepsFor(0.01f), 1);
    CHECK_EQ(substepsFor(0.1f), 1);
    CHECK_EQ(substepsFor(1.f), 1);
    CHECK_EQ(substepsFor(1.5f), 2);
    CHECK_EQ(substepsFor(10.f), 10);
    CHECK_EQ(substepsFor(1000.f), 1000);
}

TEST("physics: no substep is longer than one real frame at any speed") {
    float frame = 1.f / 60.f;
    for (int e = minSpeedExponent; e <= maxSpeedExponent; e++) {
        float speed = std::pow(10.f, float(e));
        float step = frame * speed / substepsFor(speed);
        CHECK(step <= frame * 1.0001f);
        CHECK_NEAR(step * substepsFor(speed), frame * speed, frame * speed * 1e-5);
    }
}

TEST("physics: speed exponent clamps to its limits") {
    CHECK_EQ(clampSpeedExponent(0), 0);
    CHECK_EQ(clampSpeedExponent(maxSpeedExponent), maxSpeedExponent);
    CHECK_EQ(clampSpeedExponent(maxSpeedExponent + 1), maxSpeedExponent);
    CHECK_EQ(clampSpeedExponent(minSpeedExponent), minSpeedExponent);
    CHECK_EQ(clampSpeedExponent(minSpeedExponent - 1), minSpeedExponent);
}
