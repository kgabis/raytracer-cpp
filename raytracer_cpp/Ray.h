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
    Color Trace(Scene &scene) const;
    void print() const;
    static bool sFogShadows;

private:
    Ray Reflect(const glm::vec3 &normal, const glm::vec3 &collisionPoint) const;
    Color TraceRecursive(Scene &scene, size_t depth) const;
    void TraceOnce(Scene &scene, TracingResult *result) const;
    bool TraceForShadow(Scene &scene, float lightDistance, const Triangle **lastOccluder) const;
    float GetFogIntensity(Scene &scene, float distance) const;
    float GetFogInShadowRatio(Scene &scene, float hitDistance) const;
    ShadingResult ShadeAtPoint(Scene &scene, const TracingResult &tracingResult, glm::vec3 point) const;
    
};

#endif /* defined(__raytracer_cpp__Ray__) */
