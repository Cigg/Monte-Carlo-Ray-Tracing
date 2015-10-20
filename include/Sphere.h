#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"

class Sphere : public Shape {
public:
	Sphere();
	Sphere(float radius);
	~Sphere();

	bool Intersect(const Ray &ray, float &hitDist);
	glm::vec3 GetColor(glm::vec3 &pos);

protected:
	float radius;
};

#endif // SPHERE_H