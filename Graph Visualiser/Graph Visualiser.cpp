// Graph Visualiser.cpp : Defines the entry point for the console application.

#include "stdafx.h"

using namespace std;
using namespace glm;

// Global Variables
GLuint vao = 0;
GLuint vbo;
GLuint shaderProgram;


vec3 generateVec3(vec2 xRange, vec2 yRange, vec2 zRange){

	vec3 generatedVec3; // the vec3 to be returned
	// difficult to think around so setting out intuitively
	float xMin, xMax, xValue, yMin, yMax, yValue, zMin, zMax, zValue;
	// get the min/max for each dimension
	xMin = xRange.x;	xMax = xRange.y;
	yMin = yRange.x;	yMax = yRange.y;
	zMin = zRange.x;	zMax = zRange.y;

	float randFloat = (float)rand() / (RAND_MAX + 1); // generate value betweeon 0.0 and 1.0
	xValue = (randFloat * (xMax - xMin)) + xMin; // multiply and add to get within range
	randFloat = (float)rand() / (RAND_MAX + 1); // get new value
	yValue = (randFloat * (yMax - yMin)) + yMin; // continue...
	randFloat = (float)rand() / (RAND_MAX + 1);
	zValue = (randFloat * (zMax - zMin)) + zMin;

	/* debugging console output
	cout << "Generated vector:" << endl;
	cout << "xValue: " << xValue << endl;
	cout << "yValue: " << yValue << endl;
	cout << "zValue: " << zValue << endl;
	*/

	generatedVec3 = vec3(xValue, yValue, zValue);
	return generatedVec3;
}


class Node{

public:
	vec3 vPosition; // make the position of each node public for convenience
	
	Node(){} // default constructor
	
	void setPosition(vec3 inPosition){
		vPosition = inPosition;
	}
};


void init(){
	// initialisation logic here
	cout << "Init has been called" << endl;

	cout << "Generating nodes..." << endl;

	Node ListOfNodes [50]; // array of 50 nodes
	vec2 xRange = vec2(0, 100);
	vec2 yRange = vec2(0, 100);
	vec2 zRange = vec2(0, 100);

	for (int i = 0; i < 50; i++){ // generate 50 positions
		ListOfNodes[i].setPosition(generateVec3(xRange, yRange, zRange));
		cout << "node generated" << endl;
	}
	cout << "all nodes generated." << endl;

	// so now we have all the data, we need to use it...

	
}

void display(){
	// all display related code in here
	// like glDrawArrays(...) etc.
	cout << "Display called" << endl;
	glClearColor(0.0, 0.0, 0.0, 1.0); // the color to clear to (black)
	glClear(GL_COLOR_BUFFER_BIT);

	glFlush(); // applies given commands to buffer
}

// version 0.1.1 (experimental)
int main(int argc, char *argv[])
{
	glutInit(&argc, argv); // initialise the utility toolkit
	cout << "glutInit Executed..." << endl;

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // display will be a single window using RGB
	cout << "Display Mode set to SINGLE/RGB." << endl;

	glutInitWindowSize(1280, 720); // 720p window size
	cout << "Window size set to 1280x720." << endl;

	glutInitWindowPosition(0, 0); // top left
	cout << "Window Position set to (0,0)." << endl;

	glutCreateWindow("Window Title"); // name the window
	cout << "Window title set." << endl;

	glewInit(); // initialise the extension wrangler
	cout << "glewInit Executed..." << endl;
	
	glutDisplayFunc(display); // define the callback function for the display
	cout << "Display callback run." << endl;

	init(); // custom initialisation code
	cout << "init() Executed..." << endl;

	cout << "GLUT main loop starting..." << endl;
	glutMainLoop(); // the main look of the program
	cout << "GLUT main loop stopped." << endl;
	
	return 0;
}

