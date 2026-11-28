#include "physics.hpp"
#include "body.hpp"

void stepPhysics(std::vector<Body*>& bodies) {
    for (Body* b : bodies) b->setForce(glm::vec3(0.f));

    for (size_t i = 0; i < bodies.size(); i++) {
        for (size_t j = i + 1; j < bodies.size(); j++) {
            glm::vec3 f1(0.f), f2(0.f);
            bodies[i]->calcGravForceVec(bodies[j], f1, f2);
            bodies[i]->setForce(bodies[i]->getForce() + f1);
            bodies[j]->setForce(bodies[j]->getForce() + f2);
        }
    }

    for (Body* b : bodies) {
        b->updateAcc();
        b->updateVel();
        b->updatePos();
    }
}
