/**
 * \file       model.h
 * \author     Marek Nechansky
 * \date       4.5.2018
 * \brief      Contains class, which stores spline and has all methods needed for computing
 * position and direction of entity on spline
*/
//----------------------------------------------------------------------------------------
#ifndef PGR_SPLINE_H
#define PGR_SPLINE_H

#include "pgr.h"

namespace nechamar {


    class spline {
    private:
        /// Checks if vector is zero or not.
        bool isVectorNull(const glm::vec3 &vect);
        glm::vec3 * points;
        size_t count;
    public:
        spline(glm::vec3 * points, size_t count);

        glm::mat4 alignObject(const glm::vec3 &position, const glm::vec3 &front, const glm::vec3 &up);

        glm::vec3 evaluateCurveSegment(
                const glm::vec3& P0,
                const glm::vec3& P1,
                const glm::vec3& P2,
                const glm::vec3& P3,
                const float t
        );

        glm::vec3 evaluateCurveSegment_1stDerivative(
                const glm::vec3& P0,
                const glm::vec3& P1,
                const glm::vec3& P2,
                const glm::vec3& P3,
                const float t
        );

        glm::vec3 evaluateClosedCurve(
                const float     t
        );

        glm::vec3 evaluateClosedCurve_1stDerivative(
                const float     t
        );
    };
}

#endif //PGR_SPLINE_H
