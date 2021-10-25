//----------------------------------------------------------------------------------------
/**
 * \file       camera.h
 * \author     Marek Nechansky
 * \date       2018/21/05
 * \brief      contains class camera, which is basic interface for any camera used in this project
 *
*/
//----------------------------------------------------------------------------------------
#ifndef PGR_CAMERA_H
#define PGR_CAMERA_H


#include "pgr.h"

namespace nechamar {


/// Interface that holds information about camera and necessary methods for any camera to work in this project
/**
  This interface contains information about the camera, i.e. its position in world coordinates,
  direction of the camera with respect to the position of the camera and up vector
  Interface has setters and getters for every variable and a update method, which changes properties of the variables
*/
    class camera {
    protected:
        /// vector of three floats, which provides x,y and z position of camera in world coordinates.
        glm::vec3 position;
        /// unit vector of three floats, which provides x, y and z direction of camera with respect to position of camera.
        glm::vec3 direction;
        /// unit vector of three float, which provides x, y and z of vector pointing up with respect to position of camera.
        glm::vec3 upVector;

    public:

        /// setters and getters for every class variable.
        const glm::vec3 &getPosition() const;

        void setPosition( glm::vec3 position);

        const glm::vec3 &getDirection() const;

        void setDirection( glm::vec3 direction);

        const glm::vec3 &getUpVector() const;

        void setUpVector( glm::vec3 upVector);
        /// method called in every time scene is updated if this camera is main camera. It should manage class variables
        virtual void update(){};

    };
}

#endif //PGR_CAMERA_H
