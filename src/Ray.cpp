#include "Ray.h"

Ray::Ray() {
	origin = glm::vec3(0.0f, 0.0f, 0.0f);
	direction = glm::vec3(0.0f, 0.0f, -1.0f);
	importance = 1.0f;
	numBounces = 0;	
}

Ray::~Ray() {
		
}

glm::vec3 Ray::GetRandomDirection(glm::vec3 &normal) {
	float phi; // azimuth
	float theta; // inclination
	InverseCumulativeDistributionFunction(phi, theta);

    // Orthonormal coordinate frame
    glm::vec3 sw = normal;
    glm::vec3 su = glm::normalize(glm::cross((abs(sw.x) > 0.1 ? glm::vec3(0, 1, 0) : glm::vec3(1, 0, 0)), sw));
    glm::vec3 sv = glm::cross(sw, su);

    glm::vec3 dir = (float)(cos(theta) * cos(phi)) * sw + 
    				(float)(cos(theta) * sin(phi)) * su +
    				(float)(sin(theta)) * sv;

    return glm::normalize(dir);
}

// Uses the inverse of the CDF of the PDF p(theta, phi) = cos(theta)/PI
void Ray::InverseCumulativeDistributionFunction(float &phi, float &theta) {
	float u = (float)rand()/RAND_MAX;
    float v = (float)rand()/RAND_MAX;

    phi = 2*M_PI*u;
    theta = acos(sqrt(v));
}