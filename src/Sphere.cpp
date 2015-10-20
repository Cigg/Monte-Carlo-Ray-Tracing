#include <iostream>

#include "Sphere.h"

Sphere::Sphere() {

	radius = 1.0f;
}

Sphere::Sphere(float r) {

	radius = r;
}

Sphere::~Sphere() {
	
}

bool Sphere::Intersect(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection, float &hitDist) {
	// Vector from camera to center of sphere
    glm::vec3 l = position - rayOrigin;

    // Dist to position in the middle of the two intersection points of the sphere
    float tmiddle = glm::dot(l, rayDirection);
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