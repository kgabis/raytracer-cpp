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
    this->camera = Camera(glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(0.0f, 0.0f, 1.0f),
                           430, cameraWidth, cameraHeight);
    this->ambientCoefficient = 0.3;
    this->backgroundColor = Color::Blue;
}
//
//void Scene::LoadSpheresDemo() {
//    Material materialRed(Color::Red, 0, 0, 40);
//    Material materialGreen(Color::Green, 0, 0, 40);
//    Sphere redSphere(glm::vec3(20, 20, 50), 10);
//    Sphere greenSphere(glm::vec3(0, 0, 20), 5);
//    glm::vec3 vs[10];
//    vs[0] = glm::vec3(-50, -50, 70);
//    vs[1] = glm::vec3(-50, 50,  70);
//    vs[2] = glm::vec3(50, -50, 70);
//    vs[3] = glm::vec3(50, 50, 70);
//    this->spheres.push_back(SphereSurface(redSphere, materialRed));
//    this->spheres.push_back(SphereSurface(greenSphere, materialGreen));
//    this->lights.push_back(Light(camera.position, 1));
//    Triangle t1(vs[2], vs[1], vs[0]);
//    Triangle t2(vs[2], vs[3], vs[1]);
//    this->triangles.push_back(TriangleSurface(t1, materialRed));
//    this->triangles.push_back(TriangleSurface(t2, materialGreen));
//}

void Scene::LoadDemo(void) {
    glm::vec3 vs[10];
    vs[0] = glm::vec3(-50, -50, 70);
    vs[1] = glm::vec3(-50, 50,  70);
    vs[2] = glm::vec3(50, -50, 70);
    vs[3] = glm::vec3(50, 50, 70);
    meshes.push_back(Mesh());
    Mesh &cubeMesh = meshes.back();
    cubeMesh.SetDefaultMaterial(Material(Color::Red, 0, 0, 40));
    cubeMesh.AddTriangleWithMaterial(Triangle(vs[2], vs[1], vs[0]), Material(Color::Red, 0, 0, 40));
    cubeMesh.AddTriangleWithMaterial(Triangle(vs[2], vs[3], vs[1]), Material(Color::Green, 0, 0, 40));
    this->lights.push_back(Light(camera.position, 1));
}

Scene::~Scene() {
    
}