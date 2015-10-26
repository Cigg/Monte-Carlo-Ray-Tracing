#define _USE_MATH_DEFINES

#include <iostream>
#include <cstdlib>

#include "Sphere.h"

Sphere::Sphere() {
	radius = 1.0f;
}

Sphere::Sphere(float r) {
	radius = r;
}

Sphere::~Sphere() {
	
}

bool Sphere::Intersect(const Ray &ray, float &hitDist) {
    // Vector from camera to center of sphere
    glm::vec3 l = position - ray.origin;

    // Dist to position in the middle of the two intersection points of the sphere
    float tmiddle = glm::dot(l, ray.direction);
    if (tmiddle < 0)
    	return false;

    // (Dist from center of the sphere to the position in the middle of the two intersection points)^2
    float d2 = glm::dot(l, l) - tmiddle * tmiddle;

    float r2 = radius*radius;
    if (d2 > r2)
    	return false;

    float t = sqrt(r2 - d2); 
    // Distance to first intersection point
    hitDist = tmiddle - t; 
    // Distance to second intersection point
    // hitDist2 = tca + thc; 

    return true; 
}

glm::vec3 Sphere::GetColor(glm::vec3 &pos) {
    return color;
}

glm::vec3 Sphere::GetNormal(glm::vec3 &intersection) {
    return glm::normalize(intersection - position);
}

glm::vec3 Sphere::GetRandomPosition() {
	//bad random generator? should use modern method?
	float u = (float)rand()/RAND_MAX;
	float v = (float)rand()/RAND_MAX;

	//evenly distributed
	float theta = 2*M_PI*u;
	float cosphi = 2*v - 1;
	float phi = acos(cosphi);

	float x = radius * cos(theta) * sin(phi);
	float y = radius * sin(theta) * sin(phi);
	float z = radius * cosphi;

	glm::vec3 offset = glm::vec3(x,y,z);


	return position + offset;
}

// Returns a random direction from the pos towards this shape
glm::vec3 Sphere::GetRandomDirectionTowardsShape(glm::vec3 &pos) {
    float u = (float)rand()/RAND_MAX;
    float v = (float)rand()/RAND_MAX;

    // Orthonormal coordinate frame
    glm::vec3 sw = glm::normalize(position - pos);
    glm::vec3 su = glm::normalize(glm::cross((glm::abs(sw.x) > 0.1 ? glm::vec3(0, 1, 0) : glm::vec3(1, 0, 0)), sw));
    glm::vec3 sv = glm::cross(sw, su);

    float cosAngleMax = sqrt(1.0f - radius*radius/glm::dot(pos - position, pos - position));
    float cosAngle = 1.0f - u + u*cosAngleMax;
    float sinAngle = sqrt(1.0f - cosAngle*cosAngle);
    float phi = 2*M_PI*v;

    glm::vec3 l = (float)(sinAngle * cos(phi)) * su +
                  (float)(sinAngle * sin(phi)) * sv + 
                  cosAngle * sw;

    return glm::normalize(l);
}

float Sphere::GetSamplingProbability(glm::vec3 &pos) {
    float cosTheta = sqrt(1 - radius*radius/glm::dot(pos - position, pos - position));
    return 2*M_PI*(1 - cosTheta);
}