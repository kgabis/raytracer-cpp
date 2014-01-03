//
//  Ray.cpp
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 294.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include "Ray.h"
#include "Surface.h"
#include "Camera.h"
#include "Scene.h"
#include "TracingResult.h"
#include "Color.h"
#include "Diagnostics.h"

Ray::Ray() {
    this->origin = glm::vec3(0.0f, 0.0f, 0.0f);
    this->direction = glm::vec3(0.0f, 0.0f, 0.0f);
}

Ray::Ray(glm::vec3 origin, glm::vec3 direction) {
    this->origin = origin;
    this->direction = direction;
}

Ray::Ray(const Camera &camera, float x, float y) {
    float dy = 1;
    float dx = 1;
    float py = (-camera.height / 2) + dy * (y + 0.5);
    float px = (-camera.width / 2) + dx * (x + 0.5);
    glm::vec3 p = camera.planeCenter + (camera.planeDirectionX * px) + (camera.planeDirectionY * py);
    glm::vec3 u_r = glm::normalize(p - camera.position);
    this->origin = camera.position;
    this->direction = u_r;
}

Ray Ray::Reflect(const glm::vec3 &normal, const glm::vec3 &collisionPoint) const {
    float c1 = -glm::dot(normal, this->direction);
    glm::vec3 reflectedDirection = this->direction + (normal * (2 * c1));
    return Ray(collisionPoint, reflectedDirection);
}

void Ray::print() const {
    printf("{{%.2f %.2f %.2f}, {%.2f %.2f %.2f}}\n",
           this->origin.x, this->origin.y, this->origin.z,
           this->direction.x, this->direction.y, this->direction.z);
}