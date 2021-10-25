//
// Created by nechamar on 4.5.18.
//

#include <iostream>
#include "spline.h"



bool nechamar::spline::isVectorNull(const glm::vec3 &vect) {

    return (!vect.x && !vect.y && !vect.z);
}


glm::mat4 nechamar::spline::alignObject(const glm::vec3 &position, const glm::vec3 &front, const glm::vec3 &up) {

    glm::vec3 z = -glm::normalize(front);

    if (isVectorNull(z))
        z = glm::vec3(0.0, 0.0, 1.0);

    glm::vec3 x = glm::normalize(glm::cross(up, z));

    if (isVectorNull(x))
        x =  glm::vec3(1.0, 0.0, 0.0);

    glm::vec3 y = glm::cross(z, x);
    if (isVectorNull(y))
        x =  glm::vec3(0.0, 1.0, 0.0);

    glm::mat4 matrix = glm::mat4(
            x.x,        x.y,        x.z,        0.0,
            y.x,        y.y,        y.z,        0.0,
            z.x,        z.y,        z.z,        0.0,
            position.x, position.y, position.z, 1.0
    );

    return matrix;
}


glm::vec3 nechamar::spline::evaluateCurveSegment(
        const glm::vec3& P0,
        const glm::vec3& P1,
        const glm::vec3& P2,
        const glm::vec3& P3,
        const float t
) {
    glm::vec3 result(0.0, 0.0, 0.0);

    double tt = t*t;
    double ttt = tt*t;
    double zero = -1*ttt + 3*tt -3*t + 1;
    double one =  3*ttt -6*tt + 4;
    double two =  -3*ttt + 3*tt + 3*t +1;
    double three =  1*ttt;

    result.x = (1/6.0f)*(zero*P0.x + one*P1.x + two*P2.x + three*P3.x);
    result.y = (1/6.0f)*(zero*P0.y + one*P1.y + two*P2.y + three*P3.y);
    result.z = (1/6.0f)*(zero*P0.z + one*P1.z + two*P2.z + three*P3.z);

    return result;
}

glm::vec3 nechamar::spline::evaluateCurveSegment_1stDerivative(
        const glm::vec3& P0,
        const glm::vec3& P1,
        const glm::vec3& P2,
        const glm::vec3& P3,
        const float t
) {
    glm::vec3 result(1.0, 0.0, 0.0);

    float tt = t*t;
    double zero = -3*tt + 6*t -3;
    double one =  9*tt -12*t;
    double two =  -9*tt + 6*t + 3;
    double three =  3*tt;

    result.x = 1/6.0f*(zero*P0.x + one*P1.x + two*P2.x + three*P3.x);
    result.y = (1/6.0f)*(zero*P0.y + one*P1.y + two*P2.y + three*P3.y);
    result.z = (1/6.0f)*(zero*P0.z + one*P1.z + two*P2.z + three*P3.z);

    return result;
}

glm::vec3 nechamar::spline::evaluateClosedCurve(
        const float     t
) {
    glm::vec3 result(0.0, 0.0, 0.0);

    int i = t;
    float tmp = t - i;
    while(i < 1) {
        i += count;
    }

    result = evaluateCurveSegment(points[(i-1)%count],points[(i)%count],points[(i+1)%count],points[(i+2)%count], tmp);

    return result;
}

glm::vec3 nechamar::spline::evaluateClosedCurve_1stDerivative(const float t) {
    glm::vec3 result(1.0, 0.0, 0.0);

    int i = t;
    float tmp = t - i;
    while(i < 1) {
        i += count;
    }
    result = evaluateCurveSegment_1stDerivative(points[(i-1)%count],points[(i)%count],points[(i+1)%count],points[(i+2)%count], tmp);

    return result;
}

nechamar::spline::spline(glm::vec3 * points, size_t count) : count(count) {
    this->points = new glm::vec3[count];
    for (int i = 0; i < count; ++i) {
        this->points[i] = points[i];
    }
};
