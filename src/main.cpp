#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "Camera.h"
#include "Image.h"
#include "Sphere.h"
#include "Walls.h"

// TODO: move this to a scene/world class or something
std::vector<Shape*> objects;


glm::vec3 Radiance(Ray &ray) {
	glm::vec3 resultColor = glm::vec3(0.0f);

	float t = 10000.0f;
	Shape* closestShape = NULL;
	int counter = 0;
	for(std::vector<Shape*>::iterator obj = objects.begin(); obj != objects.end(); ++obj) {
		counter++;
		float tmp;
		if((*obj)->Intersect(ray, tmp)) {
			if(tmp < t) {
				t = tmp;
				closestShape = *obj;
			}
		}
	}

	glm::vec3 intersectionPos = ray.origin + t*ray.direction;

	// Cast shadow rays here. Calculate direct illumination

	// Return depth for now
	return glm::vec3(glm::length(intersectionPos)/3.0f);
}

int main() {
	//----------------------------
	//---------INIT SCENE---------
	//----------------------------
	Walls* walls = new Walls();
	objects.push_back(walls);

	Sphere* sphere = new Sphere(0.2f);
	sphere->SetPosition(glm::vec3(0.0, 0.0, -0.5));

	objects.push_back(sphere);

	Image* img = new Image();
	Camera* cam = new Camera();

	//----------------------------
	//-----------RENDER-----------
	//----------------------------
	for(int x = 0; x < img->GetWidth(); x++) {
		for(int y = 0; y < img->GetHeight(); y++) {
			Ray cameraRay = cam->GetRayDirection(x, y, img);

			glm::vec3 color = Radiance(cameraRay);
			img->SetPixel(x, y, color);
		}	
	}

	//----------------------------
	//-----------EXPORT-----------
	//----------------------------
	char* outImage = (char*)"imagecool.ppm";
	img->WritePPM(outImage);

	return EXIT_SUCCESS;
}