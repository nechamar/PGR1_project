//
// Created by nechamar on 15.3.18.
//

#include <iostream>
#include "entityManager.h"
#include "constants.h"
#include "dynamicCamera.h"
#include "staticCamera.h"

void nechamar::entityManager::init() {
    dynamicCamera * dynamic;
    staticCamera * static1;
    staticCamera * static2;
    cameras = new camera*[4];
    dynamic = new dynamicCamera();
    dynamic->setPosition(glm::vec3(0.0f,0.0f,0.0f));
    dynamic->setDirection(glm::vec3(0,0,1));
    dynamic->setUpVector(glm::vec3(0,1,0));
    cameras[0] = dynamic;
    static1 = new staticCamera();
    static1->setPosition(glm::vec3(54,0,240));
    static1->setDirection(glm::vec3(-1,0,-1));
    static1->setUpVector(glm::vec3(0,1,0));
    cameras[1] = static1;
    static2 = new staticCamera();
    static2->setPosition(glm::vec3(559,0,62));
    static2->setDirection(glm::vec3(-1,0,-0.34));
    static2->setUpVector(glm::vec3(0,1,0));
    cameras[2] = static2;

    indexOfUsedCamera = 2;
}

void nechamar::entityManager::update() {
    /// end application
    if(handler.keys[keys::escape]) glutLeaveMainLoop();

    /// change to any static camera
    char cameraNumber = '1';
    for (int j = 0; j < NUMBER_OF_CAMERAS-1; ++j) {
        if(handler.keys[cameraNumber]){
            indexOfUsedCamera = j+1;
            break;
        }
        cameraNumber += 1;
    }

    /// change to dynamic camera
    if(handler.keys[keys::space]){
        int lastCamera = indexOfUsedCamera;
        indexOfUsedCamera = 0;
        cameras[0]->setPosition(cameras[lastCamera]->getPosition());
        cameras[0]->setDirection(cameras[lastCamera]->getDirection());
        cameras[0]->setUpVector(cameras[lastCamera]->getUpVector());
    }

    /// collision checking and updating camera
    glm::vec3 prevPos = cameras[indexOfUsedCamera]->getPosition();
    cameras[indexOfUsedCamera]->update();
    bool collision = false;
    bool prevColision = false;
    for(entity * e:entities){
        if(e->isCollision(cameras[indexOfUsedCamera]->getPosition())){
            collision = true;
            break;
        }
        if(e->isCollision(prevPos)){
            prevColision = true;
            break;
        }
    }
    if(glm::length(cameras[indexOfUsedCamera]->getPosition()) > 600) collision = true;
    if(collision && !prevColision){
        cameras[indexOfUsedCamera]->setPosition(prevPos);
    }

    /// change light position to camera position
    lights[0].spotDirection = cameras[indexOfUsedCamera]->getDirection();
    lights[0].position = cameras[indexOfUsedCamera]->getPosition();

    /// change type of light of camera
    if(handler.specKeys[GLUT_KEY_F1]){
        lights[0].type = light_type::point;
        lights[0].isOn = true;
    }else if(handler.specKeys[GLUT_KEY_F2]){
        lights[0].type = light_type::spotlight;
        lights[0].isOn = true;
    }else if(handler.specKeys[GLUT_KEY_F3]){
        lights[0].isOn = false;
    }

    /// updates entities
    /// checks if any entity is hit and then creates explosion
    for(int i = 0;i < entities.size(); i++){
        entity * entity1 = entities.at(i);
        if(idOfEntityHitByMouse == entity1->getId()){
            explosion * explosion1 = new explosion;
            entity1->hit(explosion1);
            explosions.push_back(explosion1);
            entities.erase(entities.begin() + i);
            idOfEntityHitByMouse = 0;
        }
        entity1->update();
    }

    /// changes position of sun(light and texture on skybox)
    double sunPositionInRadians =  ( sunPosition * M_PI) / 180 ;
    lights[1].position = glm::vec3(cos(sunPositionInRadians),0,sin(sunPositionInRadians));
    sunPosition+= 0.1;
    if (sunPosition >= 360) sunPosition -=360;

    /// update explosions;
    for(int i = 0; i < explosions.size(); i++){
        explosion * exp = explosions.at(i);
        exp->t++;
        if(exp->t >= 16) explosions.erase(explosions.begin() + i);
    }

    if(idOfEntityMovedByMouse != 0) move();

    handler.mouseDx = handler.mouseDy = 0;
}

void nechamar::entityManager::draw() {
    glm::mat4 projection = glm::perspective(FOV, handler.windowWidth / float(handler.windowHeight), NEAR_PLANE_DISTANCE, FAR_PLANE_DISTANCE);
    glm::mat4 view = glm::lookAt(cameras[indexOfUsedCamera]->getPosition(),cameras[indexOfUsedCamera]->getPosition() + cameras[indexOfUsedCamera]->getDirection(), cameras[indexOfUsedCamera]->getUpVector());

    if(usedSkybox != nullptr){
        drawSkybox(projection, view);
    }

    setLightUniforms(view);

    drawEntities(projection, view);

    for(explosion * e: explosions) {
        drawExplosion(projection, view,e);
    }

    CHECK_GL_ERROR();
}

void nechamar::entityManager::drawEntities(glm::mat4 projection, glm::mat4 view) {
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    for(entity * entity1 : entities){

        glUseProgram(handler.program);
        glm::mat4 pvm = projection * view * entity1->getModelMatrix();
        glUniformMatrix4fv(handler.pvmMatrix, 1, GL_FALSE, glm::value_ptr(pvm));
        glUniformMatrix4fv(handler.mMatrix, 1, GL_FALSE, glm::value_ptr(entity1->getModelMatrix()));
        glUniformMatrix4fv(handler.vMatrix, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(handler.vmMatrix, 1, GL_FALSE, glm::value_ptr(view * entity1->getModelMatrix()));
        glm::mat4 nMatrix = glm::transpose(glm::inverse(entity1->getModelMatrix()));
        glUniformMatrix3fv(handler.nMatrix, 1 , GL_FALSE, glm::value_ptr(glm::mat3(nMatrix)));

        entity1->draw();

        glBindVertexArray(0);
        glUseProgram(0);
    }
    glDisable(GL_STENCIL_TEST);

}

void nechamar::entityManager::setLightUniforms( glm::mat4 view) {
    glUseProgram(handler.program);

    for (int i = 0; i < lights.size(); ++i) {
        std::string s("lights["), tmp;
        s.append(std::to_string(i));
        s.append("]");
        tmp = s + ".isOn";
        glUniform1i(glGetUniformLocation(handler.program, tmp.data()), lights[i].isOn);
        if(!lights[i].isOn) continue;
        tmp = s + ".ambient";
        glUniform3fv(glGetUniformLocation(handler.program, tmp.data()), 1, glm::value_ptr(lights[i].ambient));
        tmp = s + ".diffuse";
        glUniform3fv(glGetUniformLocation(handler.program, tmp.data()), 1, glm::value_ptr(lights[i].diffuse));
        tmp = s + ".position";
        glm::vec3 pos = glm::vec3(view * glm::vec4(lights[i].position,0.0f));
        if(lights[i].type == spotlight || lights[i].type == point){pos = glm::vec3(view * glm::vec4(lights[i].position,1.0f));}
        glUniform3fv(glGetUniformLocation(handler.program, tmp.data()),1, glm::value_ptr(pos));
        tmp = s + ".type";
        glUniform1i(glGetUniformLocation(handler.program, tmp.data()), (int)lights[i].type);
        tmp = s + ".specular";
        glUniform3fv(glGetUniformLocation(handler.program, tmp.data()), 1, glm::value_ptr(lights[i].specular));
        if(lights[i].type == light_type::spotlight){
            tmp = s + ".spotDirection";
            glm::vec3 dir = glm::vec3(view * glm::vec4(lights[i].spotDirection,0.0f));
            glUniform3fv(glGetUniformLocation(handler.program, tmp.data()),1, glm::value_ptr(dir));
            tmp = s + ".spotCosCutOff";
            glUniform1f(glGetUniformLocation(handler.program, tmp.data()),lights[i].spotCosCutOff);
            tmp = s + ".spotExponent";
            glUniform1f(glGetUniformLocation(handler.program, tmp.data()),lights[i].spotExponent);
        }
    }
    glUseProgram(0);
}

void nechamar::entityManager::drawSkybox(glm::mat4 projection, glm::mat4 view) {
    glUseProgram(handler.skyboxProgram);

    glUniform2fv(handler.sunAngle, 1, glm::value_ptr(glm::vec2(sunPosition,100)));

    glm::mat4 matrix = projection * view;

    // crate view rotation matrix by using view matrix with cleared translation
    glm::mat4 viewRotation = view;
    viewRotation[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    // vertex shader will translate screen space coordinates (NDC) using inverse PV matrix
    glm::mat4 inversePVMatrix = glm::inverse(projection * viewRotation);

    glUniformMatrix4fv(handler.inversePVMatrix, 1, GL_FALSE, glm::value_ptr(inversePVMatrix));

    usedSkybox->draw(cameras[indexOfUsedCamera]->getDirection());
    glUseProgram(0);
}


void nechamar::entityManager::drawExplosion(glm::mat4 projection, glm::mat4 view, explosion * explosion1) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    glUseProgram(handler.expProgram);

    // just take rotation part of the view transform
    glm::mat4 billboardRotationMatrix = glm::mat4(
            view[0],
            view[1],
            view[2],
            glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
    );
    // inverse view rotation
    billboardRotationMatrix = glm::transpose(billboardRotationMatrix);

    glm::mat4 matrix = glm::translate(glm::mat4(1.0f), explosion1->position);
    matrix = glm::scale(matrix, glm::vec3(explosion1->size));
    matrix = matrix*billboardRotationMatrix; // make billboard to face the camera

    glm::mat4 PVMMatrix = projection * view * matrix;

    glUniformMatrix4fv(handler.expPvMatrix, 1, GL_FALSE, glm::value_ptr(PVMMatrix));  // model-view-projection
    glUniform1i(handler.expT, explosion1->t);
    glBindVertexArray(explosion1->modelGeometry->vertexArrayObject);
    glUniform1i(handler.expTexture,9);
    CHECK_GL_ERROR();
    glActiveTexture(GL_TEXTURE9);
    glBindTexture(GL_TEXTURE_2D, explosion1->modelGeometry->texture);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, explosion1->modelGeometry->numberOfTriangles+2);
    glDisable(GL_BLEND);
    glUseProgram(0);
}



void nechamar::entityManager::addEntity(nechamar::entity * entityToAdd) {
    entities.push_back(entityToAdd);
}

void nechamar::entityManager::addLight(light *lightToAdd) {
    lights.push_back(*lightToAdd);
}

nechamar::entity *nechamar::entityManager::getEntity(unsigned char id) {
    for(int i = 0;i < entities.size(); i++){
        entity * entity1 = entities.at(i);
        if(id == entity1->getId()){
            return entity1;
        }
    }
    return nullptr;
}

void nechamar::entityManager::move() {

    float angleX, angleY, AngleXBeforeMove ,AngleYBeforeMove;
    entity * movedEntity = getEntity(idOfEntityMovedByMouse);
    glm::vec3 positionOfEntity =  movedEntity->getPosition();
    
    
    angleX = (float)atan2(((handler.windowWidth/2)- handler.mouseX), NEAR_PLANE_DISTANCE * NEAR_MULTIPLIER);
    angleY = (float)atan2(((handler.windowHeight/2)- handler.mouseY), NEAR_PLANE_DISTANCE * NEAR_MULTIPLIER);
    AngleXBeforeMove = (float)atan2(((handler.windowWidth/2)- handler.mouseX-handler.mouseDx), NEAR_PLANE_DISTANCE *
            NEAR_MULTIPLIER);
    AngleYBeforeMove = (float)atan2(((handler.windowHeight/2)- handler.mouseY-handler.mouseDy), NEAR_PLANE_DISTANCE *
            NEAR_MULTIPLIER);

    glm::mat4 rotations = glm::rotate(glm::mat4(1.0f), (float)((angleX - AngleXBeforeMove)*180/M_PI)*3.0f, glm::vec3(0.0f, 1.0f, 0.0f))
                          * glm::rotate(glm::mat4(1.0f), (float)((angleY - AngleYBeforeMove)*180/M_PI)*3.0f,
                                        glm::cross(cameras[indexOfUsedCamera]->getDirection(),cameras[indexOfUsedCamera]->getUpVector()));

    glm::mat4 toCam = glm::translate(glm::mat4(1.0f), -cameras[indexOfUsedCamera]->getPosition());
    glm::mat4 toEntity = glm::translate(glm::mat4(1.0f), cameras[indexOfUsedCamera]->getPosition());
    glm::vec4 tmp = toEntity * rotations * toCam * glm::vec4(positionOfEntity,1.0f);
    
    positionOfEntity = glm::vec3(tmp);
    
    movedEntity->setPosition(positionOfEntity);
}

nechamar::entityManager::~entityManager() {
    /*for (int i = 0; i < entities.size(); ++i) {
        delete entities[i];
    }

    for (int j = 0; j < explosions.size(); ++j) {
        delete explosions[j];
    }

    for (int k = 0; k < 3; ++k) {
        delete cameras[k];
    }

    delete usedSkybox;*/
}
