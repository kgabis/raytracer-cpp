//
//  Camera.h
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 04.05.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef __raytracer_cpp__Camera__
#define __raytracer_cpp__Camera__

#include <glm/glm.hpp>

class Camera {
public:
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 up;
    glm::vec3 planeCenter;
    glm::vec3 planeDirectionX;
    glm::vec3 planeDirectionY;
    float focalLength;
    float width;
    float height;
    
    Camera();
    Camera(glm::vec3 position, glm::vec3 direction, float focalLength, float width, float height);
    void moveForward(float distance);
    void moveBackwards(float distance);
    void moveRight(float distance);
    void moveLeft(float distance);
    void moveUp(float distance);
    void moveDown(float distance);
    void lookUp(float degrees);
    void lookDown(float degrees);
    void lookLeft(float degrees);
    void lookRight(float degrees);
    void Update();
};

#endif /* defined(__raytracer_cpp__Camera__) */
