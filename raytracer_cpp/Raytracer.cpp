//
//  Raytracer.cpp
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 04.05.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include "Raytracer.h"
#include "Ray.h"

#define WIDTH 640
#define HEIGHT 480
#define RAND_TRESH 0.4

static Color pixels[WIDTH][HEIGHT];

void setPixel(size_t x, size_t y, Color c) {
    if (WIDTH < x || HEIGHT < y) {
        return;
    }
    pixels[x][y] = c;
}

Color getPixel(size_t x, size_t y) {
    if (WIDTH < x || HEIGHT < y) {
        return Color();
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
    float r, step = RAND_TRESH / 4;
    //this->scene.lights[0].position.z += 0.3f;
    for (size_t x = 0; x < this->resolutionX; x++) {
        for (size_t y = 0; y < this->resolutionY; y++) {
            Ray ray(this->scene.camera, x, y);
            r = (float)rand()/(float)RAND_MAX;
            if (r > RAND_TRESH) {
                color = ray.Trace(this->scene);
            } else {
                if (r < step) {
                    color = getPixel(x - 1, y);
                } else if (r < 2 * step) {
                    color = getPixel(x + 1, y);
                } else if (r < 3 * step) {
                    color = getPixel(x, y + 1);
                } else {
                    color = getPixel(x, y - 1);
                }                
            }
            drawFunction(data, color, x, y);
            setPixel(x, y, color);
        }
    }
    this->scene.test_RotateCube();
}

Raytracer::~Raytracer() {
    
}

