//
//  Sphere.h
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 17.08.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef __raytracer_cpp__Sphere__
#define __raytracer_cpp__Sphere__

#include <glm/glm.hpp>

class Ray;

class Sphere {
public:
    glm::vec3 center;
    double radius;
    
    Sphere(glm::vec3 center, double radius);
    bool CheckIntersection(const Ray &ray, float *distance) const;
    glm::vec3 GetNormalAtPoint(const glm::vec3 &point) const;
    ~Sphere() = default;
};


#endif /* defined(__raytracer_cpp__Sphere__) */
