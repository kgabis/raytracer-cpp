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
    Mesh(void);
    void AddCube(glm::vec3 a, glm::vec3 b, const Material &material);
    void AddTriangleWithMaterial(const Triangle &triangle, const Material &material);
    void FindIntersectionInRange(const Ray &ray, TracingResult *tracingResult, float range) const;
    void FindIntersection(const Ray &ray, TracingResult *tracingResult) const;
    ~Mesh(void) = default;
private:
    std::vector<std::pair<Triangle, const Material *>> _triangleMaterialPairs;
    std::vector<Material> _materials;
};

#endif /* defined(__raytracer_cpp__Mesh__) */
