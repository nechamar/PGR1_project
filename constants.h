//----------------------------------------------------------------------------------------
/**
 * \file       parameters.h
 * \author     Marek Nechansky
 * \date       10.3.2018
 * \brief      Contains all constant variables.
*/
//----------------------------------------------------------------------------------------
#ifndef PGR_CONSTANTS_H
#define PGR_CONSTANTS_H

namespace nechamar{

    const static unsigned int MS_PER_FRAME = 33;
    const static int DEFAULT_WINDOW_HEIGHT = 500;
    const static int DEFAULT_WINDOW_WIDTH = 500;
    const static char * WINDOW_TITLE = "PGR  space simulation!";
    const static char * VERTEX_SHADER_PATH = "vs.glsl";
    const static char * FRAGMENT_SHADER_PATH = "fs.glsl";
    const static char * SKYBOX_VERTEX_SHADER_PATH = "skyboxVS.glsl";
    const static char * SKYBOX_FRAGMENT_SHADER_PATH = "skyboxFS.glsl";
    const static char * EXPLOSION_FRAGMENT_SHADER_PATH = "explosionFS.glsl";
    const static char * EXPLOSION_VERTEX_SHADER_PATH = "explosionVS.glsl";
    const static float FOV = 60.0f;
    const static float NEAR_PLANE_DISTANCE = 1.0f;
    const static float FAR_PLANE_DISTANCE = 2000.0f;
    const static unsigned int NUMBER_OF_CAMERAS = 3;
    /// multiplier used to make near(1.0f) larger than width/height of window
    const static int NEAR_MULTIPLIER = 2000;

}

#endif //PGR_CONSTANTS_H
