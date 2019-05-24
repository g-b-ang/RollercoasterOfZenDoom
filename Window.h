#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include "Cube.h"
#include "shader.h"
#include "Geometry.h"
#include "Transform.h"
#include "OBJObject.h"
#include "Curveline.h"
#include "Line.h"

class Window
{
public:
	static glm::mat4 P; // P for projection
	static glm::mat4 V; // V for view

	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);

	// Callback methods
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
	static void moveRide();
	static void camRide();
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void selectPoint(double, double);
	static void movePoint(double, double);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void moveCamera(double, double);
	static void zoomCamera(int);
	static void moveGroup(double, double);

	// Maps 2d mouse coordinates to 3d trackball
	static glm::vec3 trackBall(double, double);

	// State of the mouse buttons being clicked
	static int mState;
	static bool selectPoints;

	// Window dimensions
	static int width;
	static int height;

	static glm::vec3 cam_pos;

	static double timeCheck;

	// Previous mouse coords
	static double prevXpos, prevYpos;

	static GLuint shaderProgram;
	static GLuint lshaderProgram;

};

#endif
