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
	/*
	float phi; // azimuth
	float theta; // inclination
	InverseCumulativeDistributionFunction(phi, theta);

    // Orthonormal coordinate frame
    glm::vec3 sw = normal;
    glm::vec3 su = glm::normalize(glm::cross((glm::abs(sw.x) > 0.1 ? glm::vec3(0, 1, 0) : glm::vec3(1, 0, 0)), sw));
    glm::vec3 sv = glm::cross(sw, su);

    glm::vec3 dir = (float)(cos(theta) * cos(phi)) * sw + 
    				(float)(cos(theta) * sin(phi)) * su +
    				(float)(sin(theta)) * sv;

    direction = glm::normalize(dir);
	*/

	//alternative
	//bad random generator? should use modern method?
	float u = (float)rand()/RAND_MAX;
	float v = (float)rand()/RAND_MAX;

	//evenly distributed
	float theta = 2*M_PI*u;
	float cosphi = 2*v - 1;
	float phi = acos(cosphi);

	float x = cos(theta) * sin(phi);
	float y = sin(theta) * sin(phi);
	float z = cosphi;
	direction = glm::vec3(x,y,z);
	
	if(glm::dot(normal, direction) < 0.0f) {
		direction *= -1.0f;
	}
	
}

void Ray::CalcRefractionDirection(float ratio, glm::vec3 &normal) {
	float cosTheta = glm::dot(direction,normal);
	float sinTheta2 = ratio*ratio*(1.0f - cosTheta*cosTheta);
	direction = glm::normalize(ratio*direction + (float)(ratio*cosTheta - sqrt(1 - sinTheta2))*normal);
}

// Uses the inverse of the CDF of the PDF p(theta, phi) = cos(theta)/PI
void Ray::InverseCumulativeDistributionFunction(float &phi, float &theta) {
	float u = (float)rand()/RAND_MAX;
    float v = (float)rand()/RAND_MAX;

    phi = 2*M_PI*u;
    theta = acos(sqrt(v));
}