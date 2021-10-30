//#pragma once
//#include "Rasterize.h"
//
//struct MyRectangle {
//	glm::vec3 vertices[4];
//	glm::vec3 center;
//	glm::vec2 size;
//
//	MyRectangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4) {
//		MakeVertices(v1, v2, v3, v4);
//	}
//
//	void MakeVertices(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4) {
//		vertices[0] = v1;
//		vertices[1] = v2;
//		vertices[2] = v3;
//		vertices[3] = v4;
//	}
//
//	MyRectangle(glm::vec3 center, glm::vec2 size) {
//		this->center = center;
//		this->size = size;
//		MakeVertices(
//			glm::vec3(center.x - size.x / 2, center.y - size.y / 2, center.z),
//			glm::vec3(center.x + size.x / 2, center.y - size.y / 2, center.z),
//			glm::vec3(center.x + size.x / 2, center.y + size.y / 2, center.z),
//			glm::vec3(center.x - size.x / 2, center.y + size.y / 2, center.z));
//	}
//
//	float map(float input, float input_start, float input_end, float output_start, float output_end) {
//		float slope = 1.0f * (output_end - output_start) / (input_end - input_start);
//		return (output_start + slope * (input - input_start));
//	}
//
//	void Rasterize(glm::mat4 mvp) {
//
//		Triangle(vertices[0], vertices[1], vertices[2]).Rasterize(mvp);
//		Triangle(vertices[0], vertices[2], vertices[3]).Rasterize(mvp);
//	}
//
//	void Rotate(float radian, glm::vec3 axis)
//	{
//		if (axis == glm::vec3(0.0f)) return;
//
//		for (int i = 0; i < 4; i++)
//			vertices[i] = center + glm::rotate(vertices[i] - center, radian, axis);
//	}
//};