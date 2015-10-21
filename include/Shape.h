#ifndef SHAPE_H
#define SHAPE_H

#include <glm/glm.hpp>

#include "Ray.h"

class Shape {
public:
	void SetPosition(glm::vec3 pos);
	glm::vec3 GetPosition();
	
	virtual bool Intersect(const Ray &ray, float &hitDist) = 0;
	virtual glm::vec3 GetColor(glm::vec3 &pos);
	virtual glm::vec3 GetNormal(glm::vec3 &intersection);
	virtual glm::vec3 GetRandomPosition();

	bool isLight;
	Shape() { isLight = false; }

protected:
	glm::vec3 position;
};

#endif // SHAPE_H