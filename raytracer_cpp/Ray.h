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

class Camera;
class Scene;
class Surface;

typedef struct {
    bool hit;
    const Surface *surface;
    float distance;
} TracingResult;

typedef struct {
    float diffused;
    float specular;
} ShadingResult;

class Ray {
public:
    glm::vec3 origin;
    glm::vec3 direction;
    
    Ray();
    Ray(glm::vec3 origin, glm::vec3 direction);
    Ray(const Camera &camera, size_t x, size_t y);
    
    Color Trace(const Scene &scene) const;
    void print() const;
    
private:
    Ray Reflect(const Surface &surface, glm::vec3 collisionPoint) const;
    Color TraceRecursive(const Scene &scene, size_t depth) const;
    TracingResult TraceOnce(const Scene &scene) const;
    bool TraceForShadow(const Scene &scene, const Surface &surfaceToExclude, float lightDistance) const;
    ShadingResult ShadeAtPoint(const Scene &scene, const Surface &surface, glm::vec3 point) const;
};

#endif /* defined(__raytracer_cpp__Ray__) */
