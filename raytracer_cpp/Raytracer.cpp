//
//  Raytracer.cpp
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 04.05.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include "Raytracer.h"
#include "Ray.h"

Raytracer::Raytracer(size_t resolutionX, size_t resolutionY) {
    this->scene = { resolutionX, resolutionY };
    this->resolutionX = resolutionX;
    this->resolutionY = resolutionY;
}

void Raytracer::Render(DrawFunction drawFunction, void *data) {
    this->scene.camera.position.z -= 1.0f;
    this->scene.camera.Update();
    for (size_t x = 0; x < this->resolutionX; x++) {
        for (size_t y = 0; y < this->resolutionY; y++) {
            Ray ray(this->scene.camera, x, y);
            Color color = ray.Trace(this->scene);
            drawFunction(data, color, x, y);
        }
    }
}

Raytracer::~Raytracer() {
    
}

