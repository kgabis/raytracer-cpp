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
#include "TracingResult.h"

class Camera;
class Scene;

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
    static bool sFogShadows;

private:
    Ray Reflect(const glm::vec3 &normal, const glm::vec3 &collisionPoint) const;
    Color TraceRecursive(const Scene &scene, size_t depth) const;
    void TraceOnce(const Scene &scene, TracingResult *result) const;
    bool TraceForShadow(const Scene &scene, float lightDistance) const;
    float GetFogIntensity(const Scene &scene, float distance) const;
    float GetFogInShadowRatio(const Scene &scene, float hitDistance) const;
    ShadingResult ShadeAtPoint(const Scene &scene, const TracingResult &tracingResult, glm::vec3 point) const;
    
};

#endif /* defined(__raytracer_cpp__Ray__) */
