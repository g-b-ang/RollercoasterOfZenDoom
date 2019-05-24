#pragma once
#include "OBJObject.h"

#include <iostream>
#include <algorithm>


OBJObject::OBJObject(const char *filepath)
{
	toWorld = glm::mat4(1.0f);
	pointSize = 1.0;

	mats = {
		glm::vec3(),
		glm::vec3(),
		glm::vec3(),
		0.0f,
		glm::vec3()
	};

	// Read object file and populate vertices, normals, faces 
	parse(filepath);
	//center();

	// Create array object and buffers.
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO2);
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

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);

	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);

	// We've sent the vertex data over to OpenGL, but there's still something missing.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	glBindVertexArray(0);
}

OBJObject::~OBJObject()
{
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO);

}

void OBJObject::setMat(glm::vec3 a, glm::vec3 d, glm::vec3 sp, float sh, glm::vec3 col) {
	mats = {
		a,
		d,
		sp,
		sh,
		col
	};
}

void OBJObject::parse(const char *filepath)
{
	// Populate the face indices, vertices, and normals vectors with the OBJ Object data

	using namespace glm;
	// Open file for reading
	FILE * pFile;
	pFile = fopen(filepath, "rb");

	// Check if file is valid
	if (pFile == NULL) {
		perror("Could not open file");
	}

	// Store vertex definition
	float x, y, z, r, g, b;

	// Store face indices
	int vi1, vi2, vi3;

	// Read chars to check for v/vn/f
	int c1 = 0, c2;

	vec3 newVec = vec3();

	// Read lines in file until EOF is reached
	while (c1 != EOF) {
		// Checks identifier for vectors
		c1 = fgetc(pFile);
		c2 = fgetc(pFile);

		// Skip comments
		if (c1 == '#') {
			fscanf(pFile, "%*[^\n]\n");
		}
		// If a vector, store as vector
		else if (c1 == 'v' && c2 == ' ') {
			fscanf(pFile, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
			newVec = vec3(x, y, z);
			vertices.push_back(newVec);
		}
		// Store as vector-normal
		else if (c1 == 'v' && c2 == 'n') {
			fscanf(pFile, "%f %f %f", &x, &y, &z);
			newVec = vec3(x, y, z);
			normals.push_back(newVec);
		}
		else if (c1 == 'f' && c2 == ' ') {
			fscanf(pFile, "%d %*[/] %*d %d %*[/] %*d %d %*[/] %*d", &vi1, &vi2, &vi3);
			// Offset each by 1 to match C++ indecies
			indices.push_back(vi1 - 1);
			indices.push_back(vi2 - 1);
			indices.push_back(vi3 - 1);
		}
	}

	fclose(pFile);

}

void OBJObject::draw(GLuint shaderProgram)
{
	modelview = Window::V * toWorld;

	// We need to calculate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");
	uColor = glGetUniformLocation(shaderProgram, "ucolor");
	uCamPos = glGetUniformLocation(shaderProgram, "cameraPos");


	glm::vec3 color = glm::vec3(0.5f,0.21f, 0.9f);

	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	glUniform3fv(uColor, 1, &(color[0]));
	glUniform3fv(uCamPos, 1, &(Window::cam_pos[0]));
	rMode = glGetUniformLocation(shaderProgram, "rMode");
	glUniform1i(rMode, 1);

	/* Shading experiementation stuff */
	matAmbient = glGetUniformLocation(shaderProgram, "material.ambient");
	matDiffuse = glGetUniformLocation(shaderProgram, "material.diffuse");
	matSpecular = glGetUniformLocation(shaderProgram, "material.specular");
	matShine = glGetUniformLocation(shaderProgram, "material.shininess");
	matColor = glGetUniformLocation(shaderProgram, "material.color");

	glUniform3fv(matAmbient, 1, &(mats.ambience[0]));
	glUniform3fv(matDiffuse, 1, &(mats.diffuse[0]));
	glUniform3fv(matSpecular, 1, &(mats.specular[0]));
	glUniform1f(matShine, mats.shininess);
	glUniform3fv(matColor, 1, &(mats.color[0]));

	// Draw the object. Bind the VAO associated with it.
	glBindVertexArray(VAO);
	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	// glDrawArrays(GL_LINE_STRIP, 0, vertices.size());
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}


void OBJObject::center() {

	// Find min/max x coord
	float xMax = 0;
	for (int j = 0; j < vertices.size(); j++) {
		if (vertices[j].x > xMax) {
			xMax = vertices[j].x;
		}
	}
	float xMin = (float)Window::width;
	for (int j = 0; j < vertices.size(); j++) {
		if (vertices[j].x < xMin) {
			xMin = vertices[j].x;
		}
	}

	// Find min/max y coord
	float yMax = 0;
	for (int j = 0; j < vertices.size(); j++) {
		if (vertices[j].y > yMax) {
			yMax = vertices[j].y;
		}
	}
	float yMin = (float)Window::width;
	for (int j = 0; j < vertices.size(); j++) {
		if (vertices[j].y < yMin) {
			yMin = vertices[j].y;
		}
	}

	// Find min/max z coord
	float zMax = 0;
	for (int j = 0; j < vertices.size(); j++) {
		if (vertices[j].z > zMax) {
			zMax = vertices[j].z;
		}
	}
	float zMin = 1000;
	for (int j = 0; j < vertices.size(); j++) {
		if (vertices[j].z < zMin) {
			zMin = vertices[j].z;
		}
	}

	// Get the midpoints
	float xmp = (xMax + xMin) / 2;
	float ymp = (yMax + yMin) / 2;
	float zmp = (zMax + zMin) / 2;

	// Find largest x/y/z coordinate
	std::vector<float> sorting;
	sorting.push_back(xMax);
	sorting.push_back(yMax);
	sorting.push_back(zMax);
	sorting.push_back(abs(xMin));
	sorting.push_back(abs(yMin));
	sorting.push_back(abs(zMin));

	std::sort(sorting.begin(), sorting.end());
	float scaleFactor = sorting[2];

	// Take the reciprocal so that when scaled, every value is 1/-1 or less
	sFactor = (1 / scaleFactor);

	//float sf = 1.0f;
	//while (xMax*sf > 1 || xMin*sf < -1 || yMax*sf > 1 || yMin*sf < -1 || zMax*sf > 1 || zMin*sf < -1) {
	//	sf *= 0.95;
	//}
	//std::cout << sf << "\n";

	toWorld = glm::translate(glm::mat4(1.0f), glm::vec3(-xmp, -ymp, -zmp)) * toWorld;

	toWorld = glm::scale(glm::mat4(1.0f), glm::vec3(sFactor)) * toWorld;

	toWorldCopy = toWorld;
}
void OBJObject::update()
{

	t += (1.0f / 200.0f);
	if (t > 1) t -= 1;
}

void OBJObject::spin(float deg)
{
	// This creates the matrix to rotate the object
	// toWorld = glm::rotate(glm::mat4(1.0f), this->angle / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 0.1f, 0.0f));
	glm::mat4 rMat = glm::rotate(glm::mat4(), deg, glm::vec3(0, 1, 0));
	toWorld *= rMat;

}

void OBJObject::transl8(int state)
{
	// Move right
	if (state == 0) {
		glm::mat4 tMat = glm::translate(glm::mat4(), glm::vec3(1.0f, 0.0f, 0.0f));
		this->toWorld = tMat * toWorld;
	}
	// Move left
	if (state == 1) {
		glm::mat4 tMat = glm::translate(glm::mat4(), glm::vec3(-1.0f, 0.0f, 0.0f));
		this->toWorld = tMat * toWorld;
	}
	// Move up
	if (state == 2) {
		glm::mat4 tMat = glm::translate(glm::mat4(), glm::vec3(0.0f, 1.0f, 0.0f));
		this->toWorld = tMat * toWorld;
	}
	// Move down
	if (state == 3) {
		glm::mat4 tMat = glm::translate(glm::mat4(), glm::vec3(0.0f, -1.0f, 0.0f));
		this->toWorld = tMat * toWorld;
	}
	// Move forward
	if (state == 4) {
		glm::mat4 tMat = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 1.0f));
		this->toWorld = tMat * toWorld;
	}
	// Move back
	if (state == 5) {
		glm::mat4 tMat = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -1.0f));
		this->toWorld = tMat * toWorld;
	}

}

void OBJObject::translate(double x, double y) {
	float zPos = toWorld[3][2];
	if (zPos < 1 && zPos > -1) zPos = 1;
	if (zPos > 1) zPos = 3 / zPos;
	if (zPos < -1) zPos /= 10;
	zPos = abs(zPos)*0.05f;
	glm::mat4 tMat = glm::translate(glm::mat4(), glm::vec3(x*zPos, (-y)*zPos, 0.0f));
	this->toWorld = tMat * toWorld;
}

void OBJObject::scale(int state)
{
	if (state == 0) {
		glm::mat4 tMat = glm::scale(glm::mat4(), glm::vec3(2.0f));
		toWorld *= tMat;

	}
	else {
		glm::mat4 tMat = glm::scale(glm::mat4(), glm::vec3(0.5f));
		toWorld *= tMat;
	}


}

void OBJObject::scale2() {

	glm::mat4 sMat = glm::scale(glm::mat4(1.0f), glm::vec3(sFactor));
	toWorld *= sMat;
}

void OBJObject::orbit(int x, int y) {


	//float zPos = toWorld[3][2];
	//if (zPos < 1 && zPos > -1) zPos = 1;
	//if (zPos > 1) zPos = 3 / zPos;
	//if (zPos < -1) zPos /= 10;
	//zPos = abs(zPos)*0.5;

	glm::mat4 rMat = glm::rotate(glm::mat4(), 0.05f * x, glm::vec3(0, 1, 0));
	toWorld = rMat*toWorld;
	rMat = glm::rotate(glm::mat4(), 0.05f * y, glm::vec3(1, 0, 0));
	toWorld = rMat*toWorld;

	//glm::mat4 rMat = glm::rotate(glm::mat4(), 0.05f, v);
	//toWorld = rMat*toWorld;

}

void OBJObject::rotation(double x, double y) {

	glm::vec3 v = Window::trackBall(Window::prevXpos, Window::prevYpos);
	glm::vec3 w = Window::trackBall(x, y);

	// Rotational axis is cross product v x w
	glm::vec3 rotAxis = glm::cross(v, w);

	// Rotational angle is: dot product v*w = |a||b|cos(theta)
	// since |a||b| == 1, just take arccos(dotproduct)
	float dotProduct = glm::dot(v, w);
	float angle = acos(dotProduct);

	// Multiply rotation matrix by model matrix
	//glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), angle/180.0f * glm::pi<float>(), rotAxis);
	glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), angle, rotAxis);

	toWorld = rMat * toWorld;

}

void OBJObject::reset(int state) {
	if (state == 0) {

		// Reset translation vector
		glm::vec3 tVec = glm::vec3();
		tVec.x = -(toWorld[3][0]);
		tVec.y = -(toWorld[3][1]);
		tVec.z = -(toWorld[3][2]);
		glm::mat4 tMat = glm::translate(glm::mat4(), tVec);
		toWorld = tMat * toWorld;

	}
	else {

		// Reset rotation matrix to the identity matrix
		center();

		size = 1;
		pointSize = 1;
	}
}

/* Mat testing methods*/
void OBJObject::cAmb(int state) {
	if (state == 0) {
		mats.ambience += 0.1;
	}
	else {
		mats.ambience -= 0.1;
	}
	mats.ambience = abs(mats.ambience);
}
void OBJObject::cDif(int state) {
	if (state == 0) {
		mats.diffuse += 0.1;
	}
	else {
		mats.diffuse -= 0.1;
	}
	mats.diffuse = abs(mats.diffuse);
}
void OBJObject::cSpe(int state) {
	if (state == 0) {
		mats.specular += 0.1;
	}
	else {
		mats.specular -= 0.1;
	}
	mats.specular = abs(mats.specular);
}
void OBJObject::cShi(int state) {
	if (state == 0) {
		mats.shininess *= 1.2f;
	}
	else {
		mats.shininess /= 1.2f;
	}
	if (mats.shininess > 1) {
		mats.shininess = 1.0f;
	}
}
