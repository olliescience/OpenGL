// Graph Visualiser.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "InputManager.h"
#include "ModelManager.h"
#include "ShaderManager.h"
#include "AntTweakBar.h"

#include "DataManager.h"

// TODO: 
// 1. get vbo and vao working & understood                                | 90%
// 2. update the display callback to accommodate changes                  | 90%
// 3. create shaders and get them working                                 | 100%
// 4. add keyboard controls and camera manipulation                       | 100%
// 5. add debugging text overlay (perhaps before (4)) <-raster text       | 100%
// 6. splice the source into separate files for consistency               | 100%

void createTweakBar(){
	// create tweak bar for practice
	TwBar *testBar; // create a place to hold a tweak bar 
	// create the bar, called 'Settings'	
	testBar = TwNewBar("Settings"); // set the variables relating to the bar
	TwDefine(" Settings size='300 500' position='20 20' iconifiable=false movable=false resizable=false fontsize=2 fontstyle=fixed valueswidth=100  ");
	// add a read-write variable to the bar with a set of relative definitions
	TwAddVarRW(testBar, "move speed: ", TW_TYPE_INT32, &ModelManager::moveSetValue, "min=1 max=100 step=1 ");
	TwAddVarRW(testBar, "rotation speed: ", TW_TYPE_INT32, &ModelManager::rotateSetValue, "min=1 max=100 step=1");

	// add color options
	TwAddVarRW(testBar, "pointRed", TW_TYPE_FLOAT, &ShaderManager::pointRed, "min=0 max=1 step=0.1");
	TwAddVarRW(testBar, "pointGreen", TW_TYPE_FLOAT, &ShaderManager::pointGreen, "min=0 max=1 step=0.1");
	TwAddVarRW(testBar, "pointBlue", TW_TYPE_FLOAT, &ShaderManager::pointBlue, "min=0 max=1 step=0.1");
	TwAddVarRW(testBar, "lineRed", TW_TYPE_FLOAT, &ShaderManager::lineRed, "min=0 max=1 step=0.1");
	TwAddVarRW(testBar, "lineGreen", TW_TYPE_FLOAT, &ShaderManager::lineGreen, "min=0 max=1 step=0.1");
	TwAddVarRW(testBar, "lineBlue", TW_TYPE_FLOAT, &ShaderManager::lineBlue, "min=0 max=1 step=0.1");
	
	TwDefine(" TW_HELP visible=false "); // remove the default help bar given by ATB
}

void init(){
		// initialization logic here
		std::cout << "Program started..." << std::endl;
		
		TwInit(TW_OPENGL, NULL); // initialize the ant tweak bar 
		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE); // allow shader to set point size
		glEnable(GL_BLEND);
		glEnable(GL_POINT_SMOOTH); // makes the points round (if used)

		// call relevant initializations in order
		ModelManager::setModelVariables(); // set all the variables to make model modification work
		ModelManager::initialiseModel();
		ModelManager::initialiseMatrices();
		ShaderManager::initialiseShaders();
		InputManager::resetInputs();
		createTweakBar();
	}

void drawText(const char *text, int length, int x, int y){ // code from russian youtuber
		glMatrixMode(GL_PROJECTION);
		double *matrix = new double[16];
		glGetDoublev(GL_PROJECTION_MATRIX, matrix);
		glLoadIdentity();
		glOrtho(0, ModelManager::WINDOW_WIDTH, 0, ModelManager::WINDOW_HEIGHT, -1, 1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glPushMatrix();
		glLoadIdentity();
		glRasterPos2i(x, y);
		for (int i = 0; i < length; i++){
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)text[i]);
		}
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixd(matrix);
		glMatrixMode(GL_MODELVIEW);

		delete matrix;
	}
void drawDebugText(){ // draws useful information to the screen
		int textHeight = 20;
		// mouse information
		std::string mouse = "mouse position: ";
		std::string mX = std::to_string(InputManager::mouseX);
		std::string mY = std::to_string(InputManager::mouseY);
		std::string mouseText = mouse + "(" + mX + "," + mY + ")";
		drawText(mouseText.data(), mouseText.size(), 20, ModelManager::WINDOW_HEIGHT - textHeight);

		// viewpoint information
		std::string view = "view direction: ";
		std::string vX = std::to_string(ModelManager::targetPosition.x - ModelManager::eyePosition.x);
		std::string vY = std::to_string(ModelManager::targetPosition.y - ModelManager::eyePosition.y);
		std::string vZ = std::to_string(ModelManager::targetPosition.z - ModelManager::eyePosition.z);
		// trim the text to 2 d.p. (check if first char is '-' and erase letters accordingly
		if (vX[0] != '-') vX.erase(4); // trim the X
		else vX.erase(5);
		if (vY[0] != '-') vY.erase(4); // trim the Y
		else vY.erase(5);
		if (vZ[0] != '-') vZ.erase(4); // trim the Z
		else vZ.erase(5);
		std::string viewText = view + "(" + vX + "," + vY + "," + vZ + ")";
		drawText(viewText.data(), viewText.size(), 20, ModelManager::WINDOW_HEIGHT - 2 * textHeight);

		//keyboard status
		std::string keyboardkeys = "pressed keys: ";
		std::string pressedkeys = "";
		for (int i = 0; i < 255; i++){
			if (InputManager::keyState[i] == 0){
				pressedkeys += std::to_string(i) + " ";
			}
		}
		if (pressedkeys.empty()){
			pressedkeys = "none";
		}
		std::string keyboardText = keyboardkeys + pressedkeys;
		drawText(keyboardText.data(), keyboardText.size(), 20, ModelManager::WINDOW_HEIGHT - 3 * textHeight);

		// mouse status
		std::string mousekeys = "pressed buttons: ";
		std::string mousebuttons = "";
		for (int i = 0; i < 5; i++){
			if (InputManager::mouseState[i] == 0){
				mousebuttons += std::to_string(i) + " ";
			}
		}
		if (mousebuttons.empty()){
			mousebuttons = "none";
		}
		std::string mouseKeys = mousekeys + mousebuttons;
		drawText(mouseKeys.data(), mouseKeys.size(), 20, ModelManager::WINDOW_HEIGHT - 4 * textHeight);

		// 

	}

int lastTime = 0;
void display(){
		// all display related code in here
		glClearColor(0.0, 0.0, 0.1, 1.0); // the color to clear to (dark navy)
		glClear(GL_COLOR_BUFFER_BIT); // apply the clear with the given color

		InputManager::updateControls(); // update all inputs 

		if (InputManager::showDebugText)
			drawDebugText();

		if (InputManager::showTweakBar){
			ModelManager::updateSettings();
			ShaderManager::updateShader(ShaderManager::pointShader);
			ShaderManager::updateShader(ShaderManager::lineShader);
			TwDraw(); // not sure how this functions, tweak bars are technically out of scope?
		}

		int currentTime = glutGet(GLUT_ELAPSED_TIME); // timing system making usage consistent on different processors
		ShaderManager::deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		//cout << "screen cleared" << endl;
		glUseProgram(ShaderManager::pointShader); // using a shader program from init()...
		//cout << "using shader program" << endl;
		
		glBindVertexArray(ShaderManager::vao);
		ShaderManager::updateShader(ShaderManager::pointShader);
		glDrawArrays(GL_POINTS, 0, 75); // draw the points in the currently bound vao with current shader
		
		glUseProgram(ShaderManager::lineShader); // using a shader program from init()...
		//cout << "using shader program" << endl;

		ShaderManager::updateShader(ShaderManager::lineShader);

		glDrawArrays(GL_LINES, 0, 75); // draw lines using points starting at 0, 25 lines
		glDrawArrays(GL_LINES, 1, 74); // draw some more lines xD

		//glFlush(); // applies given commands to (single) buffer
		glutSwapBuffers();
	}

// version 0.4.0 (experimental)
int main(int argc, char *argv[])
	{
		DataManager::LoadData();

		glutInit(&argc, argv); // initialize the utility toolkit

		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // display will be with a single buffer using RGB	
		glutInitWindowSize(ModelManager::WINDOW_WIDTH, ModelManager::WINDOW_HEIGHT); // 720p window size
		glutInitWindowPosition(0, 0); // top left	

		glutCreateWindow("OpenGL Incremental Development v0.4.0"); // name the window
		glewInit(); // initialize the extension wrangler

		glutDisplayFunc(display); // define the callback function for the display

		glutMouseFunc(InputManager::mouse);
		glutKeyboardFunc(InputManager::keyboard);
		glutKeyboardUpFunc(InputManager::keyboardUp);
		glutMotionFunc(InputManager::mousemove);
		glutPassiveMotionFunc(InputManager::mousepassive);

		glutIdleFunc(display); // redraws the display whenever possible
		glutReshapeFunc(ModelManager::reshape);
		init(); // custom initialization code
		glutMainLoop(); // the main look of the program

		return 0;
	}