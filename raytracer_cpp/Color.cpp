//
//  Color.cpp
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 29.04.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include "Color.h"
#include <algorithm>

Color::Color() : Color(1.0f, 0.0f, 1.0f) { }

Color::Color(int r, int g, int b) :
       Color(r / 255.0, g / 255.0, b / 255.0)
{ }

Color::Color(float r, float g, float b) {
    this->r = std::min(r, 1.0f);
    this->g = std::min(g, 1.0f);
    this->b = std::min(b, 1.0f);
}

Color::Color(double r, double g, double b) {
    this->r = std::min(r, 1.0);
    this->g = std::min(g, 1.0);
    this->b = std::min(b, 1.0);
}

Color::Color(unsigned char r, unsigned char g, unsigned char b) {
    this->r = std::min((float)r / 255.0f, 1.0f);
    this->g = std::min((float)g / 255.0f, 1.0f);
    this->b = std::min((float)b / 255.0f, 1.0f);
}

Color::Color(unsigned int hex) : Color(static_cast<unsigned char>(hex >> 16),
                                          static_cast<unsigned char>(hex >> 8),
                                          static_cast<unsigned char>(hex)) { }

Color Color::Add(Color c) const {
    Color newColor(this->r + c.r, this->g + c.g, this->b + c.b);
    return newColor;
}

Color Color::AddWeighted(double weightA, const Color &color, double weightB) const {
    double sum = weightA + weightB;
    double r = ((this->r * weightA) + (color.r * weightB)) / sum;
    double g = ((this->g * weightA) + (color.g * weightB)) / sum;
    double b = ((this->b * weightA) + (color.b * weightB)) / sum;
    return Color(r, g, b);
                   
}

Color Color::Multiply(double n) const {
    return Color(this->r * n, this->g * n, this->b * n);
}

Color Color::Blend(const Color &colorToBlend, double weight) const {
    weight = std::max(0.0, std::min(1.0, weight));
    double weightB = 1.0 - weight;
    return this->AddWeighted(weight, colorToBlend, weightB);
}

Color Color::GetHighlighted (float diffused, float specular, float ambientCoeff, float d, float maxD) const {
    float diffusedCoeff = 1.0 - ambientCoeff;
    float atten = std::max(1.0f - ((d / maxD) * (d / maxD)), 0.0f);
    Color color = this->Multiply(ambientCoeff + diffused * diffusedCoeff * atten);
    color = color.Add(Color::White.Multiply(specular * atten));
    return color;
}

const Color Color::White  = Color(1.0, 1.0, 1.0);
const Color Color::Black  = Color(0.0, 0.0, 0.0);
const Color Color::Red    = Color(189, 42, 51);
const Color Color::Green  = Color(64, 129, 86);
const Color Color::Blue   = Color(48, 55, 79);
const Color Color::Yellow = Color(214, 170, 38);

