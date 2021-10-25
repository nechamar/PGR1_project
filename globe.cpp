//
// Created by nechamar on 28.4.18.
//

#include "globe.h"

nechamar::globe::globe(nechamar::model *entityModel, long posX, long posY, long posZ, float yaw, float roll, float pitch,
             float scale, glm::vec3 posOffset) : entity(entityModel, posX, posY, posZ, yaw, roll, pitch, scale, nullptr , posOffset) {}

void nechamar::globe::setupExplosion(nechamar::explosion *explosion1) {
    entity::setupExplosion(explosion1);
    explosion1->size = sizeOfGlobe * scale;
}

bool nechamar::globe::isCollision(glm::vec3 cameraPosition) {
    return (glm::distance(cameraPosition,glm::vec3(posX, posY, posZ)) < sizeOfGlobe * scale);
}
