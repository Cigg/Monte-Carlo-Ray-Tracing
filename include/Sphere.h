#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"

class Sphere : public Shape {
public:
	Sphere();
	Sphere(float radius);
	~Sphere();

	bool Intersect(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection, float &hitDist);

protected:
	float radius;
};

#endif // SPHERE_H