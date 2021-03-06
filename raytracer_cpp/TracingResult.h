//
//  TracingResult.h
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 18.08.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef raytracer_cpp_TracingResult_h
#define raytracer_cpp_TracingResult_h

#include <glm/glm.hpp>

class Material;
class Triangle;

struct TracingResult {
    bool hit;
    float distance;
    const Material *material;
    const Triangle *triangle;
    glm::vec3 normal;
    bool isLightSource;
    
    TracingResult(void) :
                    hit(false),
                    distance(1.0f / 0.0f),
                    material(nullptr),
                    normal(glm::vec3(0.0f, 0.0f, 0.0f)),
                    isLightSource(false),
                    triangle(nullptr)
    { }
};

#endif
