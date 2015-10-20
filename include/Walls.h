#ifndef WALLS_H
#define WALLS_H

#include "Shape.h"

class Walls : public Shape {
public:
	Walls();
	~Walls();

	bool Intersect(const Ray &ray, float &hitDist);
	glm::vec3 GetColor(glm::vec3 &pos);

protected:
	float xl, xr, yl, yr, zl, zr;
};

#endif // WALLS_H