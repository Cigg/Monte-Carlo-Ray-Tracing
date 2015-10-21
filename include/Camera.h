#ifndef CAMERA_H
#define CAMERA_H
#define _USE_MATH_DEFINES

#include "Ray.h"
#include "Image.h"

class Camera {
public:
	Camera();
	~Camera();
	Ray GetRayDirection(int x, int y, Image* img);

protected:
	float fov;
	float angle;
};

#endif // CAMERA_H