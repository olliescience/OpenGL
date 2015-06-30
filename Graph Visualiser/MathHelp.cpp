#include "stdafx.h"

#include <glm\vec2.hpp>
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
#include <glm\mat4x4.hpp>
#include <glm\gtc\matrix_transform.hpp>

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