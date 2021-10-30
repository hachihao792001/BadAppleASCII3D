#include "Rasterize.h"

void Init() {

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 6;          // Width of each character in the font
	cfi.dwFontSize.Y = 6;			// Height
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas"); // Choose your font
	SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);

	std::string consoleSizeStr = "mode " + std::to_string(WIDTH + 4) + ", " + std::to_string(HEIGHT + 4);
	system(consoleSizeStr.c_str());

	/*CONSOLE_SCREEN_BUFFER_INFOEX info;
	info.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);

	GetConsoleScreenBufferInfoEx(hConsole, &info);

	info.ColorTable[0] = RGB(255, 0, 0);

	SetConsoleScreenBufferInfoEx(hConsole, &info);*/

	std::memset((COLORREF*)backBuffer, WHITE, sizeof(COLORREF) * WIDTH * HEIGHT);
	std::memset((COLORREF*)frontBuffer, BLACK, sizeof(COLORREF) * WIDTH * HEIGHT);
}

void setCursorPosition(int x, int y)
{
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout.flush();
	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(hOut, coord);
}

char toPrintCharacter(COLORREF c) {
	string grayScaleASCII = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";
	float grayScale = (GetRValue(c) + GetGValue(c) + GetBValue(c)) / 3.0f;
	int l = grayScaleASCII.length();
	return grayScaleASCII[l - 1 - (int)(grayScale / 255.0f * (l - 1))];
}

void DrawCall() {
	std::string a = "";
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			if (frontBuffer[i][j] == 0)
				a += ' ';
			else
				a += toPrintCharacter(frontBuffer[i][j]);
		}
		a += "\n";
	}
	setCursorPosition(0, 0);
	//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
	std::cout << a;

	std::cout.flush();
	std::memcpy((COLORREF*)backBuffer, (COLORREF const*)frontBuffer, sizeof(COLORREF) * WIDTH * HEIGHT);
	std::memset((COLORREF*)frontBuffer, BLACK, sizeof(COLORREF) * WIDTH * HEIGHT);
}

void Dot(Vec2 v, COLORREF c) {
	if (v.v >= 0 && v.v < HEIGHT && v.u >= 0 && v.u < WIDTH)
		frontBuffer[(int)v.v][(int)v.u] = c;
}

void DrawTriangle(Triangle tri, COLORREF color) {
	DrawLine(GetLinePixels(tri.vertices[0], tri.vertices[1]), color);
	DrawLine(GetLinePixels(tri.vertices[1], tri.vertices[2]), color);
	DrawLine(GetLinePixels(tri.vertices[2], tri.vertices[0]), color);
}

void FillTriangle(Triangle tri, COLORREF fillColor, bool hasBorder, COLORREF borderColor)
{
	std::vector<Vec2> linePixels[3];
	for (int i = 0; i < 3; i++) {
		linePixels[i] = GetLinePixels(tri.vertices[i], tri.vertices[(i + 1) % 3]);
		if (hasBorder)
			DrawLine(linePixels[i], borderColor);
	}

	std::vector<Vec2> allBorderPixels;
	allBorderPixels.reserve(sizeof(linePixels) / sizeof(Vec2));
	for (int i = 0; i < 3; i++)
		allBorderPixels.insert(allBorderPixels.end(), linePixels[i].begin(), linePixels[i].end());

	std::vector<int> allXs[HEIGHT];
	for (int i = 0; i < (int)allBorderPixels.size(); i++) {
		if (allBorderPixels[i].v >= 0 && allBorderPixels[i].v < HEIGHT)
			allXs[(int)allBorderPixels[i].v].push_back((int)allBorderPixels[i].u);
	}

	for (int i = 0; i < HEIGHT; i++) {
		if ((int)allXs[i].size() == 0) continue;
		int min = *min_element(allXs[i].begin(), allXs[i].end());
		if (min < 0) min = 0;
		int max = *max_element(allXs[i].begin(), allXs[i].end());
		if (max >= WIDTH)max = WIDTH - 1;

		for (int j = min; j <= max; j++) {
			if (frontBuffer[i][j] == BLACK)
				Dot(Vec2(j, i), fillColor);
		}
	}
}

std::vector<Vec2> GetLinePixels(Vec2 a, Vec2 b) {

	std::vector<Vec2> result;

	if (a == b) {
		result.push_back(a);
	}
	else if ((int)a.u == (int)b.u) {
		int minY = (int)(a.v < b.v ? a.v : b.v);
		int maxY = (int)(a.v > b.v ? a.v : b.v);
		for (int i = minY; i <= maxY; i++)
			result.push_back(Vec2(a.u, i));
	}
	else if ((int)a.v == (int)b.v) {
		int minX = (int)(a.u < b.u ? a.u : b.u);
		int maxX = (int)(a.u > b.u ? a.u : b.u);
		for (int j = minX; j <= maxX; j++)
			result.push_back(Vec2(j, a.v));
	}
	else {

		//https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

		if (abs(b.v - a.v) < abs(b.u - a.u)) {
			//góc thấp
			if (a.u > b.u)
				std::swap(a, b);

			int dx = (int)b.u - (int)a.u;
			int dy = (int)b.v - (int)a.v;

			int yI = 1;
			if (dy < 0) {
				yI = -1;
				dy = -dy;
			}

			int D = 2 * dy - dx;
			int y = (int)a.v;

			for (int x = (int)a.u; x <= (int)b.u; x++) {
				result.push_back(Vec2(x, y));
				if (D > 0) {
					y += yI;
					D += 2 * (dy - dx);
				}
				else
					D += 2 * dy;
			}

		}
		else {
			//góc cao
			if (a.v > b.v)
				std::swap(a, b);

			int dx = (int)b.u - (int)a.u;
			int dy = (int)b.v - (int)a.v;

			int xI = 1;
			if (dx < 0) {
				xI = -1;
				dx = -dx;
			}

			int D = 2 * dx - dy;
			int x = (int)a.u;

			for (int y = (int)a.v; y <= (int)b.v; y++) {
				result.push_back(Vec2(x, y));
				if (D > 0) {
					x += xI;
					D += 2 * (dx - dy);
				}
				else
					D += 2 * dx;
			}

		}
	}

	return result;
}

void DrawLine(std::vector<Vec2> pixels, COLORREF color) {
	for (int i = 0; i < (int)pixels.size(); i++)
		Dot(pixels[i], color);
}

mat4x4 mat4x4::PointAt(Vec3& pos, Vec3& target, Vec3& up)
{
	// Calculate new forward direction
	Vec3 newForward = target - pos;
	newForward.Normalize();

	// Calculate new Up direction
	Vec3 a = newForward * Vec3::Dot(up, newForward);
	Vec3 newUp = up - a;
	newUp.Normalize();

	// New Right direction is easy, its just cross product
	Vec3 newRight = Vec3::CrossProduct(newUp, newForward);

	// Construct Dimensioning and Translation Matrix	
	mat4x4 matrix;
	matrix.m[0][0] = newRight.x;	matrix.m[0][1] = newRight.y;	matrix.m[0][2] = newRight.z;	matrix.m[0][3] = 0.0f;
	matrix.m[1][0] = newUp.x;		matrix.m[1][1] = newUp.y;		matrix.m[1][2] = newUp.z;		matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = newForward.x;	matrix.m[2][1] = newForward.y;	matrix.m[2][2] = newForward.z;	matrix.m[2][3] = 0.0f;
	matrix.m[3][0] = pos.x;			matrix.m[3][1] = pos.y;			matrix.m[3][2] = pos.z;			matrix.m[3][3] = 1.0f;
	return matrix;

}

Vec3 Vector_IntersectPlane(Vec3& plane_p, Vec3& plane_n, Vec3& lineStart, Vec3& lineEnd, float& t)
{
	plane_n.Normalize();
	float plane_d = -Vec3::Dot(plane_n, plane_p);
	float ad = Vec3::Dot(lineStart, plane_n);
	float bd = Vec3::Dot(lineEnd, plane_n);
	t = (-plane_d - ad) / (bd - ad);
	Vec3 lineStartToEnd = lineEnd - lineStart;
	Vec3 lineToIntersect = lineStartToEnd * t;
	return lineStart + lineToIntersect;
}