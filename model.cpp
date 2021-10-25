//
// Created by nechamar on 26.3.18.
//

#include <iostream>
#include "model.h"
#include "handler.h"

    void nechamar::model::draw() {


        glUseProgram(handler.program);

        glBindVertexArray(geometry->vertexArrayObject);

        unsigned int space = 0;
        for (int i = 0; i < geometry->numberOfParts; ++i) {

            if(geometry->parts[i].meshMaterial.useDiffuseTexture){
                glUniform1i(handler.diffuseTexture,0);
                glUniform1i(handler.useDiffuseTexture, 1);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, geometry->parts[i].meshMaterial.diffuseTexture);
            }else{
                glUniform1i(handler.useDiffuseTexture, 0);
            }

            CHECK_GL_ERROR();

            if(geometry->parts[i].meshMaterial.useAmbientTexture){

                glUniform1i(handler.ambientTexture,1);
                glUniform1i(handler.useAmbientTexture, 1);
                CHECK_GL_ERROR();
                glActiveTexture(GL_TEXTURE1 );
                glBindTexture(GL_TEXTURE_2D, geometry->parts[i].meshMaterial.ambientTexture);

            }else{
                glUniform1i(handler.useAmbientTexture, 0);
            }

            CHECK_GL_ERROR();

            if(geometry->parts[i].meshMaterial.useEmissionTexture){

                glUniform1i(handler.emissionTexture,2);
                glUniform1i(handler.useEmissionTexture, 1);
                CHECK_GL_ERROR();
                glActiveTexture(GL_TEXTURE2 );
                glBindTexture(GL_TEXTURE_2D, geometry->parts[i].meshMaterial.emissionTexture);

            }else{
                glUniform1i(handler.useEmissionTexture, 0);

            }

            CHECK_GL_ERROR();

            glUniform3fv(handler.emission, 1 ,glm::value_ptr(geometry->parts[i].meshMaterial.emission));
            glUniform3fv(handler.diffuse, 1 ,glm::value_ptr(geometry->parts[i].meshMaterial.diffuse));
            glUniform3fv(handler.ambient, 1 ,glm::value_ptr(geometry->parts[i].meshMaterial.ambient));
            glUniform3fv(handler.specular, 1 ,glm::value_ptr(geometry->parts[i].meshMaterial.specular));
            glUniform1f(handler.shininess , geometry->parts[i].meshMaterial.shininess);

            glDrawElements(GL_TRIANGLES, geometry->parts[i].numTriangles, GL_UNSIGNED_INT, (void*)(space* sizeof(unsigned int)));
            space += geometry->parts[i].numTriangles;
        }

        CHECK_GL_ERROR();

        glBindVertexArray(0);
        glUseProgram(0);
    }

void nechamar::model::update() {

}

nechamar::model::~model() {
    //delete geometry;
}
