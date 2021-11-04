#pragma once
#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <Windows.h>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <fstream>
#include <vector>
#include <list>
#include "stb_image/stb_image.h"
using namespace std;

const COLORREF BLACK = RGB(0, 0, 0);
const COLORREF WHITE = RGB(255, 255, 255);

#define WIDTH 200
#define HEIGHT 100
#define FPS 30

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

extern COLORREF backBuffer[HEIGHT][WIDTH];
extern COLORREF frontBuffer[HEIGHT][WIDTH];

extern float* pDepthBuffer;

extern HANDLE hConsole;

std::string intToString(int i);

int stringToInt(std::string s);
