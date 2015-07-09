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
	GLuint vao, vbo; // vertex buffer and attribute objects
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
			"	fragColor = vec4(1.0, 0.0, 0.0, 1.0);" // RGBA
			"}";

		// compile shaders
		GLuint vS = glCreateShader(GL_VERTEX_SHADER); // create a vertex shader identifier
		glShaderSource(vS, 1, &vShader, NULL); // get the source for the shader
		glCompileShader(vS); // compile the shader
		cout << "vertex shader compiled" << endl;

		GLuint fS = glCreateShader(GL_FRAGMENT_SHADER); // create a fragment shader identifier
		glShaderSource(fS, 1, &fShader, NULL); // get the source
		glCompileShader(fS); // compile the shader
		cout << "fragment shader compiled" << endl;

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

	void display(){
		// all display related code in here
		// like glDrawArrays(...) etc.
		glClearColor(0.0f, 0.0f, 0.1f, 1.0f); // the color to clear to (dark navy)
		glClear(GL_COLOR_BUFFER_BIT); // apply the clear with the given color

		InputManager::updateControls(); // update all inputs 
		// timer code (bugged)
		deltaTime = 1;
		int currentTime = glutGet(GLUT_ELAPSED_TIME);/*
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;*/

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

		glDrawArrays(GL_POINTS, 0, 150); // draw the points in the currently bound vao with current shader
		//cout << "triangles drawn" << endl;

		glFlush(); // applies given commands to buffer

	}
}
using namespace core;
// version 0.3.0 (experimental)
int main(int argc, char *argv[])
	{
		glutInit(&argc, argv); // initialize the utility toolkit

		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // display will be a single window using RGB	
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