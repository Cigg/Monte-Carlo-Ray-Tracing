#ifndef ALOGRITHMS_H
#define ALOGRITHMS_H

#include <glm/glm.hpp>
#include <vector>
#include "Shape.h"
#include "Scene.h"
#include "Ray.h"
#include "Camera.h"

class Algorithms {
public:
	Algorithms();
	~Algorithms();

	struct Intersection {
		Shape* shape;
		Ray* ray;
		glm::vec3 position;
	};

	Intersection Trace(Ray &ray, Scene *scene);
	glm::vec3 Radiance(Ray &ray, Scene *scene);
	glm::vec3 DirectIllumination(Intersection &intersection, Scene *scene);
	glm::vec3 IndirectIllumination(Intersection &intersection, Scene *scene);
	glm::vec3 CalcRandomUniformRay(glm::vec3 &normal);
	glm::vec3 CalcRandomPDFRay(glm::vec3 &normal);
};

#endif // ALOGRITHMS_H