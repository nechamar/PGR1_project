//----------------------------------------------------------------------------------------
/**
 * \file       skybox.h
 * \author     Marek Nechansky
 * \date       1.5.2018
 * \brief      Contains class containing information and methods for skybox in scene
*/
//----------------------------------------------------------------------------------------

#ifndef PGR_SKYBOX_H
#define PGR_SKYBOX_H

#include "meshGeometry.h"

namespace nechamar {

    class skybox {
        struct skyboxModelGeometry{
            int numberOfTriangles;
            GLuint texture;
            GLuint vertexArrayObject;    // identifier for the vertex array object
            GLuint vertexBufferObject;   // identifier for the vertex buffer object
        };

    private:
        skyboxModelGeometry *modelGeometry;

        void setup(std::string pathsToSkyboxTextures[], skyboxModelGeometry **geometry);
    public:
        virtual ~skybox();
        skybox(std::string pathsToSkyboxTextures[]);
        void draw(glm::vec3 cameraDirection);
    };
}

#endif //PGR_SKYBOX_H
