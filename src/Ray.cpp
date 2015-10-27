#include "Ray.h"
#include <cstdlib>
#include <iostream>
#include <glm/gtx/rotate_vector.hpp> 
#include <glm/gtx/quaternion.hpp>

Ray::Ray() {
	origin = glm::vec3(0.0f, 0.0f, 0.0f);
	direction = glm::vec3(0.0f, 0.0f, -1.0f);
	importance = 1.0f;
	numBounces = 0;	
}

Ray::~Ray() {
		
}

//https://en.wikipedia.org/wiki/Snell%27s_law#Vector_form
void Ray::CalcRefractionDirection(float ratio, glm::vec3 &normal) {
	float c = glm::dot(-normal,direction);
	float r = ratio;
	glm::vec3 l = direction;
	glm::vec3 n = normal;
	direction = r*l + (float)(r*c - sqrt(1.0f - r*r*(1.0f - c*c)))*n;
}