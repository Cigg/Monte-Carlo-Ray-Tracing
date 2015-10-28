#define GLM_FORCE_RADIANS 
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <iomanip>

#include "Scene.h"
#include "Camera.h"
#include "Image.h"
#include "Sphere.h"
#include "Walls.h"
#include "Plane.h"
#include "Algorithms.h"

const int imgWidth = 512;
const int imgHeight = 512;
const int samplePerPixel = 200;

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
	float percent_done = 100*counter/MAX_COUNT;
	std::cout << "\r" << std::setfill(' ') << std::setw(10) << std::setprecision(4) << percent_done << "% ";
	for(int i = 0; i < percent_done/2; i ++) {
		char c = 178;
		std::cout << c;
	}
	for(int i = percent_done/2; i < 100/2; i ++) {
		char c = 176;
		std::cout << c;
	}
	++counter;
}

int main() {
	Scene *scene = new Scene();
	Shape *sh;

	//----------------------------
	//---------INIT SCENE---------
	//----------------------------
	sh = new Walls();
	scene->addObject(sh);

	sh = new Sphere(0.3f);
	sh->SetPosition(glm::vec3(0.5f, -0.1f, -0.5f));
	sh->SetColor(glm::vec3(1.0f));
	sh->roughness = 0.0f;
	scene->addObject(sh);

	sh = new Sphere(0.3f);
	sh->SetPosition(glm::vec3(-0.5f, -0.1f, -0.5f));
	sh->SetColor(glm::vec3(1.0f));
	sh->roughness = 1.0f;
	scene->addObject(sh);

	sh = new Sphere(0.3f);
	sh->isTrans = true;
	sh->SetPosition(glm::vec3(0.0f, -0.2f, 0.3f));
	scene->addObject(sh);

	//sh = new Sphere(0.2f);
	//sh->isLight = true;
	//sh->SetColor(glm::vec3(1.0f, 0.0f, 1.0f));
	//sh->SetPosition(glm::vec3(0.3, 0.95, 0));
	//scene->addObject(sh);
	//scene->addLight(sh);

	sh = new Sphere(0.3f);
	sh->isLight = true;
	sh->SetColor(glm::vec3(10.0f));
	sh->SetPosition(glm::vec3(0, 1, 0));
	scene->addObject(sh);
	scene->addLight(sh);

/*	sh = new Plane();
	sh->isLight = true;
	sh->SetColor(glm::vec3(1.0f,1.0f,1.0f));
	sh->SetPosition(glm::vec3(-0.3, 0.95, 0));
	scene->addObject(sh);
	scene->addLight(sh);*/

	Image* img = new Image(imgWidth, imgHeight);
	Camera* cam = new Camera();
	Algorithms* alg = new Algorithms();

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
				color += alg->Radiance(cameraRay, scene);
			}

			color /= samplePerPixel;

			img->SetPixel(x, y, color);
		}
		#pragma omp critical
		LoadScreen(img->GetWidth());
		//std::cout << x << std::endl;
	}

	//----------------------------
	//-----------EXPORT-----------
	//----------------------------
	char* outImage = (char*)"imagecool.ppm";
	img->WritePPM(outImage);
	return EXIT_SUCCESS;
}