#pragma once
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <algorithm>
#include "Node.h"
#include "Window.h"

class Geometry :
	public Node
{
public:
	Geometry(char*);
	~Geometry();

	// Load a 3D model
	void draw(glm::mat4);
	void update();
	char* getName();
	void print();

	/* Importing OBJ */
	void parse(const char* filepath);
	void render(GLuint);
	void center();
	void setMat(glm::mat4);
	glm::mat4 getMat();
	glm::mat4 getWorld();
	void setId(int);

	float sFactor;

	glm::mat4 modelview;

	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> vertices;

	glm::mat4 toWorld;

	glm::vec3 color;
	int id;

	// These variables are needed for the shader program
	GLuint VBO, VAO, EBO, VBO2;
	GLuint uProjection, uModelview, uColor, rMode;

	int tick;
	int tickMax;
	int tickMin;
	int tickDir;

	bool animate;

private:
	char * name;
};

#endif