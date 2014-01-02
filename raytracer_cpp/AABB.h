//
//  AABB.h
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 01.01.2014.
//  Copyright (c) 2014 Krzysztof Gabis. All rights reserved.
//

#ifndef __raytracer_cpp__AABB__
#define __raytracer_cpp__AABB__

#include <glm/glm.hpp>
#include <vector>

#include "Triangle.h"

class AABB {
public:
    AABB();
    void MakeBoundingVolume(glm::vec3 a, glm::vec3 b);
    void Update(const std::vector<Triangle> &triangles);
    bool Intersects(const Ray &ray, float range) const;
private:
    glm::vec3 _min;
    glm::vec3 _max;
    Triangle _triangles[12];
    
    bool ContainsPoint(const glm::vec3 p) const;
};

#endif /* defined(__raytracer_cpp__AABB__) */
