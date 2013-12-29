//
//  Ray.cpp
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 294.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include "Ray.h"
#include "Surface.h"
#include "Camera.h"
#include "Scene.h"
#include "TracingResult.h"
#include "Color.h"
#include "Diagnostics.h"

#define MAX_RECURSION_DEPTH 3
#define MAX_VISIBLE_DISTANCE 1000
#define MAX_LIGHT_DISTANCE 300
#define FOG_COLOR Color::White
#define FOG_INTENSITY 10

bool Ray::sFogShadows = false;

Ray::Ray() {
    this->origin = glm::vec3(0.0f, 0.0f, 0.0f);
    this->direction = glm::vec3(0.0f, 0.0f, 0.0f);
}

Ray::Ray(glm::vec3 origin, glm::vec3 direction) {
    this->origin = origin;
    this->direction = direction;
}

Ray::Ray(const Camera &camera, size_t x, size_t y) {
    float dy = 1;
    float dx = 1;
    float py = (-camera.height / 2) + dy * ((float)y + 0.5);
    float px = (-camera.width / 2) + dx * ((float)x + 0.5);
    glm::vec3 p = camera.planeCenter + (camera.planeDirectionX * px) + (camera.planeDirectionY * py);
    glm::vec3 u_r = glm::normalize(p - camera.position);
    this->origin = camera.position;
    this->direction = u_r;
}

Color Ray::Trace(Scene &scene) const {
    return this->TraceRecursive(scene, MAX_RECURSION_DEPTH);
}

Ray Ray::Reflect(const glm::vec3 &normal, const glm::vec3 &collisionPoint) const {
    float c1 = -glm::dot(normal, this->direction);
    glm::vec3 reflectedDirection = this->direction + (normal * (2 * c1));
    return Ray(collisionPoint, reflectedDirection);
}

Color Ray::TraceRecursive(Scene &scene, size_t recursionDepth) const {
    TracingResult closestHit;
    this->TraceOnce(scene, &closestHit);
    if (!closestHit.hit) { // no hit
        return this->direction.y > 0 ? scene.skyColor : scene.groundColor;
    }
    if (closestHit.isLightSource) { // hit light source's sphere
        return Light::DefaultColor;
    }
    Color resultColor = closestHit.material->color;
    glm::vec3 collisionPoint = (this->direction * closestHit.distance) + this->origin;
    const Material *material = closestHit.material;
    if (material->reflectivity > 0 && recursionDepth > 0) {
        Ray reflectedRay = this->Reflect(closestHit.normal, collisionPoint);
        Color reflectionColor = reflectedRay.TraceRecursive(scene, recursionDepth - 1);
        resultColor = reflectionColor.Blend(resultColor, material->reflectivity);
    }
    ShadingResult shadingResult = this->ShadeAtPoint(scene, closestHit, collisionPoint);
    resultColor = resultColor.GetHighlighted(shadingResult.diffused, shadingResult.specular, scene.ambientCoefficient, closestHit.distance, MAX_LIGHT_DISTANCE);
    float fogIntensity = this->GetFogIntensity(scene, closestHit.distance);
    resultColor = resultColor.AddWeighted(1.0 - fogIntensity, FOG_COLOR, fogIntensity);
    if (sFogShadows) {
        float fogInShadow = this->GetFogInShadowRatio(scene, closestHit.distance);
        resultColor = resultColor.AddWeighted(1.0 - fogInShadow, Color::Black, fogInShadow);
    }
    return resultColor;
}

void Ray::TraceOnce(Scene &scene, TracingResult *result) const {
    TracingResult currentHit;
    for (const auto &m : scene.meshes) { // finds intersections with geometries
        m.FindIntersection(*this, &currentHit);
        if (currentHit.hit && currentHit.distance < result->distance) {
            *result = currentHit;
        }
    }
    for (const auto &l : scene.lights ) { // finds intersection with light sources
        Sphere s(l.position, l.intensity * Light::DefaultRadius);
        currentHit.hit = s.CheckIntersection(*this, &currentHit.distance);
        currentHit.isLightSource = true;
        if (currentHit.hit && currentHit.distance < result->distance) {
            *result = currentHit;
        }
    }
}

bool Ray::TraceForShadow(Scene &scene, float lightDistance, const Triangle **lastOccluder) const {
    float distance = FLT_MAX;
    ++Diagnostics::shadowTests;
    if (*lastOccluder != nullptr) {
        bool hit = (*lastOccluder)->CheckIntersection(*this, &distance);
        if (hit && distance < lightDistance) {
            ++Diagnostics::cachedShadowHits;
            return true;
        }
    }
    const Triangle *t = nullptr;
    for (const auto &m : scene.meshes) {
        bool hit = m.FindFirstIntersectionInRange(*this, lightDistance, &t);
        if (hit) {
            *lastOccluder = t;
            return true;
        }
    }
    return false;   
}

ShadingResult Ray::ShadeAtPoint(Scene &scene, const TracingResult &tracingResult, glm::vec3 point) const {
    ShadingResult shadingResult = { .diffused = 0, .specular = 0 };
    float lightDistance;
    glm::vec3 lightDirection;
    Ray newRay;
    bool isInShadow = false;
    for (auto &light : scene.lights) {
        lightDirection = light.GetDirectionAtPoint(point);
        newRay.origin = point;
        newRay.direction = -lightDirection;
        lightDistance = glm::length(point - light.position);
        isInShadow = newRay.TraceForShadow(scene, lightDistance, &light.lastOccluder);
        if (!isInShadow) {
            glm::vec3 normal = tracingResult.normal;
            shadingResult.diffused += light.GetDiffusedHighlight(lightDirection, normal);
            shadingResult.specular += light.GetSpecularHighlight(lightDirection, normal, this->direction, tracingResult.material->specularity);
        }
    }
    return shadingResult;
}

void Ray::print() const {
    printf("{{%.2f %.2f %.2f}, {%.2f %.2f %.2f}}\n",
           this->origin.x, this->origin.y, this->origin.z,
           this->direction.x, this->direction.y, this->direction.z);
}

float Ray::GetFogIntensity(Scene &scene, float distance) const {
    return distance / MAX_VISIBLE_DISTANCE + 0.03 * ((float)rand()/(float)RAND_MAX);
}

float Ray::GetFogInShadowRatio(Scene &scene, float hitDistance) const {
#define FOGSTEP_MIN 10.0
#define FOGSTEP_MAX 15.0
#define RANDOM_STEP() (((float)random()/(float)RAND_MAX)*(FOGSTEP_MAX-FOGSTEP_MIN)+FOGSTEP_MIN)
    float distance = RANDOM_STEP();
    int pointsInShadow = 0;
    int npoints = 5;
    while (distance < hitDistance && distance < MAX_VISIBLE_DISTANCE) {
        glm::vec3 point = this->origin + this->direction * distance;
        for (auto &light : scene.lights) {
            glm::vec3 lightDirection = light.GetDirectionAtPoint(point);
            Ray shadowRay(point, -lightDirection);
            float lightDistance = glm::length(point - light.position);
            if (shadowRay.TraceForShadow(scene, lightDistance, &light.lastOccluder)) {
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

