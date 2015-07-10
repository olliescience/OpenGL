// file giving more sophisticated input controls
#include "stdafx.h"
#include "Graph Visualiser.h"
#include "ModelManager.h"

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

	// callback function for key releases
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
	}

	// callback for mouse click and drag (any button)
	void mousemove(int x, int y){
		mouseX = x; // set the coordinates of the mouse any time it is moved
		mouseY = y; // for maximum responsiveness (might be a little inefficient)
		inDrag = true;
	}

	// callback for any time the mouse if moved
	void mousepassive(int x, int y){
		inDrag = false;
		mouseX = x; // set the coordinates of the mouse any time it is moved
		mouseY = y; // for maximum responsiveness (might be a little inefficient)
	}
	
	// sets all inputs to 'RELEASED'
	void resetInputs(){
		inDrag = false;
		for (int c = 0; c < 255; c++){
			keyState[c] = RELEASED;
		}
		for (int n = 0; n < 5; n++){
			mouseState[n] = RELEASED;
		}
	}

	bool autoView = false; // true if automatic view is enabled
	bool isFullscreen = true; // fullscreen by default
	void updateControls(){
		// MOUSE CHECKS
		if (mouseState[GLUT_LEFT_BUTTON] == PRESSED){			
			ModelManager::mouseRotate(); // rotate camera using mouse input
			autoView = false; // cancels automatic panning
		}

		// KEY CHECKS - if statements, ugly but most efficient given input structure
		if (keyState['w'] == PRESSED || mouseState[GLUT_SCROLL_UP] == PRESSED){ // move in the direction the observer is facing
			ModelManager::moveObserver(ModelManager::targetPosition - ModelManager::eyePosition);
		}
		if (keyState['s'] == PRESSED){ // move in the opposite direction to the observer
			ModelManager::moveObserver(-(ModelManager::targetPosition - ModelManager::eyePosition));
		}
		if (keyState['d'] == PRESSED){ // strafe to the right
			ModelManager::moveObserver(-cross(ModelManager::eyePosition - ModelManager::targetPosition, ModelManager::upDirection));
			autoView = false; // cancels automatic panning
		}
		if (keyState['a'] == PRESSED){ // strafe to the left
			ModelManager::moveObserver(cross(ModelManager::eyePosition - ModelManager::targetPosition, ModelManager::upDirection));
			autoView = false; // cancels automatic panning
		}
		if (keyState['t'] == PRESSED){ // vertical up pan
			ModelManager::moveObserver(ModelManager::upDirection);
		}
		if (keyState['g'] == PRESSED){ // vertical down pan
			ModelManager::moveObserver(-ModelManager::upDirection);
		}
		if (keyState[27] == PRESSED){ // exit on escape key
			exit(0);
		}
		if (keyState['f'] == PRESSED){ // fullscreen toggle (bugged)
			glutFullScreenToggle();
		}
		if (keyState['e'] == PRESSED){ // auto-view key
			autoView = true;
		}

		// BOOLEAN CHECKS -- NO KEY/MOUSE CHECKS PAST THIS LINE
		if (autoView){
			// mimic mouse click
			mouseY = ModelManager::WINDOW_HEIGHT / 2;
			mouseX = ModelManager::WINDOW_WIDTH / 2 - 40; // distance to the left from the centre (defines look rotation speed)
			ModelManager::mouseRotate();

			// strafe right
			float autoViewRotationSpeed = 0.1f;
			glm::vec3 rotationDirection = -cross(ModelManager::eyePosition - ModelManager::targetPosition, ModelManager::upDirection);
			ModelManager::moveObserver(autoViewRotationSpeed * rotationDirection);
			
		}
	}	
}