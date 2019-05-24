#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vector>
#include <iostream>
#include "Node.h"


class Transform :
	public Node
{
public:
	Transform();
	Transform(char *, glm::mat4);

	~Transform();
	glm::mat4 tMat;
	glm::mat4 toWorld;
	std::vector<Node*> children;
	void removeChild(char*);
	void addChild(Node*);
	void draw(glm::mat4);
	void update();
	char* getName();
	void print();
	glm::vec3 getVertex();
	void setId(int);

	bool animate;
	bool point;
	glm::vec3 animAxis;
	glm::mat4 originTMat;
	float id;

	int tick;
	int tickMax;
	int tickMin;
	int tickDir;
	//Node * parent;
	void Transform::setMat(glm::mat4);
	glm::mat4 getMat();
	glm::mat4 getWorld();

private:
	char * name;
};

#endif