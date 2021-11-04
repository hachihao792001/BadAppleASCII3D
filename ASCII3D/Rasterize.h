#pragma once

#include "VectorAndMatrix.h"
#include "Mesh.h"

void DrawTriangle(Triangle tri, COLORREF c = WHITE);
void FillTriangle(Triangle tri, COLORREF fillColor, bool hasBorder = false, COLORREF borderColor = WHITE);
std::vector<Vec2> GetLinePixels(Vec2 a, Vec2 b);

void DrawLine(std::vector<Vec2> pixels, COLORREF c = WHITE);
void Init();
void setCursorPosition(int x, int y);
void DrawCall();
void DrawPixel(Vec2 v, COLORREF c = WHITE);