//
// Created by nechamar on 31.3.18.
//

#include "starship.h"

nechamar::starship::starship(nechamar::model *entityModel, long posX, long posY, long posZ, float yaw, float roll,
                       float pitch, float scale, spline * path, glm::vec3 offset) : entity(entityModel, posX, posY, posZ, yaw, roll, pitch, scale, path, offset) {

}


bool nechamar::starship::isCollision(glm::vec3 cameraPosition) {
    return (glm::distance(cameraPosition,glm::vec3(posX, posY, posZ)) < size);
}
