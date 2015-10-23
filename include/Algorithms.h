#ifndef ALOGRITHMS_H
#define ALOGRITHMS_H

#include <glm/glm.hpp>
#include <algorithm>
#include "Shape.h"
#include "Scene.h"
#include "Ray.h"

class Algorithms {
public:
	Algorithms();
	~Algorithms();

	Shape* FirstIntersection(Ray& ray, Scene *scene);
	glm::vec3 Radiance(Ray &ray, Scene *scene);

	const glm::vec3 BG_COLOR;
};

#endif // ALOGRITHMS_H