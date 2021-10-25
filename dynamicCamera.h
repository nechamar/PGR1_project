//----------------------------------------------------------------------------------------
/**
 * \file       animation.h
 * \author     Marek Nechansky
 * \date       2018/21/05
 * \brief      declares subclass of interface camera, used for moving through the scene using keys
 *
 *
*/
//----------------------------------------------------------------------------------------

#ifndef PGR_DYNAMICCAMERA_H
#define PGR_DYNAMICCAMERA_H


#include "camera.h"
namespace nechamar {

/// Subclass of interface camera, which has only one overrode method update

/**
 This class overrides only update method in which user can change camera's variables
 using keys on keyboard or mouse movement. It also changes position of the mouse to the center of the screen
 and sets variables in handler(mouseX and mouseY) to the value of center of the screen
 */
    class dynamicCamera : public nechamar::camera {
    private:
        /// how faster is camera movement when shift is pressed
        const float ACCELERATION_USING_SHIFT = 10.0f;
    public:
        void update() override;

    };
}

#endif //PGR_DYNAMICCAMERA_H
