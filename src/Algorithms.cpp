/**
 * TODO: Find a more fitting class name.
 * It is not possible to create a vec from Shape and save as,
 * std::vector<Shape*> object = scene->shapes; Needs to be fixed.
 */
#include <iostream>

#include "Algorithms.h"

Algorithms::Algorithms() {

}

Algorithms::~Algorithms() {

}

// Background color
const glm::vec3 BG_COLOR = glm::vec3(0.0f);

/************************************************************
 ** Calculate FirstIntersection (closest intersection)
 ************************************************************/
Shape* Algorithms::FirstIntersection(Ray& ray, Scene *scene) {
	//std::vector<Shape*> object = scene->shapes;
	int objects = scene->shapes->size();
	float t = 10000.0f;
	Shape* closestShape = NULL;
            

	//for(std::vector<Shape*>::iterator obj = object.begin(); obj != object.end(); ++obj) {
	for(int n = 0; n < objects; ++n) {
		float tmp;
		Shape* obj = scene->shapes->at(n);
		//if((*obj)->Intersect(ray, tmp)) {
		if(obj->Intersect(ray, tmp)) {
			if(tmp < t) {
				t = tmp;
				//closestShape = *obj;
				closestShape = obj;
			}
		}
	}

	return closestShape;
}

/************************************************************
 ** Calculate Radiance
 ************************************************************/
glm::vec3 Algorithms::Radiance(Ray &ray, Scene *scene) {
	// FirstIntersection (closest intersection)
	//std::vector<Shape*> objects = scene->shapes;
	int objects = scene->shapes->size();
	//std::cout << objects << std::endl;
	float t = 10000.0f;
	Shape* closestShape = NULL;

	//for(std::vector<Shape*>::iterator obj = objects.begin(); obj != objects.end(); ++obj) {
	for(int n = 0; n < objects; ++n) {
		float tmp;
		Shape *obj = scene->shapes->at(n);
		//if((*obj)->Intersect(ray, tmp)) {
		if(obj->Intersect(ray, tmp)) {
			if(tmp < t) {
				t = tmp;
				//closestShape = *obj;
				closestShape = obj;
			}
		}
	}

	// If no intersection, set radiance to background color
	if(closestShape == NULL) {
		std::cout << "closesShape = NULL" << std::endl;
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
	//std::vector<Shape*> lights = scene->lights;
	int numLights = scene->lights->size();
	//std::cout << objects << std::endl;
	//for each light
	//for(std::vector<Shape*>::iterator light = lights.begin(); light != lights.end(); ++light) {
	for(int n = 0; n < numLights; ++n) {
		//add loop to cast multiple shadow rays
		Shape* light = scene->lights->at(n);
		//float prob = (*light)->GetSamplingProbability(intersectionPos);
		float prob = (light)->GetSamplingProbability(intersectionPos);
		int numShadowRays = 6;
		for(int i = 0; i < numShadowRays; i++) {
			Ray shadowRay = Ray();
			shadowRay.origin = intersectionPos;
			//shadowRay.direction = (*light)->GetRandomDirection(intersectionPos);
			shadowRay.direction = (light)->GetRandomDirection(intersectionPos);

			//if light in sight add light to radiance
			//if(FirstIntersection(shadowRay, scene) == (*light)) {
			if(FirstIntersection(shadowRay, scene) == (light)) {
				float surfaceCos = glm::dot(closestShape->GetNormal(intersectionPos), shadowRay.direction);
				//float lightCos = glm::dot((*light)->GetNormal(intersectionPos), -shadowRay.direction);
				//glm::vec3 lightColor = (*light)->GetColor(intersectionPos);
				float lightCos = glm::dot((light)->GetNormal(intersectionPos), -shadowRay.direction);
				glm::vec3 lightColor = (light)->GetColor(intersectionPos);


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
		reflectedLight = surfaceColor*(newRay.importance/ray.importance)*Radiance(newRay, scene);
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
