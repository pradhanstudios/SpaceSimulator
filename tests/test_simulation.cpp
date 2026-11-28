#include "test.hpp"
#include "body.hpp"
#include "physics.hpp"
#include "deltaTime.hpp"

namespace {

struct System {
    std::vector<Body*> bodies;
    ~System() { for (Body* b : bodies) delete b; }

    void add(glm::vec3 pos, glm::vec3 vel, float mass) {
        bodies.push_back(new Body(1.f, pos, vel, mass, false));
    }

    void run(int steps, float dt) {
        physicsDeltaTime = dt;
        for (int i = 0; i < steps; i++) stepPhysics(bodies);
    }

    glm::vec3 momentum() const {
        glm::vec3 p(0.f);
        for (Body* b : bodies) p += b->getMass() * b->getVelocity();
        return p;
    }

    glm::vec3 centreOfMass() const {
        glm::vec3 c(0.f);
        float total = 0.f;
        for (Body* b : bodies) { c += b->getMass() * b->getPos(); total += b->getMass(); }
        return c / total;
    }

    double energy() const {
        double e = 0.0;
        for (size_t i = 0; i < bodies.size(); i++) {
            glm::vec3 v = bodies[i]->getVelocity();
            e += 0.5 * bodies[i]->getMass() * glm::dot(v, v);
            for (size_t j = i + 1; j < bodies.size(); j++)
                e -= G * bodies[i]->getMass() * bodies[j]->getMass() / glm::distance(bodies[i]->getPos(), bodies[j]->getPos());
        }
        return e;
    }
};

void addCircularBinary(System& s, float heavyMass, float lightMass, float radius) {
    float relativeSpeed = std::sqrt(G * (heavyMass + lightMass) / radius);
    float total = heavyMass + lightMass;
    s.add(glm::vec3(0.f), glm::vec3(-relativeSpeed * lightMass / total, 0, 0), heavyMass);
    s.add(glm::vec3(0, radius, 0), glm::vec3(relativeSpeed * heavyMass / total, 0, 0), lightMass);
}

}

TEST("simulation: one step pulls two resting bodies toward each other") {
    System s;
    s.add(glm::vec3(0.f), glm::vec3(0.f), 1.f);
    s.add(glm::vec3(10, 0, 0), glm::vec3(0.f), 1.f);
    s.run(1, 0.01f);
    CHECK(s.bodies[0]->getPos().x > 0.f);
    CHECK(s.bodies[1]->getPos().x < 10.f);
    CHECK_VEC_NEAR(s.bodies[0]->getForce() + s.bodies[1]->getForce(), glm::vec3(0.f), 1e-6f);
}

TEST("simulation: forces are recomputed from zero every step") {
    System s;
    s.add(glm::vec3(0.f), glm::vec3(0.f), 1.f);
    s.add(glm::vec3(10, 0, 0), glm::vec3(0.f), 1.f);
    s.bodies[0]->setForce(glm::vec3(1000.f));
    s.run(1, 0.f);
    CHECK_NEAR(glm::length(s.bodies[0]->getForce()), calcGravityForceMagnitude(1.f, 1.f, 10.f), 1e-6);
}

TEST("simulation: heavier body accelerates less") {
    System s;
    s.add(glm::vec3(0.f), glm::vec3(0.f), 1.f);
    s.add(glm::vec3(5, 0, 0), glm::vec3(0.f), 10.f);
    s.run(1, 0.01f);
    CHECK_NEAR(glm::length(s.bodies[0]->getAcceleration()), 10.f * glm::length(s.bodies[1]->getAcceleration()), 1e-4);
}

TEST("simulation: a lone body moves in a straight line") {
    System s;
    s.add(glm::vec3(0.f), glm::vec3(1, 2, 3), 5.f);
    s.run(100, 0.01f);
    CHECK_VEC_NEAR(s.bodies[0]->getPos(), glm::vec3(1, 2, 3), 1e-4f);
    CHECK_VEC_NEAR(s.bodies[0]->getVelocity(), glm::vec3(1, 2, 3), 0.f);
}

TEST("simulation: total momentum is conserved") {
    System s;
    s.add(glm::vec3(0.f), glm::vec3(10, 0, 0), 1.f);
    s.add(glm::vec3(0, 10, 0), glm::vec3(0.f), 1000.f);
    s.add(glm::vec3(5, -3, 2), glm::vec3(0, 1, -1), 50.f);
    glm::vec3 before = s.momentum();
    s.run(10000, 0.001f);
    CHECK_VEC_NEAR(s.momentum(), before, glm::length(before) * 1e-3f);
}

TEST("simulation: centre of mass drifts at constant velocity") {
    System s;
    s.add(glm::vec3(0.f), glm::vec3(1, 0, 0), 3.f);
    s.add(glm::vec3(0, 4, 0), glm::vec3(0, 0, 1), 1.f);
    float total = 4.f;
    glm::vec3 start = s.centreOfMass();
    glm::vec3 velocity = s.momentum() / total;
    s.run(1000, 0.001f);
    CHECK_VEC_NEAR(s.centreOfMass(), start + velocity * 1.f, 1e-2f);
}

TEST("simulation: circular orbit keeps its radius") {
    System s;
    addCircularBinary(s, 1000.f, 1.f, 10.f);
    float minDist = 1e9f, maxDist = 0.f;
    physicsDeltaTime = 0.001f;
    for (int i = 0; i < 20000; i++) {
        stepPhysics(s.bodies);
        float d = glm::distance(s.bodies[0]->getPos(), s.bodies[1]->getPos());
        minDist = std::min(minDist, d);
        maxDist = std::max(maxDist, d);
    }
    CHECK_NEAR(minDist, 10.f, 0.1);
    CHECK_NEAR(maxDist, 10.f, 0.1);
}

TEST("simulation: orbital energy drift stays bounded") {
    System s;
    addCircularBinary(s, 1000.f, 1.f, 10.f);
    double before = s.energy();
    s.run(20000, 0.001f);
    CHECK_NEAR(s.energy(), before, std::fabs(before) * 0.01);
}

TEST("simulation: symmetric triangle collapses symmetrically") {
    System s;
    float r = 10.f;
    for (int k = 0; k < 3; k++) {
        float angle = 2.f * float(M_PI) * k / 3.f;
        s.add(glm::vec3(r * std::cos(angle), r * std::sin(angle), 0), glm::vec3(0.f), 100.f);
    }
    glm::vec3 centre = s.centreOfMass();
    s.run(200, 0.001f);
    float d01 = glm::distance(s.bodies[0]->getPos(), s.bodies[1]->getPos());
    float d12 = glm::distance(s.bodies[1]->getPos(), s.bodies[2]->getPos());
    float d20 = glm::distance(s.bodies[2]->getPos(), s.bodies[0]->getPos());
    CHECK(d01 < r * std::sqrt(3.f));
    CHECK_NEAR(d01, d12, 1e-3);
    CHECK_NEAR(d12, d20, 1e-3);
    CHECK_VEC_NEAR(s.centreOfMass(), centre, 1e-3f);
}

TEST("simulation: more substeps at the same total time give the same orbit") {
    System coarse, fine;
    addCircularBinary(coarse, 1000.f, 1.f, 10.f);
    addCircularBinary(fine, 1000.f, 1.f, 10.f);
    coarse.run(1000, 0.002f);
    fine.run(2000, 0.001f);
    CHECK_VEC_NEAR(coarse.bodies[1]->getPos(), fine.bodies[1]->getPos(), 0.1f);
}

TEST("simulation: coincident bodies stay finite") {
    System s;
    s.add(glm::vec3(1, 1, 1), glm::vec3(0.f), 1.f);
    s.add(glm::vec3(1, 1, 1), glm::vec3(0.f), 1.f);
    s.run(10, 0.01f);
    for (Body* b : s.bodies) {
        glm::vec3 p = b->getPos(), v = b->getVelocity();
        CHECK(std::isfinite(p.x) && std::isfinite(p.y) && std::isfinite(p.z));
        CHECK(std::isfinite(v.x) && std::isfinite(v.y) && std::isfinite(v.z));
    }
}
