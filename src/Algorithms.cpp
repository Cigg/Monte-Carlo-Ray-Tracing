/**
 * TODO: Find a more fitting class name.
 * TODO: Create a separate class for Intersection struct
 */
#include "Algorithms.h"

#include <algorithm> // std::max, std::min
#include <iostream>

Algorithms::Algorithms() {

}

Algorithms::~Algorithms() {

}

// Constants
const glm::vec3 BG_COLOR = glm::vec3(0.0f); // Background color
const int nShadowRays = 2; // Shadow rays
const float lightIntensity = 15.0f;


/************************************************************
 ** Radiance
 ************************************************************/
glm::vec3 Algorithms::Radiance(Ray &ray, Scene* scene) {
	
	Intersection intersection = Trace(ray, scene);
	glm::vec3 radiance = glm::vec3(0.0f);
	
	if(intersection.shape == NULL) {
		return BG_COLOR;
	}
	
	glm::vec3 surfaceColor = intersection.shape->GetColor(intersection.position);
	if(intersection.shape->isLight) {
		return surfaceColor*lightIntensity;
	}

	// Can crash in here somewhere if direction is only on x-axis?
	if(intersection.shape->isTrans) {
		return RefractedIllumination(intersection, scene);
	}
	
	radiance += DirectIllumination(intersection, scene);
	radiance += IndirectIllumination(intersection, scene);
	radiance *= surfaceColor;
	return radiance;
}


/************************************************************
 ** Trace, (closest intersection)
 ************************************************************/
Algorithms::Intersection Algorithms::Trace(Ray &ray,
	Scene *scene) {
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
			if(tmp > 0.0001f && tmp < t) {
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
	
	for(int i = 0; i < nShadowRays; i++) {
		//select light source k based of lightsource pdf, should probably be a property of lightsource and depend on distance and size
		int lightIndex = rand()%scene->lights->size(); //currently just select one random lightsource
		Shape* currentLight = scene->lights->at(lightIndex);
		float lightSourcePdf = 1.0f/scene->lights->size();
		float lightPointPdf = currentLight->GetSamplingProbability(intersection.position);
		//generate shadowray to point y on light source k
		Ray shadowRay;
		shadowRay.origin = intersection.position;
		shadowRay.direction = currentLight->GetRandomDirectionTowardsShape(intersection.position);
		//estimate radiance
		Intersection possibleLight = Trace(shadowRay, scene);
		if(possibleLight.shape == currentLight) {
			float surfaceCos = std::max(0.0f, glm::dot(intersection.shape->GetNormal(intersection.position), shadowRay.direction));
			float lightCos = std::max(0.0f, glm::dot(currentLight->GetNormal(intersection.position), -shadowRay.direction));
			
			glm::vec3 lightColor = currentLight->GetColor(intersection.position);

			float radianceTransfer = surfaceCos*lightCos;
			float brdf = intersection.shape->OrenNayarBRDF(intersection.ray->direction,shadowRay.direction,intersection.position);
			radiance += (brdf*radianceTransfer*lightColor) / (lightSourcePdf);
		}

		radiance *= lightPointPdf / nShadowRays;
	}

	return radiance;
}


/************************************************************
 ** Indirect Illumination
 ************************************************************/
glm::vec3 Algorithms::IndirectIllumination(Intersection &intersection, Scene *scene) {
	glm::vec3 radiance = glm::vec3(0.0f);
	
	float absorption = 0.5f;
	int MAX_ITERATIONS = 2;
	
	//add importance for optimization
	
	if(absorption < (float)rand()/RAND_MAX && intersection.ray->numBounces < MAX_ITERATIONS && intersection.ray->importance > 0.02) {
		Ray newRay;
		newRay.origin = intersection.position;
		glm::vec3 intersectionNormal = intersection.shape->GetNormal(intersection.position);

		//// Uniform sampling of the hemisphere.
		//newRay.direction = CalcRandomUniformRay(intersectionNormal);
		//Intersection newIntersection = Trace(newRay, scene);
		//if(newIntersection.shape == NULL) {
		//	return radiance;
		//}
		//glm::vec3 surfaceColor = newIntersection.shape->GetColor(newIntersection.position);
		//float newRayCos = std::max(0.0f, glm::dot(intersection.shape->GetNormal(intersection.position), newRay.direction));
		//float pdf = 1.0f / (2.0f*M_PI); //correct probability distribution for hemisphere?
		//float brdf = intersection.shape->OrenNayarBRDF(intersection.ray->direction, newRay.direction, intersection.position);
		//radiance = brdf*newRayCos*Radiance(newRay, scene) / ((1.0f - absorption)*pdf);

		// Importance sampling
		newRay.direction = CalcRandomPDFRay(intersectionNormal);
		Intersection newIntersection = Trace(newRay, scene);
		if (newIntersection.shape == NULL) {
			return radiance;
		}

		glm::vec3 surfaceColor = newIntersection.shape->GetColor(newIntersection.position);
		//float brdf = intersection.shape->LambertianBRDF();
		float brdf = intersection.shape->OrenNayarBRDF(intersection.ray->direction, newRay.direction, intersection.position);

		newRay.numBounces = intersection.ray->numBounces + 1;
		newRay.importance = M_PI * brdf * intersection.ray->importance;
		radiance = (newRay.importance / intersection.ray->importance)*Radiance(newRay, scene)/((1.0f - absorption));
		//radiance = (float)(M_PI*brdf)*Radiance(newRay, scene) / ((1.0f - absorption));

	}
	
	return radiance;
}

/************************************************************
 ** RefractedIllumination
 ************************************************************/
glm::vec3 Algorithms::RefractedIllumination(Intersection &intersection, Scene *scene) {
	const int MAX_ITERATIONS = 4;
	if(intersection.ray->numBounces > MAX_ITERATIONS) {
		return glm::vec3(0.0f);
	}
	
	float n1, n2;
	glm::vec3 incident = intersection.ray->direction;
	glm::vec3 normal = intersection.shape->GetNormal(intersection.position);
	float cosI = glm::dot(incident, normal);

	if(cosI > 0) {
		n1 = 0.9f;
		n2 = 1.0f;
		//if ray is inside material flip normal
		normal = glm::vec3(0.0f) - normal;
	} else {
		n1 = 1.0f;
		n2 = 0.9f;
		cosI = -cosI;
	}
	float ratio = n1/n2;
	float sinT2 = ratio*ratio * (1.0f - cosI*cosI);

	float R;
	if(sinT2 > 1.0f) {
		//total internal reflection
		R = 1.0f;
	} else {
		//calculate reflection chance
		float cosT = sqrt(1.0f - sinT2);
		//fresnel equations, perhaps try schlick's approximation?
		float Rt = (n1 * cosI - n2 * cosT) / (n1 * cosI + n2 * cosT);
		float Rl = (n2 * cosI - n1 * cosT) / (n1 * cosT + n2 * cosI);
		float R = (Rt*Rt + Rl*Rl)*0.5f;
	}


	Ray newRay;
	newRay.origin = intersection.position;
	newRay.numBounces = intersection.ray->numBounces + 1;
	float r01 = (float)rand() / RAND_MAX;
	if(r01 < R) {
		//reflection
		newRay.direction = incident + 2.0f*cosI*normal;
	} else {
		//refraction
		newRay.direction = ratio*incident + normal*(ratio*cosI - sqrt(1.0f - sinT2));
	}
	return Radiance(newRay, scene);
}

// Returns a random ray in the normal oriented hemisphere. Uniformly distributed.
glm::vec3 Algorithms::CalcRandomUniformRay(glm::vec3 &normal) {
	//alternative
	//bad random generator? should use modern method?
	float u = (float)rand() / RAND_MAX;
	float v = (float)rand() / RAND_MAX;

	//evenly distributed
	float theta = 2 * M_PI*u;
	float cosphi = 2 * v - 1;
	float phi = acos(cosphi);

	float x = cos(theta) * sin(phi);
	float y = sin(theta) * sin(phi);
	float z = cosphi;
	glm::vec3 direction = glm::vec3(x, y, z);

	if (glm::dot(normal, direction) < 0.0f) {
		direction *= -1.0f;
	}
	
	return direction;
}

// Returns a random ray in the normal oriented hemisphere. Based on the
// probability distribution function cos(theta)/PI.
glm::vec3 Algorithms::CalcRandomPDFRay(glm::vec3 &normal) {
	float u = (float)rand() / RAND_MAX;
	float v = (float)rand() / RAND_MAX;

	//float r1 = 2 * M_PI * u;
	//float r2 = sqrt(v);

	//glm::vec3 rotw = normal;
	//glm::vec3 rotu = glm::cross(abs(rotw.x) > 0.1f ? glm::vec3(0, 1, 0) : glm::vec3(1, 0, 0), normal);
	//glm::vec3 rotv = glm::cross(rotw, rotu);

	//direction = glm::normalize((float)(r2*cos(r1))*rotu + (float)(r2*sin(r1))*rotv + (float)(sqrt(1 - v))*rotw);

	float x = cos(2 * M_PI*u)*sqrt(v);
	float z = sin(2 * M_PI*u)*sqrt(v);
	float y = sqrt(1 - v);

	glm::vec3 randomDirection = glm::normalize(glm::vec3(x, y, z));

	float c = glm::dot(glm::vec3(0, 1, 0), normal);

	glm::vec3 direction;
	if (c == 1.0f) {
		direction = randomDirection;
	}
	else if (c == -1.0f) {
		direction = -randomDirection;
	}
	else {
		glm::vec3 cross = glm::cross(glm::vec3(0, 1, 0), normal);
		float s = glm::length(cross);

		glm::mat3 vx(0, -(cross.z), cross.y,
			cross.z, 0, -cross.x,
			-cross.y, cross.x, 0);

		glm::mat3 rot = glm::mat3(1.0f) + vx + vx*vx*(1 - c) / (s * s);
		direction = glm::normalize(randomDirection*rot);
	}

	return direction;
}