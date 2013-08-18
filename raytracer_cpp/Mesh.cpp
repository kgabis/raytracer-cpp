//
//  Mesh.cpp
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 17.08.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include "Mesh.h"
#include "TracingResult.h"
#include "Color.h"

Mesh::Mesh(void) {
    _materials.push_back(Material(Color::Red, 0, 0, 40));
}

void AddCube(glm::vec3 a, glm::vec3 b, const Material &material) {
    
}

void Mesh::AddTriangleWithMaterial(const Triangle &triangle, const Material &material) {
    _materials.push_back(material);
    _triangleMaterialPairs.push_back(std::make_pair(triangle, &_materials.back()));
}

void Mesh::FindIntersectionInRange(const Ray &ray, TracingResult *tracingResult, float range) const {
    float closestDistance = range;
    float currentDistance = 1.0 / 0.0f;
    bool currentHit = false;
    for (const auto &tmp : _triangleMaterialPairs) {
        const Triangle &t = tmp.first;
        const Material *m = tmp.second;
        currentHit = t.CheckIntersection(ray, &currentDistance);
        if (currentHit && currentDistance < closestDistance) {
            closestDistance = currentDistance;
            tracingResult->hit = true;
            tracingResult->distance = closestDistance;
            tracingResult->material = m;
            tracingResult->normal = t.normal;
        }
    }
}

void Mesh::FindIntersection(const Ray &ray, TracingResult *tracingResult) const {
    float closestDistance = 1.0f / 0.0f;
    float currentDistance = 1.0 / 0.0f;
    bool currentHit = false;
    for (const auto &tmp : _triangleMaterialPairs) {
        const Triangle &t = tmp.first;
        const Material *m = tmp.second;
        currentHit = t.CheckIntersection(ray, &currentDistance);
        if (currentHit && currentDistance < closestDistance) {
            closestDistance = currentDistance;
            tracingResult->hit = true;
            tracingResult->distance = closestDistance;
            tracingResult->material = m;
            tracingResult->normal = t.normal;
        }
    }
}