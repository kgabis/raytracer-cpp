//
//  Scene.h
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 04.05.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef __raytracer_cpp__Scene__
#define __raytracer_cpp__Scene__

#include <vector>

#include "Mesh.h"
#include "Light.h"
#include "Camera.h"
#include "Color.h"

class Scene {
public:
    std::vector<Mesh> meshes;
    std::vector<Light> lights;
    double ambientCoefficient;
    Camera camera;
    Color groundColor;
    Color skyColor;
    
    Scene();
    Scene(size_t cameraWidth, size_t cameraHeight);
    void LoadDemo(void);
    void test_RotateCube(void);
    ~Scene();
};

#endif /* defined(__raytracer_cpp__Scene__) */
