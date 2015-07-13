#include "stdafx.h"

namespace ShaderManager{
	// Global Variables
	GLuint vao, vbo; // vertex buffer and attribute objects
	GLuint shaderProgram; // shader programs
	float deltaTime; // store time changes here so they can be easily used in shaders

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
			"uniform vec3 eyePosition;" // position of the observer (for point scaling)
			"void main(){"
			// apparently multiplying it all together works fine... xD
			"	gl_Position = projection_matrix * view_matrix * model_matrix * vec4(vertexPosition, 1);"
			"	gl_PointSize = 350 * 1/length(distance(eyePosition, vertexPosition));" // change the size of the point in the shader!!
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
}