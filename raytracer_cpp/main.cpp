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
//#include <SFML/Graphics/Sprite.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <glm/ext.hpp>

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400

static void moveCamera(Camera *camera, const sf::Event &event);

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
//    glm::dvec3 dvec(1.0, 2.0, 3.0);
//    glm::vec3 fvec(1.0f, 2.0f, 3.0f);
//    glm::ivec3 ivec(1, 2, 3);
//    size_t dvecs = sizeof(dvec.x);
//    size_t fvecs = sizeof(fvec.x);
//    size_t ivecs = sizeof(ivec.x);
//    printf("%zu %zu %zu\n", dvecs, fvecs, ivecs);
//    return 0;
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
                moveCamera(&raytracer.scene.camera, event);
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

static void moveCamera(Camera *camera, const sf::Event &event) {
    const float moveSpeed = 1;
#define HANDLE_KEY(pressedKey, fun) if(event.key.code == sf::Keyboard::pressedKey){ camera->fun(moveSpeed); }
    HANDLE_KEY(W, moveForward)
    HANDLE_KEY(S, moveBackwards)
    HANDLE_KEY(A, moveLeft)
    HANDLE_KEY(D, moveRight)
    HANDLE_KEY(E, moveUp)
    HANDLE_KEY(C, moveDown)
    HANDLE_KEY(Up, lookUp)
    HANDLE_KEY(Down, lookDown)
    HANDLE_KEY(Left, lookLeft)
    HANDLE_KEY(Right, lookRight)
#undef HANDLE
}
