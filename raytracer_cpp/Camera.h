//
//  Camera.h
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 04.05.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef __raytracer_cpp__Camera__
#define __raytracer_cpp__Camera__

#include "global.h"

class Camera {
public:
    vec3 position;
    vec3 direction;
    vec3 up;
    vec3 planeCenter;
    vec3 planeDirectionX;
    vec3 planeDirectionY;
    float focalLength;
    float width;
    float height;
    
    Camera();
    Camera(vec3 position, vec3 direction, float focalLength, float width, float height);
    void moveForward(float distance);
    void moveBackwards(float distance);
    void moveRight(float distance);
    void moveLeft(float distance);
    void moveUp(float distance);
    void moveDown(float distance);
    void Update();
};

#endif /* defined(__raytracer_cpp__Camera__) */
