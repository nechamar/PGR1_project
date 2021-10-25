
#include "dynamicCamera.h"
#include "handler.h"

void nechamar::dynamicCamera::update() {
    /// if user is not in normal mode, but in mode, where objects can be moved around the screen, he cant move nor change view of camera
    if(!handler.destroyMode){ return;}
    /// acceleration of camera, which makes camera movement slower or faster
    float acceleration = 1.0f;
    /// if shift is pressed camera movement is faster
    if(handler.specKeys[keys::shift]) {acceleration = ACCELERATION_USING_SHIFT;}
    /// if w, s, a or d is pressed camera moves in standard direction of the key
    if(handler.keys['w']) position = position + acceleration * direction;
    if(handler.keys['s']) position = position - acceleration * direction;
    if(handler.keys['a']) position = position - acceleration * glm::cross(direction, upVector);
    if(handler.keys['d']) position = position + acceleration * glm::cross(direction, upVector);
    if(handler.specKeys[GLUT_KEY_UP]) position = position + acceleration * upVector;
    if(handler.specKeys[GLUT_KEY_DOWN]) position = position - acceleration * upVector;

    /// rotates view of camera in horizontal way
    direction = glm::mat3(glm::rotate(glm::mat4(1.0f), (float)handler.mouseDx, getUpVector())) * direction;
    /// vector to rotate around when rotating vertically
    glm::vec3 toRotateAround = glm::cross(direction, upVector);

    /// remembers direction before changes if there are some problems
    glm::vec3 oldDirection  = direction;
    /// rotates view of camera in vertical way
    direction = glm::mat3(glm::rotate(glm::mat4(1.0f), (float)handler.mouseDy, toRotateAround)) * direction;
    /// user has limited view (can't bend back to see behind himself using vertex rotation)
    /// if any problem with rotations, direction is returned to the value before rotation
    if(direction.y > 0.9 || direction.y < -0.9 || (handler.mouseDy > 0 && direction.y < oldDirection.y) || (handler.mouseDy < 0 && direction.y > oldDirection.y)) direction = oldDirection;

    /// set mouse pointer to the window center
        glutWarpPointer(handler.windowWidth / 2, handler.windowHeight / 2);
        handler.mouseX = handler.windowWidth / 2;
        handler.mouseY = handler.windowHeight / 2;
}
