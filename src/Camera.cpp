#include "Camera.h"

Camera::Camera() {
	fov = 37.0f;
	angle = tan(M_PI * 0.5 * fov / 180.0);
}

Camera::~Camera() {

}

Ray Camera::GetRayDirection(int x, int y, int p, Image* img) {
	// Anti-aliasing, 2 by 2 sub-pixel sampling
	// To run without anti-aliasing, set px and py to 0.5
	float px, py;
	if(p == 0) {
		px = 0.25; py = 0.25;
	} else if(p == 1) {
		px = 0.75; py = 0.25;
	} else if(p == 2) {
		px = 0.25; py = 0.75;
	} else {
		px = 0.75; py = 0.75;
	}

	// Pixel position on the image plane
	float xx = (2 * ((x + px) / img->GetWidth()) - 1) * angle * img->GetAspectRatio(); 
	float yy = (1 - 2 * ((y + py) / img->GetHeight())) * angle; 

	Ray ray;
	ray.direction = glm::normalize(glm::vec3(xx, yy, -1.0f)); // Look in negative z-direction
	ray.origin = glm::vec3(0.0f, 0.0f, 4.0f); 

	return ray;
}
