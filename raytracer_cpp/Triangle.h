//
//  Triangle.h
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 17.08.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef __raytracer_cpp__Triangle__
#define __raytracer_cpp__Triangle__

#include <glm/glm.hpp>
#include "Ray.h"
#include "global.h"
#include "Diagnostics.h"

class Ray;

class Triangle {
public:
    glm::vec3 a;//, b, c;
    glm::vec3 edges[2];
    glm::vec3 normal;
//    char padding[14];
    
    Triangle() :
        Triangle(glm::vec3(0, 0, 0),
                 glm::vec3(0, 0, 0),
                 glm::vec3(0, 0, 0)) {
    }
    
    Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
        this->a = a;
//        this->b = b;
//        this->c = c;
        this->edges[0] = b - a;
        this->edges[1] = c - a;
        this->normal = glm::normalize(glm::cross(b - a, c - a));
//        printf("%f %f %f, %f %f %f, %f %f %f\n", a.x, a.y, a.z, b.x, b.y, b.z, c.x, c.y, c.z);
    }
    
    inline bool CheckIntersection(const Ray &ray, float *distance) const {
        glm::vec3 pvec = glm::cross(ray.direction, edges[0]);
        float det = glm::dot(edges[1], pvec);
        glm::vec3 tvec = ray.origin - this->a;
        float u = glm::dot(tvec, pvec);
        if (u < 0.0f || u > det) {
            return false;
        }
        glm::vec3 qvec = glm::cross(tvec, edges[1]);
        float v = glm::dot(ray.direction, qvec);
        if (v < 0.0f || (u + v) > det) {
            return false;
        }
        float d = glm::dot(edges[0], qvec);
        d *= 1.0f / det;
        if (d < EPSILON) {
            return false;
        }
        *distance = d;
        return true;
    }
    
    inline glm::vec3 GetNormalAtPoint(const glm::vec3 &point) const {
        return normal;
    }
    
    inline glm::vec3 GetNormal() const {
        return normal;
    }
    
    ~Triangle() {
    }
};

#endif /* defined(__raytracer_cpp__Triangle__) */
