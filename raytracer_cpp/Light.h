//
//  Light.h
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 04.05.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef __raytracer_cpp__Light__
#define __raytracer_cpp__Light__

#include <iostream>
#include <glm/glm.hpp>

#include "Color.h"

class Triangle;

class Light {
public:
    glm::vec3 position;
    float intensity;
    const Triangle *lastOccluder;
        
    Light(glm::vec3 position, float intensity) {
        this->position = position;
        this->intensity = intensity;
        this->lastOccluder = nullptr;
    }
    
    inline glm::vec3 GetDirectionAtPoint(glm::vec3 point) const {
        return point - this->position;
    }
    
    inline glm::vec3 GetNormalizedDirectionAtPoint(glm::vec3 point) const {
        return glm::normalize(point - this->position);
    }
    
    inline float GetDiffusedHighlight(glm::vec3 direction, glm::vec3 normal) const {
        float highlight = glm::dot(normal, direction);
        if ( highlight <= 0.0f ) {
            return 0.0f;
        }
        return highlight * this->intensity;
    }
    
    inline float GetSpecularHighlight(glm::vec3 direction, glm::vec3 normal, glm::vec3 rayDirection, float specularity) const {
        float highlight = glm::dot(normal, direction);
        glm::vec3 V = -rayDirection;
        glm::vec3 R = direction - (normal * highlight * 2.0f);
        float dot = glm::dot(V, R);
        if ( dot <= 0.0f ) {
            return 0.0f;
        }
        return std::pow(dot, specularity) * this->intensity;
    }

    static const Color DefaultColor;
    static const float DefaultRadius;
};

#endif /* defined(__raytracer_cpp__Light__) */
