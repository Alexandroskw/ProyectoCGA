#define _USE_MATH_DEFINES
#include <cmath>
//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

// contains new std::shuffle definition
#include <algorithm>
#include <random>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

// Shader include
#include "Headers/Shader.h"

// Model geometric includes
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
#include "Headers/FirstPersonCamera.h"
#include "Headers/ThirdPersonCamera.h"

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Texture.h"

// Include loader Model class
#include "Headers/Model.h"

// Include Terrain
#include "Headers/Terrain.h"

#include "Headers/AnimationUtils.h"

// Include Colision headers functions
#include "Headers/Colisiones.h"

#include "Headers/FontTypeRendering.h"

// OpenAL include
#include <AL/alut.h>

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

int screenWidth;
int screenHeight;

const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;

GLFWwindow *window;

Shader shader;
//Shader con skybox
Shader shaderSkybox;
//Shader con multiples luces
Shader shaderMulLighting;
//Shader para el terreno
Shader shaderTerrain;
//Shader para las particulas de fountain
Shader shaderParticlesFountain;
//Shader para las particulas de fuego
Shader shaderParticlesFire;
//Shader para visualizar el buffer de profundidad
Shader shaderViewDepth;
//Shader para dibujar el buffer de profunidad
Shader shaderDepth;

std::shared_ptr<Camera> camera(new ThirdPersonCamera());
float distanceFromTarget = 7.0;
int contador = 0;
int segundos = 0;
bool Rescate = false;
int vida = 100;

FontTypeRendering::FontTypeRendering * Contador;

Sphere skyboxSphere(20, 20);
Box boxCollider;
Sphere sphereCollider(10, 10);
Box boxViewDepth;
Box boxLightViewBox;

// Models complex instances
Model modelOnion;
Model modelEnemigo;
Model modelEnemigo2;
Model modelCastillo;
Model modelParedTorres;
Model modelTorre1;
Model modelTorre2;
Model modelCasaShrek;
Model modelAntorcha;
Model modelAntorcha2;

Model modelArbol;
Model modelParedArbol;
Model modelParedArbol1;
Model modelParedArbol2;
Model paredCastillo;
Model modelHongo;
Model modelShrek;
Model modelBurro;

Model boxImagen;

// Terrain model instance
Terrain terrain(-1, -1, 400, 16, "../Textures/mapaAlturas_PF_3.png");

GLuint textureCespedID, textureWallID, textureWindowID, textureHighwayID, textureLandingPadID;
GLuint textureTerrainBackgroundID, textureTerrainRID, textureTerrainGID, textureTerrainBID, textureTerrainBlendMapID;
GLuint textureParticleFountainID, textureParticleFireID, texId;
GLuint skyboxTextureID;

GLenum types[6] = {
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

std::string fileNames[6] = { "../Textures/ely_hills/hills_ft.tga",
		"../Textures/ely_hills/hills_bk.tga",
		"../Textures/ely_hills/hills_up.tga",
		"../Textures/ely_hills/hills_dn.tga",
		"../Textures/ely_hills/hills_rt.tga",
		"../Textures/ely_hills/hills_lf.tga" };

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;
int Mouse= 10;

// Model matrix definitions
glm::mat4 modelMatrixShrek = glm::mat4(1.0f);
glm::mat4 modelMatrixCebolla = glm::mat4(1.0f);
glm::mat4 modelMatrixBurro = glm::mat4(1.0f);
glm::mat4 modelMatrixParedArbol = glm::mat4(1.0f);
glm::mat4 modelMatrixParedArbol1 = glm::mat4(1.0f);
glm::mat4 modelMatrixParedArbol2 = glm::mat4(1.0f);
glm::mat4 modelMatrixCastillo = glm::mat4(1.0f);
glm::mat4 modelMatrixTorre1 = glm::mat4(1.0f);
glm::mat4 modelMatrixTorre2 = glm::mat4(1.0f);
glm::mat4 modelMatrixCasaShrek = glm::mat4(1.0f);
glm::mat4 modelMatrixEnemigo = glm::mat4(1.0f);
glm::mat4 modelMatrixEnemigo2 = glm::mat4(1.0f);
glm::mat4 modelMatrixHongo = glm::mat4(1.0f);
glm::mat4 modelMatrixParedTorres = glm::mat4(1.0f);
glm::mat4 modelMatrixAntorcha = glm::mat4(1.0f);
glm::mat4 modelMatrixAntorcha2 = glm::mat4(1.0f);
glm::mat4 modelMatrixImagen = glm::mat4(1.0f);

int animationIndex = 1;
int modelSelected = 2;

// Objects positions
std::vector<glm::vec3> ArbolPosition = { glm::vec3(-36.52, 0, -23.24), glm::vec3(-52.73, 0, -3.90),
glm::vec3(-70.42, 0, -7.24), glm::vec3(-36.52, 0, 4.24),
glm::vec3(-52.73, 0, 20.90), glm::vec3(-70.42, 0, 50.24),
glm::vec3(-145, 0, 20.90), glm::vec3(76.42, 0, -40.24),
glm::vec3(-281.73, 0, 59.90), glm::vec3(-109.42, 0, 50.24),
glm::vec3(-61.0, 0, -80.90), glm::vec3(-70.42, 0, 15.24),
glm::vec3(-163.466,0,-61.999), glm::vec3(-185.736,0,-62.335),
glm::vec3(-83.047, 0,-100.199), glm::vec3(-138.577,0,-32.200),
glm::vec3(65.639,0,-81.800), glm::vec3(68.805,0,59.352),
glm::vec3(-128.806,0 ,-15.116), glm::vec3(0.137,0,-41.063),
glm::vec3(25.949,0,-106.356), glm::vec3(-43.094,0,-81.699),
glm::vec3(-37.193,0,86.334) };

std::vector<float> ArbolOrientation = { 21.37 + 90, -65.0 + 90 };
 
std::vector<glm::vec3> cebollaPosition = { glm::vec3(-4.84375, 5.0, -163.671875),
glm::vec3(5.859375, 5.0, 19.140625),
glm::vec3(-35.9375, 5.0, -84.765625),
glm::vec3(83.984375, 5.0, 33.984375),
glm::vec3(-156.640625, 5.0, -175.5),
glm::vec3(67.96875, 5.0, -56.640625),
glm::vec3(-156.640625, 5.0, -91.015625),
glm::vec3(30.46875, 5.0, -48.4375),
glm::vec3(-196.875, 5.0, 7.890625),
glm::vec3(-198.046875, 5.0, -62.890625) };

std::vector<glm::vec3> hongosPosition = { glm::vec3(-50.0, 0.75, 50.0), glm::vec3(50.0, 0.75, 70.0) };

std::vector<glm::vec3> enemigoPosition = {glm::vec3(-31.44, 1.5, 0.46), glm::vec3(37.10, 1.5, -51.25), glm::vec3(-2.14, 1.5, -47.14),
glm::vec3(-28.740,4.5,-256.390), glm::vec3(50.596,1.5,-243.693), glm::vec3(42.895,3.0,-15.796),
glm::vec3(-204.680,1.5,55.401), glm::vec3(-260.738,1.8,-266.046) };


//Jump variables
bool isJump = false;
float GRAVITY = 3.0;
double tmv = 0;
double startTimeJump = 0;

// Blending model unsorted
std::map<std::string, glm::vec3> blendingUnsorted = {
		{"fire", glm::vec3(0.0, 0.0, 7.0)}
};

double deltaTime;
double currTime, lastTime;

// Definition for the particle system
GLuint initVel, startTime;
GLuint VAOParticles;
GLuint nParticles = 8000;
double currTimeParticlesAnimation, lastTimeParticlesAnimation;

// Definition for the particle system fire
GLuint initVelFire, startTimeFire;
GLuint VAOParticlesFire;
GLuint nParticlesFire = 2000;
GLuint posBuf[2], velBuf[2], age[2];
GLuint particleArray[2];
GLuint feedback[2];
GLuint drawBuf = 1;
float particleSize = 0.5, particleLifetime = 3.0;
double currTimeParticlesAnimationFire, lastTimeParticlesAnimationFire;

// Colliders
std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> > collidersOBB;
std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> > collidersSBB;

// Framesbuffers
GLuint depthMap, depthMapFBO;

/**********************
 * OpenAL config
 */

 // OpenAL Defines
#define NUM_BUFFERS 3
#define NUM_SOURCES 3
#define NUM_ENVIRONMENTS 1
// Listener
ALfloat listenerPos[] = { 0.0, 0.0, 4.0 };
ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };
ALfloat listenerOri[] = { 0.0, 0.0, 1.0, 0.0, 1.0, 0.0 };
// Source 0
ALfloat source0Pos[] = { -2.0, 0.0, 0.0 };
ALfloat source0Vel[] = { 0.0, 0.0, 0.0 };
// Source 1
ALfloat source1Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source1Vel[] = { 0.0, 0.0, 0.0 };
// Source 2
ALfloat source2Pos[] = { 2.0, 0.0, 0.0 };
ALfloat source2Vel[] = { 0.0, 0.0, 0.0 };
// Buffers
ALuint buffer[NUM_BUFFERS];
ALuint source[NUM_SOURCES];
ALuint environment[NUM_ENVIRONMENTS];
// Configs
ALsizei size, freq;
ALenum format;
ALvoid *data;
int ch;
ALboolean loop;
std::vector<bool> sourcesPlay = { true, true, true };

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
	int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void initParticleBuffers();
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);
void prepareScene();
void prepareDepthScene();
void renderScene(bool renderParticles = true);

void initParticleBuffers() {
	// Generate the buffers
	glGenBuffers(1, &initVel);   // Initial velocity buffer
	glGenBuffers(1, &startTime); // Start time buffer

	// Allocate space for all buffers
	int size = nParticles * 3 * sizeof(float);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferData(GL_ARRAY_BUFFER, nParticles * sizeof(float), NULL, GL_STATIC_DRAW);

	// Fill the first velocity buffer with random velocities
	glm::vec3 v(0.0f);
	float velocity, theta, phi;
	GLfloat *data = new GLfloat[nParticles * 3];
	for (unsigned int i = 0; i < nParticles; i++) {

		theta = glm::mix(0.0f, glm::pi<float>() / 6.0f, ((float)rand() / RAND_MAX));
		phi = glm::mix(0.0f, glm::two_pi<float>(), ((float)rand() / RAND_MAX));

		v.x = sinf(theta) * cosf(phi);
		v.y = cosf(theta);
		v.z = sinf(theta) * sinf(phi);

		velocity = glm::mix(0.6f, 0.8f, ((float)rand() / RAND_MAX));
		v = glm::normalize(v) * velocity;

		data[3 * i] = v.x;
		data[3 * i + 1] = v.y;
		data[3 * i + 2] = v.z;
	}
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

	// Fill the start time buffer
	delete[] data;
	data = new GLfloat[nParticles];
	float time = 0.0f;
	float rate = 0.00075f;
	for (unsigned int i = 0; i < nParticles; i++) {
		data[i] = time;
		time += rate;
	}
	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(float), data);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	delete[] data;

	glGenVertexArrays(1, &VAOParticles);
	glBindVertexArray(VAOParticles);
	glBindBuffer(GL_ARRAY_BUFFER, initVel);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, startTime);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void initParticleBuffersFire() {
	// Generate the buffers
	glGenBuffers(2, posBuf);    // position buffers
	glGenBuffers(2, velBuf);    // velocity buffers
	glGenBuffers(2, age);       // age buffers

	// Allocate space for all buffers
	int size = nParticlesFire * sizeof(GLfloat);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, age[1]);
	glBufferData(GL_ARRAY_BUFFER, size, 0, GL_DYNAMIC_COPY);

	// Fill the first age buffer
	std::vector<GLfloat> initialAge(nParticlesFire);
	float rate = particleLifetime / nParticlesFire;
	for (unsigned int i = 0; i < nParticlesFire; i++) {
		int index = i - nParticlesFire;
		float result = rate * index;
		initialAge[i] = result;
	}
	// Shuffle them for better looking results
	//Random::shuffle(initialAge);
	auto rng = std::default_random_engine{};
	std::shuffle(initialAge.begin(), initialAge.end(), rng);
	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, initialAge.data());

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create vertex arrays for each set of buffers
	glGenVertexArrays(2, particleArray);

	// Set up particle array 0
	glBindVertexArray(particleArray[0]);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, velBuf[0]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, age[0]);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	// Set up particle array 1
	glBindVertexArray(particleArray[1]);
	glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, velBuf[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, age[1]);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	// Setup the feedback objects
	glGenTransformFeedbacks(2, feedback);

	// Transform feedback 0
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[0]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, age[0]);

	// Transform feedback 1
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, velBuf[1]);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, age[1]);

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
}

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
			glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
			nullptr);

	if (window == nullptr) {
		std::cerr
			<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
			<< std::endl;
		destroy();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Inicializaci?n de los shaders
	shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
	shaderSkybox.initialize("../Shaders/skyBox.vs", "../Shaders/skyBox_fog.fs");
	shaderMulLighting.initialize("../Shaders/iluminacion_textura_animation_shadow.vs", "../Shaders/multipleLights_shadow.fs");
	shaderTerrain.initialize("../Shaders/terrain_shadow.vs", "../Shaders/terrain_shadow.fs");
	shaderParticlesFountain.initialize("../Shaders/particlesFountain.vs", "../Shaders/particlesFountain.fs");
	shaderParticlesFire.initialize("../Shaders/particlesFire.vs", "../Shaders/particlesFire.fs", { "Position", "Velocity", "Age" });
	shaderViewDepth.initialize("../Shaders/texturizado.vs", "../Shaders/texturizado_depth_view.fs");
	shaderDepth.initialize("../Shaders/shadow_mapping_depth.vs", "../Shaders/shadow_mapping_depth.fs");

	// Inicializacion de los objetos.
	skyboxSphere.init();
	skyboxSphere.setShader(&shaderSkybox);
	skyboxSphere.setScale(glm::vec3(100.0f, 100.0f, 100.0f));

	boxImagen.init();
	boxImagen.setShader(&shaderMulLighting);

	boxCollider.init();
	boxCollider.setShader(&shader);
	boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	sphereCollider.init();
	sphereCollider.setShader(&shader);
	sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

	boxViewDepth.init();
	boxViewDepth.setShader(&shaderViewDepth);

	boxLightViewBox.init();
	boxLightViewBox.setShader(&shaderViewDepth);

	terrain.init();
	terrain.setShader(&shaderTerrain);
	terrain.setPosition(glm::vec3(100, 0, 100));

	//Modelos
	modelArbol.loadModel("../models/N64 Tree/n64tree.obj");
	modelArbol.setShader(&shaderMulLighting);
	modelParedArbol.loadModel("../models/N64 Tree/ParedArbol.obj");
	modelParedArbol.setShader(&shaderMulLighting);
	modelParedArbol1.loadModel("../models/N64 Tree/ParedArbol.obj");
	modelParedArbol1.setShader(&shaderMulLighting);
	modelParedArbol2.loadModel("../models/N64 Tree/ParedArbol.obj");
	modelParedArbol2.setShader(&shaderMulLighting);
	modelOnion.loadModel("../models/Onion/cebollo.obj");
	modelOnion.setShader(&shaderMulLighting);
	modelEnemigo.loadModel("../models/Verdugo/verdugo.obj");
	modelEnemigo.setShader(&shaderMulLighting);
	modelEnemigo2.loadModel("../models/Verdugo/verdugo.obj");
	modelEnemigo2.setShader(&shaderMulLighting);
	modelBurro.loadModel("../models/Donkey/donkey.obj");
	modelBurro.setShader(&shaderMulLighting);
	modelCastillo.loadModel("../models/Torre/castillo.obj");
	modelCastillo.setShader(&shaderMulLighting);
	modelCasaShrek.loadModel("../models/Shrek_House/ARCHITECTURE_Shreks House.obj");
	modelCasaShrek.setShader(&shaderMulLighting);
	modelParedTorres.loadModel("../models/Torre/Torre2.obj");
	modelParedTorres.setShader(&shaderMulLighting);
	modelTorre1.loadModel("../models/Torre/torre.obj");
	modelTorre1.setShader(&shaderMulLighting);
	modelTorre2.loadModel("../models/Torre/torre.obj");
	modelTorre2.setShader(&shaderMulLighting);
	modelAntorcha.loadModel("../models/Antorcha/torch.obj");
	modelAntorcha.setShader(&shaderMulLighting);
	modelAntorcha2.loadModel("../models/Antorcha/torch.obj");
	modelAntorcha2.setShader(&shaderMulLighting);

	boxImagen.loadModel("../models/Shrek/Caja.obj");
	boxImagen.setShader(&shaderMulLighting);

	//Hongos
	modelHongo.loadModel("../models/Hongo/mushroom.obj");
	modelHongo.setShader(&shaderMulLighting);

	//Shrek
	modelShrek.loadModel("../models/Shrek/shrek_idle.fbx");
	modelShrek.setShader(&shaderMulLighting);

	camera->setPosition(glm::vec3(0.0, 0.0, 10.0));
	camera->setDistanceFromTarget(distanceFromTarget);
	camera->setSensitivity(1.0);

	// Definimos el tamanio de la imagen
	int imageWidth, imageHeight;
	FIBITMAP *bitmap;
	unsigned char *data;

	// Carga de texturas para el skybox
	Texture skyboxTexture = Texture("");
	glGenTextures(1, &skyboxTextureID);
	// Tipo de textura CUBE MAP
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(types); i++) {
		skyboxTexture = Texture(fileNames[i]);
		FIBITMAP *bitmap = skyboxTexture.loadImage(true);
		unsigned char *data = skyboxTexture.convertToData(bitmap, imageWidth,
			imageHeight);
		if (data) {
			glTexImage2D(types[i], 0, GL_RGBA, imageWidth, imageHeight, 0,
				GL_BGRA, GL_UNSIGNED_BYTE, data);
		}
		else
			std::cout << "Failed to load texture" << std::endl;
		skyboxTexture.freeImage(bitmap);
	}

	// Definiendo la textura a utilizar
	Texture textureCesped("../Textures/Pasto_PF.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureCesped.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureCesped.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureCespedID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureCespedID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureCesped.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTerrainBackground("../Textures/Pasto_PF.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainBackground.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainBackground.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainBackgroundID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainBackgroundID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainBackground.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTerrainR("../Textures/Clay_PF.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainR.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainR.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainRID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainR.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTerrainG("../Textures/Pantano_PF.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainG.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainG.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainGID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainG.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTerrainB("../Textures/Castillo_PF.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainB.loadImage();
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainB.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainBID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainB.freeImage(bitmap);

	// Definiendo la textura a utilizar
	Texture textureTerrainBlendMap("../Textures/blendMap_PF.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	bitmap = textureTerrainBlendMap.loadImage(true);
	// Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
	data = textureTerrainBlendMap.convertToData(bitmap, imageWidth,
		imageHeight);
	// Creando la textura con id 1
	glGenTextures(1, &textureTerrainBlendMapID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (data) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureTerrainBlendMap.freeImage(bitmap);

	Texture textureParticleFire("../Textures/fire.png");
	bitmap = textureParticleFire.loadImage();
	data = textureParticleFire.convertToData(bitmap, imageWidth, imageHeight);
	glGenTextures(1, &textureParticleFireID);
	glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
			GL_BGRA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture" << std::endl;
	textureParticleFire.freeImage(bitmap);

	std::uniform_real_distribution<float> distr01 = std::uniform_real_distribution<float>(0.0f, 1.0f);
	std::mt19937 generator;
	std::random_device rd;
	generator.seed(rd());
	int size = nParticlesFire * 2;
	std::vector<GLfloat> randData(size);
	for (int i = 0; i < randData.size(); i++) {
		randData[i] = distr01(generator);
	}

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_1D, texId);
	glTexStorage1D(GL_TEXTURE_1D, 1, GL_R32F, size);
	glTexSubImage1D(GL_TEXTURE_1D, 0, 0, size, GL_RED, GL_FLOAT, randData.data());
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	shaderParticlesFire.setInt("Pass", 1);
	shaderParticlesFire.setInt("ParticleTex", 0);
	shaderParticlesFire.setInt("RandomTex", 1);
	shaderParticlesFire.setFloat("ParticleLifetime", particleLifetime);
	shaderParticlesFire.setFloat("ParticleSize", particleSize);
	shaderParticlesFire.setVectorFloat3("Accel", glm::value_ptr(glm::vec3(0.0f, 0.1f, 0.0f)));
	shaderParticlesFire.setVectorFloat3("Emitter", glm::value_ptr(glm::vec3(0.0f)));

	glm::mat3 basis;
	glm::vec3 u, v, n;
	v = glm::vec3(0, 1, 0);
	n = glm::cross(glm::vec3(1, 0, 0), v);
	if (glm::length(n) < 0.00001f) {
		n = glm::cross(glm::vec3(0, 1, 0), v);
	}
	u = glm::cross(v, n);
	basis[0] = glm::normalize(u);
	basis[1] = glm::normalize(v);
	basis[2] = glm::normalize(n);
	shaderParticlesFire.setMatrix3("EmitterBasis", 1, false, glm::value_ptr(basis));

	/*******************************************
	 * Inicializacion de los buffers de la fuente
	 *******************************************/
	initParticleBuffers();

	/*******************************************
	 * Inicializacion de los buffers del fuego
	 *******************************************/
	initParticleBuffersFire();

	/*******************************************
	 * Inicializacion del framebuffer para
	 * almacenar el buffer de profunidadad
	 *******************************************/
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/*******************************************
	 * OpenAL init
	 *******************************************/
	alutInit(0, nullptr);
	alListenerfv(AL_POSITION, listenerPos);
	alListenerfv(AL_VELOCITY, listenerVel);
	alListenerfv(AL_ORIENTATION, listenerOri);
	alGetError(); // clear any error messages
	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating buffers !!\n");
		exit(1);
	}
	else {
		printf("init() - No errors yet.");
	}
	// Config source 0
	// Generate buffers, or else no sound will happen!
	alGenBuffers(NUM_BUFFERS, buffer);
	buffer[0] = alutCreateBufferFromFile("../sounds/Hurt.wav");
	buffer[1] = alutCreateBufferFromFile("../sounds/075.wav");
	buffer[2] = alutCreateBufferFromFile("../sounds/win.wav");
	int errorAlut = alutGetError();
	if (errorAlut != ALUT_ERROR_NO_ERROR) {
		printf("- Error open files with alut %d !!\n", errorAlut);
		exit(2);
	}

	alGetError(); /* clear error */
	alGenSources(NUM_SOURCES, source);

	if (alGetError() != AL_NO_ERROR) {
		printf("- Error creating sources !!\n");
		exit(2);
	}
	else {
		printf("init - no errors after alGenSources\n");
	}
	alSourcef(source[0], AL_PITCH, 1.0f);
	alSourcef(source[0], AL_GAIN, 50.0f);
	alSourcefv(source[0], AL_POSITION, source0Pos);
	alSourcefv(source[0], AL_VELOCITY, source0Vel);
	alSourcei(source[0], AL_BUFFER, buffer[0]);
	alSourcei(source[0], AL_LOOPING, AL_FALSE);
	alSourcef(source[0], AL_MAX_DISTANCE, 4000);

	alSourcef(source[1], AL_PITCH, 1.0f);
	alSourcef(source[1], AL_GAIN, 1.0f);
	alSourcefv(source[1], AL_POSITION, source1Pos);
	alSourcefv(source[1], AL_VELOCITY, source1Vel);
	alSourcei(source[1], AL_BUFFER, buffer[1]);
	alSourcei(source[1], AL_LOOPING, AL_FALSE);
	alSourcef(source[1], AL_MAX_DISTANCE, 4000);

	alSourcef(source[2], AL_PITCH, 1.0f);
	alSourcef(source[2], AL_GAIN, 50.0f);
	alSourcefv(source[2], AL_POSITION, source2Pos);
	alSourcefv(source[2], AL_VELOCITY, source2Vel);
	alSourcei(source[2], AL_BUFFER, buffer[2]);
	alSourcei(source[2], AL_LOOPING, AL_FALSE);
	alSourcef(source[2], AL_MAX_DISTANCE, 8000);

	Contador = new FontTypeRendering::FontTypeRendering(screenWidth, screenHeight);
	Contador->Initialize();
}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	// Shaders Delete
	shader.destroy();
	shaderMulLighting.destroy();
	shaderSkybox.destroy();
	shaderTerrain.destroy();
	shaderParticlesFountain.destroy();
	shaderParticlesFire.destroy();

	// Basic objects Delete
	skyboxSphere.destroy();
	boxCollider.destroy();
	sphereCollider.destroy();
	boxViewDepth.destroy();
	boxLightViewBox.destroy();

	// Terrains objects Delete
	terrain.destroy();

	// Custom objects Delete
	modelOnion.destroy();
	modelEnemigo.destroy();
	modelEnemigo2.destroy();
	modelArbol.destroy();
	modelParedArbol.destroy();
	modelParedArbol1.destroy();
	modelParedArbol2.destroy();
	modelHongo.destroy();
	modelBurro.destroy();
	modelCastillo.destroy();
	modelTorre1.destroy();
	modelTorre2.destroy();
	modelCasaShrek.destroy();
	modelParedTorres.destroy();
	modelAntorcha.destroy();
	modelAntorcha2.destroy();

	boxImagen.destroy();
	
	// Custom objects animate
	modelShrek.destroy();

	// Textures Delete
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &textureCespedID);
	glDeleteTextures(1, &textureTerrainBackgroundID);
	glDeleteTextures(1, &textureTerrainRID);
	glDeleteTextures(1, &textureTerrainGID);
	glDeleteTextures(1, &textureTerrainBID);
	glDeleteTextures(1, &textureTerrainBlendMapID);
	glDeleteTextures(1, &textureParticleFireID);

	// Cube Maps Delete
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDeleteTextures(1, &skyboxTextureID);

	// Remove the buffer of the fountain particles
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &initVel);
	glDeleteBuffers(1, &startTime);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAOParticles);

	// Remove the buffer of the fire particles
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(2, posBuf);
	glDeleteBuffers(2, velBuf);
	glDeleteBuffers(2, age);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	glDeleteTransformFeedbacks(2, feedback);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAOParticlesFire);
}

void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
	int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	if (Mouse < 10 && !Rescate)
	{
		distanceFromTarget -= yoffset;
		camera->setDistanceFromTarget(distanceFromTarget);
	}
	
}

void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod) {
	if (state == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			std::cout << "lastMousePos.x:" << lastMousePosX  <<" " << Mouse << std::endl;
			Mouse--;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		}
	}
}

bool processInput(bool continueApplication) {
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}

	if (vida > 0 && Mouse < 10 && !Rescate)
	{
		if (glfwJoystickPresent(GLFW_JOYSTICK_1) == GL_TRUE)
		{
			std::cout << "Esta presente el joystick" << std::endl;
			int axesCount, buttonCount;
			const float * axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
			std::cout << "Numero de ejes disponibles :=>" << axesCount << std::endl;
			std::cout << "Left stick X axis:" << axes[0] << std::endl;
			std::cout << "Left stick Y axis:" << axes[1] << std::endl;
			std::cout << "Left Trigger L2:" << axes[4] << std::endl;
			std::cout << "Right stick X axis:" << axes[2] << std::endl;
			std::cout << "Right stick Y axis:" << axes[3] << std::endl;
			std::cout << "Right Trigger R2:" << axes[5] << std::endl;

			if (fabs(axes[1]) > 0.2)
			{
				modelMatrixShrek = glm::translate(modelMatrixShrek, glm::vec3(0, 0, axes[1] * 1.02));
				animationIndex = 0;
			}

			if (fabs(axes[0]) > 0.2)
			{
				modelMatrixShrek = glm::rotate(modelMatrixShrek, glm::radians(-axes[0] * 3.0f), glm::vec3(0, 1, 0));
				animationIndex = 0;
			}

			if (fabs(axes[2]) > 0.2)
			{
				camera->mouseMoveCamera(-axes[2], 0.0, deltaTime);
			}
			if (fabs(axes[3]) > 0.2)
			{
				camera->mouseMoveCamera(0.0, axes[3], deltaTime);
			}

			const unsigned char * buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
			std::cout << "Numero de botones disponibles :=>" << buttonCount << std::endl;
			if (!isJump && buttons[0] == GLFW_PRESS)
			{
				isJump = true;
				startTimeJump = currTime;
				tmv = 0;
			}
		}
	}

	if (Mouse < 10 && !Rescate)
	{
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			camera->mouseMoveCamera(offsetX, 0.0, deltaTime);
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			camera->mouseMoveCamera(0.0, offsetY, deltaTime);
	}

	offsetX = 0;
	offsetY = 0;

	if (vida > 0 && Mouse < 10 && !Rescate)
	{
		if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			modelMatrixShrek = glm::rotate(modelMatrixShrek, glm::radians(1.0f), glm::vec3(0, 3, 0));
			animationIndex = 1;
		}
		else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			modelMatrixShrek = glm::rotate(modelMatrixShrek, glm::radians(-1.0f), glm::vec3(0, 3, 0));
			animationIndex = 1;
		}if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			modelMatrixShrek = glm::translate(modelMatrixShrek, glm::vec3(0, 0, 1.02));
			animationIndex = 1;
		}
		else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			modelMatrixShrek = glm::translate(modelMatrixShrek, glm::vec3(0, 0, -1.02));
			animationIndex = 1;
		}

		bool keySpaceStatus = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
		if (!isJump && keySpaceStatus)
		{
			isJump = true;
			startTimeJump = currTime;
			tmv = 0;
		}
	}

	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;

	glm::mat4 view;
	glm::vec3 axis;
	glm::vec3 target;
	float angleTarget;

	int state = 0;
	float advanceCount = 0.0;
	float rotCount = 0.0;
	int numberAdvance = 0;
	int maxAdvance = 0.0;

	modelMatrixBurro = glm::translate(modelMatrixBurro, glm::vec3(-100.0, 4.35, -245.0));
	modelMatrixBurro = glm::scale(modelMatrixBurro, glm::vec3(0.25, 0.25, 0.25));

	modelMatrixParedArbol = glm::translate(modelMatrixParedArbol, glm::vec3(-290.0, 1.0, 100.0));
	modelMatrixParedArbol = glm::rotate(modelMatrixParedArbol, glm::radians(90.0f), glm::vec3(0, 1, 0));

	modelMatrixParedArbol1 = glm::translate(modelMatrixParedArbol1, glm::vec3(105.0, 1.0, 100.0));
	modelMatrixParedArbol1 = glm::rotate(modelMatrixParedArbol1, glm::radians(90.0f), glm::vec3(0, 1, 0));

	modelMatrixParedArbol2 = glm::translate(modelMatrixParedArbol2, glm::vec3(-295.0 , 1.0, 100.0));

	modelMatrixCastillo = glm::translate(modelMatrixCastillo, glm::vec3(-100.3203f, 4.0f, -290.9609f));
	modelMatrixCastillo = glm::scale(modelMatrixCastillo, glm::vec3(0.75f, 0.75f, 0.75f));

	modelMatrixTorre1 = glm::translate(modelMatrixTorre1, glm::vec3(-61.3203f, 4.0f, -255.9609f));
	modelMatrixTorre1 = glm::scale(modelMatrixTorre1, glm::vec3(5.0f, 5.0f, 5.0f));
	modelMatrixTorre1 = glm::rotate(modelMatrixTorre1, glm::radians(-60.0f), glm::vec3(0, 1, 0));

	modelMatrixTorre2 = glm::translate(modelMatrixTorre2, glm::vec3(-135.3203f, 4.0f, -258.9609f));
	modelMatrixTorre2 = glm::scale(modelMatrixTorre2, glm::vec3(5.0f, 5.0f, 5.0f));
	modelMatrixTorre2 = glm::rotate(modelMatrixTorre2, glm::radians(-60.0f), glm::vec3(0, 1, 0));

	modelMatrixCasaShrek = glm::translate(modelMatrixCasaShrek, glm::vec3(-1.9531, 0.05, 66.4062));
	modelMatrixCasaShrek = glm::scale(modelMatrixCasaShrek, glm::vec3(4.0f, 4.0f, 4.0f));
	modelMatrixCasaShrek = glm::rotate(modelMatrixCasaShrek, glm::radians(180.0f), glm::vec3(0, 1, 0));

	modelMatrixEnemigo2 = glm::translate(modelMatrixEnemigo2, glm::vec3(1.0, 1.5, 1.0));
	modelMatrixEnemigo2 = glm::scale(modelMatrixEnemigo2, glm::vec3(0.25f, 0.25f, 0.25f));

	modelMatrixImagen = glm::translate(modelMatrixImagen, glm::vec3(15.0, 3.0, -5.0));
	modelMatrixImagen = glm::scale(modelMatrixImagen, glm::vec3(0.3, 0.3, 0.3));
	
	modelMatrixShrek = glm::translate(modelMatrixShrek, glm::vec3(13.0f, 0.05f, -5.0f));
	modelMatrixShrek = glm::rotate(modelMatrixShrek, glm::radians(-90.0f), glm::vec3(0, 1, 0));

	modelMatrixParedTorres = glm::translate(modelMatrixParedTorres, glm::vec3(90.0, 1.0, -300.0));
	modelMatrixParedTorres = glm::rotate(modelMatrixParedTorres, glm::radians(180.0f), glm::vec3(0, 1, 0));

	modelMatrixHongo = glm::scale(modelMatrixHongo, glm::vec3(2.0, 2.0, 2.0));

	modelMatrixAntorcha = glm::translate(modelMatrixAntorcha, glm::vec3(-135.0f, 12.0f, -255.0f));
	modelMatrixAntorcha = glm::scale(modelMatrixAntorcha, glm::vec3(5.0, 5.0, 5.0));
	modelMatrixAntorcha = glm::rotate(modelMatrixAntorcha, glm::radians(90.0f), glm::vec3(0, 1, 0));

	modelMatrixAntorcha2 = glm::translate(modelMatrixAntorcha2, glm::vec3(-61.0f, 12.0f, -252.0f));
	modelMatrixAntorcha2 = glm::scale(modelMatrixAntorcha2, glm::vec3(5.0, 5.0, 5.0));
	modelMatrixAntorcha2 = glm::rotate(modelMatrixAntorcha2, glm::radians(90.0f), glm::vec3(0, 1, 0));

	lastTime = TimeManager::Instance().GetTime();

	// Time for the particles animation
	currTimeParticlesAnimation = lastTime;
	lastTimeParticlesAnimation = lastTime;

	currTimeParticlesAnimationFire = lastTime;
	lastTimeParticlesAnimationFire = lastTime;

	glm::vec3 lightPos = glm::vec3(10.0, 10.0, 0.0);

	while (psi) {
		currTime = TimeManager::Instance().GetTime();
		if (currTime - lastTime < 0.016666667) {
			glfwPollEvents();
			continue;
		}
		lastTime = currTime;
		TimeManager::Instance().CalculateFrameRate(true);
		deltaTime = TimeManager::Instance().DeltaTime;
		psi = processInput(true);

		std::map<std::string, bool> collisionDetection;

		// Variables donde se guardan las matrices de cada articulacion por 1 frame

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
			(float)screenWidth / (float)screenHeight, 0.1f, 100.0f);

		if (modelSelected == 2) {
			axis = glm::axis(glm::quat_cast(modelMatrixShrek));
			angleTarget = glm::angle(glm::quat_cast(modelMatrixShrek));
			target = modelMatrixShrek[3];
		}

		if (std::isnan(angleTarget))
			angleTarget = 0.0;
		if (axis.y < 0)
			angleTarget = -angleTarget;
		if (modelSelected == 1)
			angleTarget -= glm::radians(90.0f);
		camera->setCameraTarget(target);
		camera->setAngleTarget(angleTarget);
		camera->updateCamera();
		view = camera->getViewMatrix();

		// Matriz de proyecci?n del shadow mapping
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 0.1f, far_plane = 20.0f;
		lightProjection = glm::ortho(-25.0f, 25.0f, -25.0f, 25.0f, near_plane, far_plane);
		lightView = glm::lookAt(lightPos, glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		shaderDepth.setMatrix4("lightSpaceMatrix", 1, false, glm::value_ptr(lightSpaceMatrix));

		// Settea la matriz de vista y projection al shader con solo color
		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con skybox
		shaderSkybox.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderSkybox.setMatrix4("view", 1, false,
			glm::value_ptr(glm::mat4(glm::mat3(view))));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderMulLighting.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderMulLighting.setMatrix4("view", 1, false,
			glm::value_ptr(view));
		shaderMulLighting.setMatrix4("lightSpaceMatrix", 1, false,
			glm::value_ptr(lightSpaceMatrix));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderTerrain.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderTerrain.setMatrix4("view", 1, false,
			glm::value_ptr(view));
		shaderTerrain.setMatrix4("lightSpaceMatrix", 1, false,
			glm::value_ptr(lightSpaceMatrix));
		// Settea la matriz de vista y projection al shader para el fountain
		shaderParticlesFountain.setMatrix4("projection", 1, false,
			glm::value_ptr(projection));
		shaderParticlesFountain.setMatrix4("view", 1, false,
			glm::value_ptr(view));
		// Settea la matriz de vista y projection al shader para el fuego
		shaderParticlesFire.setInt("Pass", 2);
		shaderParticlesFire.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shaderParticlesFire.setMatrix4("view", 1, false, glm::value_ptr(view));

		/*******************************************
		 * Propiedades de neblina
		 *******************************************/
		shaderMulLighting.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.5, 0.4)));
		shaderTerrain.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.5, 0.4)));
		shaderSkybox.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.5, 0.5, 0.4)));

		/*******************************************
		 * Propiedades Luz direccional
		 *******************************************/
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

		/*******************************************
		 * Propiedades Luz direccional Terrain
		 *******************************************/
		shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderTerrain.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderTerrain.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		shaderTerrain.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		shaderTerrain.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.707106781, -0.707106781, 0.0)));

		for (int i = 0; i < cebollaPosition.size(); i++) {
			glm::mat4 matrixAdjustCebolla = glm::mat4(1.0f);
			matrixAdjustCebolla = glm::translate(matrixAdjustCebolla, cebollaPosition[i]);
			matrixAdjustCebolla = glm::rotate(matrixAdjustCebolla, glm::radians(ArbolOrientation[i]), glm::vec3(0, 1, 0));
			matrixAdjustCebolla = glm::scale(matrixAdjustCebolla, glm::vec3(10.0, 10.0, 10.0));
			matrixAdjustCebolla = glm::translate(matrixAdjustCebolla, glm::vec3(0, 10.3585, 0));
			glm::vec3 cebollaPosition = glm::vec3(matrixAdjustCebolla[3]);
		}

		for (int i = 0; i < hongosPosition.size(); i++) {
			glm::mat4 MatrixAdjustHongo = glm::mat4(1.0f);
			MatrixAdjustHongo = glm::translate(MatrixAdjustHongo, hongosPosition[i]);
			MatrixAdjustHongo = glm::rotate(MatrixAdjustHongo, glm::radians(ArbolOrientation[i]), glm::vec3(0, 1, 0));
			MatrixAdjustHongo = glm::scale(MatrixAdjustHongo, glm::vec3(2.0, 2.0, 2.0));
			MatrixAdjustHongo = glm::translate(MatrixAdjustHongo, glm::vec3(0, 10.3585, 0));
			glm::vec3 hongosPosition = glm::vec3(MatrixAdjustHongo[3]);
		}

		for (int i = 0; i < enemigoPosition.size(); i++) {
			glm::mat4 matrixAdjustEnemigo = glm::mat4(1.0f);
			matrixAdjustEnemigo = glm::translate(matrixAdjustEnemigo, enemigoPosition[i]);
			matrixAdjustEnemigo = glm::rotate(matrixAdjustEnemigo, glm::radians(ArbolOrientation[i]), glm::vec3(0, 1, 0));
			matrixAdjustEnemigo = glm::scale(matrixAdjustEnemigo, glm::vec3(10.0, 10.0, 10.0));
			matrixAdjustEnemigo = glm::translate(matrixAdjustEnemigo, glm::vec3(0, 10.3585, 0));
			glm::vec3 enemigoPosition = glm::vec3(matrixAdjustEnemigo[3]);
		}
		for (int i = 0; i < ArbolPosition.size(); i++) {
			glm::mat4 matrixAdjustArbol = glm::mat4(1.0f);
			matrixAdjustArbol = glm::translate(matrixAdjustArbol, ArbolPosition[i]);
			matrixAdjustArbol = glm::rotate(matrixAdjustArbol, glm::radians(ArbolOrientation[i]), glm::vec3(0, 1, 0));
			matrixAdjustArbol = glm::scale(matrixAdjustArbol, glm::vec3(1.0005, 1.0005, 1.0005));
			matrixAdjustArbol = glm::translate(matrixAdjustArbol, glm::vec3(0.759521, 5.00174, 0));
			glm::vec3 lampPosition = glm::vec3(matrixAdjustArbol[3]);
		}

		/*******************************************
		 * 1.- We render the depth buffer
		 *******************************************/
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// render scene from light's point of view
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		//glCullFace(GL_FRONT);
		prepareDepthScene();
		renderScene(false);
		//glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		 /*******************************************
		  * 2.- We render the normal objects
		  *******************************************/
		glViewport(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		prepareScene();
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		shaderMulLighting.setInt("shadowMap", 10);
		shaderTerrain.setInt("shadowMap", 10);
		/*******************************************
		 * Skybox
		 *******************************************/
		GLint oldCullFaceMode;
		GLint oldDepthFuncMode;
		// deshabilita el modo del recorte de caras ocultas para ver las esfera desde adentro
		glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
		glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
		shaderSkybox.setFloat("skybox", 0);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
		glActiveTexture(GL_TEXTURE0);
		skyboxSphere.render();
		glCullFace(oldCullFaceMode);
		glDepthFunc(oldDepthFuncMode);
		renderScene();

		 /*******************************************
		  * Creacion de colliders
		  * IMPORTANT do this before interpolations
		  *******************************************/
		 
		//Collider del burro
		AbstractModel::SBB burroCollider;
		glm::mat4 modelMatrixColliderBurro = glm::mat4(modelMatrixBurro);
		modelMatrixColliderBurro = glm::scale(modelMatrixColliderBurro, glm::vec3(1.0, 1.0, 1.0));
		modelMatrixColliderBurro = glm::translate(modelMatrixColliderBurro, modelBurro.getSbb().c);
		burroCollider.c = glm::vec3(modelMatrixColliderBurro[3]);
		burroCollider.ratio = modelBurro.getSbb().ratio * 0.25;
		if(contador == 10)
			addOrUpdateColliders(collidersSBB, "Burro", burroCollider, modelMatrixBurro);

		//Collider Casa Shrek

		AbstractModel::SBB CasaShrekCollider;
		glm::mat4 modelMatrixColliderCasaShrek = glm::mat4(modelMatrixCasaShrek);
		modelMatrixColliderCasaShrek = glm::scale(modelMatrixColliderCasaShrek, glm::vec3(4.0, 1.0, 2.0));
		modelMatrixColliderCasaShrek = glm::translate(modelMatrixColliderCasaShrek, modelCasaShrek.getSbb().c);
		CasaShrekCollider.c = glm::vec3(modelMatrixColliderCasaShrek[3]);
		CasaShrekCollider.ratio = modelCasaShrek.getSbb().ratio * 4.0;
		addOrUpdateColliders(collidersSBB, "CasaShrek", CasaShrekCollider, modelMatrixCasaShrek);
				
		//Collider de las Cebollas
		for (int i = 0; i < cebollaPosition.size(); i++) {
			AbstractModel::OBB cebollaCollider;
			glm::mat4 modelMatrixColliderCebolla = glm::mat4(1.0);
			modelMatrixColliderCebolla = glm::translate(modelMatrixColliderCebolla, cebollaPosition[i]);
			modelMatrixColliderCebolla = glm::rotate(modelMatrixColliderCebolla, glm::radians(ArbolOrientation[i]),
			glm::vec3(0, 1, 0));
			addOrUpdateColliders(collidersOBB, "Cebolla-" + std::to_string(i), cebollaCollider, modelMatrixColliderCebolla);
			// Set the orientation of collider before doing the scale
			cebollaCollider.u = glm::quat_cast(modelMatrixColliderCebolla);
			modelMatrixColliderCebolla = glm::scale(modelMatrixColliderCebolla, glm::vec3(1.0, 1.0, 1.0));
			modelMatrixColliderCebolla = glm::translate(modelMatrixColliderCebolla, modelOnion.getObb().c);
			cebollaCollider.c = glm::vec3(modelMatrixColliderCebolla[3]);
			cebollaCollider.e = modelOnion.getObb().e * glm::vec3(1.0, 1.0, 1.0);
			std::get<0>(collidersOBB.find("Cebolla-" + std::to_string(i))->second) = cebollaCollider;
		}

		//Colliders Enemigos
		for (int i = 0; i < enemigoPosition.size(); i++) {
			AbstractModel::OBB enemigoCollider;
			glm::mat4 modelMatrixColliderEnemigo = glm::mat4(1.0);
			modelMatrixColliderEnemigo = glm::translate(modelMatrixColliderEnemigo, enemigoPosition[i]);
			modelMatrixColliderEnemigo = glm::rotate(modelMatrixColliderEnemigo, glm::radians(ArbolOrientation[i]),
				glm::vec3(0, 1, 0));
			addOrUpdateColliders(collidersOBB, "Enemigo-" + std::to_string(i), enemigoCollider, modelMatrixColliderEnemigo);
			// Set the orientation of collider before doing the scale
			enemigoCollider.u = glm::quat_cast(modelMatrixColliderEnemigo);
			modelMatrixColliderEnemigo = glm::scale(modelMatrixColliderEnemigo, glm::vec3(1.0, 0.25, 0.0));
			modelMatrixColliderEnemigo = glm::translate(modelMatrixColliderEnemigo, modelEnemigo.getObb().c);
			enemigoCollider.c = glm::vec3(modelMatrixColliderEnemigo[3]);
			enemigoCollider.e = modelEnemigo.getObb().e * glm::vec3(0.25, 0.25, 0.25);
			std::get<0>(collidersOBB.find("Enemigo-" + std::to_string(i))->second) = enemigoCollider;
		}

		// Collider del Arbol
		for (int i = 0; i < ArbolPosition.size(); i++) {
			AbstractModel::OBB ArbolCollider;
			glm::mat4 modelMatrixColliderArbol = glm::mat4(1.0);
			modelMatrixColliderArbol = glm::translate(modelMatrixColliderArbol, ArbolPosition[i]);
			modelMatrixColliderArbol = glm::rotate(modelMatrixColliderArbol, glm::radians(ArbolOrientation[i]),
				glm::vec3(0, 1, 0));
			addOrUpdateColliders(collidersOBB, "Arbol-" + std::to_string(i), ArbolCollider, modelMatrixColliderArbol);
			// Set the orientation of collider before doing the scale
			ArbolCollider.u = glm::quat_cast(modelMatrixColliderArbol);
			modelMatrixColliderArbol = glm::scale(modelMatrixColliderArbol, glm::vec3(0.5, 0.5, 0.5));
			modelMatrixColliderArbol = glm::translate(modelMatrixColliderArbol, modelArbol.getObb().c);
			ArbolCollider.c = glm::vec3(modelMatrixColliderArbol[3]);
			ArbolCollider.e = modelArbol.getObb().e * glm::vec3(0.5, 0.5, 0.5);
			std::get<0>(collidersOBB.find("Arbol-" + std::to_string(i))->second) = ArbolCollider;
		}

		// Collider de Shrek
		AbstractModel::OBB shrekCollider;
		glm::mat4 modelmatrixColliderShrek = glm::mat4(modelMatrixShrek);
		modelmatrixColliderShrek = glm::rotate(modelmatrixColliderShrek,
			glm::radians(-90.0f), glm::vec3(1, 0, 0));
		// Set the orientation of collider before doing the scale
		shrekCollider.u = glm::quat_cast(modelmatrixColliderShrek);
		modelmatrixColliderShrek = glm::scale(modelmatrixColliderShrek, glm::vec3(1.0, 0.0, 32.0));
		modelmatrixColliderShrek = glm::translate(modelmatrixColliderShrek,
			glm::vec3(modelShrek.getObb().c.x,
				modelShrek.getObb().c.y,
				modelShrek.getObb().c.z));
		shrekCollider.e = modelShrek.getObb().e * glm::vec3(1.0, 1.0, 5.150) * glm::vec3(1.5, 1.5, 1.5);
		shrekCollider.c = glm::vec3(modelmatrixColliderShrek[3]);
		addOrUpdateColliders(collidersOBB, "Shrek", shrekCollider, modelMatrixShrek);

		//Collider Castillo
		AbstractModel::OBB CastilloCollider;
		glm::mat4 modelmatrixColliderCastillo = glm::mat4(modelMatrixCastillo);
		// Set the orientation of collider before doing the scale
		CastilloCollider.u = glm::quat_cast(modelmatrixColliderCastillo);
		modelmatrixColliderCastillo = glm::scale(modelmatrixColliderCastillo, glm::vec3(1.0, 1.0, 1.0));
		modelmatrixColliderCastillo = glm::translate(modelmatrixColliderCastillo,	glm::vec3(-0.3203f, 25.0f, 0.0f));
		CastilloCollider.e = modelCastillo.getObb().e * glm::vec3(0.250, 0.250, 0.250) * glm::vec3(3.0, 1.0, 2.0);
		CastilloCollider.c = glm::vec3(modelmatrixColliderCastillo[3]);
		addOrUpdateColliders(collidersOBB, "Castillo", CastilloCollider, modelMatrixCastillo);

		//Collider Enemigo2s que se mueven
		glm::mat4 modelmatrixCollidermovEnemigo2 = glm::mat4(modelMatrixEnemigo2);
		AbstractModel::OBB movEnemigo2Collider;
		
		// Set the orientation of collider before doing the scale
		movEnemigo2Collider.u = glm::quat_cast(modelMatrixEnemigo2);
		modelmatrixCollidermovEnemigo2 = glm::scale(modelmatrixCollidermovEnemigo2, glm::vec3(1.0, 1.0, 1.0));
		modelmatrixCollidermovEnemigo2 = glm::translate(modelmatrixCollidermovEnemigo2, glm::vec3(modelEnemigo2.getObb().c.x, modelEnemigo2.getObb().c.y,
				modelEnemigo2.getObb().c.z));
		movEnemigo2Collider.c = glm::vec3(modelmatrixCollidermovEnemigo2[3]);
		movEnemigo2Collider.e = modelEnemigo2.getObb().e * glm::vec3(0.250, 0.250, 0.250) *glm::vec3(1.0, 1.0, 1.0);
		
		addOrUpdateColliders(collidersOBB, "Enemigo2", movEnemigo2Collider, modelMatrixEnemigo2);

		//Colliders de Paredes de Arbol

		AbstractModel::OBB ParedArbolCollider;
		glm::mat4 modelmatrixColliderParedArbol = glm::mat4(modelMatrixParedArbol);
		// Set the orientation of collider before doing the scale
		ParedArbolCollider.u = glm::quat_cast(modelmatrixColliderParedArbol);
		modelmatrixColliderParedArbol = glm::scale(modelmatrixColliderParedArbol, glm::vec3(1.0, 1.0, 1.0));
		modelmatrixColliderParedArbol = glm::translate(modelmatrixColliderParedArbol, glm::vec3(modelParedArbol.getObb().c.x, modelParedArbol.getObb().c.y,
			modelParedArbol.getObb().c.z));
		ParedArbolCollider.e = modelParedArbol.getObb().e * glm::vec3(1.0, 1.0, 1.0) * glm::vec3(1.0, 1.0, 1.0);
		ParedArbolCollider.c = glm::vec3(modelmatrixColliderParedArbol[3]);
		addOrUpdateColliders(collidersOBB, "ParedArbol-0", ParedArbolCollider, modelMatrixParedArbol);

		AbstractModel::OBB ParedArbol1Collider;
		glm::mat4 modelmatrixColliderParedArbol1 = glm::mat4(modelMatrixParedArbol1);
		// Set the orientation of collider before doing the scale
		ParedArbol1Collider.u = glm::quat_cast(modelmatrixColliderParedArbol1);
		modelmatrixColliderParedArbol1 = glm::scale(modelmatrixColliderParedArbol1, glm::vec3(1.0, 1.0, 1.0));
		modelmatrixColliderParedArbol1 = glm::translate(modelmatrixColliderParedArbol1, glm::vec3(modelParedArbol1.getObb().c.x, modelParedArbol1.getObb().c.y,
			modelParedArbol1.getObb().c.z));
		ParedArbol1Collider.e = modelParedArbol1.getObb().e * glm::vec3(1.0, 1.0, 1.0) * glm::vec3(1.0, 1.0, 1.0);
		ParedArbol1Collider.c = glm::vec3(modelmatrixColliderParedArbol1[3]);
		addOrUpdateColliders(collidersOBB, "ParedArbol-1", ParedArbol1Collider, modelMatrixParedArbol1);

		AbstractModel::OBB ParedArbol2Collider;
		glm::mat4 modelmatrixColliderParedArbol2 = glm::mat4(modelMatrixParedArbol2);
		// Set the orientation of collider before doing the scale
		ParedArbol2Collider.u = glm::quat_cast(modelmatrixColliderParedArbol2);
		modelmatrixColliderParedArbol2 = glm::scale(modelmatrixColliderParedArbol2, glm::vec3(1.0, 1.0, 1.0));
		modelmatrixColliderParedArbol2 = glm::translate(modelmatrixColliderParedArbol2, glm::vec3(modelParedArbol2.getObb().c.x, modelParedArbol2.getObb().c.y,
			modelParedArbol2.getObb().c.z));
		ParedArbol2Collider.e = modelParedArbol2.getObb().e * glm::vec3(1.0, 1.0, 1.0) * glm::vec3(1.0, 1.0, 1.0);
		ParedArbol2Collider.c = glm::vec3(modelmatrixColliderParedArbol2[3]);
		addOrUpdateColliders(collidersOBB, "ParedArbol-2", ParedArbol2Collider, modelMatrixParedArbol2);

		//Collider Torres
		AbstractModel::OBB ParedTorresCollider;
		glm::mat4 modelmatrixColliderParedTorres = glm::mat4(modelMatrixParedTorres);
		// Set the orientation of collider before doing the scale
		ParedTorresCollider.u = glm::quat_cast(modelmatrixColliderParedTorres);
		modelmatrixColliderParedTorres = glm::scale(modelmatrixColliderParedTorres, glm::vec3(1.0, 1.0, 1.0));
		modelmatrixColliderParedTorres = glm::translate(modelmatrixColliderParedTorres, glm::vec3(modelParedTorres.getObb().c.x, modelParedTorres.getObb().c.y,
			modelParedTorres.getObb().c.z));
		ParedTorresCollider.e = modelParedTorres.getObb().e * glm::vec3(1.0, 1.0, 1.0) * glm::vec3(1.0, 1.0, 1.0);
		ParedTorresCollider.c = glm::vec3(modelmatrixColliderParedTorres[3]);
		addOrUpdateColliders(collidersOBB, "ParedTorres-0", ParedTorresCollider, modelMatrixParedTorres);

		/*******************************************
		 * Test Colisions
		 *******************************************/
		for (std::map<std::string,
			std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
			collidersOBB.begin(); it != collidersOBB.end(); it++) {
			bool isCollision = false;
			for (std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
				collidersOBB.begin(); jt != collidersOBB.end(); jt++) {
				if (it != jt
					&& testOBBOBB(std::get<0>(it->second),
						std::get<0>(jt->second))) {
					isCollision = true;
					if (it->first == "Enemigo-0")
					{
						if (jt->first == "Shrek")
						{
							if (vida > 0)
							{
								vida--;
								alSourcePlay(source[0]);
							}
						}
					}
					if (it->first == "Enemigo-1")
					{
						if (jt->first == "Shrek")
						{
							if (vida > 0)
							{
								vida--;
								alSourcePlay(source[0]);
							}
						}
					}
					if (it->first == "Enemigo-2")
					{
						if (jt->first == "Shrek")
						{
							if (vida > 0)
							{
								vida--;
								alSourcePlay(source[0]);
							}
						}
					}
					if (it->first == "Enemigo-3")
					{
						if (jt->first == "Shrek")
						{
							if (vida > 0)
							{
								vida--;
								alSourcePlay(source[0]);
							}
						}
					}
					if (it->first == "Enemigo-4")
					{
						if (jt->first == "Shrek")
						{
							if (vida > 0)
							{
								vida--;
								alSourcePlay(source[0]);
							}
						}
					}
					if (it->first == "Enemigo-5")
					{
						if (jt->first == "Shrek")
						{
							if (vida > 0)
							{
								vida--;
								alSourcePlay(source[0]);
							}
						}
					}
					if (it->first == "Enemigo-6")
					{
						if (jt->first == "Shrek")
						{
							if (vida > 0)
							{
								vida--;
								alSourcePlay(source[0]);
							}
						}
					}
					if (it->first == "Enemigo-7")
					{
						if (jt->first == "Shrek")
						{
							if (vida > 0)
							{
								vida--;
								alSourcePlay(source[0]);
							}
						}
					}
					if (it->first == "Enemigo2")
					{
						if (jt->first == "Shrek")
						{
							if (vida > 0)
							{
								vida--;
								alSourcePlay(source[0]);
							}
						}
					}
					if (it->first == "Cebolla-0")
					{
						if (jt->first == "Shrek")
						{
							cebollaPosition[0] = glm::vec3(cebollaPosition[0].x + 500, 100.0, cebollaPosition[0].z);
							contador++;
							alSourcePlay(source[1]);
						}
					}
					if (it->first == "Cebolla-1")
					{
						if (jt->first == "Shrek")
						{
							cebollaPosition[1] = glm::vec3(cebollaPosition[1].x + 500, 100.0, cebollaPosition[1].z);
							contador++;
							alSourcePlay(source[1]);
						}
					}
					if (it->first == "Cebolla-2")
					{
						if (jt->first == "Shrek")
						{
							cebollaPosition[2] = glm::vec3(cebollaPosition[2].x + 500, 100.0, cebollaPosition[2].z);
							contador++;
							alSourcePlay(source[1]);
						}
					}
					if (it->first == "Cebolla-3")
					{
						if (jt->first == "Shrek")
						{
							cebollaPosition[3] = glm::vec3(cebollaPosition[3].x + 500, 100.0, cebollaPosition[3].z);
							contador++;
							alSourcePlay(source[1]);
						}
					}
					if (it->first == "Cebolla-4")
					{
						if (jt->first == "Shrek")
						{
							cebollaPosition[4] = glm::vec3(cebollaPosition[4].x + 500, 100.0, cebollaPosition[4].z);
							contador++;
							alSourcePlay(source[1]);
						}
					}
					if (it->first == "Cebolla-5")
					{
						if (jt->first == "Shrek")
						{
							cebollaPosition[5] = glm::vec3(cebollaPosition[5].x + 500, 100.0, cebollaPosition[5].z);
							contador++;
							alSourcePlay(source[1]);
						}
					}
					if (it->first == "Cebolla-6")
					{
						if (jt->first == "Shrek")
						{
							cebollaPosition[6] = glm::vec3(cebollaPosition[6].x + 500, 100.0, cebollaPosition[6].z);
							contador++;
							alSourcePlay(source[1]);
						}
					}
					if (it->first == "Cebolla-7")
					{
						if (jt->first == "Shrek")
						{
							cebollaPosition[7] = glm::vec3(cebollaPosition[7].x + 500, 100.0, cebollaPosition[7].z);
							contador++;
							alSourcePlay(source[1]);
						}
					}
					if (it->first == "Cebolla-8")
					{
						if (jt->first == "Shrek")
						{
							cebollaPosition[8] = glm::vec3(cebollaPosition[8].x + 500, 100.0, cebollaPosition[8].z);
							contador++;
							alSourcePlay(source[1]);
						}
					}
					if (it->first == "Cebolla-9")
					{
						if (jt->first == "Shrek")
						{
							cebollaPosition[9] = glm::vec3(cebollaPosition[9].x + 500, 100.0, cebollaPosition[9].z);
							contador++;
							alSourcePlay(source[1]);
						}
					}				
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}
		

		for (std::map<std::string,
			std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
			collidersSBB.begin(); it != collidersSBB.end(); it++) {
			bool isCollision = false;
			for (std::map<std::string,
				std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator jt =
				collidersSBB.begin(); jt != collidersSBB.end(); jt++) {
				if (it != jt
					&& testSphereSphereIntersection(std::get<0>(it->second),
						std::get<0>(jt->second))) {
					isCollision = true;
				}
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		for (std::map<std::string,
			std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
			collidersSBB.begin(); it != collidersSBB.end(); it++) {
			bool isCollision = false;
			std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
				collidersOBB.begin();
			for (; jt != collidersOBB.end(); jt++) {
				if (testSphereOBox(std::get<0>(it->second),
					std::get<0>(jt->second))) {
					isCollision = true;
					addOrUpdateCollisionDetection(collisionDetection, jt->first, isCollision);

					if (it->first == "CasaShrek")
					{
						if (jt->first == "Shrek")
						{
							if (vida < 100)
								vida++;
						}
					}
						
					if (it->first == "Burro")
					{
						if (jt->first == "Shrek")
						{	
							Rescate = true;
							alSourcePlay(source[2]);
						}
					}
				}		
			}
			addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
		}

		std::map<std::string, bool>::iterator colIt;
		for (colIt = collisionDetection.begin(); colIt != collisionDetection.end();
			colIt++) {
			std::map<std::string,
				std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> >::iterator it =
				collidersSBB.find(colIt->first);
			std::map<std::string,
				std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator jt =
				collidersOBB.find(colIt->first);
			if (it != collidersSBB.end()) {
				if (!colIt->second)
					addOrUpdateColliders(collidersSBB, it->first);
			}
			if (jt != collidersOBB.end()) {
				if (!colIt->second)
					addOrUpdateColliders(collidersOBB, jt->first);
				else {
					if (jt->first.compare("Shrek") == 0)
						modelMatrixShrek = std::get<1>(jt->second);
				}
			}
		}
		
		switch (state) {
		case 0:
			if (numberAdvance == 0)
				maxAdvance = 25.0f;
			else if (numberAdvance == 1)
				maxAdvance = 25.0;
			else if (numberAdvance == 2)
				maxAdvance = 12.5;
			else if (numberAdvance == 3)
				maxAdvance = 25.0;
			else if (numberAdvance == 4)
				maxAdvance = 12.5;
			else if (numberAdvance == 5)
				maxAdvance = 25.0;
			else if (numberAdvance == 6)
				maxAdvance = 12.5;
			state = 1;
			break;
		case 1:
			modelMatrixEnemigo2 = glm::translate(modelMatrixEnemigo2, glm::vec3(0.0f, 0.0f, 0.5));
			
			advanceCount += 0.1;
			if (advanceCount > maxAdvance) {
				advanceCount = 0;
				numberAdvance++;
				state = 2;
			}
			break;
		case 2:
			modelMatrixEnemigo2 = glm::translate(modelMatrixEnemigo2, glm::vec3(0.0f, 0.0f, 0.5f));
			modelMatrixEnemigo2 = glm::rotate(modelMatrixEnemigo2, glm::radians(0.5f), glm::vec3(0.0f, 0.5f, 0.0f));
			
			if (rotCount >= 15.0f) {
				rotCount = 0;
				state = 0;
				if (numberAdvance > 4)
					numberAdvance = 1;
			}
			break;
		default:
			break;
		}

		glEnable(GL_BLEND);

		if (Mouse >= 10)
		{
			Contador->render("Shrek Rescues ", -0.85, 0.5, 100, 0.5, 1.0, 0.2);
			Contador->render("Burro", -0.55, 0.25, 100, 0.5, 1.0, 0.2);
			Contador->render("Iniciar", -0.3, -0.25, 100, 0.5, 1.0, 0.2);
		}
		
		if (Mouse < 10)
		{
			Contador->render("Numero de cebollas: " + std::to_string(contador), -0.95, 0.9, 20, 0.5, 1.0, 0.2);
			Contador->render("Salud restante: " + std::to_string(vida), 0.5, 0.9, 20, 0.5, 1.0, 0.2);
		}
		
		if (Rescate)
		{
			Contador->render("Has rescatado", -0.85, 0.0, 100, 0.5, 1.0, 0.2);
			Contador->render("al burro", -0.55, -0.25, 100, 0.5, 1.0, 0.2);
		}
			
		if(vida ==0)
			Contador->render("Wasted", 0.0, -0.9, 100, 0.5, 1.0, 0.2);
		glDisable(GL_BLEND);
		glfwSwapBuffers(window);

		/****************************+
		 * Open AL sound data
		 */
		
		// Listener for the Thris person camera
		listenerPos[0] = modelMatrixShrek[3].x;
		listenerPos[1] = modelMatrixShrek[3].y;
		listenerPos[2] = modelMatrixShrek[3].z;
		alListenerfv(AL_POSITION, listenerPos);

		glm::vec3 upModel = glm::normalize(modelMatrixShrek[1]);
		glm::vec3 frontModel = glm::normalize(modelMatrixShrek[2]);

		listenerOri[0] = frontModel.x;
		listenerOri[1] = frontModel.y;
		listenerOri[2] = frontModel.z;
		listenerOri[3] = upModel.x;
		listenerOri[4] = upModel.y;
		listenerOri[5] = upModel.z;
		alListenerfv(AL_ORIENTATION, listenerOri);
	}
}

void prepareScene() {

	skyboxSphere.setShader(&shaderSkybox);

	modelBurro.setShader(&shaderMulLighting);

	modelParedTorres.setShader(&shaderMulLighting);

	modelEnemigo.setShader(&shaderMulLighting);
	modelEnemigo2.setShader(&shaderMulLighting);

	modelParedArbol.setShader(&shaderMulLighting);
	modelParedArbol1.setShader(&shaderMulLighting);
	modelParedArbol2.setShader(&shaderMulLighting);
	modelCastillo.setShader(&shaderMulLighting);

	modelTorre1.setShader(&shaderMulLighting);
	modelTorre2.setShader(&shaderMulLighting);
	modelCasaShrek.setShader(&shaderMulLighting);

	terrain.setShader(&shaderTerrain);

	//Modelos Extras
	modelArbol.setShader(&shaderMulLighting);
	modelParedArbol.setShader(&shaderMulLighting);
	modelOnion.setShader(&shaderMulLighting);
	modelEnemigo.setShader(&shaderMulLighting);
	modelEnemigo2.setShader(&shaderMulLighting);
	boxImagen.setShader(&shaderMulLighting);

	//Hongos
	modelHongo.setShader(&shaderMulLighting);

	modelAntorcha.setShader(&shaderMulLighting);
	modelAntorcha2.setShader(&shaderMulLighting);

	//Shrek
	modelShrek.setShader(&shaderMulLighting);
}

void prepareDepthScene() {

	skyboxSphere.setShader(&shaderDepth);

	modelBurro.setShader(&shaderDepth);

	modelParedTorres.setShader(&shaderDepth);

	modelEnemigo.setShader(&shaderDepth);
	modelEnemigo2.setShader(&shaderDepth);

	modelParedArbol.setShader(&shaderDepth);
	modelParedArbol1.setShader(&shaderDepth);
	modelParedArbol2.setShader(&shaderDepth);
	modelCastillo.setShader(&shaderDepth);

	modelTorre1.setShader(&shaderDepth);
	modelTorre2.setShader(&shaderDepth);
	modelCasaShrek.setShader(&shaderDepth);

	boxImagen.setShader(&shaderDepth);

	terrain.setShader(&shaderDepth);

	//Arbol models
	modelArbol.setShader(&shaderDepth);
	modelParedArbol.setShader(&shaderDepth);
	modelOnion.setShader(&shaderDepth);
	modelEnemigo.setShader(&shaderDepth);
	modelEnemigo2.setShader(&shaderDepth);

	//Hongo
	modelHongo.setShader(&shaderDepth);

	modelAntorcha.setShader(&shaderDepth);
	modelAntorcha2.setShader(&shaderDepth);

	//Shrek
	modelShrek.setShader(&shaderDepth);
}

void renderScene(bool renderParticles) {
	/*******************************************
	 * Terrain Cesped
	 *******************************************/
	glm::mat4 modelCesped = glm::mat4(1.0);
	modelCesped = glm::translate(modelCesped, glm::vec3(0.0, 0.0, 0.0));
	modelCesped = glm::scale(modelCesped, glm::vec3(200.0, 0.001, 200.0));
	// Se activa la textura del background
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBackgroundID);
	shaderTerrain.setInt("backgroundTexture", 0);
	// Se activa la textura de tierra
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
	shaderTerrain.setInt("rTexture", 1);
	// Se activa la textura de hierba
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
	shaderTerrain.setInt("gTexture", 2);
	// Se activa la textura del camino
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
	shaderTerrain.setInt("bTexture", 3);
	// Se activa la textura del blend map
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID);
	shaderTerrain.setInt("blendMapTexture", 4);
	shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(40, 40)));
	terrain.render();
	shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
	glBindTexture(GL_TEXTURE_2D, 0);

	/*******************************************
	 * Custom objects obj
	 *******************************************/
	
	if(contador ==10)
		modelBurro.render(modelMatrixBurro);
	modelEnemigo2.render(modelMatrixEnemigo2);
	modelParedArbol.render(modelMatrixParedArbol);
	modelParedArbol1.render(modelMatrixParedArbol1);
	modelParedArbol2.render(modelMatrixParedArbol2);
	modelParedTorres.render(modelMatrixParedTorres);
	modelCastillo.render(modelMatrixCastillo);
	modelAntorcha.render(modelMatrixAntorcha);
	modelAntorcha2.render(modelMatrixAntorcha2);

	if(Mouse >=10)
		boxImagen.render(modelMatrixImagen);

	modelTorre1.render(modelMatrixTorre1);
	modelTorre2.render(modelMatrixTorre2);
	modelCasaShrek.render(modelMatrixCasaShrek);
	// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
	glActiveTexture(GL_TEXTURE0);

	// Render the lamps
	for (int i = 0; i < cebollaPosition.size(); i++) {
		modelOnion.setPosition(cebollaPosition[i]);
		modelOnion.setScale(glm::vec3(1.0, 1.0, 1.0));
		modelOnion.setOrientation(glm::vec3(0, ArbolOrientation[i], 0));
		modelOnion.render();
	}

	for (int i = 0; i < hongosPosition.size(); i++) {
		modelHongo.setPosition(hongosPosition[i]);
		modelHongo.setScale(glm::vec3(2.0, 2.0, 2.0));
		modelHongo.setOrientation(glm::vec3(0, ArbolOrientation[i], 0));
		modelHongo.render();
	}

	for (int i = 0; i < enemigoPosition.size(); i++) {
		modelEnemigo.setPosition(enemigoPosition[i]);
		modelEnemigo.setScale(glm::vec3(0.25, 0.25, 0.25));
		modelEnemigo.setOrientation(glm::vec3(0, ArbolOrientation[i], 0));
		if(vida > 0)
			modelEnemigo.render();
	}

	for (int i = 0; i < ArbolPosition.size(); i++) {
		ArbolPosition[i].y = terrain.getHeightTerrain(ArbolPosition[i].x, ArbolPosition[i].z);
		modelArbol.setPosition(ArbolPosition[i]);
		modelArbol.setScale(glm::vec3(0.5, 0.5, 0.5));
		modelArbol.setOrientation(glm::vec3(0, ArbolOrientation[i], 0));
		modelArbol.render();
		modelArbol.setPosition(ArbolPosition[i]);
		modelArbol.setScale(glm::vec3(0.5, 0.5, 0.5));
		modelArbol.setOrientation(glm::vec3(0, ArbolOrientation[i], 0));
		modelArbol.render();
	}

	/*******************************************
	 * Custom Anim objects obj
	 *******************************************/
	modelMatrixShrek[3][1] = -GRAVITY * tmv * tmv + 3.5 * tmv + terrain.getHeightTerrain(modelMatrixShrek[3][0], modelMatrixShrek[3][2]);
	tmv = currTime - startTimeJump;
	if (modelMatrixShrek[3][1] < terrain.getHeightTerrain(modelMatrixShrek[3][0], modelMatrixShrek[3][2]))
	{
		isJump = false;
		modelMatrixShrek[3][1] = terrain.getHeightTerrain(modelMatrixShrek[3][0], modelMatrixShrek[3][2]);
	}
	glm::mat4 modelMatrixShrekBody = glm::mat4(modelMatrixShrek);
	modelMatrixShrekBody = glm::scale(modelMatrixShrekBody, glm::vec3(0.03, 0.03, 0.03));
	modelShrek.setAnimationIndex(animationIndex);
	if (segundos == 0)
	{
		if (vida > 0)
			modelShrek.render(modelMatrixShrekBody);
	}

	/**********
	 * Sorter with alpha objects
	 */
	std::map<float, std::pair<std::string, glm::vec3>> blendingSorted;
	std::map<std::string, glm::vec3>::iterator itblend;
	for (itblend = blendingUnsorted.begin(); itblend != blendingUnsorted.end(); itblend++) {
		float distanceFromView = glm::length(camera->getPosition() - itblend->second);
		blendingSorted[distanceFromView] = std::make_pair(itblend->first, itblend->second);
	}

	/**********
	 * Render de las transparencias
	 */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	for (std::map<float, std::pair<std::string, glm::vec3> >::reverse_iterator it = blendingSorted.rbegin(); it != blendingSorted.rend(); it++) {
		if (renderParticles && it->second.first.compare("fire") == 0) {
			/**********
			 * Init Render particles systems
			 */
			lastTimeParticlesAnimationFire = currTimeParticlesAnimationFire;
			currTimeParticlesAnimationFire = TimeManager::Instance().GetTime();

			shaderParticlesFire.setInt("Pass", 1);
			shaderParticlesFire.setFloat("Time", currTimeParticlesAnimationFire);
			shaderParticlesFire.setFloat("DeltaT", currTimeParticlesAnimationFire - lastTimeParticlesAnimationFire);

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_1D, texId);
			glEnable(GL_RASTERIZER_DISCARD);
			glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
			glBeginTransformFeedback(GL_POINTS);
			glBindVertexArray(particleArray[1 - drawBuf]);
			glVertexAttribDivisor(0, 0);
			glVertexAttribDivisor(1, 0);
			glVertexAttribDivisor(2, 0);
			glDrawArrays(GL_POINTS, 0, nParticlesFire);
			glEndTransformFeedback();
			glDisable(GL_RASTERIZER_DISCARD);

			shaderParticlesFire.setInt("Pass", 2);
			glm::mat4 modelFireParticles = glm::mat4(1.0);
			modelFireParticles = glm::translate(modelFireParticles, glm::vec3(-61.0f, 15.0f, -250.0f)/*it->second.second*/);
			modelFireParticles = glm::scale(modelFireParticles, glm::vec3(0.05, 1.0, 1.0));
			shaderParticlesFire.setMatrix4("model", 1, false, glm::value_ptr(modelFireParticles));

			shaderParticlesFire.turnOn();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, textureParticleFireID);
			glDepthMask(GL_FALSE);
			glBindVertexArray(particleArray[drawBuf]);
			glVertexAttribDivisor(0, 1);
			glVertexAttribDivisor(1, 1);
			glVertexAttribDivisor(2, 1);
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, nParticlesFire);
			glBindVertexArray(0);
			glDepthMask(GL_TRUE);
			drawBuf = 1 - drawBuf;
			shaderParticlesFire.turnOff();

			/****************************+
			 * Open AL sound data
			 */
			source1Pos[0] = modelFireParticles[3].x;
			source1Pos[1] = modelFireParticles[3].y;
			source1Pos[2] = modelFireParticles[3].z;
			alSourcefv(source[1], AL_POSITION, source1Pos);

			/**********
			 * End Render particles systems
			 */
		}

	}
	glEnable(GL_CULL_FACE);

}

int main(int argc, char **argv) {
	init(800, 700, "Shrek Rescues Burro", false);
	applicationLoop();
	destroy();
	return 1;
}