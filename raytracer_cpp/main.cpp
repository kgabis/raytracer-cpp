//
//  main.c
//  raytracer
//
//  Created by Krzysztof Gabis on 31.03.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//
// 17.08.2013 11:40 avg time for 2 triangles and 2 spheres: 0.04
// 26.12.2013 15:05 avg time for 4 boxes: 0.55s or 3s with shadows


#include <SFML/Graphics.hpp>
#include "Raytracer.h"
#include "Color.h"
#include "Ray.h"
#include "Diagnostics.h"
//#include <SFML/Graphics/Sprite.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <glm/ext.hpp>

#include "global.h"

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
    srand((unsigned int)time(NULL));
    Raytracer raytracer(WIDTH, HEIGHT);
    sf::VideoMode mode(WIDTH, HEIGHT, 32);
    sf::IntRect bounds(0, 0, WIDTH, HEIGHT);
    sf::RenderWindow window(mode, "RayTracerCxx");
    sf::Event event;
    sf::Image screen;
    screen.create(WIDTH, HEIGHT, sf::Color::White);
    sf::Sprite sprite;
    sf::Texture texture;
    texture.loadFromImage(screen);
    sf::Clock clock;
    sf::Time time;
    sf::Text text;
    text.setPosition(5, 5);
    text.setCharacterSize(14);
    sprite.setTexture(texture);
    raytracer.scene.LoadDemo();
    while (window.isOpen()) {
        clock.restart();
        screen.create(WIDTH, HEIGHT, sf::Color::White);
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else {
                handleInput(&raytracer, event);
            }            
        }
        raytracer.Render(draw, &screen);
        texture.loadFromImage(screen);
        texture.setSmooth(true);
        window.clear(sf::Color::White);
        
        texture.bind();
        window.draw(sprite);
        window.draw(text);
        window.display();
        
        time = clock.getElapsedTime();
        printf("Seconds per frame: %f\n", time.asSeconds());
    }
    printf("Shadow tests:\t\t%u\n", Diagnostics::shadowTests);
    printf("Cached shadow hits:\t%u\n", Diagnostics::cachedShadowHits);
    printf("Cached hits / tests ratio = %.3f%%\n",
           (float)Diagnostics::cachedShadowHits / (float)Diagnostics::shadowTests * 100.0f);
}

void handleInput(Raytracer *rt, const sf::Event &event) {
    Camera *cam = &rt->scene.camera;
    const float moveSpeed = 1;
#define HANDLE_CAM(pressedKey, fun) if(event.key.code == sf::Keyboard::pressedKey){ rt->needsUpdate(true);cam->fun(moveSpeed); }
    HANDLE_CAM(W, moveForward)
    HANDLE_CAM(S, moveBackwards)
    HANDLE_CAM(A, moveLeft)
    HANDLE_CAM(D, moveRight)
    HANDLE_CAM(Q, moveUp)
    HANDLE_CAM(E, moveDown)
    HANDLE_CAM(Up, lookUp)
    HANDLE_CAM(Down, lookDown)
    HANDLE_CAM(Left, lookLeft)
    HANDLE_CAM(Right, lookRight)    
#undef HANDLE_CAM
#define HANDLE_KEY(pressedKey, handler) if(event.key.code == sf::Keyboard::pressedKey){ rt->needsUpdate(true); handler; }
    HANDLE_KEY(T, Ray::sFogShadows = true)
    HANDLE_KEY(Y, Ray::sFogShadows = false)
    HANDLE_KEY(G, Raytracer::sRandTresh *= 0.95)
    HANDLE_KEY(H, Raytracer::sRandTresh *= 1.05)
    
    HANDLE_KEY(J, rt->scene.lights[0].position.x -= moveSpeed)
    HANDLE_KEY(L, rt->scene.lights[0].position.x += moveSpeed)
    HANDLE_KEY(K, rt->scene.lights[0].position.z -= moveSpeed)
    HANDLE_KEY(I, rt->scene.lights[0].position.z += moveSpeed)
    HANDLE_KEY(U, rt->scene.lights[0].position.y += moveSpeed)
    HANDLE_KEY(O, rt->scene.lights[0].position.y -= moveSpeed)

#undef HANDLE_KEY
}
