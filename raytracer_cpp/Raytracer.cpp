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

float Raytracer::sRandTresh = 0.4;

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
    size_t xToSet = x, yToSet = y;
    float step = Raytracer::sRandTresh / 4.0f;
    if (isRendered[x][y]) {
        return pixels[x][y];
    }
    float r = (float)random()/(float)RAND_MAX;
    if (r > Raytracer::sRandTresh) {
            pixels[x][y] = ray.Trace(scene);
            isRendered[x][y] = true;    
//    } else {
//        if (r < step)            xToSet = (x - 1) % WIDTH;
//        else if (r < (2 * step)) xToSet = (x + 1) % WIDTH;
//        else if (r < (3 * step)) yToSet = (y - 1) % HEIGHT;
//        else                     yToSet = (y + 1) % HEIGHT;
//        pixels[x][y] = ray.Trace(scene);
//        isRendered[x][y] = true;
//        pixels[xToSet][yToSet] = pixels[x][y];
//        isRendered[xToSet][yToSet] = true;
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
    resetIsRendered();
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

Raytracer::~Raytracer() {
    
}

