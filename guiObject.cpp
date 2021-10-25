//
// Created by nechamar on 2.5.18.
//

#include <fstream>
#include <iostream>
#include "guiObject.h"
#include "handler.h"
/* 
void nechamar::guiObject::setup(std::string name, nechamar::guiObject::guiModelGeometry **geometry) {
    *geometry = new guiModelGeometry;

    float data[16];
    std::ifstream myFile("res/" + name + ".btobj");
    std::string line;
    for (int i = 0; i < 16; ++i) {
        myFile >> line;
        data[i] = stof(line);
    }

    glUseProgram(handler.guiProgram);

    glGenVertexArrays(1, &((*geometry)->vertexArrayObject));
    glBindVertexArray((*geometry)->vertexArrayObject);


    glGenBuffers(1, &((*geometry)->vertexBufferObject));
    glBindBuffer(GL_ARRAY_BUFFER, (*geometry)->vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(handler.guiPosition);
    glVertexAttribPointer(handler.guiPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);
    CHECK_GL_ERROR();
    glEnableVertexAttribArray(handler.guiTexCoords);
    CHECK_GL_ERROR();
    glVertexAttribPointer(handler.guiTexCoords, 2, GL_FLOAT, GL_FALSE, 0, (void*)(8* sizeof(float)));
    CHECK_GL_ERROR();
    glBindVertexArray(0);
    glUseProgram(0); 
    CHECK_GL_ERROR();

    (*geometry)->numberOfTriangles = 2;

    (*geometry)->texture = pgr::createTexture("res/" + name + ".tga");

}

nechamar::guiObject::guiObject(std::string name) {
    setup(name, &modelGeometry);
}

void nechamar::guiObject::draw() {
    glUseProgram(handler.guiProgram);

    glUniform1i(handler.guiTexture, 6);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D,modelGeometry->texture);

    glBindVertexArray(modelGeometry->vertexArrayObject);
    glDrawArrays(GL_TRIANGLE_STRIP,0, modelGeometry->numberOfTriangles+2);

    glBindVertexArray(0);
    glUseProgram(0);
    CHECK_GL_ERROR();
}*/
