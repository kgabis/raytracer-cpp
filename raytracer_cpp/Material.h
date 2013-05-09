//
//  Material.h
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 04.05.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef __raytracer_cpp__Material__
#define __raytracer_cpp__Material__

#include <iostream>
#include "Color.h"

class Material {
public:
    Color color;
    float reflectivity;
    float specularity;
    float reflectionNoise;
    
    Material();
    Material(Color color, float reflectivity, float reflectionNoise, float specularity);
};

#endif /* defined(__raytracer_cpp__Material__) */
