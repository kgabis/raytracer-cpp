//
//  Geometry.h
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 29.04.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef __raytracer_cpp__Geometry__
#define __raytracer_cpp__Geometry__

#include <iostream>
#include <glm/glm.hpp>

class Ray;

class Geometry {
public:
    virtual bool CheckIntersection(const Ray &ray, float &distance) const = 0;
    virtual glm::vec3 GetNormalAtPoint(const glm::vec3 &point) const = 0;
    virtual ~Geometry() { printf("Destroyed!\n"); }
};

class Sphere : public Geometry {
    glm::vec3 center;
    double radius;
public:
    Sphere(glm::vec3 center, double radius);
    virtual bool CheckIntersection(const Ray &ray, float &distance) const;
    virtual glm::vec3 GetNormalAtPoint(const glm::vec3 &point) const;
    virtual ~Sphere() = default;
};

class Triangle : public Geometry {
public:
    glm::vec3 a, b, c;
    glm::vec3 edges[2];
    glm::vec3 normal;
    Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c);
    virtual bool CheckIntersection(const Ray &ray, float &distance) const;
    virtual glm::vec3 GetNormalAtPoint(const glm::vec3 &point) const;
    virtual ~Triangle() = default;
};

#endif /* defined(__raytracer_cpp__Geometry__) */
