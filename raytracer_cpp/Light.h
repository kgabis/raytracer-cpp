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

class Light {
public:
    glm::vec3 position;
    float intensity;
    
    Light( glm::vec3 position, float intensity );
    
    glm::vec3 GetDirectionAtPoint( glm::vec3 point ) const;
    float GetDiffusedHighlight   ( glm::vec3 direction, glm::vec3 normal ) const;
    float GetSpecularHighlight   ( glm::vec3 direction, glm::vec3 normal, glm::vec3 rayDirection, float specularity ) const;
    
    static const Color DefaultColor;
    static const float DefaultRadius;
};

#endif /* defined(__raytracer_cpp__Light__) */
