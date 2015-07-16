#include "stdafx.h"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "ModelManager.h"
#include "glm/vec4.hpp"

// shader loading source mostly from http://www.nexcius.net/2012/11/20/how-to-load-a-glsl-shader-in-opengl-using-c/

namespace ShaderManager{
	// Global Variables
	GLuint vao, vbo; // vertex buffer and attribute objects
	GLuint pointShader, lineShader; // shader programs
	float deltaTime; // store time changes here so they can be easily used in shaders
	glm::vec4 pointColor, lineColor;
	float pointRed, pointGreen, pointBlue; // RGB for points
	float lineRed, lineGreen, lineBlue; // RGB for lines

	std::string readFile(const char *filePath) {
		std::string content;
		std::ifstream fileStream(filePath, std::ios::in);

		if (!fileStream.is_open()) {
			std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
			return "";
		}

		std::string line = "";
		while (!fileStream.eof()) {
			std::getline(fileStream, line);
			content.append(line + "\n");
		}

		fileStream.close();
		return content;
	}
	
	GLuint LoadShader(const char *vertex_path, const char *fragment_path) {
		GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Read shaders
		std::string vertShaderStr = readFile(vertex_path);
		std::string fragShaderStr = readFile(fragment_path);
		const char *vertShaderSrc = vertShaderStr.c_str();
		const char *fragShaderSrc = fragShaderStr.c_str();
		
		GLint result = GL_FALSE;
		int logLength;

		// Compile vertex shader
		std::cout << "Compiling vertex shader." << std::endl;
		glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
		glCompileShader(vertShader);

		// Check vertex shader
		glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
		glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> vertShaderError((logLength > 1) ? logLength : 1);
		glGetShaderInfoLog(vertShader, logLength, NULL, &vertShaderError[0]);
		std::cout << &vertShaderError[0] << std::endl;

		// Compile fragment shader
		std::cout << "Compiling fragment shader." << std::endl;
		glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
		glCompileShader(fragShader);

		// Check fragment shader
		glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
		glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> fragShaderError((logLength > 1) ? logLength : 1);
		glGetShaderInfoLog(fragShader, logLength, NULL, &fragShaderError[0]);
		std::cout << &fragShaderError[0] << std::endl;

		std::cout << "Linking program" << std::endl;
		GLuint program = glCreateProgram();
		glAttachShader(program, vertShader);
		glAttachShader(program, fragShader);
		glLinkProgram(program);

		glGetProgramiv(program, GL_LINK_STATUS, &result);
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> programError((logLength > 1) ? logLength : 1);
		glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
		std::cout << &programError[0] << std::endl;

		glDeleteShader(vertShader);
		glDeleteShader(fragShader);

		return program;
	}
	
	void initialiseShaders(){ // create shaders
		pointRed = 1.0;
		pointBlue = 0.0;
		pointGreen = 0.0; // points should be red
		lineRed = 0.0;
		lineBlue = 0.0;
		lineGreen = 1.0; // lines should be green
		pointColor = glm::vec4(pointRed, pointGreen, pointBlue, 1.0);
		lineColor = glm::vec4(lineRed, lineGreen, lineBlue, 1.0);

		pointShader = LoadShader("pointVertex.txt", "pointFragment.txt");
		lineShader = LoadShader("lineVertex.txt", "lineFragment.txt");
	}

	void updateShader(GLuint shader){
		// set all of the shader matrix variables (the 'uniform' ones)
		GLuint projLoc = glGetUniformLocation(shader, "projection_matrix"); // get the location of the shader variable
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(ModelManager::Projection)); // set it to the relevant matrix above

		GLuint viewLoc = glGetUniformLocation(shader, "view_matrix"); // same for the view matrix
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(ModelManager::View)); // set it here

		GLuint modelLoc = glGetUniformLocation(shader, "model_matrix"); // same for the model matrix
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(ModelManager::Model)); // set it here

		GLuint eyeLoc = glGetUniformLocation(shader, "eyePosition"); // give the shader the position of the observer
		glUniform3f(eyeLoc, ModelManager::eyePosition.x, ModelManager::eyePosition.y, ModelManager::eyePosition.z);

		if (shader == ShaderManager::pointShader){ // sets the point colors
			GLuint colorLoc = glGetUniformLocation(shader, "inColor"); // give the shader the color
			glUniform4f(colorLoc, pointRed, pointGreen, pointBlue, 1.0);
		}
		if (shader == ShaderManager::lineShader){ // sets the line colors
			GLuint colorLoc = glGetUniformLocation(shader, "inColor"); // give the shader the color
			glUniform4f(colorLoc, lineRed, lineGreen, lineBlue, 1.0);

		}

	}

}