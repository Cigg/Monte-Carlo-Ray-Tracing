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

bool Walls::Intersect(const Ray &ray, float &hitDist) {
    
    float xmin, xmax, ymin, ymax, zmin, zmax;

    if(ray.direction.x > 0.0f) {
    	xmin = (xl - ray.origin.x) / ray.direction.x;
    	xmax = (xr - ray.origin.x) / ray.direction.x;
    }
    else {
    	xmin = (xr - ray.origin.x) / ray.direction.x;
    	xmax = (xl - ray.origin.x) / ray.direction.x;
    }

    if(ray.direction.y > 0.0f) {
    	ymin = (yl - ray.origin.y) / ray.direction.y;
    	ymax = (yr - ray.origin.y) / ray.direction.y;
    }
    else {
    	ymin = (yr - ray.origin.y) / ray.direction.y;
    	ymax = (yl - ray.origin.y) / ray.direction.y;
    }

    if(xmin > ymax || ymin > xmax) {
    	return false;
    }

    float min, max;

    if(ymin > xmin) {
    	min = ymin;
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

    if(ray.direction.z > 0.0f) {
    	zmin = (zl - ray.origin.z) / ray.direction.z;
    	zmax = (zr - ray.origin.z) / ray.direction.z;
    }
    else {
    	zmin = (zr - ray.origin.z) / ray.direction.z;
    	zmax = (zl - ray.origin.z) / ray.direction.z;
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
    }

    hitDist = max;

    return true;
}

glm::vec3 Walls::GetColor(glm::vec3 &pos) {
    glm::vec3 color = glm::vec3(0.8f);

    float epsilon = 0.0001;
    if(pos.x < xl + epsilon) {
        color = glm::vec3(1.0f, 0.0f, 0.0f);
    }
    else if(pos.x > xr - epsilon) {
        color = glm::vec3(0.0f, 1.0f, 0.0f);
    }

    return color;
}