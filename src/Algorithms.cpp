/**
 * TODO: Find a more fitting class name.
 * TODO: Create a separate class for Intersection struct
 */
#include "Algorithms.h"

#include <iostream>

Algorithms::Algorithms() {

}

Algorithms::~Algorithms() {

}

// Constants
const glm::vec3 BG_COLOR = glm::vec3(0.0f); // Background color
const int nShadowRays = 2; // Shadow rays


/************************************************************
 ** Radiance
 ************************************************************/
glm::vec3 Algorithms::Radiance(Ray &ray, Scene *scene) {
	
	Intersection intersection = Trace(ray, scene);
	glm::vec3 radiance = glm::vec3(0.0f);
	
	if(intersection.shape == NULL) {
		return BG_COLOR;
	}
	
	glm::vec3 surfaceColor = intersection.shape->GetColor(intersection.position);
	if(intersection.shape->isLight) {
		return surfaceColor;
	}
	
	radiance += DirectIllumination(intersection, scene);
	//radiance += IndirectIllumination(intersection, scene);

	return radiance;
}


/************************************************************
 ** Trace, (closest intersection)
 ************************************************************/
Algorithms::Intersection Algorithms::Trace(Ray &ray, Scene *scene) {
	int countShape = scene->shapes->size(); // Number of shapes
	float t = 10000.0f;
	Shape* currentShape;

	Intersection intersection;
	intersection.shape = NULL;
	intersection.ray = &ray;
	intersection.position = glm::vec3(0.0f);

	for(int n = 0; n < countShape; ++n) {
		float tmp;
		currentShape = scene->shapes->at(n);
		if(currentShape->Intersect(ray, tmp)) {
			if(tmp < t) {
				t = tmp;
				intersection.shape = currentShape;
			}
		}
	}

	intersection.position = ray.origin + t*ray.direction;

	return intersection;
}


/************************************************************
 ** Direct Illumination
 ************************************************************/
glm::vec3 Algorithms::DirectIllumination(Intersection &intersection, Scene *scene) {
	glm::vec3 radiance = glm::vec3(0.0f);
	glm::vec3 surfaceColor = intersection.shape->GetColor(intersection.position);

	
	for(int i = 0; i < nShadowRays; i++) {
		//select light source k based of lightsource pdf, should probably be a property of lightsource and depend on distance and size
		int lightIndex = rand()%scene->lights->size(); //currently just select one random lightsource
		Shape* currentLight = scene->lights->at(lightIndex);
		float lightSourcePdf = 1.0f/scene->lights->size();
		float lightPointPdf = 1.0f/currentLight->GetSamplingProbability(intersection.position);
		//generate shadowray to point y on light source k
		Ray shadowRay;
		shadowRay.origin = intersection.position;
		shadowRay.direction = currentLight->GetRandomDirection(intersection.position);
		//estimate radiance
		Intersection possibleLight = Trace(shadowRay, scene);
		if(possibleLight.shape == currentLight) {
			float surfaceCos = std::max(0.0f, glm::dot(intersection.shape->GetNormal(intersection.position), shadowRay.direction));
			float lightCos = std::max(0.0f, glm::dot(currentLight->GetNormal(intersection.position), -shadowRay.direction));
			
			glm::vec3 lightColor = currentLight->GetColor(intersection.position);

			float lightIntensity = 10.0f;
			glm::vec3 radianceTransfer = surfaceCos*lightCos*surfaceColor; //add distance?
			float brdf = 1.0f/M_PI;
			radiance += (brdf*radianceTransfer*lightColor*lightIntensity) / (lightSourcePdf * lightPointPdf);
		}

		radiance /= nShadowRays;
	}

	/*
	int countLights = scene->lights->size(); // Number of lights
	Shape* currentLight;
	for(int n = 0; n < countLights; ++n) {
		currentLight = scene->lights->at(n);
		// TODO: Add loop to cast multiple shadow rays
		float prob = currentLight->GetSamplingProbability(intersection.position);
		Ray shadowRay;
		shadowRay.origin = intersection.position;
		shadowRay.direction = currentLight->GetRandomDirection(intersection.position);

		// If light in sight add light to radiance
		Intersection possibleLight = Trace(shadowRay, scene);
		if(possibleLight.shape == currentLight) {
			float surfaceCos = glm::dot(intersection.shape->GetNormal(intersection.position), shadowRay.direction);
			float lightCos = glm::dot(currentLight->GetNormal(intersection.position), -shadowRay.direction);
			surfaceCos = surfaceCos < 0 ? 0 : surfaceCos;
			lightCos = lightCos < 0 ? 0: lightCos;
			
			
			glm::vec3 lightColor = currentLight->GetColor(intersection.position);
			
			// BRDF constant for lambertian reflectors
			float reflectance = 0.8f; // from 0 to 1
			float brdf = reflectance/M_PI;

			// Multiply with surfaceCos = lambertian reflectance
			// Multiply with lightCos = lambertian radiator
			float lightIntensity = 10.0f;
			
			glm::vec3 hej = brdf*surfaceCos*surfaceColor*lightColor*lightIntensity;

			radiance += (brdf*surfaceCos*surfaceColor*lightColor*lightIntensity) * (prob/nShadowRays);
		}

		// Multiply with 1/probability
		// Probability = 1/area
		
	}
	*/
	return radiance;
}


/************************************************************
 ** Indirect Illumination
 ************************************************************/
glm::vec3 Algorithms::IndirectIllumination(Intersection &intersection, Scene *scene) {
	glm::vec3 radiance = glm::vec3(0.0f);
	
	float absorption = 0.5f;
	int MAX_ITERATIONS = 30;
	
	if(absorption < (float)rand()/RAND_MAX && intersection.ray->numBounces < MAX_ITERATIONS) {
		Ray newRay;
		newRay.origin = intersection.position;
		glm::vec3 intersectionNormal = intersection.shape->GetNormal(intersection.position);
		newRay.CalcRandomDirection(intersectionNormal);

		Intersection newIntersection = Trace(newRay, scene);
		if(newIntersection.shape == NULL) {
			return radiance;
		}

		newRay.numBounces = intersection.ray->numBounces + 1;

		glm::vec3 surfaceColor = newIntersection.shape->GetColor(newIntersection.position);
		float newRayCos = glm::dot(intersection.shape->GetNormal(intersection.position), newRay.direction);
		newRayCos = newRayCos < 0 ? 0: newRayCos;
		
		radiance = surfaceColor*Radiance(newRay, scene)/(1.0f - absorption);
		
	}
	
	return radiance;
	
}
