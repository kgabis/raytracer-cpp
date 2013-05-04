//
//  Raytracer.h
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 04.05.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef __raytracer_cpp__Raytracer__
#define __raytracer_cpp__Raytracer__

#include <iostream>
#include "Color.h"

typedef void (*DrawFunction)(void *data, Color color, size_t x, size_t y);

class Raytracer {
public:
    //Scene scene;
    size_t resolutionX;
    size_t resolutionY;
    
    Raytracer(size_t resolutionX, size_t resolutionY);
    void Render(DrawFunction drawFunction, void *data) const;
    ~Raytracer();
};

#endif /* defined(__raytracer_cpp__Raytracer__) */
