#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>

class Ray {
public:
	Ray();
	~Ray();

	glm::vec3 origin;
	glm::vec3 direction;

private:
	float importance;
	int numBounces; // 0 for the ray coming from the camera
};

#endif // RAY_H