//
//  Ray.h
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 29.04.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef __raytracer_cpp__Ray__
#define __raytracer_cpp__Ray__

#include <iostream>
#include <glm/glm.hpp>

#include "Color.h"
#include "TracingResult.h"

class Camera;
class Scene;

class Ray {
public:
    glm::vec3 origin;
    glm::vec3 direction;
    
    Ray();
    Ray(glm::vec3 origin, glm::vec3 direction);
    Ray(const Camera &camera, float x, float y);
    void print() const;

    Ray Reflect(const glm::vec3 &normal, const glm::vec3 &collisionPoint) const;
};

#endif /* defined(__raytracer_cpp__Ray__) */
