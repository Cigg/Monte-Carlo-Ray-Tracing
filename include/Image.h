#ifndef IMAGE_H
#define IMAGE_H

struct Rgb {
	float r, g, b;
};

class Image {
public:
	Image();
	Image(int width, int height);
	~Image();

	void SetPixel(int x, int y, Rgb pixel);
	void WritePPM(char* filename);
	int GetWidth();
	int GetHeight();

private:
	unsigned int width;
	unsigned int height;
	Rgb* pixels;
};

#endif // IMAGE_H