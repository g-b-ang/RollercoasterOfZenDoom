#pragma once
#ifndef OBJOBJECT_H
#define OBJOBJECT_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Window.h"

class OBJObject
{
private:

public:
	OBJObject(const char* filepath);
	~OBJObject();

	void parse(const char* filepath);
	void update();
	void draw(GLuint);
	void spin(float);
	void transl8(int);
	void translate(double, double);
	void scale(int);
	void orbit(int, int);
	void rotation(double, double);
	void reset(int);
	void center();
	void setMat(glm::vec3, glm::vec3, glm::vec3, float, glm::vec3);
	void scale2();

	/* Mat Testing Methods*/
	void OBJObject::cAmb(int state);
	void OBJObject::cDif(int state);
	void OBJObject::cSpe(int state);
	void OBJObject::cShi(int state);

	float size;
	float pointSize;
	int renderMode;
	int oMode;
	float sFactor;
	glm::mat4 toWorldCopy;
	glm::mat4 modelview;
	float t = 0;

	struct Material {
		glm::vec3 ambience;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float shininess;
		glm::vec3 color;
	};

	Material mats;

	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> vertices;

	glm::mat4 toWorld;

	// These variables are needed for the shader program
	GLuint VBO, VAO, EBO, VBO2;
	GLuint uProjection, uModelview, matAmbient, matDiffuse, matSpecular, matShine, matColor;
	GLuint lightPos, rMode, uColor, uCamPos;
};

#endif