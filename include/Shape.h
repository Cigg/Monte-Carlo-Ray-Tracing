#ifndef SHAPE_H
#define SHAPE_H

#include <glm/glm.hpp>

class Shape {
public:
	virtual bool Intersect(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection, float &hitDist) = 0;
	void SetPosition(glm::vec3 pos);
	glm::vec3 GetPosition();

protected:
	glm::vec3 position;
};

#endif // SHAPE_H