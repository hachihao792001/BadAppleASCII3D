#pragma once
#include "Global.h"

class Texture {
public:
	unsigned char* bytes;
	int width, height;
	int channel;

	Texture() {
		bytes = nullptr;
		width = 0;
		height = 0;
		channel = 0;
	}

	Texture(std::string path) {
		bytes = stbi_load(path.c_str(), &width, &height, &channel, 3);
	}

	COLORREF GetPixelNormalize(float xNormalized, float yNormalized) {
		int x = (int)(xNormalized * (width - 1));
		int y = (int)(yNormalized * (height - 1));
		int r = (int)bytes[y * width * channel + x * channel];
		int g = (int)bytes[y * width * channel + x * channel + 1];
		int b = (int)bytes[y * width * channel + x * channel + 2];
		return RGB(r, g, b);
	}

	COLORREF GetPixel(int x, int y) {
		int r = (int)bytes[y * width * channel + x * channel];
		int g = (int)bytes[y * width * channel + x * channel + 1];
		int b = (int)bytes[y * width * channel + x * channel + 2];
		return RGB(r, g, b);
	}

	~Texture() {
		if (bytes != nullptr)
			stbi_image_free(bytes);
	}
};

void TexturedTriangle(int x1, int y1, float u1, float v1, float w1, int x2, int y2, float u2, float v2, float w2, int x3, int y3, float u3, float v3, float w3, Texture* tex);
