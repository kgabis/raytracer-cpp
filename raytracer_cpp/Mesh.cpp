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

#define EPSILON 0.00001 // this value does nothing, overlapping objects look ugly

void Mesh::AddCube(glm::vec3 a, glm::vec3 b, Material *mat) {
    glm::vec3 vs[8];
    vs[0] = a;
    vs[1] = glm::vec3(b.x, a.y, a.z);
    vs[2] = glm::vec3(b.x, b.y, a.z);
    vs[3] = glm::vec3(a.x, b.y, a.z);
    vs[4] = glm::vec3(a.x, a.y, b.z);
    vs[5] = glm::vec3(b.x, a.y, b.z);
    vs[6] = b;
    vs[7] = glm::vec3(a.x, b.y, b.z);
    
    AddTriangleWithMaterial(vs[0], vs[1], vs[3], mat);
    AddTriangleWithMaterial(vs[1], vs[2], vs[3], mat);
    
    AddTriangleWithMaterial(vs[1], vs[5], vs[2], mat);
    AddTriangleWithMaterial(vs[5], vs[6], vs[2], mat);
    
    AddTriangleWithMaterial(vs[3], vs[2], vs[7], mat);
    AddTriangleWithMaterial(vs[2], vs[6], vs[7], mat);
    
    AddTriangleWithMaterial(vs[4], vs[7], vs[5], mat);
    AddTriangleWithMaterial(vs[5], vs[7], vs[6], mat);
    
    AddTriangleWithMaterial(vs[0], vs[3], vs[4], mat);
    AddTriangleWithMaterial(vs[4], vs[3], vs[7], mat);
    
    AddTriangleWithMaterial(vs[0], vs[4], vs[1], mat);
    AddTriangleWithMaterial(vs[4], vs[5], vs[1], mat);
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
        if (currentHit && currentDistance < closestDistance && currentDistance > EPSILON) {
            closestDistance = currentDistance;
            tracingResult->hit = true;
            tracingResult->distance = closestDistance;
            tracingResult->material = m;
            tracingResult->normal = t->normal;
        }
    }
}

void Mesh::Rotate(glm::vec3 axis, float deg) {
//    for (auto & tmp : _triangleMaterialPairs) {
//        auto &t = tmp.first;
//        t->a = glm::rotate(t->a, deg, axis);
//        t->b = glm::rotate(t->b, deg, axis);
//        t->c = glm::rotate(t->c, deg, axis);
//    }
}

void Mesh::FindIntersection(const Ray &ray, TracingResult *tracingResult) const {
    float closestDistance = 1.0f / 0.0f;
    float currentDistance = 1.0 / 0.0f;
    bool currentHit = false;
    for (const auto &tmp : _triangleMaterialPairs) {
        const Triangle *t = tmp.first;
        const Material *m = tmp.second;
        currentHit = t->CheckIntersection(ray, &currentDistance);
        if (currentHit && currentDistance < closestDistance && currentDistance > EPSILON) {
            closestDistance = currentDistance;
            tracingResult->hit = true;
            tracingResult->distance = closestDistance;
            tracingResult->material = m;
            tracingResult->normal = t->normal;
        }
    }
}