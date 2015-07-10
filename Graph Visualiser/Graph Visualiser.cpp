// Graph Visualiser.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "InputManager.h"
#include "ModelManager.h"

// TODO: 
// 1. get vbo and vao working & understood                                | 90%
// 2. update the display callback to accommodate changes                  | 50%
// 3. create shaders and get them working                                 | 90%
// 4. add keyboard controls and camera manipulation                       | 80%
// 5. add debugging text overlay (perhaps before (4)) <-raster text       | 0%
// 6. splice the source into separate files for consistency               | 80%

using namespace std; // make writing system functions easier
namespace core{
	// Global Variables
	GLuint vao, vbo, lineAttributeBuffer, lineVertexBuffer; // vertex buffer and attribute objects
	GLuint shaderProgram; // shader programs
	int deltaTime; // holds the time since last idle call
	int lastTime; // holds the time before the current idle call

	void initialiseShaders(){ // create shaders

		const char* vShader = // pass-through vertex shader
			"#version 400\n"
			"in vec3 vPosition;"
			"void main(){"
			"	gl_Position = vec4(vPosition, 1.0);" // x,y,z,depth
			"}";

		const char* fShader = // standard shader for coloring (set to yellow)
			"#version 400\n"
			"out vec4 fragColor;"
			"void main(){ "
			"	fragColor = vec4(1.0, 1.0, 0.0, 1.0);" // RGBA
			"}";

		const char* vShader3 = // pass-through 3D vertex shader
			"#version 400\n"
			"in vec3 vertexPosition;"
			"uniform mat4 projection_matrix;" // for perspective, aspect ratio and clipping
			"uniform mat4 view_matrix;" // for manipulating viewpoint
			"uniform mat4 model_matrix;" // for moving objects
			"void main(){"
			// apparently multiplying it all together works fine... xD
			"	gl_Position = projection_matrix * view_matrix * model_matrix * vec4(vertexPosition, 1);"
			"}";

		const char* fShader3 = // standard shader for coloring
			"#version 400\n"
			"out vec4 fragColor;"
			"void main(){"
			"	fragColor = vec4(0.5, 1.0, 0.5, 1.0);" // RGBA
			"}";

		// compile shaders
		GLuint vS = glCreateShader(GL_VERTEX_SHADER); // create a vertex shader identifier
		glShaderSource(vS, 1, &vShader, NULL); // get the source for the shader
		glCompileShader(vS); // compile the shader

		GLuint fS = glCreateShader(GL_FRAGMENT_SHADER); // create a fragment shader identifier
		glShaderSource(fS, 1, &fShader, NULL); // get the source
		glCompileShader(fS); // compile the shader

		// -- 3D SHADERS -- //
		GLuint vS3 = glCreateShader(GL_VERTEX_SHADER); // define the 3D vertex shader
		glShaderSource(vS3, 1, &vShader3, NULL); // match it to the source code
		glCompileShader(vS3); // compile the shader

		GLuint fS3 = glCreateShader(GL_FRAGMENT_SHADER); // define the fragment shader
		glShaderSource(fS3, 1, &fShader3, NULL); // match the shader identifier with the source
		glCompileShader(fS3); // compile the shader

		// create shader program
		shaderProgram = glCreateProgram(); // create an empty program
		glAttachShader(shaderProgram, fS3); // attach the fragment shader
		glAttachShader(shaderProgram, vS3); // attach the vertex shader
		glLinkProgram(shaderProgram); // link them together
	}
	void init(){
		// initialization logic here
		cout << "Program started..." << endl;
		// call relevant initializations in order
		ModelManager::setModelVariables(); // set all the variables to make model modification work
		ModelManager::initialiseModel();
		ModelManager::initialiseMatrices();
		initialiseShaders();
		InputManager::resetInputs();
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
		string mouse = "mouse position: ";
		string mX = std::to_string(InputManager::mouseX);
		string mY = std::to_string(InputManager::mouseY);
		string mouseText = mouse + "(" + mX + "," + mY + ")";
		drawText(mouseText.data(), mouseText.size(), 20, ModelManager::WINDOW_HEIGHT - textHeight);

		// viewpoint information
		string view = "view direction: ";
		string vX = std::to_string(ModelManager::targetPosition.x - ModelManager::eyePosition.x);
		string vY = std::to_string(ModelManager::targetPosition.y - ModelManager::eyePosition.y);
		string vZ = std::to_string(ModelManager::targetPosition.z - ModelManager::eyePosition.z);
		// trim the text to 2 d.p. (check if first char is '-' and erase letters accordingly
		if (vX[0] != '-') vX.erase(4); // trim the X
		else vX.erase(5);
		if (vY[0] != '-') vY.erase(4); // trim the Y
		else vY.erase(5);
		if (vZ[0] != '-') vZ.erase(4); // trim the Z
		else vZ.erase(5);
		string viewText = view + "(" + vX + "," + vY + "," + vZ + ")";
		drawText(viewText.data(), viewText.size(), 20, ModelManager::WINDOW_HEIGHT - 2 * textHeight);

		//keyboard status
		string keyboardkeys = "pressed keys: ";
		string pressedkeys = "";
		for (int i = 0; i < 255; i++){
			if (InputManager::keyState[i] == 0){
				pressedkeys += std::to_string(i) + " ";
			}
		}
		if (pressedkeys.empty()){
			pressedkeys = "none";
		}
		string keyboardText = keyboardkeys + pressedkeys;
		drawText(keyboardText.data(), keyboardText.size(), 20, ModelManager::WINDOW_HEIGHT - 3 * textHeight);

		// mouse status
		string mousekeys = "pressed buttons: ";
		string mousebuttons = "";
		for (int i = 0; i < 5; i++){
			if (InputManager::mouseState[i] == 0){
				mousebuttons += std::to_string(i) + " ";
			}
		}
		if (mousebuttons.empty()){
			mousebuttons = "none";
		}
		string mouseKeys = mousekeys + mousebuttons;
		drawText(mouseKeys.data(), mouseKeys.size(), 20, ModelManager::WINDOW_HEIGHT - 4 * textHeight);

		// 

	}

	void display(){
		// all display related code in here
		// like glDrawArrays(...) etc.
		glClearColor(0.0f, 0.0f, 0.1f, 1.0f); // the color to clear to (dark navy)
		glClear(GL_COLOR_BUFFER_BIT); // apply the clear with the given color

		InputManager::updateControls(); // update all inputs 

		drawDebugText();

		// timer code (bugged)
		deltaTime = 1;
		//int currentTime = glutGet(GLUT_ELAPSED_TIME);
		//deltaTime = currentTime - lastTime;
		//lastTime = currentTime;

		//cout << "screen cleared" << endl;
		glUseProgram(shaderProgram); // using a shader program from init()...
		//cout << "using shader program" << endl;

		// set all of the shader matrix variables (the 'uniform' ones)
		GLuint projLoc = glGetUniformLocation(shaderProgram, "projection_matrix"); // get the location of the shader variable
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(ModelManager::Projection)); // set it to the relevant matrix above

		GLuint viewLoc = glGetUniformLocation(shaderProgram, "view_matrix"); // same for the view matrix
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(ModelManager::View)); // set it here

		GLuint modelLoc = glGetUniformLocation(shaderProgram, "model_matrix"); // same for the model matrix
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(ModelManager::Model)); // set it here

		glBindVertexArray(vao);

		glPointSize(10.0f); // sets the diameter of the points
		glEnable(GL_POINT_SMOOTH); // makes the points round (if used)
		
		glDrawArrays(GL_POINTS, 0, 50); // draw the points in the currently bound vao with current shader
		
		glDrawArrays(GL_LINES, 0, 50); // draw lines using points starting at 0, 25 lines
		glDrawArrays(GL_LINES, 1, 49);

		glFlush(); // applies given commands to (single) buffer

	}
}
using namespace core;
// version 0.3.0 (experimental)
int main(int argc, char *argv[])
	{
		glutInit(&argc, argv); // initialize the utility toolkit

		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // display will be with a single buffer using RGB	
		glutInitWindowSize(ModelManager::WINDOW_WIDTH, ModelManager::WINDOW_HEIGHT); // 720p window size
		glutInitWindowPosition(0, 0); // top left

		glutCreateWindow("OpenGL Incremental Development v0.3.0"); // name the window
		glewInit(); // initialize the extension wrangler

		glutDisplayFunc(display); // define the callback function for the display
		glutFullScreen(); // go fullscreen as default

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