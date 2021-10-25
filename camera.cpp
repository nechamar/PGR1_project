//
// Created by nechamar on 27.3.18.
//

#include "camera.h"

using namespace nechamar;

const glm::vec3 &camera::getPosition() const {
    return position;
}

void camera::setPosition (glm::vec3 position) {
    camera::position = position;
}

const glm::vec3 &camera::getDirection() const {
    return direction;
}

void camera::setDirection(glm::vec3 direction) {
    camera::direction = direction;
}

const glm::vec3 &camera::getUpVector() const {
    return upVector;
}

void camera::setUpVector(glm::vec3 upVector) {
    camera::upVector = upVector;
}
