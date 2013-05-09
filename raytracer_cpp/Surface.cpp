//
//  Surface.cpp
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 04.05.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include "Surface.h"
#include "Ray.h"

Surface::Surface(Geometry *geometry, Material material) {
    this->geometry = geometry;
    this->material = material;
}

Surface::~Surface() {
//    delete this->geometry;
}