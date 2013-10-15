//
//  main.c
//  raytracer
//
//  Created by Krzysztof Gabis on 31.03.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//
// 17.08.2013 11:40 avg time for 2 triangles and 2 spheres: 0.04

#include <SFML/Graphics.hpp>
#include "Raytracer.h"
#include "Color.h"
#include "Ray.h"
//#include <SFML/Graphics/Sprite.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <glm/ext.hpp>

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400

void handleInput(Raytracer *rt, const sf::Event &event);

void draw(void *data, Color color, size_t x, size_t y) {
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
    sf::RenderWindow window(mode, "RayTracerCxx");
    sf::Event event;
    sf::Image screen;
    screen.create(WINDOW_WIDTH, WINDOW_HEIGHT, sf::Color::White);
    sf::Sprite sprite;
    sf::Texture texture;
    texture.loadFromImage(screen);
    sf::Clock clock;
    sf::Time time;
    sprite.setTexture(texture);
    raytracer.scene.LoadDemo();
    while (window.isOpen()) {
        clock.restart();
        screen.create(WINDOW_WIDTH, WINDOW_HEIGHT, sf::Color::White);
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else {
                handleInput(&raytracer, event);
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

void handleInput(Raytracer *rt, const sf::Event &event) {
    Camera *cam = &rt->scene.camera;
    const float moveSpeed = 1;
#define HANDLE_CAM(pressedKey, fun) if(event.key.code == sf::Keyboard::pressedKey){ cam->fun(moveSpeed); }
    HANDLE_CAM(W, moveForward)
    HANDLE_CAM(S, moveBackwards)
    HANDLE_CAM(A, moveLeft)
    HANDLE_CAM(D, moveRight)
    HANDLE_CAM(E, moveUp)
    HANDLE_CAM(C, moveDown)
    HANDLE_CAM(Up, lookUp)
    HANDLE_CAM(Down, lookDown)
    HANDLE_CAM(Left, lookLeft)
    HANDLE_CAM(Right, lookRight)    
#undef HANDLE_CAM
#define HANDLE_KEY(pressedKey, handler) if(event.key.code == sf::Keyboard::pressedKey){ handler; }
    HANDLE_KEY(T, Ray::sFogShadows = true)
    HANDLE_KEY(Y, Ray::sFogShadows = false)
#undef HANDLE_KEY
}
