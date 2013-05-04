//
//  Ray.h
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 29.04.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef __raytracer_cpp__Ray__
#define __raytracer_cpp__Ray__

#include <iostream>
#include <glm/glm.hpp>

#include "Color.h"
#include "Surface.h"
#include "Camera.h"
#include "Scene.h"

typedef struct {
    int hit;
    Color color;
    const Surface *surface;
    double distance;
} TracingResult;

typedef struct {
    double diffused;
    double specular;
} ShadingResult;

class Ray {
public:
    glm::vec3 origin;
    glm::vec3 direction;
    
    Ray(glm::vec3 origin, glm::vec3 direction);
    Ray(const Camera *camera, size_t x, size_t y);
    Color Trace(const Scene * const scene) const;
    
private:
    Ray Reflect(const Surface &surface, glm::vec3 collisionPoint) const;
    Color TraceRecursive(const Scene &scene, glm::vec3 point) const;
    TracingResult TraceOnce(const Scene *scene) const;
    TracingResult CheckIntersection(
};

static Ray ray_addNoise(Ray ray, double magnitude);
static Ray ray_reflect(Ray ray, const Surface *surface, Vector3 point);
static Color ray_traceRecursive(Ray ray, const Scene *scene, size_t depth);
static TracingResult ray_traceOnce(Ray ray, const Scene *scene);
static TracingResult ray_checkIntersection(Ray ray, const Surface *surface);
static TracingResult ray_checkSphereIntersection_1(Ray ray, const Sphere *sphere);
static TracingResult ray_checkSphereIntersection_2(Ray ray, const Sphere *sphere);
static TracingResult ray_checkTriangleIntersectionWithCulling(Ray ray, const Triangle *triangle);

#endif /* defined(__raytracer_cpp__Ray__) */
