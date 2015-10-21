#ifndef SCENE_H
#define SCENE_H

#include <glm/glm.hpp>
#include <vector>

#include "Ray.h"
#include "Shape.h"
#include "Light.h"

class Scene {
public:
	Scene();
	~Scene();

	void AddObject(Shape* obj);
	void AddLight(Light* light);
	glm::vec3 Radiance(Ray &ray);

protected:
	std::vector<Shape*> objects;
	std::vector<Light*> lights;
};

#endif // SCENE_H