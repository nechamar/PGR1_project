//
// Created by nechamar on 15.3.18.
//

#include <iostream>
#include "entity.h"



void nechamar::entity::draw() {
        glUseProgram(handler.program);

        glStencilFunc(GL_ALWAYS, id, -1);
        entityModel->draw();

}


nechamar::entity::~entity() {
    //delete entityModel;
    //delete path;
}


void nechamar::entity::update() {
    if(path == nullptr) {
        modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale)) *
                      glm::translate(glm::mat4(1.0f), -scale*posOffset + glm::vec3(posX, posY, posZ)) *
                      glm::rotate(glm::mat4(1.0f), pitch, glm::vec3(1.0f, 0.0f, 0.0f)) *
                      glm::rotate(glm::mat4(1.0f), yaw, glm::vec3(0.0f, 1.0f, 0.0f)) *
                      glm::rotate(glm::mat4(1.0f), roll, glm::vec3(0.0f, 0.0f, 1.0f));

    }else {
        modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale)) *
                glm::rotate(glm::mat4(1.0f), pitch, glm::vec3(1.0f, 0.0f, 0.0f)) *
                glm::rotate(glm::mat4(1.0f), yaw, glm::vec3(0.0f, 1.0f, 0.0f)) *
                glm::rotate(glm::mat4(1.0f), roll, glm::vec3(0.0f, 0.0f, 1.0f));

        modelMatrix = path->alignObject(glm::vec3(posX,posY,posZ)+path->evaluateClosedCurve(splineT)+scale*posOffset, path->evaluateClosedCurve_1stDerivative(splineT), glm::vec3(0,1,0)) * modelMatrix;
        splineT += 0.1;
    }
    entityModel->update();

}

nechamar::entity::entity(nechamar::model *entityModel, long posX, long posY, long posZ, float yaw, float roll,
                         float pitch, float scale, spline * path, glm::vec3 posOffset) : entityModel(entityModel), posX(posX),
                                                                                   posY(posY), posZ(posZ), yaw(yaw),
                                                                                   roll(roll), pitch(pitch),
                                                                                   scale(scale), path(path), posOffset(posOffset){

    static unsigned char newId = 1;
    id = newId++;
}


void nechamar::entity::setupExplosion(explosion * explosion1) {
    explosion1->modelGeometry->texture = pgr::createTexture("res/explosion.tga");
    explosion1->size = 20;
}

void nechamar::entity::hit(nechamar::explosion *explosion1) {
    explosion1->modelGeometry = new explosionModelGeometry;

    static const float screenCoords[] = {
            -1.0f, -1.0f,
            1.0f, -1.0f,
            -1.0f,  1.0f,
            1.0f,  1.0f
    };

    glGenVertexArrays(1, &((explosion1->modelGeometry)->vertexArrayObject));
    glBindVertexArray((explosion1->modelGeometry)->vertexArrayObject);

    glGenBuffers(1, &((explosion1->modelGeometry)->vertexBufferObject));
    glBindBuffer(GL_ARRAY_BUFFER, (explosion1->modelGeometry)->vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(screenCoords), screenCoords, GL_STATIC_DRAW);

    glEnableVertexAttribArray(handler.expScreenCords);
    glVertexAttribPointer(handler.expScreenCords, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);
    glUseProgram(0);
    CHECK_GL_ERROR();
    CHECK_GL_ERROR();

    (explosion1->modelGeometry)->numberOfTriangles = 2;

    glActiveTexture(GL_TEXTURE9);

    glGenTextures(1, &((explosion1->modelGeometry)->texture));
    glBindTexture(GL_TEXTURE_CUBE_MAP, (explosion1->modelGeometry)->texture);

    explosion1->t = 0;
    explosion1->position = glm::vec3(posX, posY, posZ);

    setupExplosion(explosion1);
}
