//
//  main.c
//  raytracer
//
//  Created by Krzysztof Gabis on 31.03.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//
// 17.08.2013 11:40 avg time for 2 triangles and 2 spheres: 0.04
// 26.12.2013 15:05 avg time for 4 boxes: 0.55s or 3.00s with shadows
// 26.12.2013 16:51 avg time for 4 boxes: 0.51s or 2.80s with shadows
// 27.12.2013 22:19 avg time for 4 boxes: 0.49s or 2.70s with shadows
// 29.12.2013 10:36 avg time for 4 boxes: 0.47s or 2.55s with shadows
// 29.12.2013 11:22 avg time for 4 boxes: 0.46s or 2.50s with shadows
// 29.12.2013 11:29 avg time for 4 boxes: 0.37s or 2.01s with shadows
// 29.12.2013 11:29 avg time for 4 boxes: 0.34s or 2.00s with shadows

#ifdef __APPLE__
#include <GLUT/glut.h>          /* Open GL Util    APPLE */
#else
#include <GL/glut.h>            /* Open GL Util    OpenGL*/
#endif

#include <stdio.h>
#include <string.h>
#include <glm/ext.hpp>
#include <stdlib.h>

#include "Raytracer.h"
#include "Color.h"
#include "Ray.h"
#include "Diagnostics.h"
#include "global.h"

#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

void drawHandler(void);
void keyboardHandler(unsigned char key, int x, int y);
void arrowsHandler(int key, int x, int y);
void glInit(void);
void closeRaytracer(int code);

static char gBuffer[WIDTH * HEIGHT * 3];
static Raytracer gRaytracer(WIDTH, HEIGHT);

int main(int argc, char **argv) {
    srand((unsigned int)time(NULL));
    gRaytracer.scene.LoadTeapotDemo();

    glutInitWindowSize(WIDTH, HEIGHT);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutCreateWindow("Raytracer (C++ edition) - Author: Krzysztof Gabis (kgabis@gmail.com)");
    
    glInit();
    glutDisplayFunc(drawHandler);
    glutSpecialFunc(arrowsHandler);
    glutKeyboardFunc(keyboardHandler);
    
    glutMainLoop();
    return 1;
}

void setPixel(char *buffer, int x, int y, int r, int g, int b) {
    y = HEIGHT - y - 1; // upside-down hack
    buffer[(y * WIDTH + x) * 3 + 0] = r;
    buffer[(y * WIDTH + x) * 3 + 1] = g;
    buffer[(y * WIDTH + x) * 3 + 2] = b;
    
}

void draw(void *data, Color color, size_t x, size_t y) {
    char *buf = (char*)data;
    int r = (int)(color.r * 255.0);
    int g = (int)(color.g * 255.0);
    int b = (int)(color.b * 255.0);
    setPixel(buf, x, y, r, g, b);
}

void drawHandler(void) {
    int timeBeforeRender = glutGet(GLUT_ELAPSED_TIME);
    gRaytracer.Render(draw, gBuffer);
    int timeAfterRender = glutGet(GLUT_ELAPSED_TIME);
    printf("Seconds per frame: %.3f\n", (float)(timeAfterRender - timeBeforeRender) / 1000.0f);
    glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, gBuffer);
    glutSwapBuffers();
    glutPostRedisplay();
}

#define HANDLE_CAM(pressedKey, fun) if(key == (int)pressedKey){ gRaytracer.needsUpdate(true); cam->fun(moveSpeed); }
#define HANDLE_KEY(pressedKey, handler) if(key == (int)pressedKey){ gRaytracer.needsUpdate(true); handler; }

void keyboardHandler(unsigned char key, int x, int y) {
    Camera *cam = &gRaytracer.scene.camera;
    const float moveSpeed = 1;
    
    HANDLE_CAM('w', moveForward)
    HANDLE_CAM('s', moveBackwards)
    HANDLE_CAM('a', moveLeft)
    HANDLE_CAM('d', moveRight)
    HANDLE_CAM('q', moveUp)
    HANDLE_CAM('e', moveDown)
    
    HANDLE_KEY('t', Raytracer::sFogShadows = true)
    HANDLE_KEY('y', Raytracer::sFogShadows = false)
    HANDLE_KEY('g', Raytracer::sRandTresh *= 0.95)
    HANDLE_KEY('h', Raytracer::sRandTresh *= 1.05)
    
    HANDLE_KEY('j', gRaytracer.scene.lights[0].position.x -= moveSpeed)
    HANDLE_KEY('l', gRaytracer.scene.lights[0].position.x += moveSpeed)
    HANDLE_KEY('k', gRaytracer.scene.lights[0].position.z -= moveSpeed)
    HANDLE_KEY('i', gRaytracer.scene.lights[0].position.z += moveSpeed)
    HANDLE_KEY('u', gRaytracer.scene.lights[0].position.y += moveSpeed)
    HANDLE_KEY('o', gRaytracer.scene.lights[0].position.y -= moveSpeed)
    HANDLE_KEY(27, closeRaytracer(0)); // ESC
}

void arrowsHandler(int key, int x, int y) {
    Camera *cam = &gRaytracer.scene.camera;
    const float moveSpeed = 1;
    
    HANDLE_CAM(GLUT_KEY_UP, lookUp)
    HANDLE_CAM(GLUT_KEY_DOWN, lookDown)
    HANDLE_CAM(GLUT_KEY_LEFT, lookLeft)
    HANDLE_CAM(GLUT_KEY_RIGHT, lookRight)
}

#undef HANDLE_KEY
#undef HANDLE_CAM

void glInit() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void closeRaytracer(int code) {
    Diagnostics::Print();
    exit(code);
}

