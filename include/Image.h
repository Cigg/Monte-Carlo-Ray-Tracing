#ifndef IMAGE_H
#define IMAGE_H

#include <glm/glm.hpp>

class Image {
public:
	Image();
	Image(int width, int height);
	~Image();

	void SetPixel(int x, int y, glm::vec3 &pixelValue);
	void WritePPM(char* filename);
	int GetWidth();
	int GetHeight();
	float GetAspectRatio();

private:
	unsigned int width;
	unsigned int height;
	glm::vec3* pixels;
};

#endif // IMAGE_H