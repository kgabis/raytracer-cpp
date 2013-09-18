//
//  Light.cpp
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 04.05.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include "Light.h"

#define KS_CONST 1.0

Light::Light( glm::vec3 position, float intensity ) {
    this->position = position;
    this->intensity = intensity;
}

glm::vec3 Light::GetDirectionAtPoint( glm::vec3 point ) const {
    return glm::normalize(point - this->position);
}

float Light::GetDiffusedHighlight( glm::vec3 direction, glm::vec3 normal ) const {
    float highlight = glm::dot(normal, direction);
    if ( highlight < 0.0f ) {
        return 0.0f;
    }
    return highlight * this->intensity;
}

float Light::GetSpecularHighlight( glm::vec3 direction, glm::vec3 normal, glm::vec3 rayDirection, float specularity ) const {
    float highlight = glm::dot(normal, direction);
    glm::vec3 V = -rayDirection;
    glm::vec3 R = direction - (normal * highlight * 2.0f);
    float dot = glm::dot(V, R);
    if ( dot < 0.0f ) {
        return 0.0f;
    }
    return std::pow(dot, specularity) * KS_CONST * this->intensity;
}

const Color Light::DefaultColor = Color(1.0, 1.0, 1.0);
const float Light::DefaultRadius = 2.0;