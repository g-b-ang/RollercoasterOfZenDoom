#pragma once
#include "Transform.h"

Transform::Transform()
{
	this->tMat = glm::mat4(1.0f);
}
Transform::Transform(char * name, glm::mat4 T)
{
	this->name = name;
	tMat = T;
	tick = 0;
	animAxis = glm::vec3(0.0f);
	animate = false;
	point = true;
	originTMat = glm::mat4(1.0f);
	toWorld = glm::mat4(1.0f);
}


Transform::~Transform()
{
	for (int i = 0; i < children.size(); i++) {
		delete(children[i]);
		children.erase(children.begin() + i);
	}

}

void Transform::removeChild(char* name)
{
	// Loop through all children and look for a matching name,
	// If found, then free up pointer and remove from array
	for (int i = 0; i < children.size(); i++) {
		if (children[i]->getName() == name) {
			free(children[i]);
			children.erase(children.begin() + i);
		}
	}
}

void Transform::addChild(Node* child)
{
	children.push_back(child);
}

void Transform::draw(glm::mat4 C)
{
	for (int i = 0; i < children.size(); i++) {
		if (point) {
			children[i]->setId(id);
		}
		children[i]->draw(C * tMat);
	}
}

void Transform::update()
{
	glm::mat4 rMat = glm::mat4(1.0f);
	// Only access this if specifically an "animation" Transform
	if (animate) {
		if (tick == tickMax) {
			tickDir = -1;
		}
		if (tick == tickMin) {
			tickDir = 1;
		}
		tick = tick + tickDir;
		rMat = glm::rotate(glm::mat4(1.0f), (float)tickDir / 180.0f * glm::pi<float>(), animAxis);
		for (int i = 0; i < children.size(); i++) {
			// children[i]->setMat( glm::inverse(originTMat) * rMat * originTMat * children[i]->getMat());
			// children[i]->setMat(glm::inverse(originTMat) * rMat * originTMat);
			// children[i]->setMat(originTMat * rMat * children[i]->getMat());

			children[i]->setMat(children[i]->getMat() * glm::inverse(originTMat) * rMat * originTMat);


		}
	}
	for (int i = 0; i < children.size(); i++) {
		children[i]->update();
	}
}

void Transform::print()
{
	std::cout << getName() << "\n";
	for (int i = 0; i < children.size(); i++) {
		children[i]->print();
	}
}

glm::vec3 Transform::getVertex()
{
	glm::vec3 result;
	result.x = tMat[3][0];
	result.y = tMat[3][1];
	result.z = tMat[3][2];

	return result;
}

void Transform::setId(int)
{
}

char * Transform::getName()
{
	return name;
}

void Transform::setMat(glm::mat4 mat) {
	this->tMat = mat;
}
glm::mat4 Transform::getMat() {
	return tMat;
}

glm::mat4 Transform::getWorld()
{
	return toWorld;
}
