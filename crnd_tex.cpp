#include <stdlib.h>
#include <assert.h>
#include <windows.h>
#include <gl/gl.h>
#include "crenderer.h"

#define STBI_NO_SIMD
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const char* GetOpenGLError(GLenum error)
{
	if (error == GL_INVALID_ENUM)
		return "GL_INVALID_ENUM";
	else if (error == GL_INVALID_VALUE)
		return "GL_INVALID_VALUE";
	else if (error == GL_INVALID_OPERATION)
		return "GL_INVALID_OPERATION";
	else if (error == GL_STACK_OVERFLOW)
		return "GL_STACK_OVERFLOW";
	else if (error == GL_STACK_UNDERFLOW)
		return "GL_STACK_UNDERFLOW";
	else if (error == GL_OUT_OF_MEMORY)
		return "GL_OUT_OF_MEMORY";
	
	return "No Error or Unknowed";
}

void ReportOpenGLError(GLenum error, const char* file, int line)
{
	printf("OpenGL Error: %s(%i) %s\n", file, line, GetOpenGLError(error));
}

#define CHECK_GL_ERROR() \
	{ \
		GLenum error = glGetError(); \
		ReportOpenGLError(error, __FILE__, __LINE__); \
	}
	
#define REPORT_GL_ERROR(error) \
	{ \
		ReportOpenGLError(error, __FILE__, __LINE__); \
	}

CTexture::CTexture()
{
	handle = width = height = 0;
	format = PIXEL_FORMAT_UNKNOWN;
}

CTexture::~CTexture()
{
	handle = width = height = 0;
	format = PIXEL_FORMAT_UNKNOWN;
}

bool CTexture::LoadFromFile(const char* path)
{
	int channels;
	stbi_uc *data = stbi_load(path, &width, &height, &channels, STBI_rgb_alpha);
	if (!data)
	{
		printf("Failed to create texture %s\n", stbi_failure_reason());
		return false;
	}
	
	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		REPORT_GL_ERROR(error);
		
		// unbind for safety
		glBindTexture(GL_TEXTURE_2D, 0);
	
		return false;
	}
	
	// unbind for safety
	glBindTexture(GL_TEXTURE_2D, 0);	

	return true;
}
