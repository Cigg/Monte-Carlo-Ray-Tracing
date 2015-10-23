#include "Scene.h"

Scene::Scene() {
	  shapes = new std::vector<Shape*>();
    lights = new std::vector<Shape*>();
}

void Scene::addObject(Shape *sh) {
    shapes->push_back(sh);
}

void Scene::addLight(Shape *lg) {
    lights->push_back(lg);
}

