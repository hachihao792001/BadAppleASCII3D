#include "Global.h"

COLORREF backBuffer[HEIGHT][WIDTH];
COLORREF frontBuffer[HEIGHT][WIDTH];

float* pDepthBuffer = new float[WIDTH * HEIGHT];

HANDLE hConsole;

std::string intToString(int i) {
	std::stringstream ss;
	ss << i;
	return ss.str();
}

int stringToInt(std::string s) {
	std::stringstream ss(s);
	int n;
	ss >> n;
	return n;
}