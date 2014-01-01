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
#include "global.h"
#include "Diagnostics.h"

#include <glm/gtx/rotate_vector.hpp>

enum FileStatus {
    FSHeader,
    FSVertices,
    FSFaces
};

Mesh::Mesh() : _useAABB(false) { }

#define STREQ(a, b) (strcmp((a), (b)) == 0)
#define LINE_SIZE 256
void Mesh::LoadFromFile(const char *filename, Material material) {
    std::vector<glm::vec3> vertices;
    FILE *fp;
    char line[LINE_SIZE];
    double x, y, z;
    size_t vNums[3];
    glm::vec3 face[3];
    int parseStatus = FSHeader;
    int sscanfResult;
    fp = fopen(filename, "r");
    if (fp == NULL) {
        return;
    }
    while (fgets(line, LINE_SIZE, fp) != NULL) {
        switch (parseStatus) {
            case FSHeader:
                if (STREQ(line, "END_HEADER\n")) {
                    parseStatus = FSVertices;
                }
                break;
            case FSVertices:
                if (STREQ(line, "\n")) {
                    parseStatus = FSFaces;
                } else {
                    sscanfResult = sscanf(line, "%lf %lf %lf\n", &x, &y, &z);
                    if (sscanfResult == EOF) {
                        fclose(fp);
                        return;
                    }
                    vertices.push_back(glm::vec3(x, y, z));
                }
                break;
            case FSFaces:
                sscanfResult = sscanf(line, "%zu %zu %zu\n", &vNums[0], &vNums[1], &vNums[2]);
                if (sscanfResult == EOF) {
                    fclose(fp);
                    return;
                }
                face[0] = vertices[vNums[0]];
                face[1] = vertices[vNums[1]];
                face[2] = vertices[vNums[2]];
                AddTriangleWithMaterial(Triangle(face[0], face[1], face[2]), material);
                break;
            default:
                fclose(fp);
                return;
                break;
        }
    }
    fclose(fp);
//    printf("Vertices:  %zd\n", vertices.size());
//    printf("Triangles: %zd\n", _triangles.size());
    UpdateAABB();
}


void Mesh::AddCube(glm::vec3 a, glm::vec3 b, Material mat) {
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
    UpdateAABB();
}

void Mesh::AddTriangleWithMaterial(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3, Material m)
{
    AddTriangleWithMaterial(Triangle(v1, v2, v3), m);
}


void Mesh::AddTriangleWithMaterial(Triangle triangle, Material material) {
    _triangles.push_back(triangle);
    _materials.push_back(material);
    ++Diagnostics::triangleCount;
    if (_triangles.size() > 100) {
        _useAABB = true;
    }
}

bool Mesh::FindFirstIntersectionInRange(const Ray &ray, float range, const Triangle **collider) const {
    float distance = FLT_MAX;
    bool currentHit = false;
    const Triangle *t = nullptr;
    if  (!IntersectsAABB(ray, range))
        return false;
    for (size_t i = 0; i < _triangles.size(); i++) {
        t = &_triangles[i];
        float dir = glm::dot(ray.direction, t->normal);
        if (dir <= 0.0f) {
            continue;
        }
        currentHit = t->CheckIntersection(ray, &distance);
        if (currentHit && distance < range && distance > EPSILON) {
            *collider = t;
            return true;
        }
    }
    *collider = nullptr;
    return false;
}

void Mesh::FindIntersectionInRange(const Ray &ray, TracingResult *tracingResult, float range) const {
    float closestDistance = range;
    float currentDistance = 1.0 / 0.0f;
    bool currentHit = false;
    if  (!IntersectsAABB(ray, range))
        return;
    for (size_t i = 0; i < _triangles.size(); i++) {
        const Triangle *t = &_triangles[i];
        float dir = glm::dot(ray.direction, t->normal);
        if (dir <= 0.0f) {
            continue;
        }
        currentHit = t->CheckIntersection(ray, &currentDistance);
        if (currentHit && currentDistance < closestDistance && currentDistance > EPSILON) {
            closestDistance = currentDistance;
            tracingResult->hit = true;
            tracingResult->distance = closestDistance;
            tracingResult->material = &_materials[i];
            tracingResult->normal = t->GetNormal();
            tracingResult->triangle = t;
            return;
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
    if  (!IntersectsAABB(ray, FLT_MAX))
        return;
    for (size_t i = 0; i < _triangles.size(); i++) {
        const Triangle *t = &_triangles[i];
        float dir = glm::dot(ray.direction, t->normal);
        if (dir <= 0.0f) {
            continue;
        }
        currentHit = t->CheckIntersection(ray, &currentDistance);
        if (currentHit && currentDistance < closestDistance && currentDistance > EPSILON) {
            closestDistance = currentDistance;
            tracingResult->hit = true;
            tracingResult->distance = closestDistance;
            tracingResult->material = &_materials[i];
            tracingResult->normal = t->GetNormal();
            tracingResult->triangle = t;
        }
    }
}

bool Mesh::IntersectsAABB(const Ray &ray, float range) const {
    float dist;
    if (!_useAABB) {
//        return true;
    }
    if (PointInAABB(ray.origin)) {
        return true;
    }
    for (const auto &t : _aabb) {
        if (t.CheckIntersection(ray, &dist) && dist < range && dist > EPSILON) {
            return true;
        }
    }
    return false;
}

std::vector<Triangle> MakeAABB(glm::vec3 a, glm::vec3 b) {
    glm::vec3 vs[8];
    vs[0] = a;
    vs[1] = glm::vec3(b.x, a.y, a.z);
    vs[2] = glm::vec3(b.x, b.y, a.z);
    vs[3] = glm::vec3(a.x, b.y, a.z);
    vs[4] = glm::vec3(a.x, a.y, b.z);
    vs[5] = glm::vec3(b.x, a.y, b.z);
    vs[6] = b;
    vs[7] = glm::vec3(a.x, b.y, b.z);
    
    std::vector<Triangle> result;
    result.push_back(Triangle(vs[0], vs[1], vs[3]));
    result.push_back(Triangle(vs[1], vs[2], vs[3]));
    
    result.push_back(Triangle(vs[1], vs[5], vs[2]));
    result.push_back(Triangle(vs[5], vs[6], vs[2]));

    result.push_back(Triangle(vs[3], vs[2], vs[7]));
    result.push_back(Triangle(vs[2], vs[6], vs[7]));

    result.push_back(Triangle(vs[4], vs[7], vs[5]));
    result.push_back(Triangle(vs[5], vs[7], vs[6]));

    result.push_back(Triangle(vs[0], vs[3], vs[4]));
    result.push_back(Triangle(vs[4], vs[3], vs[7]));

    result.push_back(Triangle(vs[0], vs[4], vs[1]));
    result.push_back(Triangle(vs[4], vs[5], vs[1]));
    
    return result;
}

void Mesh::UpdateAABB() {
    glm::vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
    glm::vec3 max(FLT_MIN, FLT_MIN, FLT_MIN);
    glm::vec3 eps(EPSILON, EPSILON, EPSILON);
    
    for (const auto& t : _triangles) {
        glm::vec3 a = t.a;
        glm::vec3 b = t.edges[0] + t.a;
        glm::vec3 c = t.edges[1] + t.a;
#define FIND_MIN(vec, coord) if (vec.coord < min.coord) { min.coord = vec.coord; }
#define FIND_MINS(vec) FIND_MIN(vec, x); FIND_MIN(vec, y); FIND_MIN(vec, z);
        FIND_MINS(a);
        FIND_MINS(b);
        FIND_MINS(c);
#undef FIND_MIN
#undef FIND_MINS
#define FIND_MAX(vec, coord) if (vec.coord > max.coord) { max.coord = vec.coord; }
#define FIND_MAXS(vec) FIND_MAX(vec, x); FIND_MAX(vec, y); FIND_MAX(vec, z);
        FIND_MAXS(a);
        FIND_MAXS(b);
        FIND_MAXS(c);
#undef FIND_MAX
#undef FIND_MAXS
    }
    min -= eps;
    max += eps;
    _aabbMin = min;
    _aabbMax = max;
    _aabb = MakeAABB(min, max);
}

bool Mesh::PointInAABB(const glm::vec3 p) const {
    if (p.x > _aabbMax.x) return false;
    if (p.x < _aabbMin.x) return false;
    if (p.y > _aabbMax.y) return false;
    if (p.y < _aabbMin.y) return false;
    if (p.z > _aabbMax.z) return false;
    if (p.z < _aabbMin.z) return false;
    return true;
}