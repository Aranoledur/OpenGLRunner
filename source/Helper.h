#pragma once
#include "glwrapper.h"
#include <assert.h>
#include <stdio.h>

namespace EasyGodzilla
{
	extern void checkGlError(const char* op);

	extern void CheckShader(GLuint id, GLuint type, GLint *ret, const char *onfail);

	template <class T>
	inline T GetSign(T number)
	{		
		return number == 0 ? 0 : number > 0 ? 1 : -1;
	}
}