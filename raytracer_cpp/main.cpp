//
//  main.c
//  raytracer
//
//  Created by Krzysztof Gabis on 31.03.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include <SFML/Graphics.hpp>
#include "Raytracer.h"
#include "Color.h"
//#include <SFML/Graphics/Sprite.h>
#include <stdio.h>
#include <string.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

static void draw(void *data, Color color, size_t x, size_t y) {
    sf::Image *screen = (sf::Image*)data;
    sf::Color sfcolor;
    sfcolor.r = (sf::Uint8)(color.r * 255.0);
    sfcolor.g = (sf::Uint8)(color.g * 255.0);
    sfcolor.b = (sf::Uint8)(color.b * 255.0);
    sfcolor.a = 255;
    screen->setPixel((unsigned int)x, (unsigned int)y, sfcolor);
}

int main() {
    Raytracer raytracer(WINDOW_WIDTH, WINDOW_HEIGHT);
    sf::VideoMode mode(WINDOW_WIDTH, WINDOW_HEIGHT, 32);
    sf::IntRect bounds(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    sf::RenderWindow window(mode, "raytracer");
    sf::Event event;
    sf::Image screen;
    screen.create(WINDOW_WIDTH, WINDOW_HEIGHT, sf::Color::White);
    sf::Sprite sprite;
    sf::Texture texture;
    texture.loadFromImage(screen);
    sf::Clock clock;
    sf::Time time;
    sprite.setTexture(texture);
    raytracer.scene.LoadSpheresDemo();
    while (window.isOpen()) {
        clock.restart();
        screen.create(WINDOW_WIDTH, WINDOW_HEIGHT, sf::Color::White);
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        raytracer.Render(draw, &screen);
        texture.loadFromImage(screen);
        window.clear(sf::Color::White);
        texture.bind();
        window.draw(sprite);
        window.display();
        
        time = clock.getElapsedTime();
        printf("Seconds per frame: %f\n", time.asSeconds());
    }
}
