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
    if(isLight)
        return glm::vec3(1.0f);
    else
        return glm::vec3(1.0f, 0.0f, 0.0f);
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

	return position + glm::vec3(x,y,z);
}