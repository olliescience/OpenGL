// file giving more sophisticated input controls
#include "stdafx.h"
#include "Graph Visualiser.h"
#include "ModelManager.h"

#include "glm/gtx/vector_angle.hpp"
#include "glm/vec3.hpp" // DEBUGGING
using namespace glm; // DEBUGGING

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
		std::cout << key << std::endl;
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
	bool isFullscreen = false;
	bool debugInfo = false;
	unsigned char oldkeyState[255]; // array holding the states of all keys
	unsigned char oldmouseState[5]; // holds the state of the mouse
	void updateControls(){

		if (mouseState[GLUT_LEFT_BUTTON] == PRESSED){
			ModelManager::mouseRotate();
			autoView = false; // cancels automatic panning
			std::cout << mouseX << "  " << mouseY << std::endl;
		}
		if (keyState['w'] == PRESSED || mouseState[GLUT_SCROLL_UP] == PRESSED){ // move in the direction the observer is facing
			ModelManager::moveObserver(ModelManager::targetPosition - ModelManager::eyePosition);
			autoView = false; // cancels automatic panning
		}
		if (keyState['s'] == PRESSED){ // move in the opposite direction to the observer
			ModelManager::moveObserver(-(ModelManager::targetPosition - ModelManager::eyePosition));
			autoView = false; // cancels automatic panning
		}
		if (keyState['d'] == PRESSED){ // strafe to the right
			ModelManager::moveObserver(-cross(ModelManager::eyePosition - ModelManager::targetPosition, ModelManager::upDirection));
			autoView = false; // cancels automatic panning
		}
		if (keyState['a'] == PRESSED){ // strafe to the left
			ModelManager::moveObserver(cross(ModelManager::eyePosition - ModelManager::targetPosition, ModelManager::upDirection));
			autoView = false; // cancels automatic panning
		}
		if (keyState['q'] == PRESSED){ // debug info key
			debugInfo = true;
		}
		if (keyState[27] == PRESSED){ // exit on escape key
			exit(0);
		}
		if (keyState['f'] == PRESSED){
			glutFullScreenToggle();
		}
		if (keyState[' '] == PRESSED){
			autoView = true;
		}

		if (debugInfo){
			std::cout << "3D Debug Info: " << std::endl;
			std::cout << "eyePosition: " << ModelManager::eyePosition.x << "x, " << ModelManager::eyePosition.y << "y, " << ModelManager::eyePosition.z << "z." << std::endl;
			std::cout << "targetPosition: " << ModelManager::targetPosition.x << "x, " << ModelManager::targetPosition.y << "y, " << ModelManager::targetPosition.z << "z." << std::endl;
			std::cout << "eye-target distance: " << distance(ModelManager::eyePosition, ModelManager::targetPosition) << std::endl;
			debugInfo = false;
		}
		if (autoView){
			// mimic mouse click
			mouseY = ModelManager::WINDOW_HEIGHT / 2;
			mouseX = ModelManager::WINDOW_WIDTH / 2 - 50; // distance to the left of the centre (defines look rotation speed)
			ModelManager::mouseRotate();

			// strafe right
			float autoViewRotationSpeed = 0.1f;
			vec3 rotationDirection = -cross(ModelManager::eyePosition - ModelManager::targetPosition, ModelManager::upDirection);
			ModelManager::moveObserver(autoViewRotationSpeed * rotationDirection);
			
		}
	}	
}