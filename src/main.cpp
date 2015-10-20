#include <glm/glm.hpp>
#include <iostream>

#include "Image.h"
#include "Sphere.h"
#include "Walls.h"

int main() {
	glm::vec3 camPos = glm::vec3(0.0, 0.0, 4.0);

	Walls walls;

	Sphere sphere(0.2f);
	sphere.SetPosition(glm::vec3(0.0, 0.0, -0.5));

	Image img;
	float fov = 37.0f;
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

			glm::vec3 color;
            float t;
 			if(walls.Intersect(camPos, raydir, t, color)) {
 				rgb.r = color.x;
 				rgb.g = color.y;
 				rgb.b = color.z;
 			}

 			if(sphere.Intersect(camPos, raydir, t, color)) {
 			 	rgb.r = color.x;
 			 	rgb.g = color.y;
 			 	rgb.b = color.z;
 			}

			img.SetPixel(x, y, rgb);
		}	
	}

	char* outImage = (char*)"imagecool.ppm";
	img.WritePPM(outImage);

	return EXIT_SUCCESS;
}