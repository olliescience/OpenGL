#include <GL/freeglut.h>
namespace ShaderManager{
	extern GLuint vao, vbo, lineAttributeBuffer, lineVertexBuffer; // vertex buffer and attribute objects
	extern GLuint pointShader, lineShader; // shader programs
	extern glm::vec4 pointColor, lineColor;
	extern float pointRed, pointGreen, pointBlue; // RGB for points
	extern float lineRed, lineGreen, lineBlue; // RGB for lines

	extern float deltaTime;
	extern void initialiseShaders();
	extern void updateShader(GLuint shader);
}