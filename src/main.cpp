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

Shape* FirstIntersection(Ray& ray) {
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

	glm::vec3 directLight = glm::vec3(0.0f);

	// Shadow rays
	//for each light
	for(std::vector<Shape*>::iterator light = lights.begin(); light != lights.end(); ++light) {
		//add loop to cast multiple shadow rays
		float area = (*light)->GetArea();
		int numShadowRays = 6;
		for(int i = 0; i < numShadowRays; i++) {
			Ray shadowRay = Ray();
			shadowRay.origin = intersectionPos;
			shadowRay.direction = (*light)->GetRandomDirection(intersectionPos);

			//if light in sight add light to radiance
			if(FirstIntersection(shadowRay) == (*light)) {
				float surfaceCos = glm::dot(closestShape->GetNormal(intersectionPos), shadowRay.direction);
				float lightCos = glm::dot((*light)->GetNormal(intersectionPos), -shadowRay.direction);
				glm::vec3 surfaceColor = closestShape->GetColor(intersectionPos);
				glm::vec3 lightColor = (*light)->GetColor(intersectionPos);

				// BRDF constant for lambertian reflectors
				float reflectance = 1.0f; // from 0 to 1
				float brdf = reflectance/M_PI;

				// Multiply with surfaceCos = lambertian reflectance
				// Multiply with lightCos = lambertian radiator
				float lightIntensity = 30.0f;
				directLight += brdf*surfaceCos*surfaceColor*lightColor*lightIntensity;
			}
		}

		// Multiply with 1/probability
		// Probability = 1/area
		directLight *= area/numShadowRays;
	}

	// TODO: reflected light
	glm::vec3 reflectedLight = glm::vec3(0.0f);

	// Use russian roulette to determine if a new ray is spawned or not
	// If new ray:
	// Get random direction using based on the BRDF probability distribution function
	// Calc radiance in the random direction using this function recursively
	
	return reflectedLight + directLight; // glm::vec3(glm::length(intersectionPos)/3.0f);
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

	sphere = new Sphere(0.3f);
	sphere->SetPosition(glm::vec3(0.8, -0.5, -0.7));
	objects.push_back(sphere);

	Sphere* light = new Sphere(0.2f);
	light->isLight = true;
	light->SetPosition(glm::vec3(0.0, 0.5, -0.5));
	objects.push_back(light);
	lights.push_back(light);

	Image* img = new Image(512, 512);
	Camera* cam = new Camera();

	//----------------------------
	//-----------RENDER-----------
	//----------------------------
	for(int x = 0; x < img->GetWidth(); x++) {
		for(int y = 0; y < img->GetHeight(); y++) {
			glm::vec3 color = glm::vec3(0.0f);
			for(int p = 0; p < 4; p++) { //Anti-aliasing
					Ray cameraRay = cam->GetRayDirection(x, y, p, img);

					color += Radiance(cameraRay);
				}
				color = color * 0.25f;
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