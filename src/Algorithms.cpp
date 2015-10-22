#include "Algorithms.h"

const glm::vec3 BG_COLOR = glm::vec3(0.0f);

Shape* Algorithms::FirstIntersection(Ray& ray, Scene *scene) {
	std::vector<Shape*> object = scene->shapes;

	float t = 10000.0f;
	Shape* closestShape = NULL;
	for(std::vector<Shape*>::iterator obj = object.begin(); obj != object.end(); ++obj) {
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

glm::vec3 Algorithms::Radiance(Ray &ray, Scene *scene) {
	std::vector<Shape*> objects = scene->shapes;

	float t = 10000.0f;
	Shape* closestShape = NULL;
	for(std::vector<Shape*>::iterator obj = objects.begin(); obj != objects.end(); ++obj) {
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
	std::vector<Shape*> lights = scene->lights;
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
			if(FirstIntersection(shadowRay, scene) == (*light)) {
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
	//return glm::vec3(glm::length(intersectionPos)/3.0f);*/
}
