//
//  Ray.cpp
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 29.04.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include "Ray.h"
#include "Surface.h"
#include "Camera.h"
#include "Scene.h"

#define MAX_RECURSION_DEPTH 4
#define MAX_VISIBLE_DISTANCE 600.0
#define EPSILON 0.000001

Ray::Ray() {
    this->origin = glm::vec3( 0.0f, 0.0f, 0.0f );
    this->direction = glm::vec3( 0.0f, 0.0f, 0.0f );
}

Ray::Ray(glm::vec3 origin, glm::vec3 direction) {
    this->origin = origin;
    this->direction = direction;
}

Ray::Ray(const Camera &camera, size_t x, size_t y) {
    float dy = 1.0;
    float dx = 1.0;
    float py = (- camera.height / 2.0) + dy * ((float)y + 0.5);
    float px = (- camera.width / 2.0) + dx * ((float)x + 0.5);
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
        return closestHit.color;
    }
    glm::vec3 collisionPoint = (this->direction * closestHit.distance) + this->origin;
    const Material *material = &closestHit.surface->material;
    if (material->reflectivity > 0.0 && depth > 0) {
        Ray reflectedRay = this->Reflect(*closestHit.surface, collisionPoint);
        Color reflectionColor = reflectedRay.TraceRecursive(scene, depth - 1);
        closestHit.color = reflectionColor.Blend(closestHit.color, material->reflectivity);
    }
    ShadingResult shadingResult = this->ShadeAtPoint(scene, *closestHit.surface, collisionPoint);
    closestHit.color = closestHit.color.GetHighlighted(shadingResult.diffused, shadingResult.specular, scene.ambientCoefficient);
    closestHit.color = closestHit.color.Multiply((MAX_VISIBLE_DISTANCE - closestHit.distance) / MAX_VISIBLE_DISTANCE);
    return closestHit.color;    
}

TracingResult Ray::TraceOnce(const Scene &scene) const {
    TracingResult closestHit;
    closestHit.hit = 0;
    closestHit.color = scene.backgroundColor;
    closestHit.distance = 1.0 / 0.0; // infinity
    bool isHit;
    float distance = 0.0f;
    for( auto &s : scene.surfaces ) {
        Geometry *geometry = s.geometry;
        Ray ray(this->origin, this->direction);
        isHit = geometry->CheckIntersection(ray, distance);
        if (isHit && distance < closestHit.distance) {
            closestHit.hit = true;
            closestHit.distance = distance;
            closestHit.surface = &s;
            closestHit.color = s.material.color;
        }
    }
    return closestHit;
}

ShadingResult Ray::ShadeAtPoint( const Scene &scene, const Surface &surface, glm::vec3 point ) const {
    ShadingResult shadingResult = { .diffused = 0.0f, .specular = 0.0f };
    float lightDistance;
    glm::vec3 lightDirection;
    Ray newRay;
    TracingResult shadowTracingResult;
    for ( auto &light : scene.lights ) {
        lightDirection = light.GetDirectionAtPoint(point);
        newRay.direction = -lightDirection;
        lightDistance = glm::length(light.position - point);
        shadowTracingResult = newRay.TraceOnce(scene);
        if ( !shadowTracingResult.hit || shadowTracingResult.distance > lightDistance ) {
            glm::vec3 normal = surface.geometry->GetNormalAtPoint(point);
            shadingResult.diffused += light.GetDiffusedHighlight(lightDirection, normal);
            shadingResult.specular += light.GetSpecularHighlight(lightDirection, normal, this->direction, surface.material.specularity);
        }
    }
    return shadingResult;
}
