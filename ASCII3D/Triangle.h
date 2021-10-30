//#pragma once
//#include "Rasterize.h"
//
//struct ATriangle {
//	glm::vec3 vertices[3];
//
//	ATriangle(glm::vec3 v[3]) {
//		vertices[0] = v[0];
//		vertices[1] = v[1];
//		vertices[2] = v[2];
//	}
//
//	Triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {
//		vertices[0] = v1;
//		vertices[1] = v2;
//		vertices[2] = v3;
//	}
//
//	glm::vec3 getCenter() {
//		glm::vec3 center;
//		center.x = (vertices[0].x + vertices[1].x + vertices[2].x) / 3.0f;
//		center.y = (vertices[0].y + vertices[1].y + vertices[2].y) / 3.0f;
//		center.z = (vertices[0].z + vertices[1].z + vertices[2].z) / 3.0f;
//		return center;
//	}
//
//	void Rasterize(glm::mat4 mvp) {
//		glm::vec2 mvpVertices[3];
//		for (int i = 0; i < 3; i++) {
//			glm::vec4 temp = glm::vec4(vertices[i], 1.0f) * mvp;
//			if (temp.w != 0)
//				temp /= temp.w;
//
//			mvpVertices[i] = glm::vec2(temp.x, temp.y);
//			mvpVertices[i] += glm::vec2(1.0f);
//			mvpVertices[i].x = (mvpVertices[i].x / 2.0f) * (float)WIDTH;
//			mvpVertices[i].y = (mvpVertices[i].y / 2.0f) * (float)HEIGHT;
//		}
//
//		std::vector<glm::vec2> linePixels[3];
//		for (int i = 0; i < 3; i++) {
//			linePixels[i] = GetLinePixels(mvpVertices[i], mvpVertices[(i + 1) % 3]);
//			DrawLine(linePixels[i]);
//		}
//
//		std::vector<glm::vec2> allBorderPixels;
//		allBorderPixels.reserve(sizeof(linePixels) / sizeof(glm::vec2));
//		for (int i = 0; i < 3; i++)
//			allBorderPixels.insert(allBorderPixels.end(), linePixels[i].begin(), linePixels[i].end());
//
//		std::vector<int> allXs[HEIGHT];
//		for (int i = 0; i < (int)allBorderPixels.size(); i++) {
//			if (allBorderPixels[i].y >= 0 && allBorderPixels[i].y < HEIGHT)
//				allXs[(int)allBorderPixels[i].y].push_back((int)allBorderPixels[i].x);
//		}
//
//		for (int i = 0; i < HEIGHT; i++) {
//			if ((int)allXs[i].size() == 0) continue;
//			int min = *min_element(allXs[i].begin(), allXs[i].end());
//			if (min < 0) min = 0;
//			int max = *max_element(allXs[i].begin(), allXs[i].end());
//			if (max >= WIDTH)max = WIDTH - 1;
//
//			for (int j = min; j <= max; j++) {
//				if (frontBuffer[i][j] == ' ')
//					Dot(glm::vec2(j, i));
//			}
//		}
//	}
//
//	void Rotate(float radian, glm::vec3 axis)
//	{
//		if (axis == glm::vec3(0.0f)) return;
//
//		glm::vec3 center = getCenter();
//
//		for (int i = 0; i < 3; i++)
//			vertices[i] = glm::rotate(vertices[i], radian, axis);
//	}
//};