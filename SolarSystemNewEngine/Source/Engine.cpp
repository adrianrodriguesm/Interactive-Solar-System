#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../Include/SceneGraph.h"
#include "../Include/Bloom.h"

///General
const float screenWidth = 1920 / 2;
const float screenHeight = 1920 / 2;
float aspect = (float)screenWidth / screenHeight;

///Shader
Shader* bloomShader = new Shader();
Shader* blurrShader = new Shader();
Shader* bloomMergeShader = new Shader();
Shader* earthShader = new Shader();

///Mesh
Mesh* sphereSun;
Mesh* sphereEarth;

///Bloom
Bloom* bloom;

///Textures
Texture* texSun;
Texture* EarthColorMap;

///SceneGraph
SceneGraph* scene;

//Animation
std::vector<SceneNode*> nodesToAnimate;
bool animation;
float start, delta,curr_time,animationTime;;
float duration = 2.0f;
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
float lastX = screenWidth/2, lastY = screenHeight/2;
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
	//glDebugMessageCallback(error, 0);
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

/////////////////////////////////////////////////////////////////////// MODELs
void createModels()
{
	std::string sphere_dir = "../../Models/smoothSphere.obj";
	sphereSun = new Mesh();
	sphereSun->createMesh(sphere_dir);

	sphereEarth = new Mesh();
	sphereEarth->createMesh(sphere_dir);

}
/////////////////////////////////////////////////////////////////////// TEXTUREs
void createTextures()
{
	texSun = new Texture("../../Textures/yellow.jpg");
	sphereSun->setTexture(texSun);

	EarthColorMap = new Texture("../../Textures/earthbump2k.jpg");
	sphereEarth->setTexture(EarthColorMap);
	
}

/////////////////////////////////////////////////////////////////////// SHADERs
void createEarthShader() {
	earthShader->Load("Displacement_Mapping_vert.glsl", "Displacement_Mapping_frag.glsl"); //  SHADER FILES MUST BE IN SHADER FOLDER !!
	earthShader->AddAttribute(0, "inPosition");
	earthShader->AddAttribute(1, "inTexcoord");
	earthShader->AddAttribute(2, "inNormal");
	earthShader->AddUniform("ModelMatrix");
	earthShader->AddUniform("ProjectionMatrix");
	earthShader->AddUniform("ViewMatrix");
	earthShader->AddUniform("HeightMap");
	earthShader->AddUniform("ColorMap");
	earthShader->Create();
}
void createSunShader() {
	bloomShader->Load("BloomV.glsl", "bloomF.glsl");
	bloomShader->AddAttribute(0, "aPos");
	bloomShader->AddAttribute(1, "aTexCoords");
	bloomShader->AddUniform("ModelMatrix");
	bloomShader->AddUniform("ProjectionMatrix");
	bloomShader->AddUniform("ViewMatrix");
	bloomShader->AddUniform("u_texture");
	bloomShader->Create();

	blurrShader->Load("blurrV.glsl", "blurrF.glsl");
	blurrShader->AddAttribute(0, "aPos");
	blurrShader->AddAttribute(1, "aTexCoords");
	blurrShader->AddUniform("horizontal");
	blurrShader->AddUniform("image");
	blurrShader->Create();

	bloomMergeShader->Load("bloomFinalV.glsl", "bloomFinalF.glsl");
	bloomMergeShader->AddAttribute(0, "aPos");
	bloomMergeShader->AddAttribute(1, "aTexCoords");
	bloomMergeShader->AddUniform("scene");
	bloomMergeShader->AddUniform("bloomBlur");
	bloomMergeShader->AddUniform("exposure");
	bloomMergeShader->AddUniform("bloom");
	bloomMergeShader->Create();
}

void createShaderProgram()
{
	createSunShader();
	createEarthShader();

#ifndef ERROR_CALLBACK
	checkOpenGLError("ERROR: Could not create shaders.");
#endif
}

void destroyShaderProgram()
{
	////Bloom
	bloomShader->Delete();
	blurrShader->Delete();
	bloomMergeShader->Delete();

#ifndef ERROR_CALLBACK
	checkOpenGLError("ERROR: Could not destroy shaders.");
#endif
}

/////////////////////////////////////////////////////////////////////// VAOs & VBOs
void createBufferObjects()
{
	sphereSun->createBufferObjects();
	sphereEarth->createBufferObjects();

#ifndef ERROR_CALLBACK
	checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
#endif
}

void destroyBufferObjects()
{
	sphereSun->destroyBufferObjects();
	sphereEarth->destroyBufferObjects();

#ifndef ERROR_CALLBACK
	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
#endif
}
/////////////////////////////////////////////////////////////////////// INIT BLOOM
void initBloom() {
	bloom = new Bloom();
	bloom->setScreenSize(screenWidth, screenHeight);
	bloom->createBrightFilterBuffer();
	bloom->createAttachBuffer();
	bloom->createBlurBuffer();
	bloom->activateBloom(true);///Activate default
}
/////////////////////////////////////////////////////////////////////// INIT Camera
void initCamera() {
	mainCamera = new Camera();
	mainCamera->setTranslationMatrix(d);
	mainCamera->setPersProjMatrix(fov, aspect, 0.0f, 500.0f);
}
/////////////////////////////////////////////////////////////////////// SCENE
void drawScene()
{
	//makeAnimation();
	bloom->bindHDRBuffer();
	scene->draw();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	bloom->renderWithBlurr(blurrShader);
	bloom->combineProcess(bloomMergeShader);

#ifndef ERROR_CALLBACK
	checkOpenGLError("ERROR: Could not draw scene.");
#endif
}
/////////////////////////////////////////////////////////////////////// ANIMATION(DEPRECATED)
void makeAnimation() {

	curr_time = (float)glfwGetTime();

	delta = (curr_time - start);
	animationTime = animationTime + delta;

	for (SceneNode* node : nodesToAnimate) {

		node->playAnimation(duration, animationTime);
		scene->draw();

	}
	if (animationTime > duration) {
		animation = false;
	}

	start = curr_time;
}


/////////////////////////////////////////////////////////////////////// SCENE GRAPH
void createSceneGraph()
{

	SceneNode* root = new SceneNode();
	root->setShaderProgram(bloomShader);
	SceneNode* sun_Node = root->createNode();
	sun_Node->setMesh(sphereSun);

	///EARTH
	SceneNode* earthNode = root->createNode();
	earthNode->setShaderProgram(earthShader);
	earthNode->setMesh(sphereEarth);
	earthNode->setLocalMatrix(matFactory::createTranslationMat4(vec3(0, 0, 0)));

	/*SceneNode* tableGround = root->createNode();
	tableGround->setMesh(table);
	tableGround->setColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	qtrn qTable = qTable.qFromAngleAxis(90.0f, vec4(1.0f, 0.0f, 0.0f, 1.0f));///Rotacao de modo a ver de frente
	mat4 scaleTable = mathsMatFactory::matFactory::createScaleMat4(vec3(3.0f, 3.0f, 2.0f));
	tableGround->setRot(qTable);
	tableGround->setScale(scaleTable);

	///Tangram
	SceneNode* tangram = root->createNode();
	nodesToAnimate.push_back(trianguloM);
	nodesToAnimate.push_back(trianguloB1);
	nodesToAnimate.push_back(trianguloB2);
	nodesToAnimate.push_back(trianguloS1);
	nodesToAnimate.push_back(trianguloS2);
	nodesToAnimate.push_back(cubeT);
	nodesToAnimate.push_back(paraT);*/

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
			mainCamera->setPersProjMatrix(fov, aspect, 0.0f, 500.0f);
			return;
		}
		else {
			typeProj = 0;
			mainCamera->setOrthoProjMatrix(-5.0f, 5.0f, -5.0f, 5.0f, 1.0f, 20.0f);
			return;
		}
	}
	if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) {
		horizontal -= 0.1f;
		scene->root->setTrans(matFactory::createTranslationMat4(vec3(horizontal, vertical, 0.0f)));
	}
	if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) {
		horizontal += 0.1f;
		scene->root->setTrans(matFactory::createTranslationMat4(vec3(horizontal, vertical, 0.0f)));
	}

	if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) {
		vertical += 0.1f;
		scene->root->setTrans(matFactory::createTranslationMat4(vec3(horizontal, vertical, 0.0f)));
	}

	if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) {
		vertical -= 0.1f;
		scene->root->setTrans(matFactory::createTranslationMat4(vec3(horizontal, vertical, 0.0f)));
	}

	if (glfwGetKey(win, GLFW_KEY_Q) == GLFW_PRESS) {
		rotTable += 10.0f;
		qtrn qtrnAct = qtrnAct.qFromAngleAxis(rotTable, vec4(0.0f, 0.0f, 1.0f, 1.0f));
		scene->root->setRot(qtrnAct);
	}
	/////BLOOM
	if (glfwGetKey(win, GLFW_KEY_B) == GLFW_PRESS) { bloom->activateBloom(true);}
	if (glfwGetKey(win, GLFW_KEY_V) == GLFW_PRESS) { bloom->activateBloom(false); }
	if (glfwGetKey(win, GLFW_KEY_N) == GLFW_PRESS) { bloom->increaseExpresure(); }
	if (glfwGetKey(win, GLFW_KEY_M) == GLFW_PRESS) { bloom->decreaseExpresure(); }

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
		float yoffset = (float)ypos - lastY;
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
	//glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

	//Camera:
	initCamera();
	//Bloom:
	initBloom();
	//Meshes:
	createModels();
	//Textures :
	createTextures();
	//Shaders:
	createShaderProgram();
	//BufferObjects:
	createBufferObjects();
	//Scene:
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
		screenWidth, screenHeight, "Interactive-Solar-System", is_fullscreen, is_vsync);
	run(win);
	exit(EXIT_SUCCESS);
}

/////////////////////////////////////////////////////////////////////////// END