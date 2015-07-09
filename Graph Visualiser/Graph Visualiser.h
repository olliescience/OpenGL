#include <GL\glew.h>
namespace core{
	extern int deltaTime;
	extern GLuint vao, vbo, lineAttributeBuffer, lineVertexBuffer; // vertex buffer and attribute objects
	extern GLuint shaderProgram; // shader programs
	
	extern void computeMatricesFromInputs();
	extern void updateViewer();
	
}