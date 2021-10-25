//----------------------------------------------------------------------------------------
/**
 * \file       light.h
 * \author     Marek Nechansky
 * \date       2018/13/04
 * \brief      Contains enum for types of light and structure containing information about light
*/
//----------------------------------------------------------------------------------------
#ifndef PGR_LIGHT_H
#define PGR_LIGHT_H

#include <glm/glm.hpp>

namespace nechamar {

    enum light_type{point = 1, directional = 2, spotlight = 3};

    struct light {
        light_type type;
        glm::vec3  ambient;         /// intensity & color of the ambient component
        glm::vec3  diffuse;         /// intensity & color of the diffuse component
        glm::vec3  specular;        /// intensity & color of the specular component
        glm::vec3  position;        /// light position
        glm::vec3  spotDirection;   /// spotlight direction
        float spotCosCutOff;        /// cosine of the spotlight's half angle
        float spotExponent;         /// distribution of the light energy within the reflector's cone (center->cone's edge)
        bool isOn;                  /// if true light is being used, otherwise not
    };


}

#endif //PGR_LIGHT_H
