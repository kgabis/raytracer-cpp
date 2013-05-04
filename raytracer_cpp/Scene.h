//
//  Scene.h
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 04.05.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef __raytracer_cpp__Scene__
#define __raytracer_cpp__Scene__

#include <iostream>
#include <vector>

#include "Surface.h"
#include "Light.h"
#include "Camera.h"
#include "Color.h"

class Scene {
public:
    std::vector<Surface> surfaces;
    std::vector<Light> lights;
    double ambientCoefficient;
    Camera camera;
    Color backgroundColor;
    
    Scene(size_t cameraWidth, size_t cameraHeight);
    void LoadSpheresDemo();
    ~Scene();
};

#endif /* defined(__raytracer_cpp__Scene__) */
