#include "Image.h"

#include <fstream>
#include <iostream>

Image::Image() {
    width = 512;
    height = 512;

    pixels = new Rgb[width*height];
}

Image::Image(int w, int h) {
    width = w;
    height = h;

    pixels = new Rgb[width*height];
}

Image::~Image() {
    delete[] pixels;
}

void Image::SetPixel(int x, int y, Rgb pixelValue) {
    pixels[width*y + x].r = pixelValue.r;
    pixels[width*y + x].g = pixelValue.g;
    pixels[width*y + x].b = pixelValue.b;
}

void Image::WritePPM(char* filename) {
    std::ofstream ofs;
    try {
        ofs.open(filename, std::ios::binary); // need to spec. binary mode for Windows users
        if (ofs.fail()) {
            throw("Can't open output file");
        }
        ofs << "P6\n" << width << " " << height << "\n255\n";
        unsigned char r, g, b;
        // loop over each pixel in the Image, clamp and convert to byte format
        for (int i = 0; i < width * height; ++i) {
            r = static_cast<unsigned char>(std::min(1.0f, pixels[i].r) * 255);
            g = static_cast<unsigned char>(std::min(1.0f, pixels[i].g) * 255);
            b = static_cast<unsigned char>(std::min(1.0f, pixels[i].b) * 255);
            ofs << r << g << b;
        }
        ofs.close();
    }
    catch (const char *err) {
        fprintf(stderr, "%s\n", err);
        ofs.close();
    }
}

int Image::GetWidth() {
    return width;
}
int Image::GetHeight() {
    return height;
}