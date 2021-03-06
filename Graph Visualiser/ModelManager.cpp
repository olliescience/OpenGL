// provide model and camera modification logic
#include "stdafx.h" // include the standard library (modified)
#include "main.h"
#include "InputManager.h"
#include "ShaderManager.h"
#include "AntTweakBar.h"

namespace ModelManager{

	int WINDOW_WIDTH;
	int WINDOW_HEIGHT;

	float MOVESPEED; // the movement speed of the observer

	glm::mat4 Projection, // for scene projection to viewer
		 View, // location and nature of observer
		 Model; // for manipulating position/rotation

	glm::vec3 eyePosition; // default eye position (view)
	glm::vec3 targetPosition; // default target (view)
	glm::vec3 upDirection; // default sky direction (view)

	glm::vec3 moveModel; // for moving vertices's (model)
	float rotANGLE; // rotation (model)
	float rotation; // initial rotation
	glm::vec3 rotCENTER; // center of rotation

	float FOV; // field of view (projection)
	float AR; // aspect ratio (projection)
	float NEARclip; // closest object to draw distance (projection)
	float FARclip; // furthest object to draw distance (projection)

	float horizontalAngle; // angle variables for camera rotation
	float verticalAngle;
	float rotationSpeed; // the maximum speed 

	int moveSetValue;
	int rotateSetValue;

	void setModelVariables(){
		WINDOW_WIDTH = 1280;
		WINDOW_HEIGHT = 720;
		TwWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); // set the initial height for the antTweakBar
		
		moveSetValue = 25;
		MOVESPEED = (float)moveSetValue / 5000;
		
		eyePosition = glm::vec3(0, 0, 15);
		targetPosition = glm::vec3(0, 0, 14);
		upDirection = glm::vec3(0, 1, 0);

		moveModel = glm::vec3(0, 0, 0);
		rotANGLE = 0.02f;
		rotation = 0;
		rotCENTER = targetPosition - eyePosition;
		
		FOV = 45.0f;
		AR = 16 / 9;
		NEARclip = 0.0f;
		FARclip = 100.0f;

		horizontalAngle = 3.14f;
		verticalAngle = 0.0f;
		rotateSetValue = 25;
		rotationSpeed = (float)rotateSetValue / 5000000;
		ShaderManager::deltaTime = 1;
	}

	void updateSettings(){
		MOVESPEED = (float)moveSetValue / 5000;
		rotationSpeed = (float)rotateSetValue / 5000000;
	}

	void updateViewer(){
		// called when observer changes position or orientation
		View = lookAt(eyePosition, targetPosition, upDirection);
	}
	
	void mouseRotate(){

		horizontalAngle += rotationSpeed * ShaderManager::deltaTime * float(WINDOW_WIDTH / 2 - InputManager::mouseX);
		verticalAngle += rotationSpeed * ShaderManager::deltaTime * float(WINDOW_HEIGHT / 2 - InputManager::mouseY);

		if (verticalAngle > 3.14 / 2)
			verticalAngle = (float)3.14 / 2; // stop looking up more than 90d
		if (verticalAngle < -3.14 / 2)
			verticalAngle = (float)-3.14 / 2; // stop looking down more than 90d

		// get the direction to look at using the angles above
		glm::vec3 direction = glm::vec3(cos(verticalAngle) * sin(horizontalAngle), sin(verticalAngle), cos(verticalAngle) * cos(horizontalAngle));
		// get the (rightmost) perpendicular to the look direction
		glm::vec3 rightDirection = normalize(glm::vec3(sin(horizontalAngle - 3.14f / 2.0f), 0, cos(horizontalAngle - 3.14f / 2.0f)));

		targetPosition = eyePosition + direction; // set the new target position for the View matrix
		upDirection = cross(rightDirection, direction); // recalibrate the 'up' direction for the View matrix
		


		updateViewer(); // applies changes to the View matrix

	}
	void updateGeometry(){ // change the scene

		// perform any matrix alterations here, geometry modifications should only be
		// calculated when viewpoint changes or dynamic effects are realized.
		Model = rotate(Model, rotation, rotCENTER); // update any rotation
		Model = translate(Model, moveModel); // update any translation

	}
	void reshape(int x, int y){ // called when window is modified
		TwWindowSize(x, y); // tell ant tweak bar the new dimensions of the window
		WINDOW_WIDTH = x; // get the width and...
		WINDOW_HEIGHT = y; // height of the new window
		//AR = (float)WINDOW_WIDTH / WINDOW_HEIGHT; // uncomment for dynamic AR (not particularly useful)
		Projection = glm::perspective(FOV, AR, NEARclip, FARclip); // update perspective
		glViewport(0, 0, x, y);
	}
	
	glm::vec3 generateVec3(glm::vec2 xRange, glm::vec2 yRange, glm::vec2 zRange){

		glm::vec3 generatedVec3; // the vec3 to be returned
		// difficult to think around so setting out intuitively
		float xMin, xMax, xValue, yMin, yMax, yValue, zMin, zMax, zValue;
		// get the min/max for each dimension
		xMin = xRange.x;	xMax = xRange.y;
		yMin = yRange.x;	yMax = yRange.y;
		zMin = zRange.x;	zMax = zRange.y;

		float randFloat = (float)rand() / (RAND_MAX + 1); // generate value between 0.0 and 1.0
		xValue = (randFloat * (xMax - xMin)) + xMin; // multiply and add to get within range
		randFloat = (float)rand() / (RAND_MAX + 1); // get new value
		yValue = (randFloat * (yMax - yMin)) + yMin; // continue...
		randFloat = (float)rand() / (RAND_MAX + 1);
		zValue = (randFloat * (zMax - zMin)) + zMin;

		/* debugging console output*/
		/*cout << "Generated vector:" << endl;
		cout << "xValue: " << xValue << endl;
		cout << "yValue: " << yValue << endl;
		cout << "zValue: " << zValue << endl;*/
		
		generatedVec3 = glm::vec3(xValue, yValue, zValue);
		return generatedVec3;
	}	
	void initialiseModel(){ // sets data and buffers

		// generate a sea of points...
		const int dataLength = 225;
		float points[dataLength]; // creates  place to hold 150 floats (50 coordinates)
		for (int n = 0; n < dataLength; n++){
			glm::vec3 newPoint = generateVec3(glm::vec2(-10, 10), glm::vec2(-5, 5), glm::vec2(-15, 0));
			points[n] = newPoint.x;
			points[n + 1] = newPoint.y;
			points[n + 2] = newPoint.z;
			n += 2;
		}
		
		//Vertex buffer object defines where the vertices will be stored (GPU)
		int sizeOfAll = dataLength * sizeof(float);
		glGenBuffers(1, &ShaderManager::vbo);
		glBindBuffer(GL_ARRAY_BUFFER, ShaderManager::vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeOfAll, points, GL_STATIC_DRAW);
		
		// Vertex attribute object defines where the attributes for each vertex are stored and
		//how they should be interpreted by the GPU 
		glGenBuffers(1, &ShaderManager::vao); // generate a unique value for the attrib
		glBindVertexArray(ShaderManager::vao); // binds the vertex attribute array
		glEnableVertexAttribArray(0); // ?
		glBindBuffer(GL_ARRAY_BUFFER, ShaderManager::vbo); // shows that these attributes apply to the 'vbo' buffer
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL); // points to the starting location of the attribute storage
	}	
	void initialiseMatrices(){ // set Projection, Model & View

		//Projection = ortho(-4.0f / 3.0f, 4.0f / 3.0f, -1.0f, 1.0f, 0.0f, 1000.0f);
		//     orthographic    left         right      top   bot   near  far

		// only needs setting here and on window dimension changes
		Projection = glm::perspective(FOV, AR, NEARclip, FARclip); // set default projection

		//vec3 spawnPoint = vec3(0, 0, 0); // spawn position
		//Model = translate(Model, spawnPoint); // setup models

		View = lookAt(eyePosition, targetPosition, upDirection); // set up default camera
	}
	
	void moveObserver(glm::vec3 direction){
		// pretty standard - move the eye and the target together
		eyePosition += direction * MOVESPEED * ShaderManager::deltaTime;
		targetPosition += direction * MOVESPEED * ShaderManager::deltaTime;

		updateViewer();
	}

	glm::vec3 oldeyePosition, lookDirection; // for the following method
	void transitionObserver(glm::vec3 destination,  float percentage){
		lookDirection = targetPosition - eyePosition; // get the direction the camera is looking
		oldeyePosition = eyePosition; // set old position
		eyePosition = oldeyePosition + (destination - oldeyePosition) * percentage * ShaderManager::deltaTime; // move smoothly
		targetPosition = eyePosition + lookDirection; // retain looking direction
		updateViewer();
	}
}