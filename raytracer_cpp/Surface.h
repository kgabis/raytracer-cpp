//
//  Surface.h
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 04.05.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef __raytracer_cpp__Surface__
#define __raytracer_cpp__Surface__

#include "Triangle.h"
#include "Sphere.h"
#include "Material.h"

struct TriangleSurface {
    Triangle triangle;
    Material material;
    TriangleSurface(Triangle triangle, Material material) :
                    triangle(triangle),
                    material(material)
    {}
    
};

struct SphereSurface {
    Sphere sphere;
    Material material;
    SphereSurface(Sphere sphere, Material material) :
                    sphere(sphere),
                    material(material)
    {}
};

#endif /* defined(__raytracer_cpp__Surface__) */
