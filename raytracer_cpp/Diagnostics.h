//
//  Diagnostics.h
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 24.12.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef __raytracer_cpp__Diagnostics__
#define __raytracer_cpp__Diagnostics__

class Diagnostics {
public:
    static unsigned int triangleCount;
    static unsigned int shadowTests;
    static unsigned int cachedShadowHits;
    
    static void Print();
};

#endif /* defined(__raytracer_cpp__Diagnostics__) */
