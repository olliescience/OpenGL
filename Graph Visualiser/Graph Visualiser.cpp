// Graph Visualiser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <GL\glew.h> // Extension Wrangler
#include <glut.h> // Utilities Toolkit

#include <glm\vec3.hpp> // 3D vector glm::vec3
#include <glm\vec4.hpp> // 4D vector glm::vec4
#include <glm\mat4x4.hpp> // 4x4 matrices glm::mat4
#include <glm\gtc\matrix_transform.hpp> // for transforming matrices

#include <string> // for string manipulation
#include <iostream> // for input/output

using namespace std;

void init(){
	// initialisation logic here
	cout << "Init has been called" << endl;
}

void display(){
	// all display related code in here
	// like glDrawArrays(...) etc.
	cout << "Display called" << endl;
	glClearColor(0.0, 0.0, 0.0, 1.0); // the color to clear to (black)
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush(); // applies given commands to buffer
}



// version 0.1 (experimental)
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

