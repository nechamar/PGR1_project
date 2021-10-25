//----------------------------------------------------------------------------------------
/**
 * \file       handler.h
 * \author     Marek Nechansky
 * \date       2018/27/03
 * \brief      Contains enum for keys and Handler object, which stores data that is supposed to be accessible for every object
*/
//----------------------------------------------------------------------------------------

#ifndef PGR_HANDLER_H
#define PGR_HANDLER_H

#include "pgr.h"

namespace nechamar {

    /// stores integer values of special keys that can not be written as char
    enum keys{
        shift = 112,
        escape = 27,
        space = 32,
        tab = 9
    };



    struct Handler {
        /// program location for entities
        GLuint program;
        /// uniform locations for entities
        GLint pvmMatrix;
        GLint vmMatrix;
        GLint mMatrix;
        GLint vMatrix;
        GLint nMatrix;
        /// attribute locations for entities
        GLint position;
        GLint normal;
        GLint texCords;
        /// uniform material locations for entities
        GLint emission;
        GLint ambient;
        GLint diffuse;
        GLint specular;
        GLint shininess;
        /// uniform texture locations for entities
        GLint useEmissionTexture;
        GLint emissionTexture;
        GLint useDiffuseTexture;
        GLint diffuseTexture;
        GLint useAmbientTexture;
        GLint ambientTexture;

        /// program location for skybox and sun
        GLuint skyboxProgram;
        /// attribute location for skybox
        GLint screenCoords;
        /// uniform locations for skybox and sun
        GLint skyboxTexture;
        GLint inversePVMatrix;
        GLint sunTextureLocation;
        GLint sunAngle;
        GLint basicAngle;
        GLuint sunTexture;

        /// program location for explosions
        GLuint expProgram;
        /// attribute location for explosions
        GLint expScreenCords;
        /// uniforrm locations for explosions
        GLint expTexture;
        GLint expPvMatrix;
        GLint expT;

        /// key maps for normal and special keys
        bool * keys;
        bool * specKeys;
        /// mouse positions and change of mouse position since last update of entity manager
        int mouseX;
        int mouseY;
        int mouseDx;
        int mouseDy;
        /// width and height of window
        int windowWidth;
        int windowHeight;
        /// modes of scene
        /**
         * if true objects can be destroyed by left clicking on them
         * if false objects can be moved by dragging object on the screen with mouse
         */
        bool destroyMode = true;

    };
};

/// instance of Handler used in whole project
extern nechamar::Handler handler;

#endif //PGR_HANDLER_H
