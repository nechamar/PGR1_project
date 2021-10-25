//
// Created by nechamar on 2.5.18.
//
//----------------------------------------------------------------------------------------
/**
 * \file       entity.h
 * \author     Marek Nechansky
 * \date       2018/02/05
 * \brief      declares interface, that specifies any object in scene(not light, skybox nor gui)
 *
*/
#ifndef PGR_GUIOBJECT_H
#define PGR_GUIOBJECT_H

#include "pgr.h"

namespace nechamar {

    class guiObject {
        struct guiModelGeometry {
            int numberOfTriangles;
            GLuint texture;              ///< identifier for the texture of the guiObejct
            GLuint vertexArrayObject;    ///< identifier for the vertex array object
            GLuint vertexBufferObject;   ///< identifier for the vertex buffer object
        };
    private:
        void setup(std::string name, guiModelGeometry ** geometry);
        guiModelGeometry * modelGeometry;
    public:
        guiObject(std::string name);

        void draw();
    };
}

#endif //PGR_GUIOBJECT_H
