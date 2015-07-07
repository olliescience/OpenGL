// file giving more sophisticated input controls
#include "stdafx.h"
#include "Graph Visualiser.h"

using namespace core; // use some function from the core

namespace InputManager{
	int mouseX, mouseY; // holds the coordinates of the mouse
	bool inDrag; // holds whether or not the mouse is being dragged
	unsigned char keyState[255]; // array holding the states of all keys
	unsigned char mouseState[5]; // holds the state of the mouse

	const int PRESSED = 0;
	const int RELEASED = 1;

	const int GLUT_SCROLL_UP = 3;
	const int GLUT_SCROLL_DOWN = 4;

	// callback function for keystrokes
	void keyboard(unsigned char key, int x, int y){
		keyState[key] = PRESSED; // set the state of the relevant key to 1 (see #define)
	}

	//callback function for key releases
	void keyboardUp(unsigned char key, int x, int y){
		keyState[key] = RELEASED; // opposes the keystroke function
	}

	// callback function for mouse clicks
	void mouse(int button, int state, int x, int y){

		switch (button) // take the input to the function
		{
		case GLUT_LEFT_BUTTON: // compare it with the predefined GLUT values (0-5)
			mouseState[GLUT_LEFT_BUTTON] = state; // assign the relevant element with the state
			break;
		case GLUT_MIDDLE_BUTTON:
			mouseState[GLUT_MIDDLE_BUTTON] = state;
			break;
		case GLUT_RIGHT_BUTTON:
			mouseState[GLUT_RIGHT_BUTTON] = state;
			break;
		case GLUT_SCROLL_UP: // scroll wheel up
			mouseState[GLUT_SCROLL_UP] = state;
			break;
		case GLUT_SCROLL_DOWN: // scroll wheel down
			mouseState[GLUT_SCROLL_DOWN] = state;
			break;
		}
		std::cout << "button: " << button << " state: " << state << std::endl;
	}

	// callback for mouse click and drag (any button)
	void mousemove(int x, int y){
		mouseX = x; // set the coordinates of the mouse any time it is moved
		mouseY = y; // for maximum responsiveness (might be a little inefficient)
		inDrag = true;
		if (mouseState[GLUT_LEFT_BUTTON] == PRESSED){
			computeMatricesFromInputs();
		}
	}

	// callback for any time the mouse if moved
	void mousepassive(int x, int y){
		inDrag = false;
		mouseX = x; // set the coordinates of the mouse any time it is moved
		mouseY = y; // for maximum responsiveness (might be a little inefficient)
	}
	
}