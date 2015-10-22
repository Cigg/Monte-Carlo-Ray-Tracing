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

Shape* FirstIntersection(Ray& ray);

glm::vec3 Radiance(Ray &ray);

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

	Image* img = new Image(256, 256);
	Camera* cam = new Camera();

	//----------------------------
	//-----------RENDER-----------
	//----------------------------
	#pragma omp parallel for
	for(int x = 0; x < img->GetWidth(); x++) {
		for(int y = 0; y < img->GetHeight(); y++) {
			glm::vec3 color = glm::vec3(0.0f);
			// for(int p = 0; p < 4; p++) { //Anti-aliasing
			// 		Ray cameraRay = cam->GetRayDirection(x, y, p, img);

			// 		color += Radiance(cameraRay);
			// }
			// color = color * 0.25f;
			int samplePerPixel = 50;
			for(int p = 0; p < samplePerPixel; p++) {
				Ray cameraRay = cam->GetRandomRayDirection(x, y, img);
				color += Radiance(cameraRay);
			}
			color /= samplePerPixel;
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

	if(closestShape == NULL) {
		return BG_COLOR;
	}

	glm::vec3 intersectionPos = ray.origin + t*ray.direction;
	glm::vec3 intersectionNormal = closestShape->GetNormal(intersectionPos);
	glm::vec3 surfaceColor = closestShape->GetColor(intersectionPos);

	if(closestShape->isLight) {
		return closestShape->GetColor(intersectionPos);
	}

	glm::vec3 directLight = glm::vec3(0.0f);

	// Shadow rays
	//for each light
	for(std::vector<Shape*>::iterator light = lights.begin(); light != lights.end(); ++light) {
		//add loop to cast multiple shadow rays
		float prob = (*light)->GetSamplingProbability(intersectionPos);
		int numShadowRays = 6;
		for(int i = 0; i < numShadowRays; i++) {
			Ray shadowRay = Ray();
			shadowRay.origin = intersectionPos;
			shadowRay.direction = (*light)->GetRandomDirection(intersectionPos);

			//if light in sight add light to radiance
			if(FirstIntersection(shadowRay) == (*light)) {
				float surfaceCos = glm::dot(closestShape->GetNormal(intersectionPos), shadowRay.direction);
				float lightCos = glm::dot((*light)->GetNormal(intersectionPos), -shadowRay.direction);
				glm::vec3 lightColor = (*light)->GetColor(intersectionPos);

				// BRDF constant for lambertian reflectors
				float reflectance = 0.8f; // from 0 to 1
				float brdf = reflectance/M_PI;

				// Multiply with surfaceCos = lambertian reflectance
				// Multiply with lightCos = lambertian radiator
				float lightIntensity = 20.0f;
				directLight += brdf*surfaceCos*surfaceColor*lightColor*lightIntensity;
			}
		}

		// Multiply with 1/probability
		// Probability = 1/area
		directLight *= prob/numShadowRays;
	}

	// TODO: reflected light
	glm::vec3 reflectedLight = glm::vec3(0.0f);

	if(ray.numBounces < 5 && ray.importance > 0.05) {
		Ray newRay;
		newRay.origin = intersectionPos;
		newRay.CalcRandomDirection(intersectionNormal);
		newRay.numBounces = ray.numBounces + 1;

		float newRayCos = glm::dot(closestShape->GetNormal(intersectionPos), newRay.direction);
		newRay.importance = ray.importance * newRayCos * 0.8f; // oldImportance * brdf
		reflectedLight = surfaceColor*(newRay.importance/ray.importance)*Radiance(newRay);
	}

	// Use russian roulette to determine if a new ray is spawned or not
	// If new ray:
	// Get random direction using based on the BRDF probability distribution function
	// Calc radiance in the random direction using this function recursively or by just followinging it until a
	// light source is hit or if it terminates
	if(true) // if(ray.numBounces == 0)
		return reflectedLight + directLight; // glm::vec3(glm::length(intersectionPos)/3.0f);
	else
		return reflectedLight;
	//return glm::vec3(glm::length(intersectionPos)/3.0f);
}