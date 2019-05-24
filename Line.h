#pragma once
#ifndef LINE_H
#define LINE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Window.h"

class Line
{
public:
	Line(Transform*, Transform*);
	~Line();
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;

	// Holds control points
	std::vector<Transform*> cPts;

	void genPoints();
	void draw(GLuint);

	GLuint VAO, VBO, EBO;
	GLuint uProjection, uModelview, uColor;
	glm::mat4 modelview;
	glm::mat4 toWorld;

};

#endif