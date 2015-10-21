#include "Scene.h"

Scene::Scene() {

}

Scene::~Scene() {
	for(int i = 0; i < objects.size(); i++) {
		delete objects[i];
	}
	objects.clear();
}

void Scene::AddObject(Shape* obj) {
	objects.push_back(obj);
}

void Scene::AddLight(Light* light) {
	lights.push_back(light);
}

glm::vec3 Scene::Radiance(Ray &ray) {
	float t = 10000.0f;
	Shape* closestShape = NULL;
	int counter = 0;
	for(std::vector<Shape*>::iterator obj = objects.begin(); obj != objects.end(); ++obj) {
		counter++;
		float tmp;
		if((*obj)->Intersect(ray, tmp)) {
			if(tmp < t) {
				t = tmp;
				closestShape = *obj;
			}
		}
	}

	glm::vec3 intersectionPos = ray.origin + t*ray.direction;

	glm::vec3 color;
	if(closestShape)
		color = closestShape->GetColor(intersectionPos);
	else 
		color = glm::vec3(0.0f);

	// Cast shadow rays here. Calculate direct illumination

	return color;
}
