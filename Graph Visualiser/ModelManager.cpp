// provide model and camera modification logic
#include "stdafx.h" // include the standard library (modified)
#include "Graph Visualiser.h" // include the main file
#include "InputManager.h"

using namespace std; // make writing system functions easier
using namespace glm; // make writing the maths easier
using namespace core;

namespace ModelManager{

	int WINDOW_WIDTH;
	int WINDOW_HEIGHT;

	float MOVESPEED; // the movement speed of the observer

	mat4 Projection, // for scene projection to viewer
		 View, // location and nature of observer
		 Model; // for manipulating position/rotation

	vec3 eyePosition; // default eye position (view)
	vec3 targetPosition; // default target (view)
	vec3 upDirection; // default sky direction (view)

	vec3 autoViewCentre; // centre of the model for the autoview feature

	vec3 moveModel; // for moving vertices's (model)
	float rotANGLE; // rotation (model)
	float rotation; // initial rotation
	vec3 rotCENTER; // center of rotation

	float FOV; // field of view (projection)
	float AR; // aspect ratio (projection)
	float NEARclip; // closest object to draw distance (projection)
	float FARclip; // furthest object to draw distance (projection)

	float horizontalAngle; // angle variables for camera rotation
	float verticalAngle;
	float rotationSpeed; // the maximum speed 

	void setModelVariables(){
		WINDOW_WIDTH = 1280;
		WINDOW_HEIGHT = 720;
		MOVESPEED = 0.01f;

		eyePosition = vec3(0, 0, 15);
		targetPosition = vec3(0, 0, 14);
		upDirection = vec3(0, 1, 0);

		autoViewCentre = vec3(0,0,-7.5);

		moveModel = vec3(0, 0, 0);
		rotANGLE = 0.02f;
		rotation = 0;
		rotCENTER = targetPosition - eyePosition;
		
		FOV = 45.0f;
		AR = 16 / 9;
		NEARclip = 0.0f;
		FARclip = 100.0f;

		horizontalAngle = 3.14f;
		verticalAngle = 0.0f;
		rotationSpeed = 0.000001f;
		core::deltaTime = 1;
	}

	void updateViewer(){
		// called when observer changes position or orientation
		View = lookAt(eyePosition, targetPosition, upDirection);
	}
	void mouseRotate(){

		horizontalAngle += rotationSpeed * core::deltaTime * float(WINDOW_WIDTH / 2 - InputManager::mouseX);
		verticalAngle += rotationSpeed * core::deltaTime * float(WINDOW_HEIGHT / 2 - InputManager::mouseY);

		// get the direction to look at using the angles above
		vec3 direction = vec3(cos(verticalAngle) * sin(horizontalAngle), sin(verticalAngle), cos(verticalAngle) * cos(horizontalAngle));
		// get the (rightmost) perpendicular to the look direction
		vec3 rightDirection = normalize(vec3(sin(horizontalAngle - 3.14f / 2.0f), 0, cos(horizontalAngle - 3.14f / 2.0f)));

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
		WINDOW_WIDTH = x; // get the width and...
		WINDOW_HEIGHT = y; // height of the new window
		//AR = (float)WINDOW_WIDTH / WINDOW_HEIGHT; // uncomment for dynamic AR (not particularly useful)
		Projection = perspective(FOV, AR, NEARclip, FARclip); // update perspective
		glViewport(0, 0, x, y);
	}
	vec3 generateVec3(vec2 xRange, vec2 yRange, vec2 zRange){

		vec3 generatedVec3; // the vec3 to be returned
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


		generatedVec3 = vec3(xValue, yValue, zValue);
		return generatedVec3;
	}	
	void initialiseModel(){ // sets data and buffers

		// generate a sea of points...
		float points[150]; // creates  place to hold 150 floats (50 coordinates)
		for (int n = 0; n < 150; n++){
			vec3 newPoint = generateVec3(vec2(-10, 10), vec2(-5, 5), vec2(-15, 0));
			points[n] = newPoint.x;
			points[n + 1] = newPoint.y;
			points[n + 2] = newPoint.z;

			n += 2;
		}


		//float testData[] = { 0.1f, 0.1f, 0.0f, 0.1f, 0.8f, 0.0f, 0.7f, 0.45f, 0.0f, -0.1f, -0.1f, 0.0f, -0.1f, -0.8f, 0.0f, -0.7f, -0.45f, 0.0f }; // two triangles
		//float thirdTri[] = { -0.1f, 0.1f, 0.0f, -0.1f, 0.8f, 0.0f, -0.7f, 0.45f, 0.0f };

		//float allTriangles[27]; // an array to hold all triangles

		//// add all the triangles into one array
		//for (int i = 0; i < 27; i++){
		//	if (i < 18){
		//		allTriangles[i] = testData[i];
		//	}
		//	if (i >= 18){
		//		allTriangles[i] = thirdTri[i - 18];
		//	}
		//}

		//Vertex buffer object defines where the vertices will be stored (GPU)
		int sizeOfAll = 150 * sizeof(float);
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeOfAll, points, GL_STATIC_DRAW);


		// Vertex attribute object defines where the attributes for each vertex are stored and
		//how they should be interpreted by the GPU 
		glGenBuffers(1, &vao); // generate a unique value for the attrib
		glBindVertexArray(vao); // binds the vertex attribute array
		glEnableVertexAttribArray(0); // ?
		glBindBuffer(GL_ARRAY_BUFFER, vbo); // shows that these attributes apply to the 'vbo' buffer
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL); // points to the starting location of the attribute storage
	}	
	void initialiseMatrices(){ // set Projection, Model & View

		//Projection = ortho(-4.0f / 3.0f, 4.0f / 3.0f, -1.0f, 1.0f, 0.0f, 1000.0f);
		//     orthographic    left         right      top   bot   near  far

		// only needs setting here and on window dimension changes
		Projection = perspective(FOV, AR, NEARclip, FARclip); // set default projection

		vec3 spawnPoint = vec3(0, 0, -1); // spawn position
		Model = translate(Model, spawnPoint); // setup models

		View = lookAt(eyePosition, targetPosition, upDirection); // set up default camera
	}
	void moveObserver(vec3 direction){
		// pretty standard - move the eye and the target together
		eyePosition += direction * MOVESPEED;
		targetPosition += direction * MOVESPEED;

		updateViewer();
	}
}