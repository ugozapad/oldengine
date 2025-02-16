#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "crenderer.h"

/////////////////////////////////////////////////////////////////////
// Vertex Buffer implementation through glVertexPointer 

CVertexBuffer::CVertexBuffer()
{
	bufdata = NULL;
	bufsize = 0;
}

CVertexBuffer::~CVertexBuffer()
{
	bufdata = NULL;
	bufsize = 0;
}


void CVertexBuffer::Create( void* data, uint size, uint format, EVertexBufferFlags flags )
{
	// allocate and copy buffer

	bufdata = malloc(size);
	bufsize = size;
	bufformat = format;

	memcpy(bufdata, data, size);
}

void CVertexBuffer::Destroy()
{
	if (bufdata)
	{
		free(bufdata); bufdata = NULL;
	}
}

void CVertexBuffer::Bind()
{
	uint s = 0;
	
	if (bufformat & VERTEX_FORMAT_POSITION)
	{
		// enable client state
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, s, bufdata);
		s += sizeof(float) * 3;
	}

	if (bufformat & VERTEX_FORMAT_COLOR)
	{
		// enable client state
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_UNSIGNED_INT, s, bufdata);
		s += sizeof(uint);
	}

	if (bufformat & VERTEX_FORMAT_TEXCOORD0)
	{
		// enable client state
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		
		// if buffer has secondary texcoord
		if (bufformat & VERTEX_FORMAT_TEXCOORD1)
		{
			glTexCoordPointer(4, GL_FLOAT, s, bufdata);
			s += sizeof(float) * 4;
		}
		else
		{
			glTexCoordPointer(2, GL_FLOAT, s, bufdata);
			s += sizeof(float) * 2;
		}
	}
}

void CVertexBuffer::Unbind()
{
	// disable client states

	if (bufformat & VERTEX_FORMAT_TEXCOORD0)
	{
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	if (bufformat & VERTEX_FORMAT_COLOR)
	{
		glDisableClientState(GL_COLOR_ARRAY);
	}

	if (bufformat & VERTEX_FORMAT_POSITION)
	{
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}

/////////////////////////////////////////////////////////////////////
// Vertex Buffer implementation through GL_ARB_vertex_buffer_object 

class CVertexBuffer_VBO : public CVertexBuffer
{
public:
	CVertexBuffer_VBO();
	~CVertexBuffer_VBO();
};

CVertexBuffer_VBO::CVertexBuffer_VBO()
{
}

CVertexBuffer_VBO::~CVertexBuffer_VBO()
{
}

/////////////////////////////////////////////////////////////////////
// Direct3D Vertex Buffer 
class CVertexBuffer_D3D : public CVertexBuffer
{
public:
	CVertexBuffer_D3D();
	~CVertexBuffer_D3D();
};

CVertexBuffer_D3D::CVertexBuffer_D3D()
{
}

CVertexBuffer_D3D::~CVertexBuffer_D3D()
{
}
