//
//  Sphere.cpp
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 17.08.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include "Sphere.h"
#include "Ray.h"

Sphere::Sphere( glm::vec3 center, double radius ) {
    this->center = center;
    this->radius = radius;
}

bool Sphere::CheckIntersection( const Ray &ray, float *distance ) const {
    glm::vec3 EO = this->center - ray.origin;
    float v = glm::dot( EO, ray.direction );
    if ( v < 0.0f ) {
        return false;
    }
    float r = this->radius;
    float disc = ( r * r ) - ( glm::dot( EO, EO ) - ( v * v ) );
    if ( disc < 0.0f ) {
        return false;
    }
    float d = std::sqrt( disc );
    *distance = std::min( v - d, v + d );
    return true;
}

glm::vec3 Sphere::GetNormalAtPoint( const glm::vec3 &point ) const {
    return glm::normalize( this->center - point );
}
