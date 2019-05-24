#include "Window.h"
#include <time.h>

const char* window_title = "GLFW Starter Project";
Cube * cube;

OBJObject * ride;

GLuint Window::shaderProgram;
GLuint Window::lshaderProgram;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "../shader.vert"
#define FRAGMENT_SHADER_PATH "../shader.frag"

#define lVERTEX_SHADER_PATH "../lshader.vert"
#define lFRAGMENT_SHADER_PATH "../lshader.frag"

// Default camera parameters
glm::vec3 Window::cam_pos(0.0f, 0.0f, 500.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

glm::vec3 old_cam_pos(0.0f, 0.0f, 500.0f);		// e  | Position of camera
glm::vec3 old_cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 old_cam_up(0.0f, 1.0f, 0.0f);

int Window::width;
int Window::height;

Transform *modelMatrix;

// Each line is a curve whutwhut
Transform *p1, *p2, *p3, *p4;
Transform *p5, *p6, *p7;
Transform *p8, *p9, *p10;
Transform *p11, *p12, *p13;
Transform *p14, *p15, *p16;
Transform *p17, *p18, *p19;
Transform *p20, *p21, *p22;
Transform *p23, *p24;

std::vector<Transform*> cPts;
std::vector<Curveline*> curves;
std::vector<Line*> lines;


int selectIdx = 0;
int curveIdx = 0;
int highestPoint = 0;
float timet;
float maxheight = 0;
float friction = 1;
bool move = true;
bool rideCamera = false;
float constPush = 0.0005;


Curveline *curve1, *curve2, *curve3, *curve4, *curve5, *curve6, *curve7, *curve8;
Line *line1, *line2, *line3,*line4, *line5, *line6, *line7, *line8;

int Window::mState;
bool Window::selectPoints = true;

glm::mat4 Window::P;
glm::mat4 Window::V;

double Window::prevXpos, Window::prevYpos;

double Window::timeCheck = 0;

void Window::initialize_objects()
{
	// mat4 used for making initial transformation matrices
	glm::mat4 initTMat = glm::mat4(1.0f);

	// mat4 used for rotating around pivot point
	glm::mat4 oTMat = glm::mat4(1.0f);

	// mat4 used for making rotational matrices
	glm::mat4 rMat = glm::mat4(1.0f);

	// used for determining the nonanchor control points
	glm::vec3 controlVec = glm::vec3(1.0f);

	// Initialize skycube
	cube = new Cube();

	ride = new OBJObject("sphere.obj");
	ride->toWorld = glm::translate(glm::mat4(),glm::vec3(0.0f, 800.0f, 0.0f)) * glm::scale(glm::mat4(), glm::vec3(20.0f));

	// Generate each geometry being used
	Geometry * control = new Geometry("sphere.obj");
	Geometry * anchor = new Geometry("sphere.obj");

	// Model & Body
	modelMatrix = new Transform("body", glm::mat4());
	modelMatrix->point = false;

	// Curve1
	initTMat = glm::translate(glm::mat4(), glm::vec3(0.0f, 800.0f, 0.0f));
	p1 = new Transform("p1", initTMat);
	p1->addChild(anchor);
	p1->id = 250;

	controlVec = glm::vec3(60.0f, -10.0f, 100.0f);
	initTMat = glm::translate(glm::mat4(), p1->getVertex() + controlVec);
	p2 = new Transform("p2", initTMat);
	p2->addChild(control);
	p2->id = 1;

	initTMat = glm::translate(glm::mat4(), glm::vec3(100.0f, -100.0f, 100.0f));
	p4 = new Transform("p4", initTMat);
	p4->addChild(anchor);
	p4->id = 249;

	controlVec = glm::vec3(50.0f, -30.0f, 180.0f);
	initTMat = glm::translate(glm::mat4(), p4->getVertex() + controlVec);
	p3 = new Transform("p3", initTMat);
	p3->addChild(control);
	p3->id = 2;

	curve1 = new Curveline(p1, p2, p3, p4);

	modelMatrix->addChild(p1);
	modelMatrix->addChild(p2);
	modelMatrix->addChild(p3);
	modelMatrix->addChild(p4);

	// Curve2
	initTMat = glm::translate(glm::mat4(), p4->getVertex() - controlVec);
	p5 = new Transform("p5", initTMat);
	p5->addChild(control);
	p5->id = 3;

	line1 = new Line(p3, p5);

	initTMat = glm::translate(glm::mat4(), glm::vec3(150.0f, 400.0f, 170.0f));
	p7 = new Transform("p7", initTMat);
	p7->addChild(anchor);
	p7->id = 248;

	controlVec = glm::vec3(130.0f, 0.0f, 200.0f);
	initTMat = glm::translate(glm::mat4(), p7->getVertex() + controlVec);
	p6 = new Transform("p6", initTMat);
	p6->addChild(control);
	p6->id = 4;

	curve2 = new Curveline(p4, p5, p6, p7);

	modelMatrix->addChild(p5);
	modelMatrix->addChild(p6);
	modelMatrix->addChild(p7);

	// Curve3
	initTMat = glm::translate(glm::mat4(), p7->getVertex() - controlVec);
	p8 = new Transform("p8", initTMat);
	p8->addChild(control);
	p8->id = 5;

	line2 = new Line(p6, p8);

	initTMat = glm::translate(glm::mat4(), glm::vec3(-400.0f, -200.0f, 200.0f));
	p10 = new Transform("p10", initTMat);
	p10->addChild(anchor);
	p10->id = 247;

	controlVec = glm::vec3(140.0f, -60.0f, 230.0f);
	initTMat = glm::translate(glm::mat4(), p10->getVertex() + controlVec);
	p9 = new Transform("p9", initTMat);
	p9->addChild(control);
	p9->id = 6;

	curve3 = new Curveline(p7, p8, p9, p10);

	modelMatrix->addChild(p8);
	modelMatrix->addChild(p9);
	modelMatrix->addChild(p10);

	// Curve4
	initTMat = glm::translate(glm::mat4(), p10->getVertex() - controlVec);
	p11 = new Transform("p11", initTMat);
	p11->addChild(control);
	p11->id = 7;

	line3 = new Line(p9, p11);

	initTMat = glm::translate(glm::mat4(), glm::vec3(-220.0f, 200.0f, 170.0f));
	p13 = new Transform("p13", initTMat);
	p13->addChild(anchor);
	p13->id = 246;

	controlVec = glm::vec3(-170.0f, 10.0f, 60.0f);
	initTMat = glm::translate(glm::mat4(), p13->getVertex() + controlVec);
	p12 = new Transform("p12", initTMat);
	p12->addChild(control);
	p12->id = 8;

	curve4 = new Curveline(p10, p11, p12, p13);

	modelMatrix->addChild(p11);
	modelMatrix->addChild(p12);
	modelMatrix->addChild(p13);

	// Curve5
	initTMat = glm::translate(glm::mat4(), p13->getVertex() - controlVec);
	p14 = new Transform("p14", initTMat);
	p14->addChild(control);
	p14->id = 9;

	line4 = new Line(p12, p14);

	initTMat = glm::translate(glm::mat4(), glm::vec3(400.0f, -520.0f, 550.0f));
	p16 = new Transform("p16", initTMat);
	p16->addChild(anchor);
	p16->id = 245;

	controlVec = glm::vec3(-360.0f, -10.0f, -60.0f);
	initTMat = glm::translate(glm::mat4(), p16->getVertex() + controlVec);
	p15 = new Transform("p15", initTMat);
	p15->addChild(control);
	p15->id = 10;

	curve5 = new Curveline(p13, p14, p15, p16);

	modelMatrix->addChild(p14);
	modelMatrix->addChild(p15);
	modelMatrix->addChild(p16);

	// Curve6
	initTMat = glm::translate(glm::mat4(), p16->getVertex() - controlVec);
	p17 = new Transform("p17", initTMat);
	p17->addChild(control);
	p17->id = 11;

	line5 = new Line(p15, p17);

	initTMat = glm::translate(glm::mat4(), glm::vec3(-400.0f, 600.0f, -500.0f));
	p19 = new Transform("p19", initTMat);
	p19->addChild(anchor);
	p19->id = 244;

	controlVec = glm::vec3(-140.0f, -100.0f, 40.0f);
	initTMat = glm::translate(glm::mat4(), p19->getVertex() + controlVec);
	p18 = new Transform("p18", initTMat);
	p18->addChild(control);
	p18->id = 12;

	curve6 = new Curveline(p16, p17, p18, p19);

	modelMatrix->addChild(p17);
	modelMatrix->addChild(p18);
	modelMatrix->addChild(p19);

	// Curve7
	initTMat = glm::translate(glm::mat4(), p19->getVertex() - controlVec);
	p20 = new Transform("p20", initTMat);
	p20->addChild(control);
	p20->id = 13;

	line6 = new Line(p18, p20);

	initTMat = glm::translate(glm::mat4(), glm::vec3(-300.0f, -120.0f, 250.0f));
	p22 = new Transform("p22", initTMat);
	p22->addChild(anchor);
	p22->id = 243;

	controlVec = glm::vec3(-340.0f, -50.0f, -140.0f);
	initTMat = glm::translate(glm::mat4(), p22->getVertex() + controlVec);
	p21 = new Transform("p21", initTMat);
	p21->addChild(control);
	p21->id = 14;

	curve7 = new Curveline(p19, p20, p21, p22);

	modelMatrix->addChild(p20);
	modelMatrix->addChild(p21);
	modelMatrix->addChild(p22);

	// Curve8
	initTMat = glm::translate(glm::mat4(), p22->getVertex() - controlVec);
	p23 = new Transform("p23", initTMat);
	p23->addChild(control);
	p23->id = 15;

	line7 = new Line(p21, p23);

	initTMat = glm::translate(glm::mat4(), p1->getVertex() - glm::vec3(60.0f, 10.0f, 100.0f));
	p24 = new Transform("p24", initTMat);
	p24->addChild(control);
	p24->id = 16;

	line8 = new Line(p24, p2);

	curve8 = new Curveline(p22, p23, p24, p1);

	modelMatrix->addChild(p23);
	modelMatrix->addChild(p24);

	// Put all control points and curves into vectors to access for selection and curve update functionality
	cPts.push_back(p1);
	cPts.push_back(p2);
	cPts.push_back(p3);
	cPts.push_back(p4);
	cPts.push_back(p5);
	cPts.push_back(p6);
	cPts.push_back(p7);
	cPts.push_back(p8);
	cPts.push_back(p9);
	cPts.push_back(p10);
	cPts.push_back(p11);
	cPts.push_back(p12);
	cPts.push_back(p13);
	cPts.push_back(p14);
	cPts.push_back(p15);
	cPts.push_back(p16);
	cPts.push_back(p17);
	cPts.push_back(p18);
	cPts.push_back(p19);
	cPts.push_back(p20);
	cPts.push_back(p21);
	cPts.push_back(p22);
	cPts.push_back(p23);
	cPts.push_back(p24);

	curves.push_back(curve1);
	curves.push_back(curve2);
	curves.push_back(curve3);
	curves.push_back(curve4);
	curves.push_back(curve5);
	curves.push_back(curve6);
	curves.push_back(curve7);
	curves.push_back(curve8);

	lines.push_back(line1);
	lines.push_back(line2);
	lines.push_back(line3);
	lines.push_back(line4);
	lines.push_back(line5);
	lines.push_back(line6);
	lines.push_back(line7);
	lines.push_back(line8);

	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	lshaderProgram = LoadShaders(lVERTEX_SHADER_PATH, lFRAGMENT_SHADER_PATH);

}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(cube);
	delete(modelMatrix);
	//delete(p1);
	//delete(curve1);
	glDeleteProgram(shaderProgram);
	glDeleteProgram(lshaderProgram);
}

GLFWwindow* Window::create_window(int width, int height)
{
	mState = 0;
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	// Set the matrix mode to GL_PROJECTION to determine the proper camera properties
	glMatrixMode(GL_PROJECTION);
	// Load the identity matrix
	glLoadIdentity();

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 4000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{
	// Call the update function the cube
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the shader of programID
	glUseProgram(shaderProgram);

	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Set the matrix mode to GL_MODELVIEW
	glMatrixMode(GL_MODELVIEW);
	// Load the identity matrix
	glLoadIdentity();

	// Render the cube
	cube->draw(shaderProgram);

	// Use the shader of programID
	glUseProgram(lshaderProgram);

	if (rideCamera) {
		camRide();
	}
	if (!rideCamera) {
		ride->draw(lshaderProgram);
	}
	if (move) {
		moveRide();
	}

	// Render non-cube
	modelMatrix->draw(glm::mat4(1.0f));

	for (int i = 0; i < curves.size(); i++) {
		curves[i]->draw(lshaderProgram);
	}
	for (int i = 0; i < lines.size(); i++) {
		lines[i]->draw(lshaderProgram);
	}

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::moveRide() {
	//for (int i = 0; i < 4; i++) {
	//	if (curves[curveIdx]->cPts[i]->getVertex().y > maxheight) {
	//		maxheight = curves[curveIdx]->cPts[i]->getVertex().y;
	//	}
	//}

	maxheight = cPts[highestPoint]->getVertex().y;
	float velocity = sqrt((-2)*(-0.005)*(maxheight - ride->toWorld[3][1]));
	glm::mat4 tMat = glm::translate(glm::mat4(), curves[curveIdx]->bezCoord(timet)) * glm::scale(glm::mat4(), glm::vec3(20.0f));
	ride->toWorld = tMat;

	//std::cout << (velocity / (curves[curveIdx]->curveLen)) / friction + 0.0005 << "\n";
	timet += (velocity / (curves[curveIdx]->curveLen)) / friction + constPush;

	friction += 0.0003 * (curves[curveIdx]->curveLen/200);

	// Make curve 7 the accelerator curve
	if (curveIdx == 6) {
		friction *= 0.997;
	}

	if (timet > 1) timet -= 1, curveIdx += 1;
	if (timet < 0) timet += 1, curveIdx -= 1;
	
	if (curveIdx > curves.size() - 1) curveIdx = 0;
	if (curveIdx < 0) curveIdx = 7;

}

void Window::camRide() {
	float velocity = sqrt((-2)*(-0.005)*(maxheight - ride->toWorld[3][1]));
	cam_pos = glm::vec3(ride->toWorld[3][0], ride->toWorld[3][1], ride->toWorld[3][2]);
	cam_look_at = curves[curveIdx]->bezCoord(timet + 5*((velocity / (curves[curveIdx]->curveLen)) / friction + 0.0005));

	V = glm::lookAt(cam_pos, cam_look_at, cam_up);
}

glm::vec3 Window::trackBall(double x, double y) {
	glm::vec3 v;
	float d;
	v.x = (2.0 * x - width) / width;
	v.y = (height - 2.0 * y) / height;
	v.z = 0.0;
	d = glm::length(v);

	if (d > 1.0) d = 1.0;
	if (d < -1.0) d = -1.0;

	v.z = sqrtf(1.001 - d*d);
	v = glm::normalize(v);
	return v;
}

void Window::moveCamera(double x, double y) {

	glm::vec3 v = trackBall(prevXpos, prevYpos);
	glm::vec3 w = trackBall(x, y);

	// Rotational axis is cross product v x w
	glm::vec3 rotAxis = glm::cross(v, w);

	// Rotational angle is: dot product v*w = |a||b|cos(theta)
	// since |a||b| == 1, just take arccos(dotproduct)
	float dotProduct = glm::dot(v, w);
	float angle = acos(dotProduct);

	// Multiply rotation matrix by model matrix
	glm::mat4 rMat = glm::rotate(glm::mat4(1.0f), angle, rotAxis);

	//// Get new camp position
	glm::vec4 newCamPos = glm::vec4(cam_pos, 1.0f);
	newCamPos = rMat * newCamPos;
	cam_pos = glm::vec3(newCamPos.x, newCamPos.y, newCamPos.z);

	// Get new up vector based on viewer
	glm::vec4 newCamUp = glm::vec4(cam_up, 1.0f);
	newCamUp = rMat * newCamUp;
	cam_up = glm::vec3(newCamUp.x, newCamUp.y, newCamUp.z);

	V = glm::lookAt(cam_pos, cam_look_at, cam_up);

	//V = rMat * V;
}

void Window::selectPoint(double xpos, double ypos) {
	unsigned char pix[4];
	glReadPixels(xpos, height - ypos, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &pix);
	unsigned int pointId = (unsigned int)pix[0];
	for (int i = 0; i < cPts.size(); i++) {
		if (pointId == cPts[i]->id) {
			selectIdx = i;
		}
	}
}

void Window::movePoint(double xpos, double ypos) {

	double deltaX = prevXpos - xpos;
	deltaX /= 2;
	double deltaY = prevYpos - ypos;
	deltaY /= 2;

	// Move along y axis relative to camera
	glm::mat4 tMat1 = glm::translate(glm::mat4(), (float)deltaY * cam_up);

	// Move along x axis relative to camera
	glm::mat4 rMat = glm::rotate(glm::mat4(), glm::pi<float>() / 2, -cam_pos);
	glm::vec4 tMat2 = glm::vec4(cam_up,1.0f) * float(deltaX) * rMat;
	glm::mat4 tMat = tMat1 * glm::translate(glm::mat4(), glm::vec3(tMat2.x, tMat2.y, tMat2.z));

	// Moves geometry
	cPts[selectIdx]->setMat(tMat * cPts[selectIdx]->getMat());

	/* If non-anchor, move the pair control point as well */
	/* selectIdx + 1 % 3 is one side of the control pairs,
	   selectIdx - 1 % 3 is the other side of the control pairs */

	// index of the pair point
	int pair = selectIdx;
	if ((selectIdx + 1) % 3 == 0) {
		pair = selectIdx + 2;
		if (pair > 23) pair = 1;
		cPts[pair]->setMat(inverse(tMat) * cPts[pair]->getMat());
	}
	if ((selectIdx - 1) % 3 == 0) {
		pair = selectIdx - 2;
		if (pair < 0) pair = 23;
		cPts[pair]->setMat(inverse(tMat) * cPts[pair]->getMat());
	}
	// Case for if anchor point
	if (selectIdx % 3 == 0) {
		int prevIdx = selectIdx - 1;
		if (prevIdx < 0) prevIdx = 23;
		int nextIdx = selectIdx + 1;
		if (nextIdx > 23) nextIdx = 0;
		cPts[nextIdx]->setMat(tMat * cPts[nextIdx]->getMat());
		cPts[prevIdx]->setMat(tMat * cPts[prevIdx]->getMat());
	}

	// Find new highest point of the track
	for (int j = 0; j < 8; j++) {
		for (int i = 0; i < 4; i++) {
			if (curves[j]->cPts[i]->getVertex().y > maxheight) {
				maxheight = curves[j]->cPts[i]->getVertex().y;
				highestPoint = j * 3 + i;
			}
		}
	}

	

	// Update all curves and lines
	for (int i = 0; i < curves.size(); i++) {
		curves[i]->genPoints();
		lines[i]->genPoints();
	}
}


void Window::zoomCamera(int state) {

	glm::mat4 tMat;
	glm::vec3 tVec = normalize(cam_look_at - cam_pos) * 5.0f;
	//glm::vec3 tVec = glm::vec3(0.0f,0.0f,3.0f);

	if (state>0) {
		tMat = glm::translate(glm::mat4(1.0f), tVec);

	}
	if (state<0) {
		tMat = glm::translate(glm::mat4(1.0f), -tVec);
	}

	glm::vec4 newCamPos = glm::vec4(cam_pos, 1.0f);
	newCamPos = tMat * newCamPos;
	cam_pos = glm::vec3(newCamPos.x, newCamPos.y, newCamPos.z);

	V = glm::lookAt(cam_pos, cam_look_at, cam_up);

	//V = tMat * V;

	//glm::vec4 newCamPos = glm::vec4(cam_pos, 1.0f);
	//newCamPos = tMat * newCamPos;
	//cam_pos = glm::vec3(newCamPos.x, newCamPos.y, newCamPos.z);

}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_REPEAT || action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		// Print out world matrix
		if (key == GLFW_KEY_G && (mods & GLFW_MOD_SHIFT)) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
				}
				std::cout << "\n";
			}
		}

		// Camera controls
		if (key == GLFW_KEY_UP) {
			glm::vec4 newCamPos = glm::vec4(cam_pos, 1.0f);
			cam_pos = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 5.0f, 0.0f)) * newCamPos;
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
			//V = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -5.0f, 0.0f)) * V;
		}
		if (key == GLFW_KEY_DOWN) {
			glm::vec4 newCamPos = glm::vec4(cam_pos, 1.0f);
			cam_pos = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -5.0f, 0.0f)) * newCamPos;
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
			//V = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 5.0f, 0.0f)) * V;

		}
		if (key == GLFW_KEY_LEFT) {
			glm::vec4 newCamPos = glm::vec4(cam_pos, 1.0f);
			cam_pos = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 0.0f, 0.0f)) * newCamPos;
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
			//V = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 0.0f, 0.0f)) * V;

		}
		if (key == GLFW_KEY_RIGHT) {
			glm::vec4 newCamPos = glm::vec4(cam_pos, 1.0f);
			cam_pos = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 0.0f, 0.0f)) * newCamPos;
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
			//V = glm::translate(glm::mat4(1.0f), glm::vec3(-5.0f, 0.0f, 0.0f)) * V;
		}
		if (key == GLFW_KEY_1) {
			selectPoints = true;
		}
		if (key == GLFW_KEY_2) {
			selectPoints = false;
		}
		if (key == GLFW_KEY_SPACE) {
			move = !move;
		}
		if (key == GLFW_KEY_R) {
			timet = 0;
			curveIdx = highestPoint/3;
			ride->toWorld = glm::translate(glm::mat4(), cPts[highestPoint]->getVertex()) * glm::scale(glm::mat4(), glm::vec3(20.0f));
		}
		if (key == GLFW_KEY_V) {
			old_cam_pos = cam_pos;
			old_cam_look_at = cam_look_at;
			rideCamera = true;
		}
		if (key == GLFW_KEY_B) {
			cam_pos = old_cam_pos;
			cam_look_at = old_cam_look_at;
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
			rideCamera = false;
		}
		if (key == GLFW_KEY_P) {
			constPush += 0.0001;
		}
		if (key == GLFW_KEY_O) {
			constPush -= 0.0001;
		}
	}
}

void Window::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	// Only active when cursor is within the window
	if (xpos > 0 && xpos < width && ypos > 0 && ypos < height) {

		// On left click hold, rotate
		if (mState == 1) {
			if (selectPoints) {
				movePoint(xpos, ypos);
			}
			else {
				moveCamera(xpos, ypos);
			}
		}

		// On right click hold, translate
		if (mState == 2) {
		}

		// Update positions
		prevXpos = xpos;
		prevYpos = ypos;
	}
}


void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		mState = 1;
		glfwGetCursorPos(window, &prevXpos, &prevYpos);
		selectPoint(prevXpos, prevYpos);

	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		mState = 2;
		glfwGetCursorPos(window, &prevXpos, &prevYpos);
	}
	if (action == GLFW_RELEASE) {
		mState = 0;
	}
}


void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (yoffset > 0) {
		zoomCamera(1);
	}
	if (yoffset < 0) {
		zoomCamera(-1);
	}

}
