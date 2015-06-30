// Graph Visualiser.cpp : Defines the entry point for the console application.

#include "stdafx.h"

using namespace std;
using namespace glm;

// Global Variables
GLuint vao;
GLuint vbo;
GLuint shaderProgram;

// TODO: 
// 1. get vbo and vao working & understood                                | 75%
// 2. update the display callback to accommodate changes                  | 0%
// 3. create shaders and get them working                                 | 0%
// 4. add keyboard controls and camera manipuation                        | 10%
// 5. add debugging text overlay (perhaps before (4)) <-raster text       | 0%


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

	// define a buffer
	
	//Vertex buffer object defines where the vertices will be stored (GPU)
	int sizeOfData = 50*3*sizeof(float); // random value for testing
	glGenBuffers(0, &vbo); // generate a unique value for the buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // binds the value to a type GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, sizeOfData, ListOfNodes, GL_STATIC_DRAW); // define type of data and size


	// Vertex attribute object defines where the attributes for each vertex are stored and
	// how they should be interpreted by the GPU
	//glGenBuffers(1, &vao); // generate a unique value for the attrib
	//glBindVertexArray(vao); // binds the vertex attribute array
	//glEnableVertexAttribArray(0); // ?
	//glBindBuffer(GL_ARRAY_BUFFER, vbo); // ?
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL); // points to the starting location of the attribute storage

	// store data on the buffer

	// decide how to display data in the buffer

	// create shaders

	
}

void display(){
	// all display related code in here
	// like glDrawArrays(...) etc.
	cout << "Display called" << endl;
	glClearColor(0.0, 0.0, 0.0, 1.0); // the color to clear to (black)
	glClear(GL_COLOR_BUFFER_BIT);

	glFlush(); // applies given commands to buffer
}

void mouse(int button, int state, int x, int y){ // mouse callback event
	// button can be 0 (left) or 2 (right), 1 should be the wheel but isn't implemented as standard
	// state is binary
	// x is the x coordinate of the mouse
	// y is the y coordinate of the cursor

	cout << "button: " << button << ", state: " << state << endl;

	// check for left button press
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		cout << "mouse position: " << x << "," << y << endl;
	}

	// check for scroll wheel activity, live check... hello? anyone there?
	if (button == 1 && state == GLUT_DOWN){
		// glitch - only runs if the wheel is clicked in :'(
		cout << "my word, we have a scroller..." << endl;
	}

	// come on scroll wheel, give me something I can work with :P
	if (button == 3 && state == GLUT_DOWN){
		// edit: zero signs of life, TOD: 14:27 30/06/2015
		cout << "there's a third button?" << endl;
	}

	// it's like trying to find a needle in a swamp...
	if (button == 4){
		// no signal
		cout << "le 4th button calls." << endl;
	}

}

void keyboard(unsigned char key, int x, int y){ // keyboard callback event
	// key is the character the key represents
	// x is the x coordinate of the mouse at the point the key is pressed
	// y is the y coordinate of the mouse at the point the key is pressed
	cout << "key pressed: " << key << " x = " << x << " y = " << y << endl;
	

}

void mousemove(int x, int y){
	cout << ".";
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

	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(mousemove);

	init(); // custom initialisation code
	cout << "init() Executed..." << endl;

	cout << "GLUT main loop starting..." << endl;
	glutMainLoop(); // the main look of the program
	cout << "GLUT main loop stopped." << endl;
	
	return 0;
}


