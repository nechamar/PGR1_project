//----------------------------------------------------------------------------------------
/**
 * \file       entity.h
 * \author     Marek Nechansky
 * \date       2018/15/03
 * \brief      declares interface, that specifies any object in scene(not light, skybox nor gui)
 *
*/
//----------------------------------------------------------------------------------------

#ifndef PGR_ENTITY_H
#define PGR_ENTITY_H

#include <pgr.h>
#include "model.h"
#include "handler.h"
#include "spline.h"


/// interface that holds info about entity and has its basic functions
/**
  This interface contains information about the entity, i.e. its position,
  offset, rotation, scale, model matrix, spline curve and progress of
  entity on the spline
  Information about an vertices and indices are stored in entity's model
*/
namespace nechamar {

    class entity {
    private:
        unsigned char id;
    protected:
        nechamar::model * entityModel;

        double posX;
        double posY;
        double posZ;
        /// if the entity's model is not centered, it can be fixed by setting offset that changes drawing but not position
        glm::vec3 posOffset;

        float yaw;
        float roll;
        float pitch;

        float scale;

        glm::mat4 modelMatrix;

        spline * path;

        float splineT = 0;
        /**
            sets variables of explosion i.e. size and texture of explsion
            \param[out] explosion1 target explosion
         */
        virtual void setupExplosion(explosion * explosion1);
    public:
        /// Constructor
        /**
            sets values to all class variables and sets id of the object
         */
        entity(model *entityModel, long posX, long posY, long posZ, float yaw, float roll, float pitch, float scale, spline *path = nullptr, glm::vec3 posOffset = glm::vec3(0));

        virtual ~entity();

        /**
           updates model matrix, path and model
         */
        virtual void update();

        /**
         * sets stencil function to its id and calls draw of its model
         */
        virtual void draw();

        /**
            returns if camera position is in this entity's bounds
            \param[in] cameraPosition position of camera
            \return True if this entity and camera collide, otherwise false.
         */
        virtual bool isCollision(glm::vec3 cameraPosition) = 0;

        virtual glm::mat4 getModelMatrix(){return modelMatrix;}

        unsigned char getId(){ return id;}
        /**
            Creates explosion and sets its parameters and calls setupExplosion to adjust parameters for any subclass
            \param[out] explosion1 explosion of which parameters are changes in this method
         */
        void hit(explosion * explosion1);

        glm::vec3 getPosition(){return glm::vec3(posX, posY, posZ) - posOffset;}

        void setPosition(glm::vec3 position){posX = position.x;posY = position.y;posZ = position.z;}
    };

}
#endif //PGR_ENTITY_H
