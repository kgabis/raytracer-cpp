//
//  Camera.cpp
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 04.05.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include "Camera.h"

Camera::Camera() { }

Camera::Camera(vec3 position, vec3 direction, float focalLength, float width, float height){
    this->position = position;
    this->direction = glm::normalize(direction);
    this->focalLength = focalLength;
    this->up = vec3(0, 1, 0);
    this->width = width;
    this->height = height;
    this->Update();
}

void Camera::moveForward(float distance) {
    this->position += this->direction * distance;
}

void Camera::moveBackwards(float distance) {
    moveForward(-distance);
}

void Camera::moveRight(float distance) {
    vec3 right = glm::cross(this->direction, this->up);
    this->position += right * distance;
}

void Camera::moveLeft(float distance) {
    moveRight(-distance);
}

void Camera::moveUp(float distance) {
    this->position += this->up * (-distance);
}

void Camera::moveDown(float distance) {
    moveUp(-distance);
}

void Camera::Update() {
    vec3 x_c = this->position;
    vec3 u_c = this->direction;
    float z_p = this->focalLength;
    vec3 c_0 = x_c + (z_p * u_c);
    vec3 u_x = glm::normalize(glm::cross(u_c, this->up));
    vec3 u_y = glm::cross(u_c * -1.0f, u_x);
    this->planeCenter = c_0;
    this->planeDirectionX = u_x;
    this->planeDirectionY = u_y;
}
