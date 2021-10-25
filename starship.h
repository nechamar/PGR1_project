//----------------------------------------------------------------------------------------
/**
 * \file       model.h
 * \author     Marek Nechansky
 * \date       31.3.2018
 * \brief      Subclass of entity containing collision checking
*/
//----------------------------------------------------------------------------------------
#ifndef PGR_STARSHIP_H
#define PGR_STARSHIP_H


#include "entity.h"

namespace nechamar {
    /// size need for collision checking
    static const int size = 30;

    class starship : public entity {
    public:

        bool isCollision(glm::vec3 cameraPosition) override;

        starship(nechamar::model *entityModel, long posX, long posY, long posZ, float yaw, float roll, float pitch, float scale, spline * path = nullptr, glm::vec3 offset = glm::vec3(0,0,0));

    };

}
#endif //PGR_STARSHIP_H
