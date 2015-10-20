#include "Walls.h"

Walls::Walls() {
	xl = -1.0f;
	xr = 1.0f;
	yl = -1.0f;
	yr = 1.0f;
	zl = -1.0f;
	zr = 1.0f;
}

Walls::~Walls() {
	
}

bool Walls::Intersect(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection, float &hitDist, glm::vec3 &color) {
    
    float xmin, xmax, ymin, ymax, zmin, zmax;

    if(rayDirection.x > 0.0f) {
    	xmin = (xl - rayOrigin.x) / rayDirection.x;
    	xmax = (xr - rayOrigin.x) / rayDirection.x;
    	color = glm::vec3(0.0f, 1.0f, 0.0f); // Right wall
    }
    else {
    	xmin = (xr - rayOrigin.x) / rayDirection.x;
    	xmax = (xl - rayOrigin.x) / rayDirection.x;
    	color = glm::vec3(1.0f, 0.0f, 0.0f); // Left wall
    }

    if(rayDirection.y > 0.0f) {
    	ymin = (yl - rayOrigin.y) / rayDirection.y;
    	ymax = (yr - rayOrigin.y) / rayDirection.y;
    }
    else {
    	ymin = (yr - rayOrigin.y) / rayDirection.y;
    	ymax = (yl - rayOrigin.y) / rayDirection.y;
    }

    if(xmin > ymax || ymin > xmax) {
    	return false;
    }

    float min, max;

    if(ymin > xmin) {
    	min = ymin;
    	color = glm::vec3(0.8f, 0.8f, 0.8f); // Ceiling or floor
    }
    else {
    	min = xmin;
    }

    if(ymax < xmax) {
    	max = ymax;
    }
    else {
    	max = xmax;
    }

    if(rayDirection.z > 0.0f) {
    	zmin = (zl - rayOrigin.z) / rayDirection.z;
    	zmax = (zr - rayOrigin.z) / rayDirection.z;
    }
    else {
    	zmin = (zr - rayOrigin.z) / rayDirection.z;
    	zmax = (zl - rayOrigin.z) / rayDirection.z;
    }

    if(min > zmax || zmin > max) {
    	return false;
    }

    // We don't have a front wall
    // if(zmin > min) {
    // 	min = zmin;
    // }

    if(zmax < max) {
    	max = zmax;
    	color = glm::vec3(0.6f, 0.6f, 0.6f); // Back wall
    }

    return true;
}
