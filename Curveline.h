#pragma once
#ifndef CURVELINE_H
#define CURVELINE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Window.h"

class Curveline
{
public:
	Curveline(Transform*, Transform*, Transform*, Transform*);
	~Curveline();
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;

	// Holds control points
	std::vector<Transform*> cPts;

	void genPoints();
	float bern(float t, int it);
	glm::vec3 bezCoord(float t);
	float curveLen;

	int factorial(int f);

	void draw(GLuint);

	GLuint VAO, VBO, EBO;
	GLuint uProjection, uModelview, uColor;
	glm::mat4 modelview;
	glm::mat4 toWorld;



};

#endif