//
//  Camera.cpp
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 04.05.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include "Camera.h"
#include <glm/gtx/rotate_vector.hpp>

static const float cLookSpeed = 6.0;
static const float cMoveSpeed = 2.0;

typedef glm::vec3 vec3;

Camera::Camera() { }

Camera::Camera(vec3 position, vec3 direction, float focalLength, float width, float height){
    this->position = position;
    this->direction = glm::normalize(direction);
    this->focalLength = focalLength;
    this->up = vec3(0.0f, -1.0f, 0.0f);
    this->width = width;
    this->height = height;
    this->Update();
}

void Camera::moveForward(float distance) {
    this->position += this->direction * distance * cMoveSpeed;
}

void Camera::moveBackwards(float distance) {
    moveForward(-distance);
}

void Camera::moveRight(float distance) {
    vec3 right = glm::cross(this->direction, this->up);
    this->position += right * distance * cMoveSpeed;
}

void Camera::moveLeft(float distance) {
    moveRight(-distance);
}

void Camera::moveUp(float distance) {
    this->position += this->up * (-distance) * cMoveSpeed;
}

void Camera::moveDown(float distance) {
    moveUp(-distance);
}

void Camera::lookUp(float degrees) {
    vec3 right = glm::cross(this->direction, this->up);
    this->direction = glm::rotate(this->direction, -degrees * cLookSpeed, right);
}

void Camera::lookDown(float degrees) {
    vec3 right = glm::cross(this->direction, this->up);
    this->direction = glm::rotate(this->direction, degrees * cLookSpeed, right);
}

void Camera::lookLeft(float degrees) {
    this->direction = glm::rotate(this->direction, degrees * cLookSpeed, this->up);
}

void Camera::lookRight(float degrees) {
    this->direction = glm::rotate(this->direction, -degrees * cLookSpeed, this->up);
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
