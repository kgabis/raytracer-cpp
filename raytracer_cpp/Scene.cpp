//
//  Scene.cpp
//  raytracer_cpp
//
//  Created by Krzysztof Gabis on 04.05.2013.
//  Copyright (c) 2013 Krzysztof Gabis. All rights reserved.
//

#include <glm/glm.hpp>

#include "Scene.h"
#include "Material.h"
#include "Triangle.h"

Scene::Scene() : Scene(0, 0) { }

Scene::Scene(size_t cameraWidth, size_t cameraHeight) {
    this->cameraWidth = cameraWidth;
    this->cameraHeight = cameraHeight;
    this->camera = Camera(glm::vec3(0, 2, -10), glm::vec3(0, 0, 1),
                           430, cameraWidth, cameraHeight);
    this->ambientCoefficient = 0.3;
    this->groundColor = Color::Blue;
    this->skyColor = Color::White;
}

void Scene::LoadCubesDemo() {
    this->camera = Camera(glm::vec3(-10, 20, -80), glm::vec3(0, 0, 1),
                          430, cameraWidth, cameraHeight);
    Material *blueMat = new Material(Color::Blue, 0, 0, 20);
    Material *greenMat = new Material(Color::Green, 0, 0, 40);
    Material *redMat = new Material(Color::Red, 0, 0, 40);
    Material *yellowMat = new Material(Color::Yellow, 0, 0, 10);
    meshes.push_back(Mesh());
    Mesh &cube = meshes.back();
    cube.AddCube(glm::vec3(0, 0, 0), glm::vec3(25, 25, 25), *greenMat);
    cube.AddCube(glm::vec3(-60, 0, 20), glm::vec3(-35, 30, 40), *redMat);
    cube.AddCube(glm::vec3(100, 100, 100), glm::vec3(-100, 0, -150), *blueMat);
    cube.AddCube(glm::vec3(-40, 0, -20), glm::vec3(-25, 20, 0), *yellowMat);
    this->lights.push_back(Light(glm::vec3(0, 10, 90), 1));
}

void Scene::LoadTeapotDemo() {
    this->camera = Camera(glm::vec3(2, 2, -10), glm::vec3(0, 0, 1),
                          430, cameraWidth, cameraHeight);
    Material *blueMat = new Material(Color::Blue, 0, 0, 20);
    Material *yellowMat = new Material(Color::Yellow, 0.0, 0, 10);
    meshes.push_back(Mesh());
    Mesh &cube = meshes.back();
    cube.AddCube(glm::vec3(100, 100, 100), glm::vec3(-100, 0, -150), *blueMat);
    meshes.push_back(Mesh());
    Mesh &teapot = meshes.back();
    teapot.LoadFromFile("teapot.txt", *yellowMat);
    this->lights.push_back(Light(glm::vec3(80, 50, -90), 1));
}

Scene::~Scene() {
    
}
