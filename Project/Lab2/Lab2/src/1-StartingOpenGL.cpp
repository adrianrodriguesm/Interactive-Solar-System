#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../include/SceneGraph.h"

using namespace EngineSceneGraph;

///Shader
Shader* shader;

///Mesh
Mesh* table;
Mesh* cube;
Mesh* triangle;
Mesh* parallelogram;

///SceneGraph
SceneGraph* scene;

//Animation
std::vector<SceneNode*> nodesToAnimate;
bool animation;
float start;
float delta;
float curr_time;
float duration = 2.0f;
float animationTime;
bool reverse = false;

////Movimento da mesa
float horizontal, vertical;
float rotTable;
qtrn rotTableY;


///Camera
Camera* mainCamera;

////Camara Coord
bool typeProj = 1;
bool typeRot = 0;
float d = 10.0f;
float lastX = 320.0f, lastY = 240.0F;
float fov = 45.0f;
bool firstMouse = true;
bool tracking = false;

#define ERROR_CALLBACK
#ifdef  ERROR_CALLBACK
void makeAnimation();
////////////////////////////////////////////////// ERROR CALLBACK (OpenGL 4.3+)
static const std::string errorSource(GLenum source)
{
	switch (source) {
	case GL_DEBUG_SOURCE_API:				return "API";
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		return "window system";
	case GL_DEBUG_SOURCE_SHADER_COMPILER:	return "shader compiler";
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
	std::cerr << "Press <return>.";
	std::cin.ignore();
}

void setupErrorCallback()
{
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(error, 0);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, GL_TRUE);
	// params: source, type, severity, count, ids, enabled
}

#else

///////////////////////////////////////////////// ERROR HANDLING (All versions)

static const std::string errorString(GLenum error)
{
	switch (error) {
	case GL_NO_ERROR:
		return "No error has been recorded.";
	case GL_INVALID_ENUM:
		return "An unacceptable value is specified for an enumerated argument.";
	case GL_INVALID_VALUE:
		return "A numeric argument is out of range.";
	case GL_INVALID_OPERATION:
		return "The specified operation is not allowed in the current state.";
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		return "The framebuffer object is not complete.";
	case GL_OUT_OF_MEMORY:
		return "There is not enough memory left to execute the command.";
	case GL_STACK_UNDERFLOW:
		return "An attempt has been made to perform an operation that would cause an internal stack to underflow.";
	case GL_STACK_OVERFLOW:
		return "An attempt has been made to perform an operation that would cause an internal stack to overflow.";
	default: exit(EXIT_FAILURE);
	}
}

static bool isOpenGLError()
{
	bool isError = false;
	GLenum errCode;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		std::cerr << "OpenGL ERROR [" << errorString(errCode) << "]." << std::endl;
	}
	return isError;
}

static void checkOpenGLError(std::string error)
{
	if (isOpenGLError()) {
		std::cerr << error << std::endl;
		exit(EXIT_FAILURE);
	}
}

#endif // ERROR_CALLBACK

/////////////////////////////////////////////////////////////////////// SHADERs



void createShaderProgram()
{

	shader = new Shader("shaders/vertex.vert", "shaders/fragment.frag");
	shader->load();
	shader->enable();
	
	//shader->addAttribute(VERTICES, "in_Position");


#ifndef ERROR_CALLBACK
	checkOpenGLError("ERROR: Could not create shaders.");
#endif
}

void destroyShaderProgram()
{
	shader->disable();
	glDeleteProgram(shader->getProgram());

#ifndef ERROR_CALLBACK
	checkOpenGLError("ERROR: Could not destroy shaders.");
#endif
}

/////////////////////////////////////////////////////////////////////// VAOs & VBOs


void createBufferObjects()
{
	table->createBufferObjects();
	cube->createBufferObjects();
	triangle->createBufferObjects();
	parallelogram->createBufferObjects();
	

#ifndef ERROR_CALLBACK
	checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
#endif
}

void destroyBufferObjects()
{

	table->destroyBufferObjects();
	cube->destroyBufferObjects();
	triangle->destroyBufferObjects();
	parallelogram->destroyBufferObjects();

#ifndef ERROR_CALLBACK
	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
#endif
}

/////////////////////////////////////////////////////////////////////// SCENE
void drawScene()
{
	shader->enable();
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	if (typeProj == 0) {
		mainCamera->setOrthoProjMatrix(-5.0f, 5.0f, -5.0f, 5.0f, 1.0f, 20.0f);
	}
	else {
		mainCamera->setPersProjMatrix(fov, 640.0f / 480.0f, 0.0f, 20.0f);
	}

	if (animation) {
		makeAnimation();
	}
	else {
		scene->draw();
	}
			
	shader->disable();
	glBindVertexArray(0);

#ifndef ERROR_CALLBACK
	checkOpenGLError("ERROR: Could not draw scene.");
#endif
}

void makeAnimation() {
	
	curr_time = (float)glfwGetTime();
	
	delta = (curr_time - start);
	animationTime = animationTime + delta;
	
	for (SceneNode* node : nodesToAnimate) {

		node->playAnimation(duration,  animationTime);
		scene->draw();

	}
	if (animationTime > duration) {
		animation = false;
	}

	start = curr_time;
}



void createSceneGraph()
{
	SceneNode* root = new SceneNode();
	root->setShaderProgram(shader);

	SceneNode* tableGround = root->createNode();
	tableGround->setMesh(table);
	tableGround->setColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	qtrn qTable = qTable.qFromAngleAxis(90.0f, vec4(1.0f, 0.0f, 0.0f, 1.0f));///Rotacao de modo a ver de frente
	mat4 scaleTable = mathsMatFactory::matFactory::createScaleMat4(vec3(3.0f, 3.0f, 2.0f));
	tableGround->setRot(qTable);
	tableGround->setScale(scaleTable);

	///Tangram
	SceneNode* tangram = root->createNode();
	

	////Triangulo
	SceneNode* trianguloM = tangram->createNode();
	trianguloM->setMesh(triangle);
	trianguloM->setColor(vec4(1.0f, 1.0f, 0.0f, 1.0f));	
	mat4 transT1 = mathsMatFactory::matFactory::createTranslationMat4(vec3(0.0f, -1.55f, 0.0f));
	trianguloM->setTrans(transT1);
	qtrn qRot = qRot.qFromAngleAxis(90.0f, vec4(0.0f, 1.0f, 0.0f, 1.0f));///Rotacao de modo a ver de frente
	trianguloM->setRot(qTable * qRot);
	mat4 scaleM = mathsMatFactory::matFactory::createScaleMat4(vec3(1.1f, -1.0f, 1.1f));
	trianguloM->setScale(scaleM);
	State tM = State(transT1, qTable * qRot);
	trianguloM->setInitialState(tM);
	

	///Bigs triangles
	SceneNode* trianguloB1 = tangram->createNode();
	trianguloB1->setMesh(triangle);
	trianguloB1->setColor(vec4(1.0f, 0.0f, 1.0f, 1.0f));
	trianguloB1->setRot(qTable * qRot);
	mat4 scaleTB = mathsMatFactory::matFactory::createScaleMat4(vec3(1.5f, 1.0f, 1.5f));
	trianguloB1->setScale(scaleTB);
	mat4 transB1 = mathsMatFactory::matFactory::createTranslationMat4(vec3(-1.5f, 1.0f, 0.0f));
	trianguloB1->setTrans(transB1);
	State tB1 = State(transB1, qTable * qRot);
	trianguloB1->setInitialState(tB1);


	SceneNode* trianguloB2 = tangram->createNode();
	trianguloB2->setMesh(triangle);
	trianguloB2->setColor(vec4(1.0f, 0.0f, 0.5f, 1.0f));
	trianguloB2->setRot(qTable * qRot);
	trianguloB2->setScale(scaleTB);
	mat4 transB2 = mathsMatFactory::matFactory::createTranslationMat4(vec3(1.5f, 1.0f, 0.0f));
	trianguloB2->setTrans(transB2);
	State tB2 = State(transB2, qTable * qRot);
	trianguloB2->setInitialState(tB2);

	///Small Triangles
	SceneNode* trianguloS1 = tangram->createNode();
	trianguloS1->setMesh(triangle);
	trianguloS1->setColor(vec4(0.5f, 0.0f, 1.0f, 1.0f));
	mat4 scaleS1 = mathsMatFactory::matFactory::createScaleMat4(vec3(0.74f, 1.0f, 0.74f));
	trianguloS1->setScale(scaleS1);
	trianguloS1->setRot(qTable);
	mat4 transS1 = mathsMatFactory::matFactory::createTranslationMat4(vec3(0.74f, 0.25f, 0.0f));
	trianguloS1->setTrans(transS1);
	State tS1 = State(transS1, qTable);
	trianguloS1->setInitialState(tS1);

	SceneNode* trianguloS2 = tangram->createNode();
	trianguloS2->setMesh(triangle);
	trianguloS2->setColor(vec4(0.0f, 1.0f, 0.5f, 1.0f));
	trianguloS2->setScale(scaleS1);
	qtrn qS2 = qS2.qFromAngleAxis(-90.0f, vec4(0.0f, 1.0f, 0.0f, 1.0f));
	trianguloS2->setRot(qTable * qS2);
	mat4 transS2 = mathsMatFactory::matFactory::createTranslationMat4(vec3(0.0f, 0.95f, 0.0f));
	trianguloS2->setTrans(transS2);
	State tS2 = State(transS2, qTable * qS2);
	trianguloS2->setInitialState(tS2);
	

	///Cube
	SceneNode* cubeT = tangram->createNode();
	cubeT->setMesh(cube);
	cubeT->setColor(vec4(0.5f, 1.0f, 0.0f, 1.0f));
	mat4 scaleSquare = mathsMatFactory::matFactory::createScaleMat4(vec3(0.5f, 0.5f, 0.2f));
	cubeT->setScale(scaleSquare);
	mat4 transC = mathsMatFactory::matFactory::createTranslationMat4(vec3(0.0f, -1.f, 0.0f));
	cubeT->setTrans(transC);
	State sQ = State(transC, qtrn(1.0f, 0.0f, 0.0f, 0.0f));
	cubeT->setInitialState(sQ);

	//Parallelogram
	SceneNode* paraT = tangram->createNode();
	paraT->setMesh(parallelogram);
	paraT->setColor(vec4(1.0f, 0.5f, 0.0f, 1.0f));
	mat4 transPara = mathsMatFactory::matFactory::createTranslationMat4(vec3(-0.35f, 0.575f, 0.0f));
	paraT->setTrans(transPara);
	mat4 scalePara = mathsMatFactory::matFactory::createScaleMat4(vec3(0.5f, 1.0f, 0.5f));
	paraT->setScale(scalePara);
	qtrn qPara = qPara.qFromAngleAxis(45.0f, vec4(0.0f, 1.0f, 0.0f, 1.0f));///Rotacao de modo a ver de frente
	paraT->setRot(qTable* qPara);
	State sP = State(transPara, qTable * qPara);
	paraT->setInitialState(sP);
	

	///SQUARE

	transB1 = mathsMatFactory::matFactory::createTranslationMat4(vec3(-1.0f, 0.0f, 0.0f));
	State sB1 = State(transB1, qTable);
	trianguloB1->setFinalState(sB1);

	transB2 = mathsMatFactory::matFactory::createTranslationMat4(vec3(-1.0f, 0.0f, 0.0f));
	qtrn qQB2 = qQB2.qFromAngleAxis(90.0f, vec4(0.0f, 1.0f, 0.0f, 1.0f));
	State sB2 = State(transB1, qTable * qQB2);
	trianguloB2->setFinalState(sB2);

	transPara = mathsMatFactory::matFactory::createTranslationMat4(vec3(0.14f, -0.42f, 0.0f));
	State sPara = State(transPara, qTable * qPara);
	paraT->setFinalState(sPara);

	transS1 = mathsMatFactory::matFactory::createTranslationMat4(vec3(-0.95f, -0.1f, 0.0f));
	qtrn qQS1 = qQB2.qFromAngleAxis(180.0f, vec4(0.0f, 1.0f, 0.0f, 1.0f));
	State sS1 = State(transS1, qTable* qQS1);
	trianguloS1->setFinalState(sS1);

	transS2 = mathsMatFactory::matFactory::createTranslationMat4(vec3(-1.67f, 0.71f, 0.0f));
	State sS2= State(transS2, qTable * qS2);
	trianguloS2->setFinalState(sS2);

	qtrn qQM = qQM.qFromAngleAxis(45.0f, vec4(0.0f, 1.0f, 0.0f, 1.0f));
	mat4 transM = mathsMatFactory::matFactory::createTranslationMat4(vec3(0.5f, 1.5f, 0.0f));
	State sM = State(transM, qTable * qQM);
	trianguloM->setFinalState(sM);

	transC = mathsMatFactory::matFactory::createTranslationMat4(vec3(-0.95f, 0.65f, 0.0f));
	qtrn qC = qC.qFromAngleAxis(45.0f, vec4(0.0f, 0.0f, 1.0f, 1.0f));
	State sQ1 = State(transC, qC);
	cubeT->setFinalState(sQ1);

	nodesToAnimate.push_back(trianguloM);
	nodesToAnimate.push_back(trianguloB1);
	nodesToAnimate.push_back(trianguloB2);
	nodesToAnimate.push_back(trianguloS1);
	nodesToAnimate.push_back(trianguloS2);
	nodesToAnimate.push_back(cubeT);
	nodesToAnimate.push_back(paraT);
	

	scene = new SceneGraph(mainCamera, root);

	
}

///////////////////////////////////////////////////////////////////// CALLBACKS

void window_close_callback(GLFWwindow* win)
{
	destroyShaderProgram();
	destroyBufferObjects();
}

void window_size_callback(GLFWwindow* win, int winx, int winy)
{
	glViewport(0, 0, winx, winy);
}

void key_callback(GLFWwindow* win, int key, int scancode, int action, int mods)
{
	
	if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(win, GLFW_TRUE);
		window_close_callback(win);
	}
	if (glfwGetKey(win, GLFW_KEY_P) == GLFW_PRESS) {
		if (typeProj == 0) {
			typeProj = 1;
			return;
		}
		else {
			typeProj = 0;
			return;
		}

	}
	if (glfwGetKey(win, GLFW_KEY_F) == GLFW_PRESS) {
		delta = 0;
		animationTime = 0;
		animation = true;
		start = (float)glfwGetTime();
		if (reverse) {
			for (SceneNode* node : nodesToAnimate) {
				node->switchState();
			}
			reverse = false;
		}

		
	}

	if (glfwGetKey(win, GLFW_KEY_R) == GLFW_PRESS) {
		
		delta = 0;
		animationTime =0;
		animation = true;
		start = (float)glfwGetTime();
		if (!reverse) {
			for (SceneNode* node : nodesToAnimate) {
				node->switchState();
			}
			reverse = true;
		}

	}

	if (glfwGetKey(win, GLFW_KEY_Y) == GLFW_PRESS) {
		animation = false;

	}

	if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) {
		horizontal -= 0.1f;
		scene->root->setTrans(mathsMatFactory::matFactory::createTranslationMat4(vec3(horizontal, vertical, 0.0f)));

	}

	if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) {
		horizontal += 0.1f;
		scene->root->setTrans(mathsMatFactory::matFactory::createTranslationMat4(vec3(horizontal, vertical, 0.0f)));

	}

	if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) {
		vertical += 0.1f;
		scene->root->setTrans(mathsMatFactory::matFactory::createTranslationMat4(vec3(horizontal, vertical, 0.0f)));
	}

	if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) {
		vertical -= 0.1f;
		scene->root->setTrans(mathsMatFactory::matFactory::createTranslationMat4(vec3(horizontal, vertical, 0.0f)));
	}

	if (glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS) {
		rotTable += 10.0f ;
		qtrn qtrnAct = qtrnAct.qFromAngleAxis(rotTable,vec4( 0.0f, 0.0f, 1.0f,1.0f));
		
		
		scene->root->setRot(qtrnAct);
	}
}

void mouse_button_callback(GLFWwindow* win, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		tracking = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		tracking = false;
	}
	

}

void mouse_callback(GLFWwindow* win, double xpos, double ypos)
{
	if (tracking) {

			if (firstMouse)
			{
				lastX = (float)xpos;
				lastY = (float)ypos;
				firstMouse = false;
			}

			float xoffset = (float)xpos - lastX;
			float yoffset =  (float)ypos - lastY;
			lastX = (float)xpos;
			lastY = (float)ypos;

			float sensitivity = 0.1f;
			xoffset *= sensitivity;
			yoffset *= sensitivity;

			mainCamera->setRotMatrix(xoffset, yoffset);
	}
	

}


void scroll_callback(GLFWwindow* win, double xoffset, double yoffset)
{
	if (yoffset > 0) {
		d -= 1.0f;
	}
	else if (yoffset < 0) {
		d += 1.0f;
	}
	mainCamera->setTranslationMatrix(d);
	
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
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(win, key_callback);
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
#ifdef ERROR_CALLBACK
	setupErrorCallback();
#endif
	std::string cube_dir = "models/cube_vn.obj";
	std::string triangle_dir = "models/triangle.obj";
	std::string parallelogram_dir = "models/parallelogram.obj";
	std::string table_dir = "models/table.obj";

	table = new Mesh();
	cube = new Mesh();
	triangle = new Mesh();
	parallelogram = new Mesh();

	cube->createMesh(cube_dir);
	triangle->createMesh(triangle_dir);
	parallelogram->createMesh(parallelogram_dir);
	table->createMesh(table_dir);

	mainCamera = new Camera();
	mainCamera->setTranslationMatrix(d);

	createShaderProgram();
	createBufferObjects();
	createSceneGraph();
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
		display(win, elapsed_time);
		glfwSwapBuffers(win);
		glfwPollEvents();
#ifndef ERROR_CALLBACK
		checkOpenGLError("ERROR: MAIN/RUN");
#endif
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
		640, 480, "Tangram", is_fullscreen, is_vsync);
	run(win);
	exit(EXIT_SUCCESS);
}

/////////////////////////////////////////////////////////////////////////// END