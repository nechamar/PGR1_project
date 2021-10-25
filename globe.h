/**
 * \file       entity.h
 * \author     Marek Nechansky
 * \date       2018/28/04
 * \brief      subclass of entity for globe or sphere entities
 *
*/
#ifndef PGR_GLOBE_H
#define PGR_GLOBE_H

#include "entity.h"

/**
 * Subclass of entity, which overrides setupExplosion and isCollision
 * specified for globes e.g. moon
 */
namespace nechamar {

    class globe : public nechamar::entity {
    private:
        /// size used for calculation size of explosions and collision bounds
        static const int sizeOfGlobe = 120;
    protected:
        void setupExplosion(explosion *explosion1) override;
    public:
        /// Constructor
        /**
            Calls entity constructor with its parameters
            Sets value of spline pointer to null pointer (globes does not move)
         */
        globe(nechamar::model *entityModel, long posX, long posY, long posZ, float yaw, float roll, float pitch,
              float scale, glm::vec3 posOffset = glm::vec3(0));
        /**
            Returns if the cameraPosition is within specified sphere
         */
        bool isCollision(glm::vec3 cameraPosition) override;

    };

}


#endif //PGR_GLOBE_H
