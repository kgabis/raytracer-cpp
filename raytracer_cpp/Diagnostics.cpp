//
//  Diagnostics.cpp
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 24.12.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include "Diagnostics.h"
#include <stdio.h>

unsigned int Diagnostics::triangleCount = 0;

unsigned int Diagnostics::cachedShadowHits = 0;
unsigned int Diagnostics::shadowTests = 0;

void Diagnostics::Print() {
    printf("Triangles: %u\n", triangleCount);
    printf("Shadow tests:\t\t%u\n", shadowTests);
    printf("Cached shadow hits:\t%u\n", cachedShadowHits);
    printf("Cached hits / tests ratio = %.3f%%\n",
           (float)cachedShadowHits / (float)shadowTests * 100.0f);
}