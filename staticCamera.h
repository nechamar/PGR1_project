//----------------------------------------------------------------------------------------
/**
 * \file       model.h
 * \author     Marek Nechansky
 * \date       2.4.2018
 * \brief      Contains subclass of camera, which does not do any movement(static)
*/
//----------------------------------------------------------------------------------------

#ifndef PGR_STATICCAMERA_H
#define PGR_STATICCAMERA_H

#include "camera.h"

namespace nechamar {
    class staticCamera : public nechamar::camera {
    public:
        void update() override;
    };
}

#endif //PGR_STATICCAMERA_H
