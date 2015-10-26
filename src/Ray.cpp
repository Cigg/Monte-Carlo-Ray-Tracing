#include "Ray.h"
#include <cstdlib>
#include <iostream>

Ray::Ray() {
	origin = glm::vec3(0.0f, 0.0f, 0.0f);
	direction = glm::vec3(0.0f, 0.0f, -1.0f);
	importance = 1.0f;
	numBounces = 0;	
}

Ray::~Ray() {
		
}

void Ray::CalcRandomDirection(glm::vec3 &normal) {
	float u = (float)rand()/RAND_MAX;
    float v = (float)rand()/RAND_MAX;

    float x = cos(2*M_PI*u)*sqrt(1 - v);
    float y = sin(2*M_PI*u)*sqrt(1 - v);
    float z = sqrt(v);

    glm::vec3 direction = glm::normalize(glm::vec3(x, y, z));

    // Orthonormal coordinate frame
    glm::vec3 sw = normal;
    glm::vec3 su = glm::normalize(glm::cross((glm::abs(sw.x) > 0.1 ? glm::vec3(0, 1, 0) : glm::vec3(1, 0, 0)), sw));
    glm::vec3 sv = glm::cross(sw, su);

    glm::mat3 rot = glm::mat3(sv, sw, su);

    direction = direction*rot;

	// //alternative
	// //bad random generator? should use modern method?
	// float u = (float)rand()/RAND_MAX;
	// float v = (float)rand()/RAND_MAX;

	// //evenly distributed
	// float theta = 2*M_PI*u;
	// float cosphi = 2*v - 1;
	// float phi = acos(cosphi);

	// float x = cos(theta) * sin(phi);
	// float y = sin(theta) * sin(phi);
	// float z = cosphi;
	// direction = glm::vec3(x,y,z);
	
	// if(glm::dot(normal, direction) < 0.0f) {
	// 	direction *= -1.0f;
	// }
	
}

//https://en.wikipedia.org/wiki/Snell%27s_law#Vector_form
void Ray::CalcRefractionDirection(float ratio, glm::vec3 &normal) {
	float c = glm::dot(-normal,direction);
	float r = ratio;
	glm::vec3 l = direction;
	glm::vec3 n = normal;
	direction = r*l + (float)(r*c - sqrt(1.0f - r*r*(1.0f - c*c)))*n;
}

// Uses the inverse of the CDF of the PDF p(theta, phi) = cos(theta)/PI
void Ray::InverseCumulativeDistributionFunction(float &phi, float &theta) {
	float u = (float)rand()/RAND_MAX;
    float v = (float)rand()/RAND_MAX;

    phi = 2*M_PI*u;
    theta = acos(sqrt(v));
}