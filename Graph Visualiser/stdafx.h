// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

// TODO: reference additional headers your program requires...

#include <cstdlib> // for random number generation

#include <GL\glew.h> // Extension Wrangler
#include <GL\freeglut.h> // updated Utilities Toolkit

#include <glm\vec3.hpp> // 3D vector glm::vec3
#include <glm\vec4.hpp> // 4D vector glm::vec4
#include <glm\mat4x4.hpp> // 4x4 matrices glm::mat4
#include <glm\gtc\matrix_transform.hpp> // for transforming matrices
#include <glm\gtc\type_ptr.hpp> // conversions for OpenGL accepted types

#include <string> // for string manipulation
#include <iostream> // for input/output