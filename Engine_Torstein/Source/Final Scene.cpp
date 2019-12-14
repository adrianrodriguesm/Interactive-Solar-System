///////////////////////////////////////////////////////////////////////////////
//
//  Loading OBJ mesh from external file
//
//	Final individual assignment:
//	1.	Create classes: Scene, Camera and Mesh (that loads a mesh from a 
//      Wavefront OBJ file to an indexed format) and build a small scenegraph
//      of your tangram scene (you may create more classes if needed).
//	2.	Create a ground object and couple the tangram figure to the ground. 
//      Press keys to move the ground about: the tangram figure must follow the
//      ground.
//	3.	Animate between closed puzzle (initial square) and tangram figure by
//      pressing a key.
//	4.	Spherical camera interaction through mouse. It should be possible to 
//      interact while animation is playing.
//
// (c) 2013-19 by Carlos Martinho
//
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <string>

#include "Quaternions.h"
#include "Matrices.h"
#include "Vectors.h"
#include "Camera.h"
#include "Vectors.h"
#include "Shader.h"
#include "Scenegraph.h"
#include "Mesh.h"
#include "SceneNode.h"
#include "Texture.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define BATFIGURE 1
#define SQUAREFIGURE -1


////////////////////////////////////////////////// VARIABLES

//General
const int screenWidth = 1920 / 2;
const int screenHeight = 1920 / 2;
float aspect = screenWidth / screenHeight;
vec4 xAxis = vec4(1, 0, 0, 1);
vec4 yAxis = vec4(0, 1, 0, 1);
vec4 zAxis = vec4(0, 0, -1, 1);

//Meshes
Mesh sphereMesh;

//Shader files must be in Shader folder:
string vertexShaderFile = "TextureTest_vert.glsl";
string fragmentShaderFile = "TextureTest_frag.glsl";
//string vertexShaderFile = "Displacement_Mapping_vert.glsl";
//string fragmentShaderFile = "Displacement_Mapping_frag.glsl";
Shader earthShader = Shader();

//Camera
bool mouseChange = true; //Boolean for mouse input
bool scrollChange = true; //Boolean for scroll input
Camera cam = Camera(vec3(0,0,1), vec3(0,0,0), vec3(0,1,0));
float cameraDistance = 1;
mat4 cameraRotation;
mat4 cameraTranslation;

//Scene:
SceneGraph* scenegraph;
SceneNode* ground, * Earth;

//Animation (Press 'P' to animate)
float animationSpeed = 0.005;
float currentAnimationStep;
bool animationInProgress = false;
float finalFigure = SQUAREFIGURE;

typedef struct {
	SceneNode* node;
	vec3 pos1, pos2;
	qtrn q1, q2;
} animationObject;

vector<animationObject> animationObjects = vector<animationObject>();

////////////////////////////////////////////////// ERROR CALLBACK (OpenGL 4.3+)

static const std::string errorSource(GLenum source)
{
	switch (source) {
	case GL_DEBUG_SOURCE_API:				return "API";
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		return "window system";
	case GL_DEBUG_SOURCE_SHADER_COMPILER:	return "earthShader compiler";
	case GL_DEBUG_SOURCE_THIRD_PARTY:		return "third party";
	case GL_DEBUG_SOURCE_APPLICATION:		return "application";
	case GL_DEBUG_SOURCE_OTHER:				return "other";
	default:								exit(EXIT_FAILURE);
	}
}

static const std::string errorType(GLenum type)
{
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:				return "error";
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	return "deprecated behavior";
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	return "undefined behavior";
	case GL_DEBUG_TYPE_PORTABILITY:			return "portability issue";
	case GL_DEBUG_TYPE_PERFORMANCE:			return "performance issue";
	case GL_DEBUG_TYPE_MARKER:				return "stream annotation";
	case GL_DEBUG_TYPE_PUSH_GROUP:			return "push group";
	case GL_DEBUG_TYPE_POP_GROUP:			return "pop group";
	case GL_DEBUG_TYPE_OTHER_ARB:			return "other";
	default:								exit(EXIT_FAILURE);
	}
}

static const std::string errorSeverity(GLenum severity)
{
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:			return "high";
	case GL_DEBUG_SEVERITY_MEDIUM:			return "medium";
	case GL_DEBUG_SEVERITY_LOW:				return "low";
	case GL_DEBUG_SEVERITY_NOTIFICATION:	return "notification";
	default:								exit(EXIT_FAILURE);
	}
}

static void error(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
	const GLchar* message, const void* userParam)
{
	std::cerr << "GL ERROR:" << std::endl;
	std::cerr << "  source:     " << errorSource(source) << std::endl;
	std::cerr << "  type:       " << errorType(type) << std::endl;
	std::cerr << "  severity:   " << errorSeverity(severity) << std::endl;
	std::cerr << "  debug call: " << std::endl << message << std::endl;
	//if (severity != GL_DEBUG_SEVERITY_NOTIFICATION) {
	//	std::cerr << "Press <return>.";
	//	std::cin.ignore();
	//}
}

void setupErrorCallback()
{
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(error, 0);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, GL_TRUE);
	// params: source, type, severity, count, ids, enabled
}

/////////////////////////////////////////////////////////////////////// TEXTURES

void createTextures() {
	Texture EarthHeightMap = Texture("../../Textures/earthbump1k.jpg");
	//Texture EarthColorMap = Texture("../../Textures/earthmap1k.jpg");
	Texture EarthColorMap = Texture("../../Textures/yellow.jpg");

	earthShader.Use();

	//EarthHeightMap.Bind();
	//glUniform1i(earthShader.Uniforms["HeightMap"], EarthHeightMap.GetId());

	EarthColorMap.Bind();
	glUniform1i(earthShader.Uniforms["ColorMap"], EarthColorMap.GetId());

	EarthColorMap.Unbind();
	glUseProgram(0);

}

/////////////////////////////////////////////////////////////////////// MESHES

void createMeshes() {
	std::string mesh_dir = "../../Blender Objects/";

	std::string spherePath = mesh_dir + "sphere.obj";
	sphereMesh = Mesh(spherePath);
}

void destroyMeshes() {
	sphereMesh.destroy();
}

/////////////////////////////////////////////////////////////////////// SHADER

void createShader() {
	earthShader.Load(vertexShaderFile, fragmentShaderFile);
	earthShader.AddAttribute(0, "inPosition");
	earthShader.AddAttribute(1, "inTexcoord");
	earthShader.AddAttribute(2, "inNormals");
	earthShader.AddUniform("ModelMatrix");
	earthShader.AddUniform("ProjectionMatrix");
	earthShader.AddUniform("ViewMatrix");
	//earthShader.AddUniform("HeightMap");
	earthShader.AddUniform("ColorMap");
	earthShader.Create();
}

/////////////////////////////////////////////////////////////////////// SCENE
const float SMALL = 3.5f;
const float MEDIUM = 5.0f;
const float LARGE = 7.0f;
const float XXLARGE = 25.0f;
const float figureOffset = XXLARGE / 2;

const vec4 colorGrey = vec4(0.2,0.2,0.2);

mat4 Scale_S = MatrixFactory::createScaleMat4(vec3(SMALL, SMALL, 1));
mat4 Scale_M = MatrixFactory::createScaleMat4(vec3(MEDIUM, MEDIUM, 1));
mat4 Scale_L = MatrixFactory::createScaleMat4(vec3(LARGE, LARGE, 1));

void createScene(SceneGraph* scenegraph) {
	
	ground = scenegraph->createNode();
	ground->setShader(&earthShader);
	
	Earth = ground->createNode();
	Earth->setShader(&earthShader);
	Earth->setMesh(&sphereMesh);
}

void createSceneGraph(Camera& cam) {
	scenegraph = new SceneGraph();
	scenegraph->setCamera(cam);

	scenegraph->getCamera()->ProjectionMatrix = MatrixFactory::createPerspectiveProjectionMatrix(30, aspect, 1, 500);
	SceneNode* n = scenegraph->getRoot();
	n->setShader(&earthShader);

	createScene(scenegraph);
}

void updateAnimation() {
	currentAnimationStep += animationSpeed * finalFigure;

	if (currentAnimationStep > 1 || currentAnimationStep < 0) {
		currentAnimationStep = fmax(0, fmin(currentAnimationStep, 1));
		animationInProgress = false;
	}

	vec3 currentPos;
	qtrn currentAngle;

	for (animationObject currentObject : animationObjects){
		currentPos = vecLerp(currentObject.pos1, currentObject.pos2, currentAnimationStep);
		currentAngle = qLerp(currentObject.q1, currentObject.q2, currentAnimationStep);
		currentObject.node->setMatrix(MatrixFactory::createTranslationMat4(currentPos) *
			matrixFromQtrn(currentAngle));
	}
}

void drawScene() {
	if (animationInProgress) updateAnimation();
	scenegraph->draw();
}

void window_close_callback(GLFWwindow* win)
{
	earthShader.Delete();
	destroyMeshes();
}

void window_size_callback(GLFWwindow* win, int winx, int winy)
{
	glViewport(0, 0, winx, winy);
}

////////////////////////////////////////////////////////////////////////// INPUT

//Keyboard variables:
bool keyW, keyD, keyS, keyA, keyR, keyP, keyG, keyESC, keyE, keyQ;
float movStep = 0.1f;
float angStep = 1;
float timeoutKeyP = 0.0f;
float timeoutKeyG = 0.0f;

//Mouse Variables:
float sensitivity = 0.5;
double lastX = screenWidth / 2; // Set to center of screen
double lastY = screenHeight / 2;
bool firstMouseCall = true;
bool mouseClicked;
qtrn rotQtrn = qtrn(1, 0, 0, 0);

//Scrolling (zooming)
float scrollSensitivity = 4;

//Shape movement
float shapeMovementX = 0, shapeMovementY = 0;
float shapeRotation = 0;

void initialize_inputs() {
	keyW = false;
	keyD = false;
	keyS = false;
	keyA = false;
	keyR = false;
	keyP = false;
	keyG = false;
	keyE = false;
	keyQ = false;
	keyESC = false;
}

void get_keyboard_input(GLFWwindow* win) {
	if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) { keyW = true; }
	else if (glfwGetKey(win, GLFW_KEY_W) == GLFW_RELEASE) { keyW = false; }

	if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) { keyS = true; }
	else if (glfwGetKey(win, GLFW_KEY_S) == GLFW_RELEASE) { keyS = false; }

	if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) { keyA = true; }
	else if (glfwGetKey(win, GLFW_KEY_A) == GLFW_RELEASE) { keyA = false; }

	if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) { keyD = true; }
	else if (glfwGetKey(win, GLFW_KEY_D) == GLFW_RELEASE) { keyD = false; }

	if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS) { keyESC = true; }
	else if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_RELEASE) { keyESC = false; }

	if (glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS) { keyQ = true; }
	else if (glfwGetKey(win, GLFW_KEY_Q) == GLFW_RELEASE) { keyQ = false; }

	if (glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS) { keyE = true; }
	else if (glfwGetKey(win, GLFW_KEY_E) == GLFW_RELEASE) { keyE = false; }

	//RESET:
	if (glfwGetKey(win, GLFW_KEY_R) == GLFW_PRESS) keyR = true;
	else keyR = false;

	//Start Animation
	if ((glfwGetKey(win, GLFW_KEY_P) == GLFW_PRESS) && (timeoutKeyP <= 0)) {
		keyP = true;
		timeoutKeyP = 10;
	}
	else {
		keyP = false;
		timeoutKeyP = fmaxf(--timeoutKeyP, -0.1f);
	}
}

void process_keyboard_input(GLFWwindow* win) {
	if (keyP) {
		finalFigure = finalFigure == BATFIGURE ? SQUAREFIGURE : BATFIGURE; // Switch destination figure
		animationInProgress = true; // Start animation
	}
	if (keyA) shapeMovementX -= movStep;
	if (keyD) shapeMovementX += movStep;
	if (keyS) shapeMovementY -= movStep;
	if (keyW) shapeMovementY += movStep;
	if (keyQ) shapeRotation -= angStep;
	if (keyE) shapeRotation += angStep;
	ground->setMatrix(MatrixFactory::createRoationMat4(shapeRotation, zAxis) * 
						MatrixFactory::createTranslationMat4(vec3(-figureOffset + shapeMovementX, -figureOffset + shapeMovementY, -0.1)));

}

void mouse_callback(GLFWwindow* win, double xpos, double ypos) {

	if (!mouseClicked) {
		firstMouseCall = true;
		return;
	}

	if (firstMouseCall) {
		lastX = xpos;
		lastY = ypos;
		firstMouseCall = false;
	}

	float xoffset = xpos- lastX;
	float yoffset = ypos - lastY;
	lastX = xpos;
	lastY = ypos;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	qtrn q1 = qtrn::qFromAngleAxis(yoffset, xAxis);
	qtrn q2 = qtrn::qFromAngleAxis(xoffset, yAxis);
	rotQtrn = q1 * q2 * rotQtrn;

	mouseChange = true;
}

void mouse_button_callback(GLFWwindow* win, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) mouseClicked = true;
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) mouseClicked = false;
}

void scroll_callback(GLFWwindow* win, double xOffset, double yOffset) {
	cameraDistance -= yOffset * scrollSensitivity;
	scrollChange = true;
}


void mouse_initialize(GLFWwindow* win) {
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void mouse_update(GLFWwindow* win) {
	cameraTranslation = MatrixFactory::createTranslationMat4(vec3(0, 0, -cameraDistance));
	cameraRotation = matrixFromQtrn(rotQtrn);
	cam.ViewMatrix = cameraTranslation * cameraRotation;
}

///////////////////////////////////////////////////////////////////////// SETUP

void glfw_error_callback(int error, const char* description)
{
	std::cerr << "GLFW Error: " << description << std::endl;
}

GLFWwindow* setupWindow(int winx, int winy, const char* title,
	int is_fullscreen, int is_vsync)
{
	GLFWmonitor* monitor = is_fullscreen ? glfwGetPrimaryMonitor() : 0;
	GLFWwindow* win = glfwCreateWindow(winx, winy, title, monitor, 0);
	if (!win)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(win);
	glfwSwapInterval(is_vsync);
	return win;
}

void setupCallbacks(GLFWwindow* win)
{
	glfwSetWindowCloseCallback(win, window_close_callback);
	glfwSetWindowSizeCallback(win, window_size_callback);
	glfwSetCursorPosCallback(win, mouse_callback);
	glfwSetMouseButtonCallback(win, mouse_button_callback);
	glfwSetScrollCallback(win, scroll_callback);
}

GLFWwindow* setupGLFW(int gl_major, int gl_minor,
	int winx, int winy, const char* title, int is_fullscreen, int is_vsync)
{
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_minor);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

	GLFWwindow* win = setupWindow(winx, winy, title, is_fullscreen, is_vsync);
	setupCallbacks(win);

#if _DEBUG
	std::cout << "GLFW " << glfwGetVersionString() << std::endl;
#endif
	return win;
}

void setupGLEW()
{
	glewExperimental = GL_TRUE;
	// Allow extension entry points to be loaded even if the extension isn't 
	// present in the driver's extensions string.
	GLenum result = glewInit();
	if (result != GLEW_OK)
	{
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	}
	GLenum err_code = glGetError();
	// You might get GL_INVALID_ENUM when loading GLEW.
}

void checkOpenGLInfo()
{
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	std::cerr << "OpenGL Renderer: " << renderer << " (" << vendor << ")" << std::endl;
	std::cerr << "OpenGL version " << version << std::endl;
	std::cerr << "GLSL version " << glslVersion << std::endl;
}

void setupOpenGL(int winx, int winy)
{
#if _DEBUG
	checkOpenGLInfo();
#endif
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glViewport(0, 0, winx, winy);
}

GLFWwindow* setup(int major, int minor,
	int winx, int winy, const char* title, int is_fullscreen, int is_vsync)
{
	GLFWwindow* win =
		setupGLFW(major, minor, winx, winy, title, is_fullscreen, is_vsync);
	setupGLEW();
	setupOpenGL(winx, winy);
	setupErrorCallback();
	//Input:
	initialize_inputs();
	mouse_initialize(win);
	//Meshes:
	createMeshes();
	//Shader:
	createShader();
	//Texture:
	createTextures();
	//Scene Setup
	createSceneGraph(cam);

	return win;
}

////////////////////////////////////////////////////////////////////////// RUN

void display(GLFWwindow* win, double elapsed_sec)
{
	drawScene();
}

void run(GLFWwindow* win)
{
	double last_time = glfwGetTime();
	while (!glfwWindowShouldClose(win))
	{
		double time = glfwGetTime();
		double elapsed_time = time - last_time;
		last_time = time;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//INPUT:
		get_keyboard_input(win);
		process_keyboard_input(win);
		//Pressing escape exits the window:
		if (keyESC) break;
		//
		mouse_update(win);
		//
		display(win, elapsed_time);
		glfwSwapBuffers(win);
		glfwPollEvents();
	}
	glfwDestroyWindow(win);
	glfwTerminate();
}




////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char* argv[])
{
	int gl_major = 4, gl_minor = 3;
	int is_fullscreen = 0;
	int is_vsync = 1;
	GLFWwindow* win = setup(gl_major, gl_minor,
		screenWidth, screenHeight, "Hello 3D tangram", is_fullscreen, is_vsync);
	run(win);
	exit(EXIT_SUCCESS);
}

/////////////////////////////////////////////////////////////////////////// END