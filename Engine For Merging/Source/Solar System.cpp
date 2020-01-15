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
#include "Bloom.h"
#include "snapshot.h"
#include "FlareManager.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>



////////////////////////////////////////////////// VARIABLES

//General
const float earthTilt = 23.5;
int screenWidth = 1920;
int screenHeight = 1080;
float aspect = (float)screenWidth / screenHeight;
vec4 xAxis = vec4(1, 0, 0, 1);
vec4 yAxis = vec4(0, 1, 0, 1);
vec4 zAxis = vec4(0, 0, -1, 1);
vec4 earthAxis = normalize(yAxis * cos(earthTilt * DEGREES_TO_RADIANS) + xAxis * sin(earthTilt * DEGREES_TO_RADIANS));

//Camera
const int cameraMaxDistance = 1000;
bool mouseChange = true; //Boolean for mouse input
bool scrollChange = true; //Boolean for scroll input
Camera cam = Camera(vec3(0,0,1), vec3(0,0,0), vec3(0,1,0)); //Initial camera center and up vector
float cameraDistance = 20;									//Initial camera distance from center defined in the line above
mat4 cameraRotation;
mat4 cameraTranslation;

//Declaration of Textures:
Texture* EarthColorMapLowResu;
Texture* EarthColorMapHighResu;
Texture* EarthHeightMapLowResu;
Texture* EarthHeightMapHighResu;
Texture* EarthSpecularMap;
Texture* SunTex;
Texture* skyBoxTex;
Texture* JupiterTex;
Texture* LensTex1; Texture* LensTex2; Texture* LensTex3;
/////////////////

///LensFlare
FlareTexture* flare1; FlareTexture* flare2; FlareTexture* flare3; FlareTexture* flare4; FlareTexture* flare5;
FlareTexture* flare6;
FlareTexture* flareTex[6];
FlareManager* flareManager;

bool highResu = false; // Bool for resolution change

//Declaration of meshes:
Mesh* sphereMesh;
Mesh* skyBoxMesh;
Mesh* quadMesh;
/////////////////

//Declaration of Shaders:
Shader* earthShader = new Shader();
Shader* earthShaderV2 = new Shader();
Shader* bloomShader = new Shader();
Shader* blurrShader = new Shader();
Shader* bloomMergeShader = new Shader();
Shader* skyBoxShader = new Shader();
Shader* jupiterShader = new Shader();
Shader* lensFlareShader = new Shader();
/////////////////


///Bloom
Bloom* bloom;

//Snapshot 
Snapshot snapshot;
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
	if (severity != GL_DEBUG_SEVERITY_NOTIFICATION) {
		std::cerr << "Press <return>.";
		std::cin.ignore();
	}
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

void createskyBoxTextures() {
	std::string skyDir = "../../Textures/Skybox/";
	std::vector<std::string> faces{
		skyDir + "bkg1_right.png",
		skyDir + "bkg1_left.png",
		skyDir + "bkg1_top.png",
		skyDir + "bkg1_bot.png",
		skyDir + "bkg1_front.png",
		skyDir + "bkg1_back.png"
	};

	skyBoxTex = new Texture(faces);
}
void createLensTextures() {

	LensTex1 = new Texture("../../Textures/tex10.png");
	LensTex2 = new Texture("../../Textures/tex11.png");
	LensTex3 = new Texture("../../Textures/tex12.png");

	flare1 = new FlareTexture();
	flare1->tex = LensTex1;
	flare1->scale = 0.3f;
	flareTex[0] = flare1;

	flare2 = new FlareTexture();
	flare2->tex = LensTex2;
	flare2->scale = 0.4f;
	flareTex[1] = flare2;


	flare3 = new FlareTexture();
	flare3->scale = 0.5f;
	flare3->tex = LensTex2;
	flareTex[2] = flare3;


	flare4 = new FlareTexture();
	flare4->scale = 0.6f;
	flare4->tex = LensTex3;
	flareTex[3] = flare4;


	flare5 = new FlareTexture();
	flare5->scale = 0.6f;
	flare5->tex = LensTex3;
	flareTex[4] = flare5;


	flare6 = new FlareTexture();
	flare6->scale = 0.55f;
	flare6->tex = LensTex3;
	flareTex[5] = flare6;

	flareManager = new FlareManager(flareTex, 0.2f);
}
void createTextures() {

	//Stars
	createskyBoxTextures();
	skyBoxShader->Use();
	glUniform1i(skyBoxShader->Uniforms["SkyBox"], 0);
	glUseProgram(0);
	///

	//Earth
	EarthColorMapLowResu = new Texture("../../Textures/Earth/earthmap1k.jpg");
	EarthHeightMapLowResu = new Texture("../../Textures/Earth/earthbump1k.jpg");
	//EarthColorMapHighResu = new Texture("../../Textures/Earth/earthmap2k.jpg");
	EarthHeightMapHighResu = new Texture("../../Textures/Earth/earthbump2k.jpg");

	//test
	EarthColorMapHighResu = new Texture("../../Textures/yellow.jpg");
	//

	EarthSpecularMap = new Texture("../../Textures/Earth/earthspec1k.jpg");

	earthShader->Use();
	glUniform1i(earthShader->Uniforms["ColorMap"], 0);
	glUniform1i(earthShader->Uniforms["HeightMap"],1);

	earthShaderV2->Use();
	glUniform1i(earthShaderV2->Uniforms["ColorMap"], 0);
	glUniform1i(earthShaderV2->Uniforms["HeightMap"], 1);
	glUniform1i(earthShaderV2->Uniforms["SpecularMap"], 2);
	glUseProgram(0);
	///

	//Sun
	SunTex = new Texture("../../Textures/yellow.jpg");
	bloomShader->Use();
	glUniform1i(bloomShader->Uniforms["u_Texture"], 10);
	glUseProgram(0);

	JupiterTex = new Texture();
	JupiterTex->createPerlinNoiseTexture();
	jupiterShader->Use();
	JupiterTex->Bind(JupiterTex->GetId());
	glUniform1i(jupiterShader->Uniforms["u_Texture"], JupiterTex->GetId());
	glUseProgram(0);

	createLensTextures();

}

/////////////////////////////////////////////////////////////////////// MESHES

void createMeshes() {
	std::string mesh_dir = "../../Blender Objects/";		// This is the working mesh dir relative to this main

	std::string spherePath = mesh_dir + "smoothSphere.obj"; // Just add your mesh.obj in the folder "Blender Objects, and put the file name here
	sphereMesh = new Mesh(spherePath);							//Create it by using the constructor of the mesh class

	std::string cube_dir = mesh_dir + "Cube.obj";
	skyBoxMesh = new Mesh(cube_dir);

	std::string quadPath = mesh_dir + "quad.obj";
	quadMesh = new Mesh(quadPath);
}

//Add your mesh.destroy here afterwards:
void destroyMeshes() {
	sphereMesh->destroy();
	skyBoxMesh->destroy();
	quadMesh->destroy();
}

/////////////////////////////////////////////////////////////////////// SHADER

//Make a "createYOURShader" function for each of your shaders like this:

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

	//Temporary V2 to test specular shading (Victor do your thing! :P )
	earthShaderV2->Load("Displacement_Mapping_vert_V2.glsl", "Displacement_Mapping_frag_V2.glsl"); //  SHADER FILES MUST BE IN SHADER FOLDER !!
	earthShaderV2->AddAttribute(0, "inPosition");
	earthShaderV2->AddAttribute(1, "inTexcoord");
	earthShaderV2->AddAttribute(2, "inNormal");
	earthShaderV2->AddUniform("SunPosition");
	earthShaderV2->AddUniform("ModelMatrix");
	earthShaderV2->AddUniform("ProjectionMatrix");
	earthShaderV2->AddUniform("ViewMatrix");
	earthShaderV2->AddUniform("HeightMap");
	earthShaderV2->AddUniform("ColorMap");
	earthShaderV2->AddUniform("SpecularMap");

	earthShaderV2->AddUniform("lightPosition");
	earthShaderV2->AddUniform("cameraValue");
	earthShaderV2->AddUniform("lightColor");
	earthShaderV2->AddUniform("att.constant");
	earthShaderV2->AddUniform("att.linear");
	earthShaderV2->AddUniform("att.quadratic");
	earthShaderV2->Create();
}

void createskyBoxShader() {
	skyBoxShader->Load("Skybox_vert.glsl", "Skybox_frag.glsl");
	skyBoxShader->AddAttribute(0, "inPosition");
	skyBoxShader->AddUniform("ModelMatrix");
	skyBoxShader->AddUniform("ProjectionMatrix");
	skyBoxShader->AddUniform("ViewMatrix");
	skyBoxShader->AddUniform("SkyBox");
	skyBoxShader->Create();
}

void createBloomShader() {
	bloomShader->Load("bloomV.glsl", "bloomF.glsl");
	bloomShader->AddAttribute(0, "aPos");
	bloomShader->AddAttribute(1, "aTexCoords");
	bloomShader->AddUniform("ModelMatrix");
	bloomShader->AddUniform("ProjectionMatrix");
	bloomShader->AddUniform("ViewMatrix");
	bloomShader->AddUniform("u_Texture");
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
	bloomMergeShader->Create();
}

void createJupiterShader() 
{
	jupiterShader->Load("jupiterV.glsl", "jupiterF.glsl");
	jupiterShader->AddAttribute(0, "in_Position");
	jupiterShader->AddAttribute(1, "texCoord");
	jupiterShader->AddAttribute(2, "normals");

	jupiterShader->AddUniform("ModelMatrix");
	jupiterShader->AddUniform("ViewMatrix");
	jupiterShader->AddUniform("ProjectionMatrix");
	jupiterShader->AddUniform("lightPosition");
	jupiterShader->AddUniform("cameraValue");
	jupiterShader->AddUniform("u_Texture");
	jupiterShader->AddUniform("lightColor");
	jupiterShader->AddUniform("att.constant");
	jupiterShader->AddUniform("att.linear");
	jupiterShader->AddUniform("att.quadratic");
	jupiterShader->Create();
}

void createLensFlareShader() {
	lensFlareShader->Load("flareV.glsl", "flaref.glsl");
	lensFlareShader->AddAttribute(0, "in_position");
	lensFlareShader->AddUniform("ModelMatrix");
	lensFlareShader->AddUniform("ProjectionMatrix");
	lensFlareShader->AddUniform("ViewMatrix");
	lensFlareShader->AddUniform("u_Texture");
	lensFlareShader->AddUniform("transform");
	lensFlareShader->AddUniform("brightness");
	lensFlareShader->Create();
}
//Then add your function to this createShaders function which is called in the "Setup" function.
void createShaders() {
	createskyBoxShader();
	createEarthShader();
	createBloomShader();
	createJupiterShader();
	createLensFlareShader();
}

// Also add the delete function for your shader here:
void deleteShaders() {
	earthShader->Delete();

	////Bloom
	bloomShader->Delete();
	blurrShader->Delete();
	bloomMergeShader->Delete();

	skyBoxShader->Delete();

	//Jupiter
	jupiterShader->Delete();
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

void lightingSetUp(Shader *shader) 
{
	shader->Use();
	glUniform4f(shader->Uniforms["lightPosition"], 0.0f, 0.0f, 0.0f, 1.0f);
	glUniform4f(shader->Uniforms["lightColor"], 1.0f,1.0f,1.0f,1.0f);
	glUseProgram(0);
}

void updateCameraInShader(Shader* shader)
{
	shader->Use();
	glUniform1f(shader->Uniforms["cameraValue"], cameraDistance);
	glUseProgram(0);
}

void attenuationSetUp(Shader* shader, float linear, float quadratic)
{
	shader->Use();
	glUniform1f(shader->Uniforms["att.constant"], 1.0f);
	glUniform1f(shader->Uniforms["att.linear"], linear);
	glUniform1f(shader->Uniforms["att.quadratic"],quadratic);
	glUseProgram(0);
}
/////////////////////////////////////////////////////////////////////// SCENE

SceneGraph* scenegraph;

//Initialize nodes here:

SceneNode* base;
SceneNode* skyBoxNode;
SceneNode* sunNode;
SceneNode* earthNode;
SceneNode* jupiterNode;
SceneNode* lensFlare;

///////////////////
void createAnimationObjects();

///SKYBOX
float skyBoxSize = 200;
mat4 skyBoxScale = MatrixFactory::createScaleMat4(vec3(skyBoxSize));
///

void createScene(SceneGraph* scenegraph) {
	
	lightingSetUp(jupiterShader);
	attenuationSetUp(jupiterShader, 0.014f, 0.0007f);
	lightingSetUp(earthShaderV2);
	attenuationSetUp(earthShaderV2, 0.014f, 0.0007f);
	base = scenegraph->createNode();
	
	skyBoxNode = base->createNode();
	skyBoxNode->setMesh(skyBoxMesh);
	skyBoxNode->setShader(skyBoxShader);
	skyBoxNode->setTexture(skyBoxTex);
	skyBoxNode->setScaleMatrix(skyBoxScale);

	sunNode = base->createNode();
	sunNode->setMesh(sphereMesh);
	sunNode->setTexture(SunTex);
	sunNode->setShader(bloomShader);
	sunNode->setMatrix(MatrixFactory::createTranslationMat4(vec3(0, 0, 0)));
	sunNode->setScaleMatrix(MatrixFactory::createScaleMat4(vec3(5)));

	earthNode = base->createNode();
	earthNode->setMesh(sphereMesh);
	//earthNode->setShader(earthShader);
	earthNode->setShader(earthShaderV2);
	earthNode->setTexture(EarthColorMapLowResu);
	earthNode->setMatrix(MatrixFactory::createTranslationMat4(vec3(4, 0, 0)));

	jupiterNode = base->createNode();
	jupiterNode->setMesh(sphereMesh);
	jupiterNode->setShader(jupiterShader);
	jupiterNode->setTexture(JupiterTex);
	jupiterNode->setMatrix(MatrixFactory::createTranslationMat4(vec3(8, 0, 0)));
	jupiterNode->setScaleMatrix(MatrixFactory::createScaleMat4(vec3(2)));

	lensFlare = base->createNode();
	lensFlare->setMesh(quadMesh);
	lensFlare->setShader(lensFlareShader);

	createAnimationObjects();
}

void createSceneGraph(Camera& cam) {

	//Initialization of the scenegraph:
	scenegraph = new SceneGraph();
	scenegraph->setCamera(cam);
	scenegraph->getCamera()->ProjectionMatrix = MatrixFactory::createPerspectiveProjectionMatrix(30, aspect, 1, cameraMaxDistance);
	SceneNode* n = scenegraph->getRoot();
	//////////////////////

	// n->setShader(&earthShader); //Set the base shader here (if any). The child nodes will use the parent's shader 
	// if they don't have a shader assigned to themselves.

	createScene(scenegraph);
}


/////////////////////////////////////////////////////////////////////// ANIMATION


//In the "animationObject" struct we have to fill in all information that is relative to the animation.
typedef struct {
	SceneNode* node;
	float selfRotateSpeed; //The speed at which the planet rotates around itself
	float orbitSpeed; //The speed of the orbit around the sun
	qtrn currentSelfRoation;
	qtrn currentOrbitRotation;
	qtrn tilt; 
	mat4 sunDistance;
} animationObject;
////////////////////////

//We have to fill this vector with animationObjects when we create the sceneNodes:
vector<animationObject> animationObjects = vector<animationObject>(); 
////////////////////////

void createAnimationObjects() {

	//Example on how to set an animationobject:
	animationObject earthAnimObj = animationObject{
		earthNode,
		20,
		5,
		qtrn::qFromAngleAxis(0, yAxis),
		qtrn::qFromAngleAxis(0, yAxis),
		qtrn::qFromAngleAxis(earthTilt, zAxis),
		MatrixFactory::createTranslationMat4(vec3(10 + 4,0,0))
	};

	animationObjects.push_back(earthAnimObj);
	////

	//Jupiter:
	animationObject jupiterAnimObj = animationObject{
	jupiterNode,
	5,
	3,
	qtrn::qFromAngleAxis(0, yAxis),
	qtrn::qFromAngleAxis(0, yAxis),
	qtrn::qFromAngleAxis(0, zAxis),
	MatrixFactory::createTranslationMat4(vec3(10 + 10,0,0))
	};

	animationObjects.push_back(jupiterAnimObj);
}

float animationSpeed = 0.1;

void updateAnimation() {
	int i = 0;
	for (animationObject obj : animationObjects) {

		//Self Rotation:
		obj.currentSelfRoation = obj.currentSelfRoation * qtrn::qFromAngleAxis(animationSpeed * obj.selfRotateSpeed, yAxis);
		mat4 selfRot = matrixFromQtrn(obj.currentSelfRoation);
		///

		//Orbit:
		obj.currentOrbitRotation = obj.currentOrbitRotation * qtrn::qFromAngleAxis(animationSpeed * obj.orbitSpeed, yAxis);
		mat4 orbitRot = matrixFromQtrn(obj.currentOrbitRotation);
		///

		//Tilt:
		mat4 compensatedTilt = matrixFromQtrn(inverse(obj.currentOrbitRotation) * obj.tilt);
		///

		//Set the transformation matrix (This will overwrite anything else in the transformation):
		obj.node->setMatrix(orbitRot * obj.sunDistance * compensatedTilt * selfRot);

		//Update the actual animation object with the copy:
		animationObjects[i] = obj;
		i++;
	}
}

/////////////////////////////////////////////////////////////////////// DRAW SCENE
void checkResolution() {
	vec3 earthPos = vec3(earthNode->getMatrix() * vec4(0, 0, 0, 1));
	highResu = fabs((earthPos - cam.Eye).MagnitudeSqrd()) < 100? true : false;
}

void drawSkyBox() {

	glFrontFace(GL_CW);//To make the faces drawn towards us (inside the cube)
	glDepthFunc(GL_LEQUAL);//The depth in clip space is set to 1, so we need to change the depth test from less to less or equal for it to be drawn

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxTex->GetId());

	// I am copying the camera to remove its translation part so that the distance to the 'universe' does not depend on the camera position
	Camera camCopy = cam;
	camCopy.ViewMatrix = MatrixFactory::createMat4FromMat3(MatrixFactory::createMat3FromMat4(camCopy.ViewMatrix));

	skyBoxNode->draw(&cam);

	glDepthFunc(GL_LESS);
	glFrontFace(GL_CCW);
}

void drawLensFlare() {
	//glClear(GL_DEPTH_BUFFER_BIT);

	if (cameraDistance > 0) {
		flareManager->render(&cam, vec4(0, 0, 0.9f, 1));

		for (int i = 0; i < 6; i++)
		{
			lensFlareShader->Use();
			flareTex[i]->tex->Bind(flareTex[i]->tex->GetId());
			float x = flareTex[i]->pos.x;
			float y = flareTex[i]->pos.y;
			float scaleX = flareTex[i]->scale;
			float scaleY = flareTex[i]->scale * aspect;
			glUniform1i(lensFlareShader->Uniforms["u_Texture"], flareTex[i]->tex->GetId());
			glUniform4f(lensFlareShader->Uniforms["transform"], x, y, scaleX, scaleY);
			glUniform1f(lensFlareShader->Uniforms["brightness"], flareTex[i]->brightness);
			glUseProgram(0);
			lensFlare->draw(&cam);
		}
	}
}

void drawScene() {
	updateAnimation();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	bloom->bindHDRBuffer();
	
	//We have to draw everything but the bloom in here (skybox last for performance reasons):
	EarthSpecularMap->Bind(2);
	checkResolution();
	if (highResu) {
		EarthColorMapHighResu->Bind();
		EarthHeightMapHighResu->Bind(1);
	}
	else {
		EarthColorMapLowResu->Bind();
		EarthHeightMapLowResu->Bind(1);
	}	
	updateCameraInShader(earthShaderV2);
	earthNode->draw(&cam);

	updateCameraInShader(jupiterShader);
	JupiterTex->Bind(JupiterTex->GetId());
	jupiterNode->draw(&cam);


	drawSkyBox();
	/////////////

	SunTex->Bind(10);
	sunNode->draw(&cam);

	scenegraph->draw();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	bloom->renderWithBlurr(blurrShader);
	bloom->combineProcess(bloomMergeShader);	
	drawLensFlare();
}

/////////////////////////////////////////////////////////////////////// WINDOW CALLBACKS

//Here we add all the destroy/Delete functions. Maybe we should add one for textures.
void window_close_callback(GLFWwindow* win)
{
	deleteShaders();
	destroyMeshes();
}
///////////////////////

void window_size_callback(GLFWwindow* win, int winx, int winy)
{
	glViewport(0, 0, winx, winy);
	bloom->setScreenSize(winx, winy);
	aspect = (float)winx / (float)winy;
	screenHeight = winy;
	screenWidth = winx;
	cout << "aspect: " << aspect << " | width : " << winx << " | height : " << winy << std::endl;
	scenegraph->getCamera()->ProjectionMatrix = MatrixFactory::createPerspectiveProjectionMatrix(45, aspect, 1, cameraMaxDistance);
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
float scrollSensitivity = 0.5;

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

	//This is a toggle key, it has a timer so that it doesn't "spam" itself when you press it:
	if ((glfwGetKey(win, GLFW_KEY_P) == GLFW_PRESS) && (timeoutKeyP <= 0)) {
		keyP = true;
		timeoutKeyP = 10;
	}
	else {
		keyP = false;
		timeoutKeyP = fmaxf(--timeoutKeyP, -0.1f);
	}

	/////BLOOM
	if (glfwGetKey(win, GLFW_KEY_B) == GLFW_PRESS) { bloom->activateBloom(true); }
	if (glfwGetKey(win, GLFW_KEY_V) == GLFW_PRESS) { bloom->activateBloom(false); }
	if (glfwGetKey(win, GLFW_KEY_N) == GLFW_PRESS) { bloom->increaseExpresure(); }
	if (glfwGetKey(win, GLFW_KEY_M) == GLFW_PRESS) { bloom->decreaseExpresure(); }

	//Snapshot
	if (glfwGetKey(win, GLFW_KEY_Z) == GLFW_PRESS) { snapshot.captureSnapshot(); }
}

void process_keyboard_input(GLFWwindow* win) {
	if (keyA) shapeMovementX -= movStep;
	if (keyD) shapeMovementX += movStep;
	if (keyS) shapeMovementY -= movStep;
	if (keyW) shapeMovementY += movStep;
	if (keyQ) shapeRotation -= angStep;
	if (keyE) shapeRotation += angStep;

	// THIS WILL MOVE THE ENTIRE SOLAR SYSTEM
	base->setMatrix(
		MatrixFactory::createRoationMat4(shapeRotation, zAxis) * 
		MatrixFactory::createTranslationMat4(vec3(shapeMovementX, shapeMovementY, 0))
	);
	/////////////////////////

	if (keyP) highResu = !highResu; //Change resolution
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
	mat4 inv = inverse(cam.ViewMatrix);
	cam.Eye = vec3(inv.data[11], inv.data[12], inv.data[13]);
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

	//Snapshot:
	snapshot = Snapshot();

	///Init Bloom frameBuffers:
	initBloom();
	//Meshes:
	createMeshes();
	//Shader:
	createShaders();
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
		screenWidth, screenHeight, "Interactive Solar System", is_fullscreen, is_vsync);
	run(win);
	exit(EXIT_SUCCESS);
}

/////////////////////////////////////////////////////////////////////////// END