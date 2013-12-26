//
//  Mesh.h
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 17.08.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef __raytracer_cpp__Mesh__
#define __raytracer_cpp__Mesh__

#include <glm/glm.hpp>
#include <vector>

#include "Triangle.h"
#include "Material.h"

class TracingResult;
class Ray;

class Mesh {
public:
    Mesh(void) = default;
    void AddCube(glm::vec3 a, glm::vec3 b, Material *mat);
    void AddTriangleWithMaterial(Triangle *triangle, Material *material);
    void AddTriangleWithMaterial(Triangle triangle, Material material);
    void AddTriangleWithMaterial(const glm::vec3 &v1,
                                 const glm::vec3 &v2,
                                 const glm::vec3 &v3,
                                 Material *m);
    void Rotate(glm::vec3 axis, float deg);
    bool FindFirstIntersectionInRange(const Ray &ray, float range, const Triangle **collider) const;
    void FindIntersectionInRange(const Ray &ray, TracingResult *tracingResult, float range) const;
    void FindIntersection(const Ray &ray, TracingResult *tracingResult) const;
    ~Mesh(void) = default;
private:
    std::vector<std::pair<Triangle*, Material*>> _triangleMaterialPairs;
    std::vector<Triangle> _triangles;
    std::vector<Material> _materials;
//    std::vector<Material> _materials;
};

#endif /* defined(__raytracer_cpp__Mesh__) */
