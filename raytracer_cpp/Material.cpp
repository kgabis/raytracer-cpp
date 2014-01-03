//
//  Material.cpp
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 04.05.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include "Material.h"

Material::Material() : Material(Color::White, 0.0f, 0.0f, 0.0f) { }

Material::Material(Color color, float reflectivity, float reflectionNoise, float specularity) {
    this->color = color;
    this->reflectivity = reflectivity;
    this->reflectionNoise = reflectionNoise;
    this->specularity = specularity;
}