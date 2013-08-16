//
//  Scene.cpp
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 04.05.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include "Scene.h"
#include <glm/glm.hpp>

Scene::Scene() : Scene(0, 0) { }

Scene::Scene(size_t cameraWidth, size_t cameraHeight) {
    this->camera = Camera(glm::vec3(0, 0, -10), glm::vec3(0, 0, 1),
                           430, cameraWidth, cameraHeight);
    this->ambientCoefficient = 0.3;
    this->backgroundColor = Color::Blue;
}
/*void scene_loadSpheresDemo(Scene *scene) {
    Surface spheres[5];
    Surface triangles[12];
    Vector3 vs[12];
    Light lights[4];
    scene->ambientCoefficient = 0.7;
    camera_init(&scene->camera, vec3_make(0, 35, -70), vec3_make(0, 0, 1), 500.0,
                scene->camera.width, scene->camera.height);
    lights[0] = light_make(vec3_make(60, 80, -30), 1.0);
    lights[1] = light_make(vec3_make(-30, 65, -300), 0.3);
    spheres[1] = surface_initSphere(vec3_make(45, -20, 70), 20,
                                    material_make(COLOR_GREEN, 0.5, 0.05, 20.2));
    spheres[2] = surface_initSphere(vec3_make(-35, -25, 80), 15,
                                    material_make(COLOR_BLUE, 0.5, 0.02, 100));
    spheres[0] = surface_initSphere(vec3_make(10, -10, 110), 30,
                                    material_make(COLOR_RED, 0.5, 0.01, 40.0));
    spheres[3] = surface_initSphere(vec3_make(10, 40, 110), 25,
                                    material_make(COLOR_BLUE, 0.5, 0, 40));
    spheres[4] = surface_initSphere(vec3_make(10, 80, 110), 20,
                                    material_make(COLOR_GREEN, 0.5, 0, 40));
    Material sideWallMaterial1 = material_make(COLOR_WHITE, 0.0, 0, 40);
    Material sideWallMaterial2 = material_make(COLOR_WHITE, 0.0, 0, 40);
    Material ceilingMaterial = material_make(COLOR_WHITE, 0.4, 0.15, 40);
    Material floorMaterial = material_make(COLOR_WHITE, 0.4, 0.15, 40);
    vs[0] = vec3_make(-75, -40, 0);
    vs[1] = vec3_make(-75, -40, 150);
    vs[2] = vec3_make(75, -40, 0);
    vs[3] = vec3_make(75, -40, 150);
    vs[4] = vec3_make(-75, 110, 0);
    vs[5] = vec3_make(-75, 110, 150);
    vs[6] = vec3_make(75, 110, 0);
    vs[7] = vec3_make(75, 110, 150);
    vs[8] = vec3_make(-75, -40, -700);
    vs[9] = vec3_make(75, -40, -700);
    vs[10] = vec3_make(75, 110, -700);
    vs[11] = vec3_make(-75, 110, -700);
    //Floor
    triangles[0] = surface_initTriangle(vs[2], vs[1], vs[0], floorMaterial);
    triangles[1] = surface_initTriangle(vs[2], vs[3], vs[1], floorMaterial);
    //Left wall
    triangles[2] = surface_initTriangle(vs[0], vs[1], vs[4], sideWallMaterial1);
    triangles[3] = surface_initTriangle(vs[1], vs[5], vs[4], sideWallMaterial1);
    //Right wall
    triangles[4] = surface_initTriangle(vs[6], vs[3], vs[2], sideWallMaterial1);
    triangles[5] = surface_initTriangle(vs[6], vs[7], vs[3], sideWallMaterial1);
    //Ceiling
    triangles[6] = surface_initTriangle(vs[4], vs[5], vs[6], ceilingMaterial);
    triangles[7] = surface_initTriangle(vs[5], vs[7], vs[6], ceilingMaterial);
    //Back
    triangles[8] = surface_initTriangle(vs[3], vs[7], vs[5], sideWallMaterial2);
    triangles[9] = surface_initTriangle(vs[5], vs[1], vs[3], sideWallMaterial2);
    triangles[10] = surface_initTriangle(vs[8], vs[9], vs[10], sideWallMaterial1);
    triangles[11] = surface_initTriangle(vs[8], vs[10], vs[11], sideWallMaterial1);
    scene_addSurfaceRange(scene, spheres, 5);
    scene_addSurfaceRange(scene, triangles, 12);
    scene_AddLightRange(scene, lights, 2);
}*/

void Scene::LoadSpheresDemo() {
    Material materialRed(Color::Red, 0, 0, 40);
    Material materialGreen(Color::Green, 0, 0, 40);
    Sphere *redSpehre = new Sphere(glm::vec3(20, 20, 50), 10);
    Sphere *greenSpehre = new Sphere(glm::vec3(0, 0, 20), 5);
    glm::vec3 vs[10];
    vs[0] = glm::vec3(-50, -50, 70);
    vs[1] = glm::vec3(-50, 50,  70);
    vs[2] = glm::vec3(50, -50, 70);
    vs[3] = glm::vec3(50, 50, 70);
    this->surfaces.push_back(Surface(redSpehre, materialRed));
    this->surfaces.push_back(Surface(greenSpehre, materialGreen));
    this->lights.push_back(Light(camera.position, 1));
    Triangle *t1 = new Triangle(vs[2], vs[1], vs[0]);
    Triangle *t2 = new Triangle(vs[2], vs[3], vs[1]);
    this->surfaces.push_back(Surface(t1, materialRed));
    this->surfaces.push_back(Surface(t2, materialGreen));
}

Scene::~Scene() {
    
}