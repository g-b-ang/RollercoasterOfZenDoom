#pragma once
#include "Geometry.h"



Geometry::Geometry(char * type)
{
	name = type;
	toWorld = glm::mat4(1.0f);
	tick = 0;
	int id = 1;

	// Read object file and populate vertices, normals, faces 
	parse(type);
	// center();

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

Geometry::~Geometry()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO);
}

void Geometry::draw(glm::mat4 C)
{
	modelview = Window::V * C;
	render(Window::lshaderProgram);
}

void Geometry::render(GLuint shaderProgram) {

	float idColor = (float)id / 255;
	color = glm::vec3(idColor, 0.0f, 0.0f);
	modelview *= glm::scale(glm::mat4(), glm::vec3(10.0f));
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");
	uColor = glGetUniformLocation(shaderProgram, "ucolor");
	rMode = glGetUniformLocation(shaderProgram, "rMode");
	glUniform1i(rMode, 0);

	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	glUniform3fv(uColor, 1, &(color[0]));

	// Now draw the cube. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);
	// Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_LINE_STRIP, 0, vertices.size());

	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
	
}

void Geometry::update()
{

}

char * Geometry::getName()
{
	return name;
}

void Geometry::print()
{
	std::cout << getName() << "\n";
}

/* OBJ import */
void Geometry::parse(const char * filepath) {
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

void Geometry::center() {

}

void Geometry::setMat(glm::mat4)
{
}

glm::mat4 Geometry::getMat()
{
	return glm::mat4();
}

glm::mat4 Geometry::getWorld()
{
	return toWorld;
}

void Geometry::setId(int i)
{
	id = i;
}
