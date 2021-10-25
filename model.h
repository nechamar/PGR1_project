
//----------------------------------------------------------------------------------------
/**
 * \file       model.h
 * \author     Marek Nechansky
 * \date       26.3.2018
 * \brief      Contains class that represents model of entity
*/
//----------------------------------------------------------------------------------------
#ifndef PGR_MODEL_H
#define PGR_MODEL_H

#include "pgr.h"
#include "meshGeometry.h"

namespace nechamar {

    class model {
    protected:

        nechamar::modelGeometry * geometry;

    public:
        model(nechamar::modelGeometry ** modelGeometry){
            this->geometry = *modelGeometry;
        }

        virtual ~model();
        /**
         * sets uniforms and draws vertices
         */
        virtual void draw();

        virtual void update();
    };
}

#endif //PGR_MODEL_H
