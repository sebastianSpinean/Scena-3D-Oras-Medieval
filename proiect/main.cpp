#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp" //core glm functionality
#include "glm/gtc/matrix_transform.hpp" //glm extension for generating common transformation matrices
#include "glm/gtc/matrix_inverse.hpp" //glm extension for computing inverse matrices
#include "glm/gtc/type_ptr.hpp" //glm extension for accessing the internal data structure of glm types

#include "Window.h"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Model3D.hpp"

#include <iostream>
#include<list>

// window
gps::Window myWindow;

// matrices
glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;
glm::mat3 normalMatrix;

glm::mat4 modelElice;
glm::mat3 normalMatrixElice;
glm::mat4 viewElice;
glm::mat4 modelCar;
glm::mat4 modelRoataDreapta;
glm::mat4 modelRoataStanga;


// light parameters
glm::vec3 lightDir;
glm::vec3 lightColor;

glm::vec3 lightPos;

// shader uniform locations
GLuint modelLoc;
GLuint viewLoc;
GLuint projectionLoc;
GLuint normalMatrixLoc;
GLuint lightDirLoc;
GLuint lightColorLoc;

GLuint lightPosLoc;
GLuint shadowMapFBO;
GLuint depthMapTexture;


gps::Camera myCamera(
	glm::vec3(0.0f, 0.0f, 3.0f),
	glm::vec3(0.0f, 0.0f, -10.0f),
	glm::vec3(0.0f, 1.0f, 0.0f));

	
	


GLfloat cameraSpeed = 0.1f;

GLboolean pressedKeys[1024];

// models
gps::Model3D teapot;
GLfloat angle;

// shaders
gps::Shader myBasicShader;


gps::Shader depthMapShader;



gps::Model3D sky;
gps::Model3D groundCity;

gps::Model3D scena;
gps::Model3D lampi;
gps::Model3D elice;
gps::Model3D car;
gps::Model3D roata_dreapta;
gps::Model3D roata_stanga;
gps::Model3D ploaie;
gps::Model3D skyCloud;
gps::Model3D barn;
gps::Model3D stoneBarn;








float delta = 0;


const unsigned int SHADOW_WIDTH = 2048;
const unsigned int SHADOW_HEIGHT = 2048;

int fog = 1;
int pornesteLuminaDir = 1;

float carZ = 0.0f;


float roataDreaptaZ = 0.004498f;


float roataStangaZ = 0.004911f;
float rotireRoata;

bool mersFata = true;

std::vector<glm::vec3> picaturiPloaie;

int pornirePloaie = 0;




GLenum glCheckError_(const char *file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR) {
		std::string error;
		switch (errorCode) {
            case GL_INVALID_ENUM:
                error = "INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                error = "INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                error = "INVALID_OPERATION";
                break;
            case GL_STACK_OVERFLOW:
                error = "STACK_OVERFLOW";
                break;
            case GL_STACK_UNDERFLOW:
                error = "STACK_UNDERFLOW";
                break;
            case GL_OUT_OF_MEMORY:
                error = "OUT_OF_MEMORY";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error = "INVALID_FRAMEBUFFER_OPERATION";
                break;
        }
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	fprintf(stdout, "Window resized! New width: %d , and height: %d\n", width, height);
	//TODO
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

	if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            pressedKeys[key] = true;
        } else if (action == GLFW_RELEASE) {
            pressedKeys[key] = false;
        }
    }
}

float xpos0 = 0, ypos0 = 0;
float mouseSpeed = 0.02f;
GLboolean begin=true;
void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    //TODO
	if (begin) {
		xpos0 = xpos;
		ypos0 = ypos;
		begin = false;
	}
	else {
		double deltaX = xpos - xpos0;
		double deltaY = ypos - ypos0;
		myCamera.rotate(-1 * deltaY * mouseSpeed, deltaX * mouseSpeed);

		//update view matrix
		view = myCamera.getViewMatrix();
		myBasicShader.useShaderProgram();
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));

		xpos0 = xpos;
		ypos0 = ypos;
	}
}


void renderScene();
void animatieCamera() {
	std::list<glm::vec3> pozitii;
	pozitii.push_back(glm::vec3(-1.37391f, 0.648636f, 1.84957f));
	float x = -1.37391;
	while (x < 1.5f){
		x = x + 0.01f;
		pozitii.push_back(glm::vec3(x, 0.648636, 1.84957f));
		

	}

	while (pozitii.size()!=0) {
		glm::vec3 camPos = pozitii.front();
		pozitii.pop_front();
		myCamera = gps::Camera(camPos,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);
		
		renderScene();

		glfwPollEvents();
		glfwSwapBuffers(myWindow.getWindow());
	}

	
	pozitii.push_back(glm::vec3(1.5f, 0.648636f, 1.84957f));
	float z = 1.84957f;
	while (z > -2.12836f) {
		z = z - 0.01f;
		pozitii.push_back(glm::vec3(1.5f, 0.648636, z));
	
	}

	while (pozitii.size() != 0) {
		glm::vec3 camPos = pozitii.front();
		pozitii.pop_front();
		myCamera = gps::Camera(camPos,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);
		
		renderScene();

		glfwPollEvents();
		glfwSwapBuffers(myWindow.getWindow());
	}

	x = 1.5f;
	while (x > -1.37391f) {
		pozitii.push_back(glm::vec3(x, 0.648636, -2.12836f));
		x = x - 0.01f;
		

	}
	while (pozitii.size() != 0) {
		glm::vec3 camPos = pozitii.front();
		pozitii.pop_front();
		myCamera = gps::Camera(camPos,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);
		
		renderScene();

		glfwPollEvents();
		glfwSwapBuffers(myWindow.getWindow());
	}

    z = -2.12836f;
	while (z < 1.84957f) {
		pozitii.push_back(glm::vec3(-1.37391f, 0.648636, z));
		z = z + 0.01f;
		

	}

	while (pozitii.size() != 0) {
		glm::vec3 camPos = pozitii.front();
		pozitii.pop_front();
		myCamera = gps::Camera(camPos,
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);

		renderScene();

		glfwPollEvents();
		glfwSwapBuffers(myWindow.getWindow());
	}

	myCamera = gps::Camera(glm::vec3(0.0f, 0.5f, 3.0f),
		glm::vec3(0.0f, 0.5f, -10.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));





}


bool coliziune() {
	
	float x = myCamera.getCameraPosition().x;
	float y = myCamera.getCameraPosition().y;
	float z = myCamera.getCameraPosition().z;


	glm::vec3 sferaMoara = glm::vec3(5.74687f, 0.160284f, -9.06672f);

	float razaMoara = glm::length(glm::vec3(5.73367, 0.183486, -8.94603f) - sferaMoara);

	if ((x - sferaMoara.x) * (x - sferaMoara.x) + (y - sferaMoara.y) * (y - sferaMoara.y) + (z - sferaMoara.z) * (z - sferaMoara.z) <= razaMoara) {
		return true;
	}

	glm::vec3 sferaStoneBarn = glm::vec3(7.74386f, 0.064164f, -5.48822f);
	float razaStoneBarn = glm::length(glm::vec3(7.63157f, 0.080956f, -5.47162f) - sferaStoneBarn);
	if ((x - sferaStoneBarn.x) * (x - sferaStoneBarn.x) + (y - sferaStoneBarn.y) * (y - sferaStoneBarn.y) + (z - sferaStoneBarn.z) * (z - sferaStoneBarn.z) <= razaStoneBarn) {
		return true;
	}

	glm::vec3 sferaBarn = glm::vec3(7.03862f, 0.070755f, -7.05404f);
	float razaBarn = glm::length(glm::vec3(6.83571f, 0.1525f, -6.93457f) - sferaBarn);
	if ((x - sferaBarn.x) * (x - sferaBarn.x) + (y - sferaBarn.y) * (y - sferaBarn.y) + (z - sferaBarn.z) * (z - sferaBarn.z) <= razaBarn) {
		return true;
	}

	return false;
}

void processMovement() {
	if (pressedKeys[GLFW_KEY_W]) {

		myCamera.move(gps::MOVE_FORWARD, cameraSpeed);
		if (coliziune()) {
			myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
			return;
		}
		myCamera.move(gps::MOVE_FORWARD, cameraSpeed);
		//update view matrix
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
       
        normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	}

	if (pressedKeys[GLFW_KEY_S]) {

		myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
		if (coliziune()) {
			myCamera.move(gps::MOVE_FORWARD, cameraSpeed);
			return;
		}

		myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
        //update view matrix
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
       
        normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	}

	if (pressedKeys[GLFW_KEY_A]) {

		myCamera.move(gps::MOVE_LEFT, cameraSpeed);
		if (coliziune()) {
			myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
			return;
		}



		myCamera.move(gps::MOVE_LEFT, cameraSpeed);
        //update view matrix
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        
        normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	}

	if (pressedKeys[GLFW_KEY_D]) {

		myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
		if (coliziune()) {
			myCamera.move(gps::MOVE_LEFT, cameraSpeed);
			return;
		}


		myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
        //update view matrix
        view = myCamera.getViewMatrix();
        myBasicShader.useShaderProgram();
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        
        normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	}

   

	//rotatia elicei
	if (pressedKeys[GLFW_KEY_R]) {
		delta += 0.01;
		
	}

	//deplasare car
	if (pressedKeys[GLFW_KEY_Y]) {
		if (mersFata) {
			carZ -= 0.001f;
			roataDreaptaZ -= 0.001f;
			roataStangaZ -= 0.001f;
		}
		else {
			carZ += 0.001f;
			roataDreaptaZ += 0.001f;
			roataStangaZ += 0.001f;
		}
		if (carZ <= -1.2f) {
			mersFata = false;
		}
		if (carZ >= 0.0f) {
			mersFata = true;
		}
		rotireRoata += 0.1f;


		
	}

	
	if (pressedKeys[GLFW_KEY_M]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
	}

	if (pressedKeys[GLFW_KEY_N]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (pressedKeys[GLFW_KEY_B]) {
		glfwWindowHint(GLFW_SAMPLES, 4);
		glEnable(GL_MULTISAMPLE);
		
	}
	if (pressedKeys[GLFW_KEY_V]) {
		glDisable(GL_MULTISAMPLE);
	}


	//ceata
	if (pressedKeys[GLFW_KEY_C]) {
		if (fog == 1) {
			fog = 0;
		}
		else {
			fog = 1;
		}
		myBasicShader.useShaderProgram();
		glUniform1i(glGetUniformLocation(myBasicShader.shaderProgram, "fog"), fog);
	}

	//animatia scenei
	if (pressedKeys[GLFW_KEY_T]) {

		animatieCamera();
	}

	//ploaie
	if (pressedKeys[GLFW_KEY_P]) {

		if (pornirePloaie == 0) {
			pornirePloaie = 1;
		}
		else if (pornirePloaie == 1) {
			pornirePloaie = 0;
		}
		
		

		
	}

	//lumini
	if (pressedKeys[GLFW_KEY_L]) {
		if (pornesteLuminaDir == 1) {
			pornesteLuminaDir = 0;
		}
		else {
			pornesteLuminaDir = 1;
		}
		myBasicShader.useShaderProgram();
		glUniform1i(glGetUniformLocation(myBasicShader.shaderProgram, "pornesteLuminaDir"), pornesteLuminaDir);
	}

}



void initOpenGLWindow() {
    myWindow.Create(1024, 768, "OpenGL Project Core");
	
}

void setWindowCallbacks() {
	glfwSetWindowSizeCallback(myWindow.getWindow(), windowResizeCallback);
    glfwSetKeyCallback(myWindow.getWindow(), keyboardCallback);
    glfwSetCursorPosCallback(myWindow.getWindow(), mouseCallback);
}

void initOpenGLState() {
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	glViewport(0, 0, myWindow.getWindowDimensions().width, myWindow.getWindowDimensions().height);
    glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise
}

void initModels() {
   
	sky.LoadModel("models/sky/sky4.obj");
	scena.LoadModel("models/Final/scena5.obj");
	//copaci.LoadModel("models/copac_foaie/copaci.obj");
	lampi.LoadModel("models/lampa/lampi.obj");
	elice.LoadModel("models/moara/elice.obj");
	car.LoadModel("models/car/car.obj");
	roata_dreapta.LoadModel("models/car/roata_dreapta.obj");
	roata_stanga.LoadModel("models/car/roata_stanga.obj");
	
	
	ploaie.LoadModel("models/ploaie/ploaie.obj");
	skyCloud.LoadModel("models/sky/skyCloud.obj");
	barn.LoadModel("models/barn/barn.obj");
	stoneBarn.LoadModel("models/stoneBarn/stoneBarn.obj");


}

void initShaders() {
	myBasicShader.loadShader(
        "shaders/basic.vert",
        "shaders/basic.frag");
	depthMapShader.loadShader(
		"shaders/shadow.vert",
		"shaders/shadow.frag");


}

void initUniforms() {
	myBasicShader.useShaderProgram();

    // create model matrix for teapot
    model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	modelLoc = glGetUniformLocation(myBasicShader.shaderProgram, "model");

	// get view matrix for current camera
	view = myCamera.getViewMatrix();
	viewLoc = glGetUniformLocation(myBasicShader.shaderProgram, "view");
	// send view matrix to shader
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    // compute normal matrix for teapot
    normalMatrix = glm::mat3(glm::inverseTranspose(view*model));
	normalMatrixLoc = glGetUniformLocation(myBasicShader.shaderProgram, "normalMatrix");

	// create projection matrix
	projection = glm::perspective(glm::radians(45.0f),
                               (float)myWindow.getWindowDimensions().width / (float)myWindow.getWindowDimensions().height,
                               0.1f, 500.0f);
	projectionLoc = glGetUniformLocation(myBasicShader.shaderProgram, "projection");
	// send projection matrix to shader
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));	

	//set the light direction (direction towards the light)
	lightDir = glm::vec3(0.0f, 1.0f, 1.0f);
	//lightDir = glm::vec3(88, 244, 59);
	lightDirLoc = glGetUniformLocation(myBasicShader.shaderProgram, "lightDir");
	// send light dir to shader
	glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDir));

	//set light color
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f); //white light
	lightColorLoc = glGetUniformLocation(myBasicShader.shaderProgram, "lightColor");
	// send light color to shader
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));



	///////////////////////
	lightPos = glm::vec3(-0.362422f, 0.057025, -0.63171f);
	lightPosLoc = glGetUniformLocation(myBasicShader.shaderProgram, "lightPos");
	glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
	//////////////////////


	

	
}


void initFBO() {
	//TODO - Create the FBO, the depth texture and attach the depth texture to the FBO
	glGenFramebuffers(1, &shadowMapFBO);
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
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture,
		0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void renderSky(gps::Shader shader) {
	// select active shader program
	shader.useShaderProgram();

	
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	
	sky.Draw(shader);

	
} 

void renderSkyCloud(gps::Shader shader) {
	// select active shader program
	shader.useShaderProgram();

	
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	
	skyCloud.Draw(shader);

	
}




void renderCar(gps::Shader shader, bool shadow) {
	// select active shader program
	shader.useShaderProgram();
	
	modelCar = glm::mat4(1.0f);
	modelCar = glm::translate(modelCar, glm::vec3(-0.96886f , 0.01887f, carZ));

	
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelCar));

	
	if (shadow == false) {
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}

	car.Draw(shader);

}

void renderRoataDreapta(gps::Shader shader, bool shadow) {

	shader.useShaderProgram();

	modelRoataDreapta = glm::mat4(1.0f);
	modelRoataDreapta = glm::translate(modelRoataDreapta, glm::vec3(-0.9825f, 0.017346f, roataDreaptaZ));
	if (mersFata) {
		modelRoataDreapta = glm::rotate(modelRoataDreapta, -rotireRoata, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	else {
		modelRoataDreapta = glm::rotate(modelRoataDreapta, rotireRoata, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	
	modelRoataDreapta = glm::translate(modelRoataDreapta, glm::vec3(0.9825f,-0.017346f,-roataDreaptaZ));

	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelRoataDreapta)); 

	modelRoataDreapta = glm::translate(modelRoataDreapta, glm::vec3(-0.9825f, 0.017346f, roataDreaptaZ));

	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelRoataDreapta));

	if (shadow == false) {
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}

	
	roata_dreapta.Draw(shader);



}

void renderRoataStanga(gps::Shader shader, bool shadow) {

	shader.useShaderProgram();

	modelRoataStanga = glm::mat4(1.0f);
	modelRoataStanga = glm::translate(modelRoataStanga, glm::vec3(-0.95415f, 0.016949f, roataStangaZ));
	if (mersFata) {
		modelRoataStanga = glm::rotate(modelRoataStanga, -rotireRoata, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	else {
		modelRoataStanga = glm::rotate(modelRoataStanga, rotireRoata, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	
	modelRoataStanga = glm::translate(modelRoataStanga, glm::vec3(0.95415f, -0.016949f, -roataStangaZ));

	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelRoataStanga));

	modelRoataStanga = glm::translate(modelRoataStanga, glm::vec3(-0.95415f, 0.016949f, roataStangaZ));

	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelRoataStanga));

	if (shadow == false) {
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}

	
	roata_stanga.Draw(shader);



}


void renderElice(gps::Shader shader, bool shadow) {
	// select active shader program
	shader.useShaderProgram();

	
	modelElice = glm::mat4(1.0f);
	
	
    modelElice = glm::translate(modelElice, glm::vec3(5.7481f, 0.24276f, -9.0206f));
	
	modelElice = glm::rotate(modelElice, delta, glm::vec3(0, 0, 1));
	
	
	
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelElice));

	

	
	if (shadow == false) {
        glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}
	
	
	elice.Draw(shader);
	




}


int fulger = 0;
void renderScena(gps::Shader shader, bool shadow) {
	// select active shader program
	shader.useShaderProgram();

	
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	
	if (shadow == false) {
		glUniformMatrix3fv(glGetUniformLocation(shader.shaderProgram, "normalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}
	
	
	scena.Draw(shader);
	

}



void renderLampa(gps::Shader shader, bool shadow) {
	// select active shader program
	shader.useShaderProgram();

	
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	
	if (shadow == false) {
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}

	
	lampi.Draw(shader);




}


void renderBarn(gps::Shader shader, bool shadow) {
	// select active shader program
	shader.useShaderProgram();

	
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	
	if (shadow == false) {
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}

	
	barn.Draw(shader);


}


void renderStoneBarn(gps::Shader shader, bool shadow) {
	// select active shader program
	shader.useShaderProgram();

	
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	
	if (shadow == false) {
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}

	
	stoneBarn.Draw(shader);


}






float yPloaie = 3.0f;
void renderPloaie(gps::Shader shader) {
	// select active shader program
	shader.useShaderProgram();

	for (int i = 0; i < 1000; i++) {
		glm::vec3 picatura = glm::vec3(picaturiPloaie.at(i).x, yPloaie, picaturiPloaie.at(i).z);

		yPloaie -= 0.01f;
		if (yPloaie < 0.0f) {
			yPloaie = 3.0f;
		}
		

		

		glm::mat4 modelPloaie = glm::mat4(1.0f);
		modelPloaie = glm::translate(modelPloaie, picatura);

		
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPloaie));

		
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

		
		ploaie.Draw(shader);

	}
}






glm::mat4 computeLightSpaceTrMatrix() {



	glm::mat4 lightView = glm::lookAt(lightDir, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	const GLfloat near_plane = 0.1f, far_plane = 5.0f;
	glm::mat4 lightProjection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, near_plane, far_plane);
	glm::mat4 lightSpaceTrMatrix = lightProjection * lightView;

	return lightSpaceTrMatrix;
}









void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	depthMapShader.useShaderProgram();
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "lightSpaceTrMatrix"),
		1,
		GL_FALSE,
		glm::value_ptr(computeLightSpaceTrMatrix()));
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	renderElice(depthMapShader, true);
	renderScena(depthMapShader, true);
	
	renderLampa(depthMapShader, true);
	renderCar(depthMapShader, true);
	renderRoataDreapta(depthMapShader, true);
	renderRoataStanga(depthMapShader, true);
	renderBarn(depthMapShader, true);
	renderStoneBarn(depthMapShader, true);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);



	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	myBasicShader.useShaderProgram();

	glUniformMatrix4fv(glGetUniformLocation(myBasicShader.shaderProgram, "lightSpaceTrMatrix"),
		1,
		GL_FALSE,
		glm::value_ptr(computeLightSpaceTrMatrix()));

	view = myCamera.getViewMatrix();
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glUniform1i(glGetUniformLocation(myBasicShader.shaderProgram, "shadowMap"), 3);

	glViewport(0, 0, (float)myWindow.getWindowDimensions().width, (float)myWindow.getWindowDimensions().height);
	

	myBasicShader.useShaderProgram();
	glUniform1i(glGetUniformLocation(myBasicShader.shaderProgram, "pornirePloaie"), pornirePloaie);
	renderElice(myBasicShader,false);
	renderScena(myBasicShader,false);
	
	renderLampa(myBasicShader,false);
	renderCar(myBasicShader, false);
	renderRoataDreapta(myBasicShader, false);
	renderRoataStanga(myBasicShader, false);
	renderBarn(myBasicShader, false);
	renderStoneBarn(myBasicShader, false);
	

	if (pornirePloaie == 1) {
		renderPloaie(myBasicShader);
		renderSkyCloud(myBasicShader);
		fulger++;
		myBasicShader.useShaderProgram();
		glUniform1i(glGetUniformLocation(myBasicShader.shaderProgram, "fulger"), fulger);
		
		if (fulger == 1000) {
			fulger = 0;
		}
		
		
		
	}
	else {
		renderSky(myBasicShader);
	}
	
}

void cleanup() {
	glDeleteTextures(1, &depthMapTexture);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &shadowMapFBO);
    myWindow.Delete();
    //cleanup code for your own data
}




int main(int argc, const char * argv[]) {

    try {
        initOpenGLWindow();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

	//genereaza pozitii random pentru ploaie
	for (int i = 0; i < 1000; i++) {
		float x = (rand() % 1800) / 1000.0f;
		float z = (rand() % 2350) / 1000.0f;
		float signX = (rand() % 4 + 1);
		float signZ = (rand() % 4 + 1);
		if ((int)signX % 2 != 0) {
			x = -x;
		}
		if ((int)signZ % 2 != 0) {
			z = -z;
		}
		
		picaturiPloaie.push_back(glm::vec3(x, 3.0f, z));
	}

	

    initOpenGLState();
	initFBO();
	initModels();
	initShaders();
	initUniforms();
    setWindowCallbacks();
	

	glCheckError();
	// application loop
	while (!glfwWindowShouldClose(myWindow.getWindow())) {
        processMovement();
	    renderScene();

		glfwPollEvents();
		glfwSwapBuffers(myWindow.getWindow());

		glCheckError();
	}

	cleanup();

    return EXIT_SUCCESS;
}
