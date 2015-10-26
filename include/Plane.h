#ifndef PLANE_H
#define PLANE_H

#include "Shape.h"

class Plane : public Shape {
public:
	Plane();
	~Plane();

	glm::vec3 a, b, c;
	glm::vec3 calcNormal();

	// Algorithms
	bool Intersect(const Ray &ray, float &hitDist);
	glm::vec3 GetColor(glm::vec3 &pos);
	glm::vec3 GetNormal(glm::vec3 &intersection);
	glm::vec3 GetRandomPosition();
	glm::vec3 GetRandomDirection(glm::vec3 &pos);
	float GetSamplingProbability(glm::vec3 &pos);

protected:
	glm::vec3 normal;
};

#endif // PLANE_H