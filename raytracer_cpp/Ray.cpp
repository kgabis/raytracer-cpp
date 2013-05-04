//
//  Ray.cpp
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 29.04.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include "Ray.h"



Ray::Ray(glm::vec3 origin, glm::vec3 direction) {
    this->origin = origin;
    this->direction = direction;
}