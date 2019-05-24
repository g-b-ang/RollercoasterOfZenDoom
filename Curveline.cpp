#include "Curveline.h"
#include <math.h>
#include <iostream>

Curveline::Curveline( Transform* p1, Transform* p2, Transform* p3, Transform* p4)
{
	cPts.push_back(p1);
	cPts.push_back(p2);
	cPts.push_back(p3);
	cPts.push_back(p4);
	genPoints();
}


Curveline::~Curveline()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	std::vector<Transform*>temp;
	cPts.swap(temp);
}

void Curveline::genPoints()
{
	// t value in the range of [0-1]
	float tInterval;
	curveLen = 0;
	glm::vec3 q = glm::vec3(0.0f);
	std::vector<glm::vec3> verticesSwap;
	vertices.swap(verticesSwap);
	// Extract each line segment from each t
	for (int t = 0; t <= 200; t++) {
		q = glm::vec3(0.0f);
		tInterval = (float)t / 200;
		// Get the average at t by using each control point
		for (int j = 0; j < 4; j++) {
			q += bern(tInterval,j) * cPts[j]->getVertex();
			//printf("%f, %f, %f\n", cPts[j].x, cPts[j].y, cPts[j].z);

		}
		vertices.push_back(q);
		// if t > 1 then start adding vertices??
		if (t > 0) {
			// Adds the distance between each point
			curveLen += sqrt(pow(vertices[t].x-vertices[t-1].x, 2)+pow(vertices[t].y - vertices[t - 1].y, 2)+pow(vertices[t].z - vertices[t - 1].z, 2));
		}
	}

	std::vector<unsigned int> indicesSwap;
	indices.swap(indicesSwap);
	for (int i = 0; i < vertices.size(); i++) {
		indices.push_back(i);
		indices.push_back(i);
		indices.push_back(i);
	}

	// Delete old ones
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// Create array object and buffers.
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
	// Consider the VAO as a container for all your buffers.
	glBindVertexArray(VAO);

	// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
	// you want to draw, such as vertices, normals, colors, etc.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

	// Enable the usage of layout location 0 (check the vertex shader to see what this is)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	// We've sent the vertex data over to OpenGL, but there's still something missing.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	glBindVertexArray(0);

	//for (int i = 0; i < vertices.size(); i++) {
	//	printf("%f, %f, %f\n", vertices[i].x, vertices[i].y, vertices[i].z);
	//}
}

float Curveline::bern(float t, int it)
{
	float result;
	result = (factorial(3) / (factorial(3 - it) * factorial(it))) * pow(t, it) * pow(1 - t, 3 - it);
	return result;
}

glm::vec3 Curveline::bezCoord(float t)
{
	glm::vec3 result = glm::vec3(0.0f);
	// Get the average at t by using each control point
	for (int j = 0; j < 4; j++) {
		result += bern(t, j) * cPts[j]->getVertex();
	}
	return result;
}

int Curveline::factorial(int f)
{
	if (f == 0) {
		return 1;
	}

	int result = f;
	while (f > 1) {
		result *= --f;
	}
	return result;
}

void Curveline::draw(GLuint shaderProgram)
{
	modelview = Window::V * toWorld;
	glm::vec3 color = glm::vec3(0.0f);

	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");
	uColor = glGetUniformLocation(shaderProgram, "ucolor");

	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	glUniform3fv(uColor, 1, &(color[0]));

	// Draw the object. Bind the VAO associated with it.
	glBindVertexArray(VAO);
	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	//glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glLineWidth(3.0f);
	glDrawArrays(GL_LINE_STRIP, 0, vertices.size());
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);

}
