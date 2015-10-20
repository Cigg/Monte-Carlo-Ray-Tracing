#ifndef WALLS_H
#define WALLS_H

#include "Shape.h"

class Walls : public Shape {
public:
	Walls();
	~Walls();

	bool Intersect(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection, float &hitDist, glm::vec3 &color);

protected:
	float xl, xr, yl, yr, zl, zr;
};

#endif // WALLS_H