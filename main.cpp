
//  main.cpp
//  OpenGL Advances Lighting
//
//  Created by CGIS on 28/11/16.
//  Copyright � 2016 CGIS. All rights reserved.
//

#if defined (_APPLE_)
#define GLFW_INCLUDE_GLCOREARB
#define GL_SILENCE_DEPRECATION
#else
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"
#include "Model3D.hpp"
#include "Camera.hpp"
#include "SkyBox.hpp"

#include <iostream>

int glWindowWidth = 800;
int glWindowHeight = 600;
int retina_width, retina_height;
GLFWwindow* glWindow = NULL;

const unsigned int SHADOW_WIDTH = 2048;
const unsigned int SHADOW_HEIGHT = 2048;

glm::mat4 model;
GLuint modelLoc;
glm::mat4 view;
GLuint viewLoc;
glm::mat4 projection;
GLuint projectionLoc;
glm::mat3 normalMatrix;
GLuint normalMatrixLoc;
glm::mat4 lightRotation;








glm::vec3 pointLightAPosition;
glm::vec3 pointLightAAmbient;
glm::vec3 pointLightADiffuse;
glm::vec3 pointLightASpecular;



GLuint pointLightAPositionLoc;
GLuint pointLightAAmbientLoc;
GLuint pointLightADiffuseLoc;
GLuint pointLightASpecularLoc;


glm::vec3 pointLightBPosition;
glm::vec3 pointLightBAmbient;
glm::vec3 pointLightBDiffuse;
glm::vec3 pointLightBSpecular;



GLuint pointLightBPositionLoc;
GLuint pointLightBAmbientLoc;
GLuint pointLightBDiffuseLoc;
GLuint pointLightBSpecularLoc;

glm::vec3 lightDir = glm::vec3(0.0f, 1.0f, 1.0f);
glm::vec3 satelitPoz1 = glm::vec3(0.0f, 10.730f, 0.0f);
glm::vec3 satelitPoz2 = glm::vec3(0.0f, 10.730f, 10.0f);
glm::vec3 satelitPoz3 = glm::vec3(10.0f, 10.730f, 0.0f);
glm::vec3 satelitPoz4 = glm::vec3(10.0f, 10.730f, 10.0f);


glm::vec3 satelitPoz6 = glm::vec3(0.0f, 10.730f, 40.0f);
glm::vec3 satelitPoz7 = glm::vec3(40.0f, 10.730f, 0.0f);
glm::vec3 satelitPoz8 = glm::vec3(40.0f, 10.730f, 40.0f);

glm::vec3 satelitPoz9 = glm::vec3(0.0f, 10.730f, -40.0f);
glm::vec3 satelitPoz10 = glm::vec3(-40.0f, 10.730f, 0.0f);
glm::vec3 satelitPoz11 = glm::vec3(-40.0f, 10.730f, -40.0f);


glm::vec3 satelitPoz12 = glm::vec3(0.0f, 10.730f, 80.0f);
glm::vec3 satelitPoz13 = glm::vec3(80.0f, 10.730f, 0.0f);
glm::vec3 satelitPoz14 = glm::vec3(80.0f, 10.730f, 80.0f);

glm::vec3 satelitPoz15 = glm::vec3(0.0f, 10.730f, -80.0f);
glm::vec3 satelitPoz16 = glm::vec3(-80.0f, 10.730f, 0.0f);
glm::vec3 satelitPoz17 = glm::vec3(-80.0f, 10.730f, -80.0f);


GLuint lightDirLoc;
glm::vec3 lightColor;
GLuint lightColorLoc;



gps::Camera myCamera(
	glm::vec3(0.0f, 2.0f, 5.5f),
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f));
float cameraSpeed = 0.3f;

bool pressedKeys[1024];
float angleY = 0.0f;
float angleCamera = 0.0f;
float angleSatelit = 0.0f;
float dist = 122.45f;
GLfloat lightAngle;

gps::Model3D object;
gps::Model3D nanosuit;
gps::Model3D ground;
gps::Model3D lightCube;
gps::Model3D screenQuad;
gps::Model3D moara;
gps::Model3D avion;
gps::Model3D satelit;
gps::Model3D car;

gps::Shader myCustomShader;
gps::Shader lightShader;
gps::Shader screenQuadShader;

gps::Shader depthShader;

GLuint shadowMapFBO;
GLuint depthMapTexture;

bool showDepthMap;


gps::SkyBox mySkyBox;
gps::Shader skyboxShader;


GLuint fogDensityLoc;
GLfloat fogDensity;
GLuint noapteLoc;
GLint noapte;


GLenum glCheckError_(const char* file, int line) {
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError() glCheckError_(_FILE, __LINE_)

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	fprintf(stdout, "window resized to width: %d , and height: %d\n", width, height);
	//TODO	
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_M && action == GLFW_PRESS)
		showDepthMap = !showDepthMap;

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			pressedKeys[key] = true;
		else if (action == GLFW_RELEASE)
			pressedKeys[key] = false;
	}
}

GLboolean firstMouse = true;
float lastX = 400, lastY = 300;
double yaw = -90.0f, pitch;

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	myCamera.rotate(pitch, yaw);
}

void processMovement()
{
	if (pressedKeys[GLFW_KEY_Q]) {
		angleY -= 1.0f;
	}

	if (pressedKeys[GLFW_KEY_E]) {
		angleY += 1.0f;
	}
	if (pressedKeys[GLFW_KEY_V]) {
		noapte = 1;

		myCustomShader.useShaderProgram();
		glUniform1i(noapteLoc, noapte);
	}

	if (pressedKeys[GLFW_KEY_C]) {
		noapte = 0;

		myCustomShader.useShaderProgram();
		glUniform1i(noapteLoc, noapte);
	}

	if (pressedKeys[GLFW_KEY_C]) {
		noapte = 0;
	}

	if (pressedKeys[GLFW_KEY_J]) {
		lightAngle -= 1.0f;
	}

	if (pressedKeys[GLFW_KEY_L]) {
		lightAngle += 1.0f;
	}
	if (pressedKeys[GLFW_KEY_N]) {
		angleSatelit -= 1.0f;
	}

	if (pressedKeys[GLFW_KEY_U]) {
		dist -= 0.1f;

	}
	if (pressedKeys[GLFW_KEY_I]) {
		dist += 0.1f;

	}
	if (pressedKeys[GLFW_KEY_X]) {
		angleCamera += 1.0f;

	}
	if (pressedKeys[GLFW_KEY_1]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (pressedKeys[GLFW_KEY_2]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}
	if (pressedKeys[GLFW_KEY_3]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (pressedKeys[GLFW_KEY_W]) {
		myCamera.move(gps::MOVE_FORWARD, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_S]) {
		myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_A]) {
		myCamera.move(gps::MOVE_LEFT, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_D]) {
		myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
	}

	if (pressedKeys[GLFW_KEY_Z]) {
		myCamera.rotate(0.0f, angleCamera);
	}

	if (pressedKeys[GLFW_KEY_F]) {
		fogDensity += 0.002f;
		if (fogDensity >= 0.3f)
			fogDensity = 0.3f;
		myCustomShader.useShaderProgram();
		glUniform1fv(fogDensityLoc, 1, &fogDensity);
	}

	if (pressedKeys[GLFW_KEY_G]) {
		fogDensity -= 0.002f;
		if (fogDensity <= 0.0f)
			fogDensity = 0.0f;
		myCustomShader.useShaderProgram();
		glUniform1fv(fogDensityLoc, 1, &fogDensity);
	}
}

void updateDelta(double elapsedSeconds) {
	angleCamera = angleCamera + 10.0f * elapsedSeconds;
}
double lastTimeStamp = glfwGetTime();

bool initOpenGLWindow()
{
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	//window scaling for HiDPI displays
	glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

	//for sRBG framebuffer
	glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);

	//for antialising
	glfwWindowHint(GLFW_SAMPLES, 4);

	glWindow = glfwCreateWindow(glWindowWidth, glWindowHeight, "OpenGL Shader Example", NULL, NULL);
	if (!glWindow) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return false;
	}

	glfwSetWindowSizeCallback(glWindow, windowResizeCallback);
	glfwSetKeyCallback(glWindow, keyboardCallback);
	glfwSetCursorPosCallback(glWindow, mouseCallback);
	//glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetCursorPosCallback(glWindow, mouseCallback);
	glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwMakeContextCurrent(glWindow);

	glfwSwapInterval(1);

#if not defined (_APPLE_)
	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();
#endif

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	//for RETINA display
	glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);

	return true;
}

void initOpenGLState()
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glViewport(0, 0, retina_width, retina_height);

	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise

	glEnable(GL_FRAMEBUFFER_SRGB);
}

void initObjects() {
	nanosuit.LoadModel("objects/nanosuit/nanosuit.obj");
	//object.LoadModel("objects/obiect1/test.obj");
	ground.LoadModel("objects/Scena/ScenaFinala.obj");
	moara.LoadModel("objects/Moara/Moara.obj");
	lightCube.LoadModel("objects/cube/cube.obj");
	screenQuad.LoadModel("objects/quad/quad.obj");
	avion.LoadModel("objects/Avion/Avionn.obj");
	satelit.LoadModel("objects/Satelit/Satelit.obj");
	car.LoadModel("objects/Car/Car.obj");

}

void initShaders() {
	myCustomShader.loadShader("shaders/shaderStart.vert", "shaders/shaderStart.frag");
	myCustomShader.useShaderProgram();
	lightShader.loadShader("shaders/lightCube.vert", "shaders/lightCube.frag");
	lightShader.useShaderProgram();
	screenQuadShader.loadShader("shaders/screenQuad.vert", "shaders/screenQuad.frag");
	screenQuadShader.useShaderProgram();

	depthShader.loadShader("shaders/depth.vert", "shaders/depth.frag");
	depthShader.useShaderProgram();

	skyboxShader.loadShader("shaders/skyboxShader.vert", "shaders/skyboxShader.frag");
	skyboxShader.useShaderProgram();
}

void initUniforms() {
	myCustomShader.useShaderProgram();

	model = glm::mat4(1.0f);
	modelLoc = glGetUniformLocation(myCustomShader.shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	view = myCamera.getViewMatrix();
	viewLoc = glGetUniformLocation(myCustomShader.shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
	normalMatrixLoc = glGetUniformLocation(myCustomShader.shaderProgram, "normalMatrix");
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
	projectionLoc = glGetUniformLocation(myCustomShader.shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));


	pointLightAPosition = glm::vec3(-47.97f, 8.47, -66.48f);
	pointLightAPositionLoc = glGetUniformLocation(myCustomShader.shaderProgram, "pointLights[0].position");
	glUniform3fv(pointLightAPositionLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view)) * pointLightAPosition));

	pointLightAAmbient = glm::vec3(100.0f, 100.0f, 100.0f);
	pointLightAAmbientLoc = glGetUniformLocation(myCustomShader.shaderProgram, "pointLights[0].ambient");
	glUniform3fv(pointLightAAmbientLoc, 1, glm::value_ptr(pointLightAAmbient));

	pointLightADiffuse = glm::vec3(100.0f, 100.0f, 100.0f);
	pointLightADiffuseLoc = glGetUniformLocation(myCustomShader.shaderProgram, "pointLights[0].diffuse");
	glUniform3fv(pointLightADiffuseLoc, 1, glm::value_ptr(pointLightADiffuse));

	pointLightASpecular = glm::vec3(100.0f, 100.0f, 100.0f);
	pointLightASpecularLoc = glGetUniformLocation(myCustomShader.shaderProgram, "pointLights[0].specular");
	glUniform3fv(pointLightASpecularLoc, 1, glm::value_ptr(pointLightASpecular));


	pointLightBPosition = glm::vec3(-192.44f, 8.47, -166.89f);
	pointLightBPositionLoc = glGetUniformLocation(myCustomShader.shaderProgram, "pointLights[1].position");
	glUniform3fv(pointLightBPositionLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view)) * pointLightBPosition));

	pointLightBAmbient = glm::vec3(400.0f, 400.0f, 400.0f);
	pointLightBAmbientLoc = glGetUniformLocation(myCustomShader.shaderProgram, "pointLights[1].ambient");
	glUniform3fv(pointLightBAmbientLoc, 1, glm::value_ptr(pointLightBAmbient));

	pointLightBDiffuse = glm::vec3(200.0f, 200.0f, 200.0f);
	pointLightBDiffuseLoc = glGetUniformLocation(myCustomShader.shaderProgram, "pointLights[1].diffuse");
	glUniform3fv(pointLightBDiffuseLoc, 1, glm::value_ptr(pointLightBDiffuse));

	pointLightBSpecular = glm::vec3(200.0f, 200.0f, 200.0f);
	pointLightBSpecularLoc = glGetUniformLocation(myCustomShader.shaderProgram, "pointLights[1].specular");
	glUniform3fv(pointLightBSpecularLoc, 1, glm::value_ptr(pointLightBSpecular));


	//set the light direction (direction towards the light)
	//lightDir = glm::vec3(0.0f, 1.0f, 1.0f);

	lightDir = glm::vec3(0.0f, 1.0f, 1.0f);

	lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));

	////my modifications
	//// Apply rotation to the point
	//glm::vec4 rotatedPoint = lightRotation * glm::vec4(lightDir, 1.0f); // Extend point to a vec4

	//// Extract the new coordinates after rotation
	//lightDir = glm::vec3(rotatedPoint.x, rotatedPoint.y, rotatedPoint.z);
	////

	lightDirLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightDir");

	glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));
	//set light color
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f); //white light
	lightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor");
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

	//fog density
	fogDensity = 0.0f;
	fogDensityLoc = glGetUniformLocation(myCustomShader.shaderProgram, "fogDensity");
	glUniform1fv(fogDensityLoc, 1, &fogDensity);

	noapte = 0;
	noapteLoc = glGetUniformLocation(myCustomShader.shaderProgram, "noapte");
	glUniform1i(noapteLoc, noapte);

	lightShader.useShaderProgram();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


}




void initFBO() {
	//TODO - Create the FBO, the depth texture and attach the depth texture to the FBO
	//generate FBO ID
	glGenFramebuffers(1, &shadowMapFBO);

	//create depth texture for FBO
	glGenTextures(1, &depthMapTexture);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);


	//attach texture to FBO
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

glm::mat4 computeLightSpaceTrMatrix() {
	//TODO - Return the light-space transformation matrix

	glm::mat4 lightView = glm::lookAt(lightDir, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	const GLfloat near_plane = 0.1f, far_plane = 130.0f;
	glm::mat4 lightProjection = glm::ortho(-140.0f, 140.0f, -140.0f, 140.0f, near_plane, far_plane);
	glm::mat4 lightSpaceTrMatrix = lightProjection * lightView;


	return lightSpaceTrMatrix;
}

void rotateAroundPoint(gps::Shader shader, glm::vec3 satelitPoz)
{
	shader.useShaderProgram();

	glm::mat4 Translatie = glm::translate(glm::mat4(1.0f), satelitPoz);
	glm::mat4 Rotatie = glm::rotate(glm::mat4(1.0f), glm::radians(angleSatelit), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Inversa = glm::translate(glm::mat4(1.0f), -satelitPoz);
	glm::mat4 Finala = Translatie * Rotatie * Inversa;

	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(Finala));



	satelit.Draw(shader);
}

bool checkSphereCollision(const glm::vec3& position1, float radius1, const glm::vec3& position2, float radius2) {
	float distance = glm::distance(position1, position2);
	float sumOfRadii = radius1 + radius2;

	return distance < sumOfRadii;
}

void handleCollision(glm::vec3& movingSpherePosition, float movingSphereRadius, const glm::vec3& staticSpherePosition, float staticSphereRadius, glm::mat4 model) {
	if (checkSphereCollision(movingSpherePosition, movingSphereRadius, staticSpherePosition, staticSphereRadius)) {
		// Perform some action upon collision
		std::cout << "Collision detected! Updating position." << std::endl;

		// Move the moving sphere to a new position (you can implement your own logic here)
		angleY = angleY - 1.0f;
		movingSpherePosition = glm::vec3(angleY - 1.0f, 0.0f, 0.0f);
		//model = glm::translate(glm::mat4(1.0f), movingSpherePosition);

	}
}
void drawObjects(gps::Shader shader, bool depthPass) {

	shader.useShaderProgram();

	//model = glm::rotate(glm::mat4(1.0f), glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::vec3 movingSpherePosition(angleY, 0.0f, 0.0f);
	float movingSphereRadius = 1.0f;

	glm::vec3 staticSpherePosition(17.0f, 0.0f, 0.0f);

	glm::vec3 staticSpherePosition1(-10.0f, 0.0f, 0.0f);
	float staticSphereRadius = 1.0f;

	// Check for collision and handle it
	//if (movingSpherePosition.x>staticSpherePosition.x) {
	//handleCollision(movingSpherePosition, movingSphereRadius, staticSpherePosition, staticSphereRadius, model);
	handleCollision(movingSpherePosition, movingSphereRadius, staticSpherePosition1, staticSphereRadius, model);
	//}

	// Output the updated position of the moving sphere
	std::cout << "Updated position: (" << movingSpherePosition.x << ", " << movingSpherePosition.y << ", " << movingSpherePosition.z << ")" << std::endl;

	//model = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 20.0f, 3.0f));
	//model = glm::scale(model, glm::vec3(0.5f));
	//glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	// do not send the normal matrix if we are rendering in the depth map
	//if (!depthPass) {
		//normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		//glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	//}

	//ground.Draw(shader);

	//model = lightRotation;
	//model = glm::translate(model, 1.0f * glm::vec3(10.0f, 5.0f, 0.0f));
	//model = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 5.0f, 3.0f));


		//model = glm::rotate(glm::mat4(1.0f), glm::radians(angleY), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(glm::mat4(1.0f), glm::vec3(angleY, -1.0f, -45.0f));

	//model = glm::rotate(glm::mat4(1.0f), glm::radians(angleY), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(glm::mat4(1.0f), glm::radians(angleY), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::translate(glm::mat4(1.0f), glm::vec3(angleY, -1.0f, -45.0f));
	model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	car.Draw(shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(dist, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.9f));
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	// do not send the normal matrix if we are rendering in the depth map
	//if (!depthPass) {
		//normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		//glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	//}

	avion.Draw(shader);


	//object.Draw(shader);
	rotateAroundPoint(shader, satelitPoz1);
	rotateAroundPoint(shader, satelitPoz3);
	rotateAroundPoint(shader, satelitPoz4);
	//rotateAroundPoint(shader, satelitPoz5);
	rotateAroundPoint(shader, satelitPoz6);
	rotateAroundPoint(shader, satelitPoz7);
	rotateAroundPoint(shader, satelitPoz8);
	rotateAroundPoint(shader, satelitPoz9);
	rotateAroundPoint(shader, satelitPoz10);
	rotateAroundPoint(shader, satelitPoz11);
	rotateAroundPoint(shader, satelitPoz12);
	rotateAroundPoint(shader, satelitPoz13);
	rotateAroundPoint(shader, satelitPoz14);
	rotateAroundPoint(shader, satelitPoz15);
	rotateAroundPoint(shader, satelitPoz16);
	rotateAroundPoint(shader, satelitPoz17);
	//rotateAroundPoint(shader, satelitPoz1);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f));
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	//rotateAroundPoint(shader, satelitPoz1);

	// do not send the normal matrix if we are rendering in the depth map


	ground.Draw(shader);


}


void initSkybox()
{
	std::vector<const GLchar*> faces;
	faces.push_back("skybox/right.tga");
	faces.push_back("skybox/left.tga");
	faces.push_back("skybox/top.tga");
	faces.push_back("skybox/bottom.tga");
	faces.push_back("skybox/back.tga");
	faces.push_back("skybox/front.tga");
	mySkyBox.Load(faces);
}



void renderScene() {


	// render depth map on screen - toggled with the M key

	//render the scene to the depth buffer
	depthShader.useShaderProgram();
	glUniformMatrix4fv(glGetUniformLocation(depthShader.shaderProgram, "lightSpaceTrMatrix"),
		1,
		GL_FALSE,
		glm::value_ptr(computeLightSpaceTrMatrix()));
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	drawObjects(depthShader, true);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (showDepthMap) {
		glViewport(0, 0, retina_width, retina_height);

		glClear(GL_COLOR_BUFFER_BIT);

		screenQuadShader.useShaderProgram();

		//bind the depth map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMapTexture);
		glUniform1i(glGetUniformLocation(screenQuadShader.shaderProgram, "depthMap"), 0);

		glDisable(GL_DEPTH_TEST);
		screenQuad.Draw(screenQuadShader);
		glEnable(GL_DEPTH_TEST);

	}
	else {

		// final scene rendering pass (with shadows)

		glViewport(0, 0, retina_width, retina_height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		myCustomShader.useShaderProgram();

		view = myCamera.getViewMatrix();
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));

		//bind the shadow map
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, depthMapTexture);
		glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "shadowMap"), 3);

		glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "lightSpaceTrMatrix"),
			1,
			GL_FALSE,
			glm::value_ptr(computeLightSpaceTrMatrix()));

		drawObjects(myCustomShader, false);

		//draw a white cube around the light

		lightShader.useShaderProgram();

		glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
		//double currentTimeStamp = glfwGetTime();
		//updateDelta(currentTimeStamp - lastTimeStamp);
		//lastTimeStamp = currentTimeStamp;


		double lastTime = glfwGetTime();
		double currentTime;
		double deltaTime;
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		// Update your camera angle here based on the elapsed time
		angleCamera += 1.0f * deltaTime;


		model = lightRotation;
		model = glm::translate(model, 100.0f * lightDir);
		model = glm::scale(model, glm::vec3(5.05f, 5.05f, 5.05f));
		glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

		lightCube.Draw(lightShader);




		mySkyBox.Draw(skyboxShader, view, projection);
	}
}

void cleanup() {
	glDeleteTextures(1, &depthMapTexture);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &shadowMapFBO);
	glfwDestroyWindow(glWindow);
	//close GL context and any other GLFW resources
	glfwTerminate();
}

int main(int argc, const char* argv[]) {

	if (!initOpenGLWindow()) {
		glfwTerminate();
		return 1;
	}

	initOpenGLState();
	initObjects();
	initShaders();
	initUniforms();
	initFBO();
	initSkybox();

	//glCheckError();

	while (!glfwWindowShouldClose(glWindow)) {
		processMovement();
		renderScene();

		glfwPollEvents();
		glfwSwapBuffers(glWindow);
	}

	cleanup();

	return 0;
}