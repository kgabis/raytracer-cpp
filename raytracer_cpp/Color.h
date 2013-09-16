//
//  Color.h
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 29.04.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#ifndef __raytracer_cpp__Color__
#define __raytracer_cpp__Color__

#include <iostream>

class Color {
public:
    double r, g, b;
    
    Color();
    Color(int r, int g, int b);
    Color(double r, double g, double b) ;
    Color(unsigned char r, unsigned char g, unsigned char b);
    Color(unsigned int hex);
    
    Color Add            (Color c) const;
    Color AddWeighted    (double weightA, const Color &color, double weightB) const;
    Color Multiply       (double n) const;
    Color Blend          (const Color &colorToBlend, double weight) const;
    Color GetHighlighted (float diffused, float specular, float ambientCoeff) const;
    
    static const Color White; 
    static const Color Black; 
    static const Color Red;   
    static const Color Green; 
    static const Color Blue;
    static const Color Yellow;
};


#endif /* defined(__raytracer_cpp__Color__) */
