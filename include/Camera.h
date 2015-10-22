#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"
#include "Image.h"

class Camera {
public:
	Camera();
	~Camera();
	Ray GetRayDirection(int x, int y, int p, Image* img);
	Ray GetRandomRayDirection(int x, int y, Image* img);

protected:
	float fov;
	float angle;
};

#endif // CAMERA_H