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

#define MAX_RECURSION_DEPTH 4
#define MAX_VISIBLE_DISTANCE 600
#define EPSILON 000001

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

Color Ray::Trace(const Scene &scene) const {
    return this->TraceRecursive(scene, MAX_RECURSION_DEPTH);
}

Ray Ray::Reflect(const glm::vec3 &normal, const glm::vec3 &collisionPoint) const {
    float c1 = -glm::dot(normal, this->direction);
    glm::vec3 reflectedDirection = this->direction + (normal * (2 * c1));
    return Ray(collisionPoint, reflectedDirection);
}

Color Ray::TraceRecursive(const Scene &scene, size_t depth) const {
    TracingResult closestHit;
    this->TraceOnce(scene, &closestHit);
    if (!closestHit.hit) {
        return scene.backgroundColor;
    }
    if (closestHit.isLightSource) {
        return Light::DefaultColor;
    }
    Color closestHitColor = closestHit.material->color;
    glm::vec3 collisionPoint = (this->direction * closestHit.distance) + this->origin;
    const Material *material = closestHit.material;
    if (material->reflectivity > 0 && depth > 0) {
        Ray reflectedRay = this->Reflect(closestHit.normal, collisionPoint);
        Color reflectionColor = reflectedRay.TraceRecursive(scene, depth - 1);
        closestHitColor = reflectionColor.Blend(closestHitColor, material->reflectivity);
    }
    ShadingResult shadingResult = this->ShadeAtPoint(scene, closestHit, collisionPoint);
    closestHitColor = closestHitColor.GetHighlighted(shadingResult.diffused, shadingResult.specular, scene.ambientCoefficient);
    closestHitColor = closestHitColor.Multiply((MAX_VISIBLE_DISTANCE - closestHit.distance) / MAX_VISIBLE_DISTANCE);
    return closestHitColor;
}

void Ray::TraceOnce(const Scene &scene, TracingResult *result) const {
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

bool Ray::TraceForShadow(const Scene &scene, float lightDistance) const {
    TracingResult currentTrace;
    for (const auto &m : scene.meshes) {
        m.FindIntersectionInRange(*this, &currentTrace, lightDistance);
        if (currentTrace.hit) {
            return true;
        }
    }
    return false;   
}

ShadingResult Ray::ShadeAtPoint(const Scene &scene, const TracingResult &tracingResult, glm::vec3 point) const {
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
        isInShadow = newRay.TraceForShadow(scene, lightDistance);
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
