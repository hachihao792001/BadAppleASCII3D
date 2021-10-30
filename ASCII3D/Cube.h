//#pragma once
//#include "Rectangle.h"
//
//struct Cube {
//	glm::vec3 vertices[8];
//	glm::vec3 center;
//	glm::vec3 size;
//
//	Cube(glm::vec3 center, glm::vec3 size) {
//		this->center = center;
//		this->size = size;
//		vertices[0] = glm::vec3(center.x - size.x / 2, center.y - size.y / 2, center.z - size.z / 2);
//		vertices[1] = glm::vec3(center.x + size.x / 2, center.y - size.y / 2, center.z - size.z / 2);
//		vertices[2] = glm::vec3(center.x + size.x / 2, center.y - size.y / 2, center.z + size.z / 2);
//		vertices[3] = glm::vec3(center.x - size.x / 2, center.y - size.y / 2, center.z + size.z / 2);
//
//		vertices[4] = glm::vec3(center.x - size.x / 2, center.y + size.y / 2, center.z - size.z / 2);
//		vertices[5] = glm::vec3(center.x + size.x / 2, center.y + size.y / 2, center.z - size.z / 2);
//		vertices[6] = glm::vec3(center.x + size.x / 2, center.y + size.y / 2, center.z + size.z / 2);
//		vertices[7] = glm::vec3(center.x - size.x / 2, center.y + size.y / 2, center.z + size.z / 2);
//
//		vertices[0] = glm::vec3(center.x - size.x / 2, center.y - size.y / 2, center.z - size.z / 2);
//		vertices[3] = glm::vec3(center.x - size.x / 2, center.y - size.y / 2, center.z + size.z / 2);
//		vertices[7] = glm::vec3(center.x - size.x / 2, center.y + size.y / 2, center.z + size.z / 2);
//		vertices[4] = glm::vec3(center.x - size.x / 2, center.y + size.y / 2, center.z - size.z / 2);
//
//		vertices[1] = glm::vec3(center.x + size.x / 2, center.y - size.y / 2, center.z - size.z / 2);
//		vertices[2] = glm::vec3(center.x + size.x / 2, center.y - size.y / 2, center.z + size.z / 2);
//		vertices[6] = glm::vec3(center.x + size.x / 2, center.y + size.y / 2, center.z + size.z / 2);
//		vertices[5] = glm::vec3(center.x + size.x / 2, center.y + size.y / 2, center.z - size.z / 2);
//
//		vertices[0] = glm::vec3(center.x - size.x / 2, center.y - size.y / 2, center.z - size.z / 2);
//		vertices[1] = glm::vec3(center.x + size.x / 2, center.y - size.y / 2, center.z - size.z / 2);
//		vertices[5] = glm::vec3(center.x + size.x / 2, center.y + size.y / 2, center.z - size.z / 2);
//		vertices[4] = glm::vec3(center.x - size.x / 2, center.y + size.y / 2, center.z - size.z / 2);
//
//		vertices[3] = glm::vec3(center.x - size.x / 2, center.y - size.y / 2, center.z + size.z / 2);
//		vertices[2] = glm::vec3(center.x + size.x / 2, center.y - size.y / 2, center.z + size.z / 2);
//		vertices[6] = glm::vec3(center.x + size.x / 2, center.y + size.y / 2, center.z + size.z / 2);
//		vertices[7] = glm::vec3(center.x - size.x / 2, center.y + size.y / 2, center.z + size.z / 2);
//	}
//
//	void Rasterize(glm::mat4 mvp) {
//		MyRectangle rec1(vertices[0], vertices[1], vertices[2], vertices[3]);
//		MyRectangle rec2(vertices[4], vertices[5], vertices[6], vertices[7]);
//		MyRectangle rec3(vertices[0], vertices[3], vertices[7], vertices[4]);
//		MyRectangle rec4(vertices[1], vertices[2], vertices[6], vertices[5]);
//		MyRectangle rec5(vertices[0], vertices[1], vertices[5], vertices[4]);
//		MyRectangle rec6(vertices[3], vertices[2], vertices[6], vertices[7]);
//
//		rec1.Rasterize(mvp);
//		rec2.Rasterize(mvp);
//		rec3.Rasterize(mvp);
//		rec4.Rasterize(mvp);
//		rec5.Rasterize(mvp);
//		rec6.Rasterize(mvp);
//	}
//
//	void Rotate(float radian, glm::vec3 axis)
//	{
//		if (axis == glm::vec3(0.0f)) return;
//
//		for (int i = 0; i < 8; i++)
//			vertices[i] = center + glm::rotate(vertices[i] - center, radian, axis);
//	}
//};