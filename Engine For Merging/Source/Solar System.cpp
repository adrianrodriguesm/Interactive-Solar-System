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
#include "Loader.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>




////////////////////////////////////////////////// VARIABLES

//General
const float earthTilt = 23.5;
int screenWidth = 1920/2;
int screenHeight = 1080/2;
float aspect = (float)screenWidth / screenHeight;
vec4 xAxis = vec4(1, 0, 0, 1);
vec4 yAxis = vec4(0, 1, 0, 1);
vec4 zAxis = vec4(0, 0, -1, 1);
vec4 earthAxis = normalize(yAxis * cos(earthTilt * DEGREES_TO_RADIANS) + xAxis * sin(earthTilt * DEGREES_TO_RADIANS));

//Camera
const int cameraMaxDistance = 1000;
Camera cam = Camera(vec3(0,0,1), vec3(0,0,0), vec3(0,1,0)); //Initial camera center and up vector
float cameraDistance = 50;									//Initial camera distance from the center defined in the line above
mat4 cameraRotation;
mat4 cameraTranslation;
mat4 cameraTargetRotation = MatrixFactory::createRoationMat4(0, yAxis);
mat4 cameraTargetTranslation = MatrixFactory::createTranslationMat4(vec3(0));

//Declaration of Textures:
Texture* EarthColorMapLowResu;
Texture* EarthColorMapHighResu;
Texture* EarthHeightMap;
Texture* EarthSpecularMap;
Texture* EarthClouds;
Texture* EarthCloudTransparacy;
Texture* EarthNightHighResu;
Texture* EarthNightLowResu;
Texture* SunTex;
Texture* skyBoxTex;
Texture* JupiterTex;
Texture* LensTex1; Texture* LensTex2; Texture* LensTex3;
Texture* MercuryTex;
Texture* VenusTex;
Texture* MarsTex;
Texture* SaturnTex;
Texture* SaturnRingTex;
Texture* NeptuneTex;
Texture* UranusTex;
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
Mesh* torusMesh;
/////////////////

//Declaration of Shaders:
Shader* earthShaderV2 = new Shader();
Shader* bloomShader = new Shader();
Shader* blurrShader = new Shader();
Shader* bloomMergeShader = new Shader();
Shader* skyBoxShader = new Shader();
Shader* jupiterShader = new Shader();
Shader* blinnPhongShader = new Shader();
Shader* lensFlareShader = new Shader();
/////////////////

///Bloom
Bloom* bloom;

//Snapshot 
Snapshot* snapshot;

///Loader
Loader load;
bool isEmpty = false;
bool justOnce = true;
void rebootAnin();

///Stencil buffer id's
unsigned int stencilId = 0;

#define MERCURY 1
#define VENUS 2
#define EARTH 3
#define MARS 4
#define JUPITER 5
#define SATURN 6
#define URANUS 7
#define NEPTUN 8

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

	flareManager = new FlareManager(flareTex, 0.0000000001f);
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
	EarthColorMapHighResu = new Texture("../../Textures/Earth/earthmap4k.jpg");
	EarthNightHighResu = new Texture("../../Textures/Earth/earthlights4k.jpg");
	EarthNightLowResu = new Texture("../../Textures/Earth/earthlights1k.jpg");

	EarthHeightMap = new Texture("../../Textures/Earth/earthbump1k.jpg");
	EarthSpecularMap = new Texture("../../Textures/Earth/earthspec1k.jpg");
	EarthClouds = new Texture("../../Textures/Earth/earthcloudmap.jpg");
	EarthCloudTransparacy = new Texture("../../Textures/Earth/earthcloudmaptrans.jpg");

	earthShaderV2->Use();
	glUniform1i(earthShaderV2->Uniforms["ColorMap"], 0);
	glUniform1i(earthShaderV2->Uniforms["HeightMap"], 1);
	glUniform1i(earthShaderV2->Uniforms["SpecularMap"], 2);
	glUniform1i(earthShaderV2->Uniforms["Clouds"], 3);
	glUniform1i(earthShaderV2->Uniforms["CloudTransparancy"], 4);
	glUniform1i(earthShaderV2->Uniforms["NightMap"], 5);

	glUseProgram(0);
	///

	//Sun
	SunTex = new Texture("../../Textures/yellow.jpg");
	bloomShader->Use();
	glUniform1i(bloomShader->Uniforms["u_Texture"], 10);
	glUseProgram(0);

	//Jupiter, uses Perlin Noise
	JupiterTex = new Texture();
	JupiterTex->createPerlinNoiseTexture();
	jupiterShader->Use();
	JupiterTex->Bind(JupiterTex->GetId());
	glUniform1i(jupiterShader->Uniforms["u_Texture"], JupiterTex->GetId());
	glUseProgram(0);


	//Mercury
	MercuryTex = new Texture("../../Textures/Mercury.jpg");
	//Venus
	VenusTex = new Texture("../../Textures/Venus.jpg");
	//Mars
	MarsTex = new Texture("../../Textures/Mars.jpg");
	//Saturn
	SaturnTex = new Texture("../../Textures/Saturn.jpg");
	//Saturn Ring
	SaturnRingTex = new Texture("../../Textures/SaturnRing.png");
	//Neptune
	NeptuneTex = new Texture("../../Textures/Neptune.jpg");
	//Uranus
	UranusTex = new Texture("../../Textures/Uranus.jpg");


	createLensTextures();

}

//Refactor that's used by all the planets using the BlinnPhong Shader
void bindTextureToShader(Texture* tex, Shader* shader) 
{
	shader->Use();
	tex->Bind(tex->GetId());
	glUniform1i(shader->Uniforms["Texture"], tex->GetId());
	glUseProgram(0);

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

	std::string torusPath = mesh_dir + "torusHQ.obj"; //Torus used for the ring in Saturn
	torusMesh = new Mesh(torusPath);
}

//Add your mesh.destroy here afterwards:
void destroyMeshes() {
	sphereMesh->destroy();
	skyBoxMesh->destroy();
	quadMesh->destroy();
	torusMesh->destroy();
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
	earthShaderV2->AddUniform("Clouds");
	earthShaderV2->AddUniform("CloudTransparancy");
	earthShaderV2->AddUniform("NightMap");

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

void createBlinnPhongShader()
{
	blinnPhongShader->Load("blinnPhongV.glsl", "blinnPhongF.glsl");
	blinnPhongShader->AddAttribute(0, "in_Position");
	blinnPhongShader->AddAttribute(1, "texCoord");
	blinnPhongShader->AddAttribute(2, "normals");
	blinnPhongShader->AddUniform("ModelMatrix");
	blinnPhongShader->AddUniform("ViewMatrix");
	blinnPhongShader->AddUniform("ProjectionMatrix");
	blinnPhongShader->AddUniform("lightPosition");
	blinnPhongShader->AddUniform("cameraValue");
	blinnPhongShader->AddUniform("Texture");
	blinnPhongShader->AddUniform("lightColor");
	blinnPhongShader->AddUniform("att.constant");
	blinnPhongShader->AddUniform("att.linear");
	blinnPhongShader->AddUniform("att.quadratic");
	blinnPhongShader->Create();
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
	createBlinnPhongShader();
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

	//Blinn Phong
	blinnPhongShader->Delete();
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

//Sets up attenuation values, point light and light color.
void lightingSetUp(Shader *shader) 
{
	shader->Use();
	glUniform4f(shader->Uniforms["lightPosition"], 0.0f, 0.0f, 0.0f, 1.0f); //Point light that represents the Sun
	glUniform4f(shader->Uniforms["lightColor"], 1.0f,1.0f,1.0f,1.0f); // White light
	//Attenuation values
	glUniform1f(shader->Uniforms["att.constant"], 1.0f);
	glUniform1f(shader->Uniforms["att.linear"], 0.0014f);
	glUniform1f(shader->Uniforms["att.quadratic"], 0.000007f);
	glUseProgram(0);
}

//Updates camera position in shaders
void updateCameraInShader(Shader* shader)
{
	shader->Use();
	glUniform1f(shader->Uniforms["cameraValue"], cameraDistance);
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
SceneNode* mercuryNode;
SceneNode* venusNode;
SceneNode* marsNode;
SceneNode* saturnNode;
SceneNode* saturnRingNode;
SceneNode* uranusNode;
SceneNode* neptuneNode;
SceneNode* lensFlare;

///////////////////
void createAnimationObjects();

///SKYBOX
float skyBoxSize = 200;
mat4 skyBoxScale = MatrixFactory::createScaleMat4(vec3(skyBoxSize));
///

//Thus helps create the planet's node
void createPlanetNode(SceneNode* node, Mesh* mesh, Shader* shader, Texture* tex, vec3 translationVector, float scaleValue)
{
	node->setMesh(mesh);
	node->setShader(shader);
	node->setTexture(tex);
	node->setMatrix(MatrixFactory::createTranslationMat4(translationVector));
	node->setScaleMatrix(MatrixFactory::createScaleMat4(vec3(scaleValue)));
}

////////////////////////////////////////////////LOADER
void loadScene() {
	load.readState();
	mat4 identity = MatrixFactory::createIdentityMat4();
	base->setMatrix(load.Matrices["base"]);

	skyBoxNode->setMatrix(load.Matrices["skyboxNode"]);
	skyBoxNode->setScaleMatrix(identity);

	sunNode->setMatrix(load.Matrices["sunNode"]);
	sunNode->setScaleMatrix(identity);

	earthNode->setMatrix(load.Matrices["earthNode"]);
	earthNode->setScaleMatrix(identity);

	mercuryNode->setMatrix(load.Matrices["mercuryNode"]);
	mercuryNode->setScaleMatrix(identity);

	venusNode->setMatrix(load.Matrices["venusNode"]);
	venusNode->setScaleMatrix(identity);

	marsNode->setMatrix(load.Matrices["marsNode"]);
	marsNode->setScaleMatrix(identity);

	jupiterNode->setMatrix(load.Matrices["jupiterNode"]);
	jupiterNode->setScaleMatrix(identity);

	saturnNode->setMatrix(load.Matrices["saturnNode"]);
	saturnNode->setScaleMatrix(identity);

	saturnRingNode->setScaleMatrix(MatrixFactory::createScaleMat4(vec3(0.1f, 0.005f, 0.1f)));

	uranusNode->setMatrix(load.Matrices["uranusNode"]);
	uranusNode->setScaleMatrix(identity);

	neptuneNode->setMatrix(load.Matrices["neptuneNode"]);
	neptuneNode->setScaleMatrix(identity);

	lensFlare->setMatrix(load.Matrices["lensFlare"]);
	lensFlare->setScaleMatrix(identity);

	scenegraph->getCamera()->ViewMatrix = load.Matrices["View"];
	scenegraph->getCamera()->ProjectionMatrix = load.Matrices["Projection"];
	cameraDistance = load.camDist;	
	

	
}
/////////////////////////////////////////////////////////////////

void createScene(SceneGraph* scenegraph) {
	
	lightingSetUp(jupiterShader);
	lightingSetUp(earthShaderV2);
	lightingSetUp(blinnPhongShader);
	base = scenegraph->createNode();
	base->setName("base");
	
	//Skybox
	skyBoxNode = base->createNode();
	skyBoxNode->setMesh(skyBoxMesh);
	skyBoxNode->setShader(skyBoxShader);
	skyBoxNode->setTexture(skyBoxTex);
	skyBoxNode->setScaleMatrix(skyBoxScale);
	skyBoxNode->setName("skyboxNode");

	//Sun
	sunNode = base->createNode();
	sunNode->setMesh(sphereMesh);
	sunNode->setTexture(SunTex);
	sunNode->setShader(bloomShader);
	sunNode->setMatrix(MatrixFactory::createTranslationMat4(vec3(0, 0, 0)));
	sunNode->setScaleMatrix(MatrixFactory::createScaleMat4(vec3(5)));
	sunNode->setName("sunNode");

	//Earth
	earthNode = base->createNode();
	earthNode->setMesh(sphereMesh);
	//earthNode->setShader(earthShader);
	earthNode->setShader(earthShaderV2);
	earthNode->setTexture(EarthColorMapLowResu);
	earthNode->setMatrix(MatrixFactory::createTranslationMat4(vec3(20, 0, 0)));
	earthNode->setName("earthNode");

	//Mercury
	mercuryNode = base->createNode();
	createPlanetNode(mercuryNode, sphereMesh, blinnPhongShader, MercuryTex, vec3(10, 0, 0), 0.5f);
	mercuryNode->setName("mercuryNode");

	//Venus
	venusNode = base->createNode();
	createPlanetNode(venusNode, sphereMesh, blinnPhongShader, VenusTex, vec3(15, 0, 0), 0.8f);
	venusNode->setName("venusNode");

	//Mars
	marsNode = base->createNode();
	createPlanetNode(marsNode, sphereMesh, blinnPhongShader, MarsTex, vec3(25, 0, 0), 0.6f);
	marsNode->setName("marsNode");

	//Jupiter
	jupiterNode = base->createNode();
	createPlanetNode(jupiterNode, sphereMesh, jupiterShader, JupiterTex, vec3(30, 0, 0), 2.5f);
	jupiterNode->setName("jupiterNode");

	//Saturn
	saturnNode = base->createNode();
	createPlanetNode(saturnNode, sphereMesh, blinnPhongShader, SaturnTex, vec3(35, 0, 0), 2.0f);
	saturnNode->setName("saturnNode");

	//Saturn Ring
	saturnRingNode = saturnNode->createNode();
	createPlanetNode(saturnRingNode, torusMesh, blinnPhongShader, SaturnRingTex, vec3(0, 0, 0), 1.0f);
	saturnRingNode->setScaleMatrix(MatrixFactory::createScaleMat4(vec3(0.2f, 0.01f, 0.2f)));
	saturnRingNode->setName("saturnRingNode");

	//Uranus
	uranusNode = base->createNode();
	createPlanetNode(uranusNode, sphereMesh, blinnPhongShader, UranusTex, vec3(40, 0, 0), 1.5f);
	uranusNode->setName("uranusNode");

	//Neptune
	neptuneNode = base->createNode();
	createPlanetNode(neptuneNode, sphereMesh, blinnPhongShader, NeptuneTex, vec3(45, 0, 0), 1.5f);
	neptuneNode->setName("neptuneNode");

	//Lens Flare
	lensFlare = base->createNode();
	lensFlare->setMesh(quadMesh);
	lensFlare->setShader(lensFlareShader);
	lensFlare->setName("lensFlare");

	if (!isEmpty) {
		loadScene();
	}

	createAnimationObjects();
	if (!isEmpty) {
		rebootAnin();
	}
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

//////////////////////////////////////////////////////////////////////// LOADER
void checkInformation() {
	isEmpty = load.is_empty();
	if (isEmpty) {
		std::cout << "You don't any information saved. Loading scene...." << std::endl;
	}
	else {
		std::cout << "Loading scene..." << std::endl;
	}
}

/////////////////////////////////////////////////////////////////////// ANIMATION
bool animationInProgress = true;

//In the "animationObject" struct we have to fill in all information that is relative to the animation.
typedef struct {
	SceneNode* node;
	float selfRotateSpeed; //The speed at which the planet rotates around itself
	float orbitSpeed; //The speed of the orbit around the sun
	qtrn currentSelfRoation;
	qtrn currentOrbitRotation;
	qtrn tilt; 
	mat4 sunDistance; //Position relative to the Sun
	unsigned int stencilId;
} animationObject;
////////////////////////

//We have to fill this vector with animationObjects when we create the sceneNodes:
vector<animationObject> animationObjects = vector<animationObject>(); 
////////////////////////

void createAnimationObjects() {

	//Example on how to set an animationobject:

	//Mercury
	animationObject mercuryAnimObj = animationObject{
		mercuryNode,
		30,
		17,
		qtrn::qFromAngleAxis(0, yAxis),
		qtrn::qFromAngleAxis(0, yAxis),
		qtrn::qFromAngleAxis(0, zAxis),
		MatrixFactory::createTranslationMat4(vec3(10 + 10,0,0)),
		MERCURY
	};
	animationObjects.push_back(mercuryAnimObj);

	//Venus
	animationObject venusAnimObj = animationObject{
		venusNode,
		25,
		15,
		qtrn::qFromAngleAxis(0, yAxis),
		qtrn::qFromAngleAxis(0, yAxis),
		qtrn::qFromAngleAxis(0, zAxis),
		MatrixFactory::createTranslationMat4(vec3(10 + 15,0,0)),
		VENUS
	};
	animationObjects.push_back(venusAnimObj);

	//Earth
	animationObject earthAnimObj = animationObject{
		earthNode,
		100,
		11,
		qtrn::qFromAngleAxis(0, yAxis),
		qtrn::qFromAngleAxis(0, yAxis),
		qtrn::qFromAngleAxis(earthTilt, zAxis),
		MatrixFactory::createTranslationMat4(vec3(10 + 20,0,0)),
		EARTH
	};
	animationObjects.push_back(earthAnimObj);

	//Mars
	animationObject marsAnimObj = animationObject{
	marsNode,
	22,
	13,
	qtrn::qFromAngleAxis(0, yAxis),
	qtrn::qFromAngleAxis(0, yAxis),
	qtrn::qFromAngleAxis(0, zAxis),
	MatrixFactory::createTranslationMat4(vec3(10 + 25,0,0)),
	MARS
	};
	animationObjects.push_back(marsAnimObj);

	//Jupiter:
	animationObject jupiterAnimObj = animationObject{
	jupiterNode,
	17,
	9,
	qtrn::qFromAngleAxis(0, yAxis),
	qtrn::qFromAngleAxis(0, yAxis),
	qtrn::qFromAngleAxis(0, zAxis),
	MatrixFactory::createTranslationMat4(vec3(10 + 30,0,0)),
	JUPITER
	};

	animationObjects.push_back(jupiterAnimObj);

	//Saturn:
	animationObject saturnAnimObj = animationObject{
	saturnNode,
	14,
	7,
	qtrn::qFromAngleAxis(0, yAxis),
	qtrn::qFromAngleAxis(0, yAxis),
	qtrn::qFromAngleAxis(0, zAxis),
	MatrixFactory::createTranslationMat4(vec3(10 + 35,0,0)),
	SATURN
	};
	animationObjects.push_back(saturnAnimObj);

	//Uranus
	animationObject uranusAnimObj = animationObject{
		uranusNode,
		11,
		5,
		qtrn::qFromAngleAxis(0, yAxis),
		qtrn::qFromAngleAxis(0, yAxis),
		qtrn::qFromAngleAxis(0, zAxis),
		MatrixFactory::createTranslationMat4(vec3(10 + 40,0,0)),
		URANUS
	};
	animationObjects.push_back(uranusAnimObj);

	//Neptune
	animationObject neptuAnimObj = animationObject{
	neptuneNode,
	8,
	3,
	qtrn::qFromAngleAxis(0, yAxis),
	qtrn::qFromAngleAxis(0, yAxis),
	qtrn::qFromAngleAxis(0, zAxis),
	MatrixFactory::createTranslationMat4(vec3(10 + 45,0,0)),
	NEPTUN
	};
	animationObjects.push_back(neptuAnimObj);
}

float animationSpeed = 0.01;

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

		//Update the camera position and center
		if (obj.stencilId == stencilId) {
			cameraTargetTranslation = obj.sunDistance;
			cameraTargetRotation = orbitRot;
		}
		//

		//Update the actual animation object with the copy:
		animationObjects[i] = obj;
		i++;
	}
}

//////LOADER
void rebootAnin() {
	int i = 0;
	for (animationObject obj : animationObjects) {

		obj.sunDistance = obj.node->getMatrix();

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

	skyBoxNode->draw(&camCopy);

	//glDepthFunc(GL_LESS);
	glFrontFace(GL_CCW);
}

void drawLensFlare() {
	//glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (cameraDistance > 0) {
		/*vec4 flare = scenegraph->getCamera()->ProjectionMatrix * scenegraph->getCamera()->ViewMatrix * vec4(0, 0, -0.9f, 1);
		flare.w = 1.0f;
		std::cout << flare << std::endl;*/
		flareManager->render(&cam, vec4(0, 0, -0.9f, 1));

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

void drawStencilBuffer() {
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	//

	glStencilFunc(GL_ALWAYS, EARTH, -1);
	updateCameraInShader(earthShaderV2);
	earthNode->draw(&cam);

	glStencilFunc(GL_ALWAYS, JUPITER, -1);
	updateCameraInShader(jupiterShader);
	JupiterTex->Bind(JupiterTex->GetId());
	jupiterNode->draw(&cam);

	updateCameraInShader(blinnPhongShader);

	glStencilFunc(GL_ALWAYS, MERCURY, -1);
	bindTextureToShader(MercuryTex, blinnPhongShader);
	mercuryNode->draw(&cam);

	glStencilFunc(GL_ALWAYS, VENUS, -1);
	bindTextureToShader(VenusTex, blinnPhongShader);
	venusNode->draw(&cam);

	glStencilFunc(GL_ALWAYS, MARS, -1);
	bindTextureToShader(MarsTex, blinnPhongShader);
	marsNode->draw(&cam);

	glStencilFunc(GL_ALWAYS, SATURN, -1);
	bindTextureToShader(SaturnTex, blinnPhongShader);
	saturnNode->draw(&cam);

	bindTextureToShader(SaturnRingTex, blinnPhongShader);
	saturnRingNode->draw(&cam);

	glStencilFunc(GL_ALWAYS, URANUS, -1);
	bindTextureToShader(UranusTex, blinnPhongShader);
	uranusNode->draw(&cam);

	glStencilFunc(GL_ALWAYS, NEPTUN, -1);
	bindTextureToShader(NeptuneTex, blinnPhongShader);
	neptuneNode->draw(&cam);

	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);//This will prevent the following objects to overwrite the stencil ID's
	glDisable(GL_STENCIL_TEST);
}

void drawScene() {
	if (animationInProgress) updateAnimation();

	drawStencilBuffer();

	bloom->bindHDRBuffer();
	
	//We have to draw everything but the bloom in here (skybox last for performance reasons):

	//Earth
	EarthHeightMap->Bind(1);
	EarthSpecularMap->Bind(2);
	EarthClouds->Bind(3);
	EarthCloudTransparacy->Bind(4);

	checkResolution();
	if (highResu) {
		EarthColorMapHighResu->Bind();
		EarthNightHighResu->Bind(5);
	}
	else {
		EarthColorMapLowResu->Bind();
		EarthNightLowResu->Bind(5);
	}

	updateCameraInShader(earthShaderV2);
	earthNode->draw(&cam);
	
	//Jupiter
	updateCameraInShader(jupiterShader);
	JupiterTex->Bind(JupiterTex->GetId());
	jupiterNode->draw(&cam);

	//This update affects all other planets
	updateCameraInShader(blinnPhongShader);

	//Mercury
	bindTextureToShader(MercuryTex, blinnPhongShader);
	mercuryNode->draw(&cam);
	//Venus
	bindTextureToShader(VenusTex, blinnPhongShader);
	venusNode->draw(&cam);
	//Mars
	bindTextureToShader(MarsTex, blinnPhongShader);
	marsNode->draw(&cam);
	//Saturn
	bindTextureToShader(SaturnTex, blinnPhongShader);
	saturnNode->draw(&cam);
	//Saturn Ring
	bindTextureToShader(SaturnRingTex, blinnPhongShader);
	saturnRingNode->draw(&cam);
	//Uranus
	bindTextureToShader(UranusTex, blinnPhongShader);
	uranusNode->draw(&cam);
	//Neptune
	bindTextureToShader(NeptuneTex, blinnPhongShader);
	neptuneNode->draw(&cam);

	drawSkyBox();
	/////////////

	SunTex->Bind(10);
	sunNode->draw(&cam);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	bloom->renderWithBlurr(blurrShader);
	bloom->combineProcess(bloomMergeShader);	
	drawLensFlare();
}

/////////////////////////////////////////////////////////////////////// WINDOW CALLBACKS

//Here we add all the destroy/Delete functions.
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
bool doubleClicked;
float doubleClickTimer = 0.0f;
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
	//if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) { keyW = true; }
	//else if (glfwGetKey(win, GLFW_KEY_W) == GLFW_RELEASE) { keyW = false; }

	//if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) { keyS = true; }
	//else if (glfwGetKey(win, GLFW_KEY_S) == GLFW_RELEASE) { keyS = false; }

	//if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) { keyA = true; }
	//else if (glfwGetKey(win, GLFW_KEY_A) == GLFW_RELEASE) { keyA = false; }

	//if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) { keyD = true; }
	//else if (glfwGetKey(win, GLFW_KEY_D) == GLFW_RELEASE) { keyD = false; }

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
	/*if (glfwGetKey(win, GLFW_KEY_B) == GLFW_PRESS) { bloom->activateBloom(true); }
	if (glfwGetKey(win, GLFW_KEY_V) == GLFW_PRESS) { bloom->activateBloom(false); }
	if (glfwGetKey(win, GLFW_KEY_N) == GLFW_PRESS) { bloom->increaseExpresure(); }
	if (glfwGetKey(win, GLFW_KEY_M) == GLFW_PRESS) { bloom->decreaseExpresure(); }*/

	//Snapshot. Press Z to create one
	if (glfwGetKey(win, GLFW_KEY_Z) == GLFW_PRESS) { snapshot->captureSnapshot(); }
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

	if (keyP) animationInProgress = !animationInProgress; // Pause / Unpause animation
}

void mouse_callback(GLFWwindow* win, double xpos, double ypos) {

	//Stencil buffer:
	if (doubleClicked) {
		glReadPixels(xpos, screenHeight - 1 - ypos, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &stencilId);
		cout << stencilId << endl;
		cam.Up = vec3(0, 1, 0);
		doubleClicked = false;
		return;
	}
	//

	if (!mouseClicked) {
		firstMouseCall = true;
		return;
	}

	if (firstMouseCall) {
		lastX = xpos;
		lastY = ypos;
		firstMouseCall = false;
	}

	float xoffset = xpos - lastX;
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
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		if (doubleClickTimer <= 0) {
			mouseClicked = true;
			doubleClicked = false;
			doubleClickTimer = 30;
		}
		else {
			mouseClicked = false;
			doubleClicked = true;
			doubleClickTimer = 0;
		}

	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		mouseClicked = false;
	}
}

void scroll_callback(GLFWwindow* win, double xOffset, double yOffset) {
	cameraDistance -= yOffset * scrollSensitivity;
	scrollChange = true;
}


void mouse_initialize(GLFWwindow* win) {
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void mouse_update(GLFWwindow* win) {
	//cam.ViewMatrix = cameraTranslation * cameraRotation;
	if (!isEmpty && justOnce) {
		justOnce = false;
		rotQtrn = load.rot * rotQtrn;
	}
	cameraTranslation = MatrixFactory::createTranslationMat4(vec3(0, 0, cameraDistance));
	cameraRotation = matrixFromQtrn(rotQtrn);

	if (stencilId == 0) cameraTargetTranslation = cameraTargetRotation = mat4(1);//i.e. We are looking at the sun
			
	cam.Eye = vec3(cameraTargetRotation * cameraTargetTranslation * cameraRotation * cameraTranslation * vec4(0, 0, 0, 1));
	cam.Center = vec3(cameraTargetRotation * cameraTargetTranslation * vec4(0, 0, 0, 1));
	cam.update();
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
	snapshot = new Snapshot();

	///SceneLoader:
	load = Loader();
	checkInformation();

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

		glClearStencil(0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		doubleClickTimer--;

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
	std::cout << "Saving the scene...." << std::endl;
	////Save the last state of the scene
	load.updateState(scenegraph, cameraDistance, rotQtrn);
	//////////
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