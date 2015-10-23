#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <iomanip>

#include "Camera.h"
#include "Image.h"
#include "Sphere.h"
#include "Walls.h"

// TODO: move this to a scene/world class or something
std::vector<Shape*> objects;
std::vector<Shape*> lights;

const glm::vec3 BG_COLOR = glm::vec3(0.0f);
const int nShadowRays = 1;
const int imgWidth = 512;
const int imgHeight = 512;
const int samplePerPixel = 500;

struct Intersection {
	Shape* shape;
	Ray* ray;
	glm::vec3 position;
};

Intersection Trace(Ray &ray);
glm::vec3 DirectIllumination(Intersection &intersection);
glm::vec3 IndirectIllumination(Intersection &intersection);
glm::vec3 Radiance(Ray &ray);

void FineArt() {
	std::cout << "\n\n\n\n\n\n";
	std::cout << "   ______   _______ __   __ _______ ______   _______ _______ _______ ______   " << std::endl;
	std::cout << "  |    _ | |   _   |  | |  |       |    _ | |   _   |       |       |    _ |  " << std::endl;
	std::cout << "  |   | || |  |_|  |  |_|  |_     _|   | || |  |_|  |       |    ___|   | ||  " << std::endl;
	std::cout << "  |   |_||_|       |       | |   | |   |_||_|       |       |   |___|   |_||_ " << std::endl;
	std::cout << "  |    __  |       |_     _| |   | |    __  |       |      _|    ___|    __  |" << std::endl;
	std::cout << "  |   |  | |   _   | |   |   |   | |   |  | |   _   |     |_|   |___|   |  | |" << std::endl;
	std::cout << "  |___|  |_|__| |__| |___|   |___| |___|  |_|__| |__|_______|_______|___|  |_|" << std::endl;
	std::cout << " " << std::setfill(' ') << std::setw(62) << "Anton Osterblad,  Mikael Zackrisson,  Aron Tornberg" << std::endl;
}

void LoadScreen(int MAX_COUNT) {
	static int counter = 0.0f;
	float percent_done = 100*counter/MAX_COUNT + 1;
	std::cout << "\r" << std::setfill(' ') << std::setw(10) << std::setprecision(4) << percent_done << "% ";
	for(int i = 0; i <= percent_done; i += 2) {
		char c = 178;
		std::cout << c;
	}
	for(int i = percent_done; i < 99; i += 2) {
		char c = 176;
		std::cout << c;
	}
	++counter;
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

	Image* img = new Image(imgWidth, imgHeight);
	Camera* cam = new Camera();

	//----------------------------
	//-----------RENDER-----------
	//----------------------------
	FineArt();

	#pragma omp parallel for
	for(int x = 0; x < img->GetWidth(); x++) {
		for(int y = 0; y < img->GetHeight(); y++) {
			glm::vec3 color = glm::vec3(0.0f);
			for(int p = 0; p < samplePerPixel; p++) {
				Ray cameraRay = cam->GetRandomRayDirection(x, y, img);
				color += Radiance(cameraRay);
			}

			color /= samplePerPixel;
			img->SetPixel(x, y, color);
		}
		LoadScreen(img->GetWidth());
	}

	//----------------------------
	//-----------EXPORT-----------
	//----------------------------
	char* outImage = (char*)"imagecool.ppm";
	img->WritePPM(outImage);
	return EXIT_SUCCESS;
}

glm::vec3 Radiance(Ray &ray) {
	
	Intersection intersection = Trace(ray);
	glm::vec3 radiance = glm::vec3(0.0f);
	
	if(intersection.shape == NULL) {
		return BG_COLOR;
	}
	
	glm::vec3 surfaceColor = intersection.shape->GetColor(intersection.position);
	if(intersection.shape->isLight) {
		return surfaceColor;
	}
	
	radiance += DirectIllumination(intersection);
	radiance += IndirectIllumination(intersection);

	return radiance;
}

Intersection Trace(Ray& ray) {
	float t = 10000.0f;
	Intersection intersection;
	intersection.shape = NULL;
	intersection.ray = &ray;
	intersection.position = glm::vec3(0.0f);

	int counter = 0;
	for(std::vector<Shape*>::iterator obj = objects.begin(); obj != objects.end(); ++obj) {
		counter++;
		float tmp;
		if((*obj)->Intersect(ray, tmp)) {
			if(tmp < t) {
				t = tmp;
				intersection.shape = *obj;
			}
		}
	}

	intersection.position = ray.origin + t*ray.direction;

	return intersection;
}

glm::vec3 DirectIllumination(Intersection &intersection) {
	glm::vec3 radiance = glm::vec3(0.0f);
	glm::vec3 surfaceColor = intersection.shape->GetColor(intersection.position);

	for(std::vector<Shape*>::iterator light = lights.begin(); light != lights.end(); ++light) {
		
		//add loop to cast multiple shadow rays
		float prob = (*light)->GetSamplingProbability(intersection.position);
		Ray shadowRay;
		shadowRay.origin = intersection.position;
		shadowRay.direction = (*light)->GetRandomDirection(intersection.position);

		//if light in sight add light to radiance
		Intersection possibleLight = Trace(shadowRay);
		if(possibleLight.shape == (*light)) {
			float surfaceCos = glm::dot(intersection.shape->GetNormal(intersection.position), shadowRay.direction);
			float lightCos = glm::dot((*light)->GetNormal(intersection.position), -shadowRay.direction);
			surfaceCos = surfaceCos < 0 ? 0 : surfaceCos;
			lightCos = lightCos < 0 ? 0: lightCos;
			
			
			glm::vec3 lightColor = (*light)->GetColor(intersection.position);

			// BRDF constant for lambertian reflectors
			float reflectance = 0.8f; // from 0 to 1
			float brdf = reflectance/M_PI;

			// Multiply with surfaceCos = lambertian reflectance
			// Multiply with lightCos = lambertian radiator
			float lightIntensity = 1.0f;
			radiance += brdf*surfaceCos*surfaceColor*lightColor*lightIntensity;
		}

		// Multiply with 1/probability
		// Probability = 1/area
		radiance *= prob/nShadowRays;
		
	}
	
	return radiance;
}

glm::vec3 IndirectIllumination(Intersection &intersection) {
	glm::vec3 radiance = glm::vec3(0.0f);
	
	float absorption = 0.1f;
	int MAX_ITERATIONS = 30;
	
	if(absorption < (float)rand()/RAND_MAX && intersection.ray->numBounces < MAX_ITERATIONS) {
		Ray newRay;
		newRay.origin = intersection.position;
		newRay.CalcRandomDirection(intersection.shape->GetNormal(intersection.position));
		float x = (float)rand()/RAND_MAX - 0.5f;
		float y = (float)rand()/RAND_MAX - 0.5f;
		float z = (float)rand()/RAND_MAX - 0.5f;
		newRay.direction = glm::normalize(glm::vec3(x,y,z));
		Intersection newIntersection = Trace(newRay);
		if(newIntersection.shape == NULL) {
			return radiance;
		}

		newRay.numBounces = intersection.ray->numBounces + 1;

		glm::vec3 surfaceColor = newIntersection.shape->GetColor(newIntersection.position);
		float newRayCos = glm::dot(intersection.shape->GetNormal(intersection.position), newRay.direction);
		newRayCos = newRayCos < 0 ? 0: newRayCos;
		
		radiance = surfaceColor*Radiance(newRay)/(1.0f - absorption);
		
	}
	
	return radiance;
	
}