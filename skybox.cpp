//
// Created by nechamar on 1.5.18.
//

#include <iostream>
#include "skybox.h"
#include "handler.h"

void nechamar::skybox::setup(std::string pathsToSkyboxTextures[], skyboxModelGeometry **geometry) {
    *geometry = new skyboxModelGeometry;

    static const float screenCoords[] = {
            -1.0f, -1.0f,
            1.0f, -1.0f,
            -1.0f,  1.0f,
            1.0f,  1.0f
    };

    glGenVertexArrays(1, &((*geometry)->vertexArrayObject));
    glBindVertexArray((*geometry)->vertexArrayObject);

    glGenBuffers(1, &((*geometry)->vertexBufferObject));
    glBindBuffer(GL_ARRAY_BUFFER, (*geometry)->vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(screenCoords), screenCoords, GL_STATIC_DRAW);


    glEnableVertexAttribArray(handler.screenCoords);
    glVertexAttribPointer(handler.screenCoords, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);
    glUseProgram(0);

    (*geometry)->numberOfTriangles = 2;

    glActiveTexture(GL_TEXTURE5);

    glGenTextures(1, &((*geometry)->texture));
    glBindTexture(GL_TEXTURE_CUBE_MAP, (*geometry)->texture);

    const char * suffixes[] = { "posx", "negx", "posy", "negy", "posz", "negz" };
    GLuint targets[] = {
            GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    };
    for( int i = 0; i < 6; i++ ) {
        std::string texName = std::string("res/skybox") + "_" + suffixes[i] + ".tga";
        std::cout << "Loading cube map texture: " << texName << std::endl;
        if(!pgr::loadTexImage2D(pathsToSkyboxTextures[i], targets[i])) {
            pgr::dieWithError("Skybox cube map loading failed!");
        }
    }


    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

}

nechamar::skybox::skybox(std::string pathsToSkyboxTextures[]) {
    setup(pathsToSkyboxTextures, &modelGeometry);
}

void nechamar::skybox::draw(glm::vec3 cameraDirection) {

    float ax  = atan2(sqrt(0 + cameraDirection.z*cameraDirection.z),cameraDirection.x) * (180.0/3.141592653589793238463);
    float ay  = atan2(sqrt(cameraDirection.x*cameraDirection.x + cameraDirection.z*cameraDirection.z),cameraDirection.y) * (180.0/3.141592653589793238463);
    if(cameraDirection.z < 0)
        ax  = 360 - ax;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glUniform2fv(handler.basicAngle, 1, glm::value_ptr(glm::vec2(ax,90 +(90-ay))));
    glUniform1i(handler.sunTextureLocation, 8);
    glActiveTexture(GL_TEXTURE8);
    glBindTexture(GL_TEXTURE_2D,handler.sunTexture);


    glUseProgram(handler.skyboxProgram);
    glUniform1i(handler.skyboxTexture, 5);

    glBindVertexArray(modelGeometry->vertexArrayObject);
    glBindTexture(GL_TEXTURE_CUBE_MAP, modelGeometry->texture);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, modelGeometry->numberOfTriangles+2);

    glBindVertexArray(0);
    glUseProgram(0);
}

nechamar::skybox::~skybox() {
    //delete modelGeometry;
}
