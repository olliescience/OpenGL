// Class giving more sophisticated input controls
#include "stdafx.h"

#define PRESSED 1; // better semantics for button states
#define RELEASED 0; // ""

class InputManager{
public:
	int mouseX, mouseY; // holds the coordinates of the mouse
	bool inDrag = false; // holds whether or not the mouse is being dragged
	unsigned char keyState[255]; // array holding the states of all keys
	unsigned char mouseState[5]; // holds the state of the mouse

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
		if (state == 1){
			switch (button) // take the input to the function
			{
			case GLUT_LEFT_BUTTON: // compare it with the predefined GLUT values (0-5)
				mouseState[0] = PRESSED; // assign the relevant element with the state
				break;
			case GLUT_MIDDLE_BUTTON:
				mouseState[1] = PRESSED;
				break;
			case GLUT_RIGHT_BUTTON:
				mouseState[2] = PRESSED;
				break;
			case 3: // scroll wheel up
				mouseState[3] = PRESSED;
				break;
			case 4: // scroll wheel down
				mouseState[4] = PRESSED;
				break;
			}
		}else{
			switch (button) // take the input to the function
			{
			case GLUT_LEFT_BUTTON: // compare it with the predefined GLUT values (0-5)
				mouseState[0] = RELEASED; // assign the relevant element with the state
				break;
			case GLUT_MIDDLE_BUTTON:
				mouseState[1] = RELEASED;
				break;
			case GLUT_RIGHT_BUTTON:
				mouseState[2] = RELEASED;
				break;
			case 3: // scroll wheel up
				mouseState[3] = RELEASED;
				break;
			case 4: // scroll wheel down
				mouseState[4] = RELEASED;
				break;
			}
		}
	}

	// callback for mouse click and drag (any button)
	void mousemove(int x, int y){
		inDrag = true;

	}

	// callback for any time the mouse if moved
	void mousepassive(int x, int y){
		mouseX = x; // set the coordinates of the mouse any time it is moved
		mouseY = y; // for maximum responsiveness (might be a little inefficient)
	}	
};