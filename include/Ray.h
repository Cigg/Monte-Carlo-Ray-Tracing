#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>

class Ray {
public:
	Ray();
	~Ray();

	glm::vec3 origin;
	glm::vec3 direction;
	float importance;
	int depth; // 0 for the ray coming from the camera

	void CalcRefractionDirection(float ratio, glm::vec3 &normal);
};

#endif // RAY_H