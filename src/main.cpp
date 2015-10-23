#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "Scene.h"
#include "Camera.h"
#include "Image.h"
#include "Sphere.h"
#include "Walls.h"
#include "Algorithms.h"

int main() {
	Scene *scene = new Scene();
	Shape *sh;

	//----------------------------
	//---------INIT SCENE---------
	//----------------------------
	sh = new Walls();
	scene->addObject(sh);

	sh = new Sphere(0.2f);
	sh->SetPosition(glm::vec3(0.0, 0.0, -0.5));
	scene->addObject(sh);

	sh = new Sphere(0.3f);
	sh->SetPosition(glm::vec3(0.8, -0.5, -0.7));
	scene->addObject(sh);

	sh = new Sphere(0.2f);
	sh->isLight = true;
	sh->SetPosition(glm::vec3(0.0, 0.5, -0.5));
	scene->addLight(sh);

	Image* img = new Image(256, 256);
	Camera* cam = new Camera();

	Algorithms* alg = new Algorithms();

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
				color += alg->Radiance(cameraRay, scene);
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