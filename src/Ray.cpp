#include "Ray.h"

Ray::Ray() {
	origin = glm::vec3(0.0f, 0.0f, 0.0f);
	direction = glm::vec3(0.0f, 0.0f, -1.0f);
	importance = 1.0f;
	numBounces = 0;	
}

Ray::~Ray() {
		
}