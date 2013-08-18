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

class Ray;

class Triangle {
public:
    glm::vec3 a, b, c;
    glm::vec3 edges[2];
    glm::vec3 normal;
    Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c);
    bool CheckIntersection(const Ray &ray, float *distance) const;
    glm::vec3 GetNormalAtPoint(const glm::vec3 &point) const;
    ~Triangle() = default;
};

#endif /* defined(__raytracer_cpp__Triangle__) */
