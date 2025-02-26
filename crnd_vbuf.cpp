#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "crenderer.h"

#include <d3d.h>

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

#if 0

IDirect3D7* g_pD3D = NULL;
IDirect3DDevice7* g_pD3DDevice = NULL;

size_t GetStride( uint format )
{
	size_t stride = 0;
	
	if (format & VERTEX_FORMAT_POSITION)
		stride += sizeof(float) * 3;
	if (format & VERTEX_FORMAT_COLOR)
		stride += sizeof(uint);
	if (format & VERTEX_FORMAT_TEXCOORD0)
		stride += sizeof(float) * 2;
	if (format & VERTEX_FORMAT_TEXCOORD1)
		stride += sizeof(float) * 2;
	
	return stride;
}

class CVertexBuffer_D3D : public CVertexBuffer
{
public:
	CVertexBuffer_D3D();
	~CVertexBuffer_D3D();
	
	void Create( void* data, uint size, uint format, EVertexBufferFlags flags );
	void Destroy();

	void Bind();
	void Unbind();
	
private:
	IDirect3DVertexBuffer7* vbuf;
	
};

CVertexBuffer_D3D::CVertexBuffer_D3D()
{
	vbuf = NULL;
}

CVertexBuffer_D3D::~CVertexBuffer_D3D()
{
	Destroy();
}

void CVertexBuffer_D3D::Create( void* data, uint size, uint format, EVertexBufferFlags flags )
{
	D3DVERTEXBUFFERDESC vertexBufferDesc;
	memset(&vertexBufferDesc, 0, sizeof(vertexBufferDesc));
	vertexBufferDesc.dwSize = sizeof(D3DVERTEXBUFFERDESC);
	vertexBufferDesc.dwNumVertices = size / GetStride(format);
	vertexBufferDesc.dwCaps = D3DVBCAPS_WRITEONLY;
	
	if (format & VERTEX_FORMAT_POSITION)
		vertexBufferDesc.dwFVF |= D3DFVF_XYZ;
	if (format & VERTEX_FORMAT_COLOR)
		vertexBufferDesc.dwFVF |= D3DFVF_DIFFUSE;
	if (format & VERTEX_FORMAT_TEXCOORD0)
		vertexBufferDesc.dwFVF |= D3DFVF_TEX0;
	if (format & VERTEX_FORMAT_TEXCOORD1)
		vertexBufferDesc.dwFVF |= D3DFVF_TEX1;

	HRESULT hr = g_pD3D->CreateVertexBuffer(&vertexBufferDesc,
		&vbuf, 0);
		
	// lock buffer and copy vertices	
	void* buf;
	vbuf->Lock(DDLOCK_WAIT | DDLOCK_WRITEONLY | DDLOCK_NOOVERWRITE, &buf, NULL);
	memcpy(buf, data, size);
	vbuf->Unlock();
	buf = NULL;
}

void CVertexBuffer_D3D::Destroy()
{
	if (vbuf)
	{
		vbuf->Release();
		vbuf = NULL;
	}
}

void CVertexBuffer_D3D::Bind()
{
//	g_pD3DDevice->
}

void CVertexBuffer_D3D::Unbind()
{
	
}

#endif
