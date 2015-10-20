#include <glm/glm.hpp>
#include <iostream>

#include "Image.h"
#include "Sphere.h"

int main() {
	Sphere sphere;
	sphere.SetPosition(glm::vec3(0.0, 0.0, -5.0));

	Image img;
	float fov = 30.0f;
	float angle = tan(M_PI * 0.5 * fov / 180.0);

	for(int x = 0; x < img.GetWidth(); x++) {
		for(int y = 0; y < img.GetHeight(); y++) {
			// Pixel position on the image plane
			float xx = (2 * ((x + 0.5) / img.GetWidth()) - 1) * angle * img.GetAspectRatio(); 
            float yy = (1 - 2 * ((y + 0.5) / img.GetHeight())) * angle; 

            // Ray direction from the camera to the image plane
            glm::vec3 raydir(xx, yy, -1.0f);
            raydir = glm::normalize(raydir);

			Rgb rgb;
			rgb.r = 0.0f;
			rgb.g = 0.0f;
			rgb.b = 0.0f;

            float t;
 			if(sphere.Intersect(glm::vec3(0.0, 0.0, 1.0), raydir, t)) {
 				rgb.r = 1.0f;
 			}
 			else {

 			}
            //std::cout << "t: " << t << std::endl;

			img.SetPixel(x, y, rgb);
		}	
	}

	char* outImage = (char*)"imagecool.ppm";
	img.WritePPM(outImage);

	return EXIT_SUCCESS;
}