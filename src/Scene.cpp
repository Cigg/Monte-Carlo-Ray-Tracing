#include "Scene.h"

Scene::Scene() {
	  shapes = new std::vector<Shape*>();
    lights = new std::vector<Shape*>();
}

void Scene::addObject(shapes *ob) {
    shapes->push_back(ob);
}

void Scene::addLight(lights *lg) {
    lights->push_back(lg);
}

