#pragma once
#ifndef NODE_H
#define NODE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Node
{
public:
	//Node(void) {};
	//~Node(void) {};
	virtual char * getName() = 0;
	virtual void update() = 0;
	virtual void draw(glm::mat4) = 0;
	virtual void print() = 0;
	virtual void setMat(glm::mat4) = 0;
	virtual glm::mat4 getMat() = 0;
	virtual glm::mat4 getWorld() = 0;
	virtual void setId(int) = 0;

	glm::vec3 animAxis;
	bool animate;
	bool point;
	glm::mat4 tMat;
	float id;
	int tick;
	int tickMax;
	int tickMin;
	int tickDir;

private:
	char * name;

};


#endif