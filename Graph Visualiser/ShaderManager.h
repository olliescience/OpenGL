#include <GL/freeglut.h>
namespace ShaderManager{
	extern GLuint vao, vbo, lineAttributeBuffer, lineVertexBuffer; // vertex buffer and attribute objects
	extern GLuint shaderProgram; // shader programs
	extern float deltaTime;
	extern void initialiseShaders();
}