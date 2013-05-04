//
//  Color.cpp
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 29.04.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include "Color.h"
#include <algorithm>

Color::Color( double r, double g, double b ) {
    this->r = std::min(r, 1.0);
    this->g = std::min(g, 1.0);
    this->b = std::min(b, 1.0);
}

Color::Color( unsigned char r, unsigned char g, unsigned char b ) {
    this->r = static_cast<double>(r) / 255.0;
    this->g = static_cast<double>(g) / 255.0;
    this->b = static_cast<double>(b) / 255.0;
}

Color::Color( unsigned int hex ) : Color( static_cast<unsigned char>(hex >> 16),
                                          static_cast<unsigned char>(hex >> 8),
                                          static_cast<unsigned char>(hex) ) { }

Color Color::Add( Color c ) const {
    Color newColor(this->r + c.r, this->g + c.g, this->b + c.b);
    return newColor;
}

Color Color::AddWeighted( double weightA, const Color *color, double weightB ) const {
    double sum = weightA + weightB;
    double r = ((this->r * weightA) + (color->r * weightB)) / sum;
    double g = ((this->g * weightA) + (color->g * weightB)) / sum;
    double b = ((this->b * weightA) + (color->b * weightB)) / sum;
    return Color( r, g, b );
                   
}

Color Color::Multiply( double n ) const {
    return Color( this->r * n, this->g * n, this->b * n);
}

Color Color::Blend( const Color *colorToBlend, double weight ) const {
    weight = std::max(0.0, std::min(1.0, weight));
    double weightB = 1.0 - weight;
    return this->AddWeighted(weight, colorToBlend, weightB);
}