#include <iostream>

#include "Image.h"

int main() {
	std::cout << "Hello world" << std::endl;

	Image img(1024, 1024);

	for(int i = 0; i < img.GetWidth(); i++) {
		for(int j = 0; j < img.GetHeight(); j++) {
			Rgb rgb;
			rgb.r = 1.0f;
			rgb.g = 1.0f;
			rgb.b = 0.0f;
			img.SetPixel(i, j, rgb);
		}	
	}

	img.WritePPM("imagecool.ppm");

	return 0;
}