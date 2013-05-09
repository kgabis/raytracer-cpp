//
//  Surface.h
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 04.05.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef __raytracer_cpp__Surface__
#define __raytracer_cpp__Surface__

#include <iostream>
#include "Geometry.h"
#include "Material.h"

class Ray;

class Surface {
public:
    Geometry *geometry;
    Material material;
    
    Surface(Geometry *geometry, Material material);
    ~Surface();
};

#endif /* defined(__raytracer_cpp__Surface__) */
