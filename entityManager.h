//----------------------------------------------------------------------------------------
/**
 * \file       animation.h
 * \author     Marek Nechansky
 * \date       2018/15/03
 * \brief      declares class, that handles every object in a scene
 *
*/
//----------------------------------------------------------------------------------------
#include <pgr.h>
#include "entity.h"
#include "camera.h"
#include "light.h"
#include "skybox.h"
#include "guiObject.h"

#ifndef PGR_ENTITTYMANAGER_H
#define PGR_ENTITTYMANAGER_H

namespace nechamar {
/// class of an object, which controls every object within the scene
/**
 * Stores entities, lights, gui objects and explosions, skybox and cameras
 * holds index of current camera, id of hit entity and id of entity, which is moved by user
 * and sun's position (where it is on a circle)
 */

    class entityManager {
    private:
        std::vector<nechamar::entity *> entities;
        std::vector<light> lights;
        std::vector<explosion *> explosions;
        camera ** cameras;
        skybox * usedSkybox;
        unsigned int indexOfUsedCamera;
        unsigned char idOfEntityHitByMouse = 0;
        unsigned char idOfEntityMovedByMouse = 0;

        float sunPosition = 0;

        entity * getEntity(unsigned char id);

        void drawExplosion(glm::mat4 projection, glm::mat4 view, explosion * explosion1);

        void drawSkybox(glm::mat4 projection, glm::mat4 view);

        void setLightUniforms(glm::mat4 view);

        void drawEntities(glm::mat4 projection, glm::mat4 view);

    public:

        ~entityManager();
        /**
            Creates cameras and stores them
         */
        void init();

        /**
            Updates whole scene
         */
        void update();

        /**
            Calculates matrices and call draw functions of
            skybox, entities and explosions(respectively)
            sets up light uniforms
         */
        void draw();

        void addEntity(entity *entityToAdd);

        void addLight(light *lightToAdd);

        void addSkybox(skybox * skyboxToAdd){this->usedSkybox = skyboxToAdd;}


        void setIdOfHitEntity(unsigned char idOfHitEntity){idOfEntityHitByMouse = idOfHitEntity;}
        /**
         * If in moving mode, this method moves entity specified by id
         * move is dependent on mouse positions
         */
        void move();

        void setIdOfMovedEntity(unsigned char idOfMovedEntity){idOfEntityMovedByMouse = idOfMovedEntity;}
    };


}
#endif //PGR_ENTITTYMANAGER_H
