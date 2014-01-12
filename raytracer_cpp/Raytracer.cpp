//
//  Raytracer.cpp
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 04.05.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include "Raytracer.h"
#include "Ray.h"
#include "Sphere.h"
#include "global.h"

#define MAX_RECURSION_DEPTH 2
#define MAX_LIGHT_DISTANCE 300
#define FOG_COLOR Color::White
#define FOG_INTENSITY 10
#define MAX_VISIBLE_DISTANCE 1000

typedef struct {
    float diffused;
    float specular;
} ShadingResult;

static Color pixels[WIDTH][HEIGHT];
static bool  isRendered[WIDTH][HEIGHT];

float Raytracer::sRandTresh = 0.95;
bool  Raytracer::sFogShadows = false;

Color TraceRecursive(const Ray &ray, Scene &scene, size_t recursionDepth);
void TraceOnce(const Ray &ray, Scene &scene, TracingResult *result);
bool TraceForShadow(const Ray &ray, Scene &scene, float lightDistance, const Triangle **lastOccluder);
ShadingResult ShadeAtPoint(const Ray &ray, Scene &scene, const TracingResult &tracingResult, glm::vec3 point);
ShadingResult ShadeAtPoint(const Ray &ray, Scene &scene, const TracingResult &tracingResult, glm::vec3 point);
float GetFogIntensity(float distance);
float GetFogInShadowRatio(const Ray &ray, Scene &scene, float hitDistance);

Color TraceRecursive(const Ray &ray, Scene &scene, size_t recursionDepth) {
    TracingResult closestHit;
    TraceOnce(ray, scene, &closestHit);
    if (!closestHit.hit) { // no hit
        return ray.direction.y > 0 ? scene.skyColor : scene.groundColor;
    }
    if (closestHit.isLightSource) { // hit light source's sphere
        return Light::DefaultColor;
    }
    Color resultColor = closestHit.material->color;
    glm::vec3 collisionPoint = (ray.direction * closestHit.distance) + ray.origin;
    const Material *material = closestHit.material;
    if (material->reflectivity > 0 && recursionDepth > 0) {
        Ray reflectedRay = ray.Reflect(closestHit.normal, collisionPoint);
        Color reflectionColor = TraceRecursive(reflectedRay, scene, recursionDepth - 1);
        resultColor = reflectionColor.Blend(resultColor, material->reflectivity);
    }
    ShadingResult shadingResult = ShadeAtPoint(ray, scene, closestHit, collisionPoint);
    resultColor = resultColor.GetHighlighted(shadingResult.diffused, shadingResult.specular, scene.ambientCoefficient, closestHit.distance, MAX_LIGHT_DISTANCE);
    float fogIntensity = GetFogIntensity(closestHit.distance);
    resultColor = resultColor.AddWeighted(1.0 - fogIntensity, FOG_COLOR, fogIntensity);
    if (Raytracer::sFogShadows) {
        float fogInShadow = GetFogInShadowRatio(ray, scene, closestHit.distance);
        resultColor = resultColor.AddWeighted(1.0 - fogInShadow, Color::Black, fogInShadow);
    }
    return resultColor;
}

void TraceOnce(const Ray &ray, Scene &scene, TracingResult *result) {
    TracingResult currentHit;
    for (const auto &m : scene.meshes) { // finds intersections with geometries
        m.FindIntersection(ray, &currentHit);
        if (currentHit.hit && currentHit.distance < result->distance) {
            *result = currentHit;
        }
    }
    for (const auto &l : scene.lights ) { // finds intersection with light sources
        Sphere s(l.position, l.intensity * Light::DefaultRadius);
        currentHit.hit = s.CheckIntersection(ray, &currentHit.distance);
        currentHit.isLightSource = true;
        if (currentHit.hit && currentHit.distance < result->distance) {
            *result = currentHit;
        }
    }
}

bool TraceForShadow(const Ray &ray, Scene &scene, float lightDistance, const Triangle **lastOccluder) {
    float distance = FLT_MAX;
    //++Diagnostics::shadowTests;
    if (*lastOccluder != nullptr) {
        bool hit = (*lastOccluder)->CheckIntersection(ray, &distance);
        if (hit && distance < lightDistance) {
	  //++Diagnostics::cachedShadowHits;
            return true;
        }
    }
    const Triangle *t = nullptr;
    for (const auto &m : scene.meshes) {
        bool hit = m.FindFirstIntersectionInRange(ray, lightDistance, &t);
        if (hit) {
	  //*lastOccluder = t;
            return true;
        }
    }
    return false;
}

ShadingResult ShadeAtPoint(const Ray &ray, Scene &scene, const TracingResult &tracingResult, glm::vec3 point) {
    ShadingResult shadingResult = { .diffused = 0, .specular = 0 };
    float lightDistance;
    glm::vec3 lightDirection;
    Ray newRay;
    bool isInShadow = false;
    for (auto &light : scene.lights) {
        lightDirection = light.GetDirectionAtPoint(point);
        float dirdot = glm::dot(lightDirection, tracingResult.normal);
        if (dirdot < -EPSILON) {
            continue;
        }
        lightDirection = glm::normalize(lightDirection);
        newRay.origin = point;
        newRay.direction = -lightDirection;
        lightDistance = glm::length(point - light.position);
        isInShadow = TraceForShadow(newRay, scene, lightDistance, &light.lastOccluder);
        if (!isInShadow) {
            glm::vec3 normal = tracingResult.normal;
            shadingResult.diffused += light.GetDiffusedHighlight(lightDirection, normal);
            shadingResult.specular += light.GetSpecularHighlight(lightDirection, normal, ray.direction, tracingResult.material->specularity);
        }
    }
    return shadingResult;
}

float GetFogIntensity(float distance) {
    return distance / MAX_VISIBLE_DISTANCE + 0.03 * ((float)rand()/(float)RAND_MAX);
}

float GetFogInShadowRatio(const Ray &ray, Scene &scene, float hitDistance) {
#define FOGSTEP_MIN 10.0
#define FOGSTEP_MAX 15.0
#define RANDOM_STEP() (((float)random()/(float)RAND_MAX)*(FOGSTEP_MAX-FOGSTEP_MIN)+FOGSTEP_MIN)
    float distance = RANDOM_STEP();
    int pointsInShadow = 0;
    int npoints = 5;
    while (distance < hitDistance && distance < MAX_VISIBLE_DISTANCE) {
        glm::vec3 point = ray.origin + ray.direction * distance;
        for (auto &light : scene.lights) {
            glm::vec3 lightDirection = light.GetNormalizedDirectionAtPoint(point);
            Ray shadowRay(point, -lightDirection);
            float lightDistance = glm::length(point - light.position);
            if (TraceForShadow(shadowRay, scene, lightDistance, &light.lastOccluder)) {
                pointsInShadow++;
            }
            npoints++;
        }
        distance += RANDOM_STEP();
    }
    return (float)pointsInShadow / (float)npoints;
#undef FOGSTEP_MIN
#undef FOGSTEP_MAX
#undef RANDOM_STEP
}

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
            pixels[x][y] = TraceRecursive(ray, scene, MAX_RECURSION_DEPTH);
            isRendered[x][y] = true;
    }
    return pixels[x][y];
#else
    return TraceRecursive(ray, scene, MAX_RECURSION_DEPTH);
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
    #pragma omp parallel for
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

