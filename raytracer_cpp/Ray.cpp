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

#define MAX_RECURSION_DEPTH 4
#define MAX_VISIBLE_DISTANCE 600
#define EPSILON 000001

Ray::Ray() {
    this->origin = glm::vec3(0, 0, 0);
    this->direction = glm::vec3(0, 0, 0);
}

Ray::Ray(glm::vec3 origin, glm::vec3 direction) {
    this->origin = origin;
    this->direction = direction;
}

Ray::Ray(const Camera &camera, size_t x, size_t y) {
    float dy = 1;
    float dx = 1;
    float py = (- camera.height / 2) + dy * ((float)y + 0.5);
    float px = (- camera.width / 2) + dx * ((float)x + 0.5);
    glm::vec3 p = camera.planeCenter + (camera.planeDirectionX * px) + (camera.planeDirectionY * py);
    glm::vec3 u_r = glm::normalize(p - camera.position);
    this->origin = camera.position;
    this->direction = u_r;
}

Color Ray::Trace(const Scene &scene) const {
    return this->TraceRecursive(scene, MAX_RECURSION_DEPTH);
}

Ray Ray::Reflect(const Surface &surface, glm::vec3 collisionPoint) const {
    glm::vec3 N = surface.geometry->GetNormalAtPoint(collisionPoint);
    float c1 = - glm::dot(N, this->direction);
    glm::vec3 RI = this->direction + (N * (2 * c1));
    return Ray(collisionPoint, RI);
}

Color Ray::TraceRecursive(const Scene &scene, size_t depth) const {
    TracingResult closestHit = this->TraceOnce(scene);
    if (!closestHit.hit) {
        return scene.backgroundColor;
    }
    Color closestHitColor = closestHit.surface->material.color;
    glm::vec3 collisionPoint = (this->direction * closestHit.distance) + this->origin;
    const Material *material = &closestHit.surface->material;
    if (material->reflectivity > 0 && depth > 0) {
        Ray reflectedRay = this->Reflect(*closestHit.surface, collisionPoint);
        Color reflectionColor = reflectedRay.TraceRecursive(scene, depth - 1);
        closestHitColor = reflectionColor.Blend(closestHitColor, material->reflectivity);
    }
    ShadingResult shadingResult = this->ShadeAtPoint(scene, *closestHit.surface, collisionPoint);
    closestHitColor = closestHitColor.GetHighlighted(shadingResult.diffused, shadingResult.specular, scene.ambientCoefficient);
    closestHitColor = closestHitColor.Multiply((MAX_VISIBLE_DISTANCE - closestHit.distance) / MAX_VISIBLE_DISTANCE);
    return closestHitColor;
}

TracingResult Ray::TraceOnce(const Scene &scene) const {
    TracingResult closestHit = { .hit = false, .distance = static_cast<float>(1.0 / 0.0) };
    TracingResult currentHit = { .hit = false, .distance = 0 };
    for (auto &s : scene.surfaces) {
        Geometry *geometry = s.geometry;
        currentHit.hit = geometry->CheckIntersection(*this, &currentHit.distance);
        if (currentHit.hit && currentHit.distance < closestHit.distance) {
            closestHit.hit = true;
            closestHit.distance = currentHit.distance;
            closestHit.surface = &s;
        }
    }
    return closestHit;
}

bool Ray::TraceForShadow(const Scene &scene, const Surface &surfaceToExclude, float lightDistance) const {
    TracingResult currentTrace;
    for (auto &s : scene.surfaces) {
        if (&s == &surfaceToExclude) {
            continue;
        }
        const Geometry *g = s.geometry;
        currentTrace.hit = g->CheckIntersection(*this, &currentTrace.distance);
        if (currentTrace.hit && currentTrace.distance < lightDistance) {
            return true;
        }
    }
    return false;    
}

ShadingResult Ray::ShadeAtPoint(const Scene &scene, const Surface &surface, glm::vec3 point) const {
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
        isInShadow = newRay.TraceForShadow(scene, surface, lightDistance);
        if (!isInShadow) {
            glm::vec3 normal = surface.geometry->GetNormalAtPoint(point);
            shadingResult.diffused += light.GetDiffusedHighlight(lightDirection, normal);
            shadingResult.specular += light.GetSpecularHighlight(lightDirection, normal, this->direction, surface.material.specularity);
        }
    }
    return shadingResult;
}

void Ray::print() const {
    printf("{{%.2f %.2f %.2f}, {%.2f %.2f %.2f}}\n",
           this->origin.x, this->origin.y, this->origin.z,
           this->direction.x, this->direction.y, this->direction.z);
}

