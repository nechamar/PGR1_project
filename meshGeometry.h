/**
 * \file       meshGeometry.h
 * \author     Marek Nechansky
 * \date       2018/27/03
 * \brief      Contains enums that store informations about explosion, explosion's model, material,
*/
//----------------------------------------------------------------------------------------
#ifndef PGR_MESHGEOMETRY_H
#define PGR_MESHGEOMETRY_H

#include "pgr.h"
namespace nechamar {

    struct explosionModelGeometry{
        int numberOfTriangles;
        GLuint texture;
        GLuint vertexArrayObject;
        GLuint vertexBufferObject;
    };

    struct explosion{
        explosionModelGeometry * modelGeometry;
        glm::vec3 position;
        int t;
        float size;
    };

    struct material{
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        glm::vec3 emission;
        float shininess;

        bool useEmissionTexture;
        GLuint emissionTexture;
        bool useAmbientTexture;
        GLuint ambientTexture;
        bool useDiffuseTexture;
        GLuint diffuseTexture;
    };

    /// one model can have more materials
    struct meshGeometry{
        unsigned int numTriangles;
        material meshMaterial;
    };

    struct modelGeometry{
        GLuint elementBufferObject;
        GLuint vertexArrayObject;
        GLuint vertexBufferObject;
        meshGeometry * parts;
        unsigned int numberOfParts;
    };

}
#endif //PGR_MESHGEOMETRY_H
