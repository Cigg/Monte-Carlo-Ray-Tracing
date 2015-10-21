#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "Camera.h"
#include "Image.h"
#include "Sphere.h"
#include "Walls.h"

// TODO: move this to a scene/world class or something
std::vector<Shape*> objects;
std::vector<Shape*> lights;

const glm::vec3 BG_COLOR = glm::vec3(0.0f);

Shape* firstIntersection(Ray& ray) {
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

	return closestShape;
}

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
	
	if(closestShape == NULL) {
		return BG_COLOR;
	}
	if(closestShape->isLight) {
		return closestShape->GetColor(intersectionPos);
	}
	// Cast shadow rays here. Calculate direct illumination

	//for each light
	for(std::vector<Shape*>::iterator light = lights.begin(); light != lights.end(); ++light) {
		//add loop to cast multiple shadow rays
		glm::vec3 target = (*light)->GetPosition();
		Ray shadowRay = Ray();
		shadowRay.origin = intersectionPos;
		shadowRay.direction = glm::normalize(target - intersectionPos);

		//if light in sight add light to radiance
		if(firstIntersection(shadowRay) == (*light)) {
			//fixa!
			//float surfaceCos = (closestShape->GetNormal(intersectionPos)).dot(shadowRay.direction);
			//float lightCos = ((*light)->GetNormal(intersectionPos)).dot(shadowRay.direction);
			resultColor += (*light)->GetColor(intersectionPos)/3.0f;
		}
	}
	

	// Return depth for now
	return resultColor + glm::vec3(glm::length(intersectionPos)/3.0f);
	//return glm::vec3(glm::length(intersectionPos)/3.0f);
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

	Sphere* light = new Sphere(0.2f);
	light->isLight = true;
	light->SetPosition(glm::vec3(0.0, 0.5, -0.5));
	objects.push_back(light);
	lights.push_back(light);

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