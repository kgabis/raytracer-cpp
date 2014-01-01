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

float Raytracer::sRandTresh = 0.95;

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

inline Color getPixel(size_t x, size_t y, const Ray &ray, Scene &scene) {
#if RANDOMIZE_ON
    float r = (float)random()/(float)RAND_MAX;
    if (!isRendered[x][y] && r > Raytracer::sRandTresh) {
            pixels[x][y] = ray.Trace(scene);
            isRendered[x][y] = true;
    }
    return pixels[x][y];
#else
    return ray.Trace(scene);
#endif
}

inline Color blendColors(Color *colors, int len) {
    Color output;
    output.r = colors[0].r / len;
    output.g = colors[0].g / len;
    output.b = colors[0].b / len;
    for (int i = 1; i < len; i++) {
        output.r += colors[i].r / len;
        output.g += colors[i].g / len;
        output.b += colors[i].b / len;
    }
    return output;
}

Raytracer::Raytracer(size_t resolutionX, size_t resolutionY) {
    this->scene = { resolutionX, resolutionY };
    this->resolutionX = resolutionX;
    this->resolutionY = resolutionY;
}

void Raytracer::Render(DrawFunction drawFunction, void *data) {
    this->scene.camera.Update();
    Color color;
    size_t blockSize = 8;
    for (size_t x = 0; x < this->resolutionX; x+=blockSize) {
        for (size_t y = 0; y < this->resolutionY; y+=blockSize) {
            for (size_t blockX = 0; blockX < blockSize; blockX++) {
                for (size_t blockY = 0; blockY < blockSize; blockY++) {
#if AA_ON
                    Color colors[5];
                    Ray ray(this->scene.camera, x + blockX - AA_STEP, y + blockY);
                    colors[0] = getPixel(x + blockX, y + blockY, ray, scene);
                    ray = Ray(this->scene.camera, x + blockX + AA_STEP, y + blockY);
                    colors[1] = getPixel(x + blockX, y + blockY, ray, scene);
                    ray = Ray(this->scene.camera, x + blockX, y + blockY - AA_STEP);
                    colors[2] = getPixel(x + blockX, y + blockY, ray, scene);
                    ray = Ray(this->scene.camera, x + blockX, y + blockY + AA_STEP);
                    colors[3] = getPixel(x + blockX, y + blockY, ray, scene);
                    ray = Ray(this->scene.camera, x + blockX, y + blockY);
                    colors[4] = getPixel(x + blockX, y + blockY, ray, scene);
                    color = blendColors(colors, 5);
                    drawFunction(data, color, x + blockX, y + blockY);
#else
                    Ray ray(this->scene.camera, x + blockX, y + blockY);
                    color = getPixel(x + blockX, y + blockY, ray, scene);
                    drawFunction(data, color, x + blockX, y + blockY);

#endif
                }
            }

        }
    }
}

void Raytracer::needsUpdate(bool value) {
    if (value) {
        resetIsRendered();
    }
}

Raytracer::~Raytracer() {
    
}

