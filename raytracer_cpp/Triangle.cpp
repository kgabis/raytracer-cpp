//
//  Triangle.cpp
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 17.08.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include "Triangle.h"
#include "Ray.h"

Triangle::Triangle( glm::vec3 a, glm::vec3 b, glm::vec3 c ) {
    this->a = a;
    this->b = b;
    this->c = c;
    this->edges[0] = b - a;
    this->edges[1] = c - a;
    this->normal = glm::normalize( glm::cross( b - a, c - a ) );
}

bool Triangle::CheckIntersection( const Ray &ray, float *distance ) const {
    glm::vec3 pvec = glm::cross( ray.direction, this->edges[0] );
    float det = glm::dot( this->edges[1], pvec );
    float const epsilon = 0.000001f;
    if ( det < epsilon ) {
        return false;
    }
    glm::vec3 tvec = ray.origin - this->a;
    float u = glm::dot( tvec, pvec );
    if ( u < 0.0f || u > det ) {
        return false;
    }
    glm::vec3 qvec = glm::cross( tvec, this->edges[1] );
    float v = glm::dot( ray.direction, qvec );
    if ( v < 0.0f || ( u + v ) > det ) {
        return false;
    }
    float d = glm::dot(this->edges[0], qvec);
    float inv_det = 1.0f / det;
    d *= inv_det;
    *distance = d;
    return true;
}

glm::vec3 Triangle::GetNormalAtPoint( const glm::vec3 &point ) const {
    return this->normal;
}