#include "stdafx.h"

using namespace glm;

class Camera{

public:
	vec3 camPosition;
	vec3 lookDirection;
	vec3 upDirection;

	mat4 getWorldToViewMatrix(){

		return lookAt(camPosition, camPosition + lookDirection, upDirection);
	}

};