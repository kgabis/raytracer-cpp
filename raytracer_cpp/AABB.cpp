//
//  AABB.cpp
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 01.01.2014.
//  Copyright (c) 2014 Krzysztof Gabis. All rights reserved.
//

#include "AABB.h"

AABB::AABB() { }

bool AABB::Intersects(const Ray &ray, float range) const {
    float dist;
    if (ContainsPoint(ray.origin)) {
        return true;
    }
    for (const auto &t : _triangles) {
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

void AABB::Update(const std::vector<Triangle> &triangles) {
    glm::vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
    glm::vec3 max(FLT_MIN, FLT_MIN, FLT_MIN);
    glm::vec3 eps(EPSILON, EPSILON, EPSILON);
    
    for (const auto& t : triangles) {
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
    _min = min;
    _max = max;
    _triangles = MakeAABB(min, max);
}

bool AABB::ContainsPoint(const glm::vec3 p) const {
    if (p.x > _max.x) return false;
    if (p.x < _min.x) return false;
    if (p.y > _max.y) return false;
    if (p.y < _min.y) return false;
    if (p.z > _max.z) return false;
    if (p.z < _min.z) return false;
    return true;
}