//
//  Raytracer.cpp
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 04.05.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include "Raytracer.h"
#include "Ray.h"

#include "global.h"

static Color pixels[WIDTH][HEIGHT];
static bool  isRendered[WIDTH][HEIGHT];

float Raytracer::sRandTresh = 0.8;

static void resetIsRendered( void ) {
    for (size_t x = 0; x < WIDTH; x++) {
        for (size_t y = 0; y < HEIGHT; y++) {
            isRendered[x][y] = false;
        }
    }
}

void setPixel(size_t x, size_t y, Color c) {
    if (WIDTH < x || HEIGHT < y) {
        return;
    }
    pixels[x][y] = c;
    isRendered[x][y] = true;
}

Color getPixel(size_t x, size_t y, const Ray &ray, const Scene &scene) {
    if (WIDTH < x || HEIGHT < y) {
        return Color();
    }
    float r = (float)random()/(float)RAND_MAX;
    if (!isRendered[x][y] && r > Raytracer::sRandTresh) {
            pixels[x][y] = ray.Trace(scene);
            isRendered[x][y] = true;    
    }
    return pixels[x][y];
}

Raytracer::Raytracer(size_t resolutionX, size_t resolutionY)
{
    this->scene = { resolutionX, resolutionY };
    this->resolutionX = resolutionX;
    this->resolutionY = resolutionY;
}

void Raytracer::Render(DrawFunction drawFunction, void *data) {
    this->scene.camera.Update();
    Color color;
    //this->scene.lights[0].position.z += 0.3f;
    for (size_t x = 0; x < this->resolutionX; x++) {
        for (size_t y = 0; y < this->resolutionY; y++) {
            Ray ray(this->scene.camera, x, y);
            color = getPixel(x, y, ray, scene);
            drawFunction(data, color, x, y);
        }
    }
    this->scene.test_RotateCube();
}

void Raytracer::needsUpdate(bool value) {
    if (value) {
        resetIsRendered();
    }
}

Raytracer::~Raytracer() {
    
}

