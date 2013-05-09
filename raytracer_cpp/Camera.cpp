//
//  Camera.cpp
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 04.05.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include "Camera.h"

Camera::Camera() { }

Camera::Camera(glm::vec3 position, glm::vec3 direction, float focalLength, float width, float height){
    this->position = position;
    this->direction = glm::normalize(direction);
    this->focalLength = focalLength;
    this->width = width;
    this->height = height;
    this->Update();
}

void Camera::Update() {
    glm::vec3 x_c = this->position;
    glm::vec3 u_c = this->direction;
    float z_p = this->focalLength;
    glm::vec3 v_up(0.0, -1.0, 0.0);
    glm::vec3 c_0 = x_c + (z_p * u_c);
    glm::vec3 u_x = glm::normalize(glm::cross(u_c, v_up));
    glm::vec3 u_y = glm::cross(u_c * -1.0f, u_x);
    this->planeCenter = c_0;
    this->planeDirectionX = u_x;
    this->planeDirectionY = u_y;
}