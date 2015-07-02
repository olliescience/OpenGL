// Graph Visualiser.cpp : Defines the entry point for the console application.

#include "stdafx.h"

using namespace std;
using namespace glm;

// Global Variables
GLuint vao, vbo;
GLuint shaderProgram, shaderProg3d;
GLuint vbo2;

// manipulate uniform values in the vertex shader
mat4 Projection, // for scene projection to viewer
	 View, // location and nature of observer
	 Model; // for manipulating position/rotation

float FOV = 45.0f; // field of view (projection)
float AR = 1.0f; // aspect ratio (projection)
float NEARclip = 0.0f; // closest object to draw distance (projection)
float FARclip = 100.0f; // furthest object to draw distance (projection)

float ROTATION = 0.0f; // rotation (model)

// TODO: 
// 1. get vbo and vao working & understood                                | 90%
// 2. update the display callback to accommodate changes                  | 50%
// 3. create shaders and get them working                                 | 90%
// 4. add keyboard controls and camera manipuation                        | 60%
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

	/* debugging console output*/
	cout << "Generated vector:" << endl;
	cout << "xValue: " << xValue << endl;
	cout << "yValue: " << yValue << endl;
	cout << "zValue: " << zValue << endl;
	

	generatedVec3 = vec3(xValue, yValue, zValue);
	return generatedVec3;
}

void init(){
	// initialisation logic here
	cout << "Init has been called" << endl;

	//cout << "Generating nodes..." << endl;

	//Node ListOfNodes [50]; // array of 50 nodes
	//vec2 xRange = vec2(0, 1);
	//vec2 yRange = vec2(0, 1);
	//vec2 zRange = vec2(-1, 0);

	//for (int i = 0; i < 50; i++){ // generate 50 positions
	//	ListOfNodes[i].setPosition(generateVec3(xRange, yRange, zRange));
	//	cout << "node generated" << endl;
	//}
	//cout << "all nodes generated." << endl;

	// so now we have all the data, we need to use it...

	// define a buffer
	float testData[] = { 0.1f, 0.1f, 0.0f, 0.1f, 0.8f, 0.0f, 0.7f, 0.45f, 0.0f, -0.1f, -0.1f, 0.0f, -0.1f, -0.8f, 0.0f, -0.7f, -0.45f, 0.0f }; // two triangles
	float thirdTri[] = { -0.1f, 0.1f, 0.0f, -0.1f, 0.8f, 0.0f, -0.7f, 0.45f, 0.0f };

	float allTriangles[27]; // an array to hold all triangles

	// add all the triangles into one array
	for (int i = 0; i < 27; i++){
		if (i < 18){
			allTriangles[i] = testData[i];
		}
		if (i >= 18){
			allTriangles[i] = thirdTri[i - 18];
		}
		// print the values to the console for debugging
		cout << allTriangles[i] << endl;
	}

	//Vertex buffer object defines where the vertices will be stored (GPU)
	int sizeOfData = 18 * sizeof(float); // value for testing
	glGenBuffers(1, &vbo); // generate a unique value for the buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // binds the value to a type GL_ARRAY_BUFFER (activates it)
	glBufferData(GL_ARRAY_BUFFER, sizeOfData, testData, GL_STATIC_DRAW); // define type of data and size
	cout << "vertex buffer object defined" << endl;

	int sizeOfAll = 27 * sizeof(float);
	glGenBuffers(1, &vbo2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	glBufferData(GL_ARRAY_BUFFER, sizeOfAll, allTriangles, GL_STATIC_DRAW);


	// Vertex attribute object defines where the attributes for each vertex are stored and
	//how they should be interpreted by the GPU 
	glGenBuffers(1, &vao); // generate a unique value for the attrib
	glBindVertexArray(vao); // binds the vertex attribute array
	glEnableVertexAttribArray(0); // ?
	glBindBuffer(GL_ARRAY_BUFFER, vbo2); // shows that these arrtibuts apply to the 'vbo' buffer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL); // points to the starting location of the attribute storage
	cout << "vertex attribute object defined" << endl;

	// decide how to display data in the buffer...

	// create shaders
	const char* vShader =
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

	const char* vShader3 =
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
	glAttachShader(shaderProgram, fS); // attach the fragment shader
	glAttachShader(shaderProgram, vS); // attach the vertex shader
	glLinkProgram(shaderProgram); // link them together
	
	shaderProg3d = glCreateProgram(); // shader with 3D capabilities
	glAttachShader(shaderProg3d, fS3); // fragment shader only handles color
	glAttachShader(shaderProg3d, vS3); // vertex shader has 3 matrices for manipulation
	glLinkProgram(shaderProg3d); // link the two into one program
	
}

void display(){
	// all display related code in here
	// like glDrawArrays(...) etc.
	glClearColor(0.0f, 0.0f, 0.1f, 1.0f); // the color to clear to (black)
	glClear(GL_COLOR_BUFFER_BIT);

	//cout << "screen cleared" << endl;
	glUseProgram(shaderProg3d); // using a shader program from init()...
	//cout << "using shader program" << endl;
	
	// perform any matrix alterations here...
	//Projection = ortho(-4.0f / 3.0f, 4.0f / 3.0f, -1.0f, 1.0f, 0.0f, 1000.0f);
	//     orthographic    left         right      top   bot   near  far
	
	Projection = perspective(FOV, AR, 0.0f, 10.0f);

	Model = rotate(Model, ROTATION, vec3(0.0f, 0.0f, -0.2f)); // rotate then translate for normal effects
	Model = translate(Model, vec3(0.0f, 0.0f, -5.0f)); // move left, up and back
	//                mat4         x     y      z

	View = translate(View, vec3(0.0f, 0.0f, 3.0f));

	GLuint projLoc = glGetUniformLocation(shaderProg3d, "projection_matrix"); // get the location of the shader variable
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(Projection)); // set it to the relevant matrix above

	GLuint viewLoc = glGetUniformLocation(shaderProg3d, "view_matrix"); // same for the view matrix
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(View)); // set it here

	GLuint modelLoc = glGetUniformLocation(shaderProg3d, "model_matrix"); // same for the model matrix
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(Model)); // set it here

	glBindVertexArray(vao);
	//cout << "vao bound" << endl;
	glDrawArrays(GL_TRIANGLES, 0, 9); // draw the points in the currently bound vao with current shader
	//cout << "triangles drawn" << endl;

	glFlush(); // applies given commands to buffer
}

void mouse(int button, int state, int x, int y){ // mouse callback event
	// button can be 0 (left) or 2 (right), 1 should be the wheel but isn't implemented as standard
	// state is binary
	// x is the x coordinate of the mouse
	// y is the y coordinate of the cursor
	cout << "button: " << button << ", state: " << state << endl;
}

void keyboard(unsigned char key, int x, int y){ // keyboard callback event
	// key is the character the key represents
	// x is the x coordinate of the mouse at the point the key is pressed
	// y is the y coordinate of the mouse at the point the key is pressed
	cout << "key pressed: " << key << " x = " << x << " y = " << y << endl;

	if (key == 27){ // escape key to close
		exit(0);
	}
	if (key == 'w'){
		FOV += 0.2f;
		cout << FOV << endl;
	}
	if (key == 's'){
		FOV -= 0.2f;
		cout << FOV << endl;
	}
	if (key == 'a'){
		ROTATION -= 0.02f;
	}
	if (key == 'd'){
		ROTATION += 0.02f;
	}

	glutPostRedisplay();
}

void mousemove(int x, int y){ // only called when button is down and mouse is moved
	cout << ".";
}

void idle(){
	// things to do when nothing else is happening
	// it is called very quickly! (timer mayb?)
};

// version 0.1.4 (experimental)
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

	glutCreateWindow("OpenGL Incremental Development v0.1.3"); // name the window
	cout << "Window title set." << endl;

	glewInit(); // initialise the extension wrangler
	cout << "glewInit Executed..." << endl;
	
	glutDisplayFunc(display); // define the callback function for the display
	cout << "Display callback run." << endl;

	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(mousemove);
	glutIdleFunc(idle);

	init(); // custom initialisation code
	cout << "init() Executed..." << endl;

	cout << "GLUT main loop starting..." << endl;
	glutMainLoop(); // the main look of the program
	cout << "GLUT main loop stopped." << endl;
	
	return 0;
}


