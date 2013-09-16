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
#include <glm/gtx/rotate_vector.hpp>

void Mesh::AddCube(glm::vec3 a, glm::vec3 b) {
    glm::vec3 vs[8];
    vs[0] = a;
    vs[1] = glm::vec3(b.x, a.y, a.z);
    vs[2] = glm::vec3(b.x, b.y, a.z);
    vs[3] = glm::vec3(a.x, b.y, a.z);
    vs[4] = glm::vec3(a.x, a.y, b.z);
    vs[5] = glm::vec3(b.x, a.y, b.z);
    vs[6] = b;
    vs[7] = glm::vec3(a.x, b.y, b.z);
    Material *redMat = new Material(Color::Red, 0, 0, 40);
    Material *greenMat = new Material(Color::Green, 0, 0, 40);
    
    AddTriangleWithMaterial(vs[0], vs[1], vs[3], redMat);
    AddTriangleWithMaterial(vs[1], vs[2], vs[3], greenMat);
    
    AddTriangleWithMaterial(vs[1], vs[5], vs[2], redMat);
    AddTriangleWithMaterial(vs[5], vs[6], vs[2], greenMat);
    
    AddTriangleWithMaterial(vs[3], vs[2], vs[7], redMat);
    AddTriangleWithMaterial(vs[2], vs[6], vs[7], greenMat);
    
    AddTriangleWithMaterial(vs[4], vs[7], vs[5], redMat);
    AddTriangleWithMaterial(vs[5], vs[7], vs[6], greenMat);
    
    AddTriangleWithMaterial(vs[0], vs[3], vs[4], redMat);
    AddTriangleWithMaterial(vs[4], vs[3], vs[7], greenMat);
    
    AddTriangleWithMaterial(vs[0], vs[4], vs[1], redMat);
    AddTriangleWithMaterial(vs[4], vs[5], vs[1], greenMat);
}

void Mesh::AddTriangleWithMaterial(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3, Material *m)
{
    AddTriangleWithMaterial(new Triangle(v1, v2, v3), m);
}

void Mesh::AddTriangleWithMaterial(Triangle *triangle, Material *material) {
    _triangleMaterialPairs.push_back(std::make_pair(triangle, material));
}

void Mesh::FindIntersectionInRange(const Ray &ray, TracingResult *tracingResult, float range) const {
    float closestDistance = range;
    float currentDistance = 1.0 / 0.0f;
    bool currentHit = false;
    for (const auto &tmp : _triangleMaterialPairs) {
        const Triangle *t = tmp.first;
        const Material *m = tmp.second;
        currentHit = t->CheckIntersection(ray, &currentDistance);
        if (currentHit && currentDistance < closestDistance && currentDistance > 0) {
            closestDistance = currentDistance;
            tracingResult->hit = true;
            tracingResult->distance = closestDistance;
            tracingResult->material = m;
            tracingResult->normal = t->normal;
        }
    }
}

void Mesh::Rotate(glm::vec3 axis, float deg) {
// todo: implementation
}

void Mesh::FindIntersection(const Ray &ray, TracingResult *tracingResult) const {
    float closestDistance = 1.0f / 0.0f;
    float currentDistance = 1.0 / 0.0f;
    bool currentHit = false;
    for (const auto &tmp : _triangleMaterialPairs) {
        const Triangle *t = tmp.first;
        const Material *m = tmp.second;
        currentHit = t->CheckIntersection(ray, &currentDistance);
        if (currentHit && currentDistance < closestDistance && currentDistance > 0) {
            closestDistance = currentDistance;
            tracingResult->hit = true;
            tracingResult->distance = closestDistance;
            tracingResult->material = m;
            tracingResult->normal = t->normal;
        }
    }
}