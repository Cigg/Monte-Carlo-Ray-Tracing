#include "Camera.h"

Camera::Camera() {
	fov = 37.0f;
	angle = tan(M_PI * 0.5 * fov / 180.0);
}

Camera::~Camera() {

}

Ray Camera::GetRayDirection(int x, int y, Image* img) {
	// Pixel position on the image plane
	float xx = (2 * ((x + 0.5) / img->GetWidth()) - 1) * angle * img->GetAspectRatio(); 
	float yy = (1 - 2 * ((y + 0.5) / img->GetHeight())) * angle; 

	Ray ray;
	ray.direction = glm::normalize(glm::vec3(xx, yy, -1.0f)); // Look in negative z-direction
	ray.origin = glm::vec3(0.0f, 0.0f, 4.0f); 

	return ray;
}
