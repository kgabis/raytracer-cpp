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
    this->camera = Camera(glm::vec3(0.0f, 0.0f, -100.0f), glm::vec3(0.0f, 0.0f, 1.0f),
                           430, cameraWidth, cameraHeight);
    this->ambientCoefficient = 0.3;
    this->backgroundColor = Color::Blue;
}

void Scene::LoadDemo(void) { 
    meshes.push_back(Mesh());
    Mesh &cube = meshes.back();
    cube.AddCube(glm::vec3(0, 0, 0), glm::vec3(20, 20, 20));
    cube.AddCube(glm::vec3(-20, -20, -20), glm::vec3(0, 0, 0));
    this->lights.push_back(Light(glm::vec3(0, 200, -100), 3));
//    this->lights.push_back(Light(glm::vec3(0, -200, -100), 1));
//    this->lights.push_back(Light(glm::vec3(-200, 0, -100), 1));
//    this->lights.push_back(Light(glm::vec3(200, 0, -100), 1));
}

void Scene::test_RotateCube(void) {
    //todo: implementation
}

Scene::~Scene() {
    
}