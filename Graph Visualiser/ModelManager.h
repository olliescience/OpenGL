#include <glm\vec3.hpp> // 3D vector glm::vec3
#include <glm\mat4x4.hpp> // 4x4 matrices glm::mat4
#include <GL\glew.h> // Extension Wrangler
namespace ModelManager{

	extern int WINDOW_WIDTH;
	extern int WINDOW_HEIGHT;

	extern float MOVESPEED; // the movement speed of the observer

	extern glm::mat4 Projection, // for scene projection to viewer
		View, // location and nature of observer
		Model; // for manipulating position/rotation

	extern glm::vec3 eyePosition; // default eye position (view)
	extern glm::vec3 targetPosition; // default target (view)
	extern glm::vec3 upDirection; // default sky direction (view)

	extern glm::vec3 autoViewCentre;

	extern glm::vec3 moveModel; // for moving vertices's (model)
	extern float rotANGLE; // rotation (model)
	extern float rotation; // initial rotation
	extern glm::vec3 rotCENTER; // center of rotation

	extern float FOV; // field of view (projection)
	extern float AR; // aspect ratio (projection)
	extern float NEARclip; // closest object to draw distance (projection)
	extern float FARclip; // furthest object to draw distance (projection)

	extern float horizontalAngle; // angle variables for camera rotation
	extern float verticalAngle;
	extern float rotationSpeed; // the maximum speed 

	extern void setModelVariables();
	extern void updateViewer();
	extern void rotateMouse();
	extern void updateGeometry();
	extern void reshape(int x, int y);
	extern void initialiseModel();
	extern void initialiseMatrices();
	extern glm::vec3 generateVec3(glm::vec2 xRange, glm::vec2 yRange, glm::vec2 zRange);
	extern void moveObserver(glm::vec3 direction);
}