#ifndef CRND_D3D7_H
#define CRND_D3D7_H

#include "crenderer.h"

#include <d3d.h>

class CRenderer_D3D
{
public:
	IDirect3D7* 		g_pD3D;
	IDirect3DDevice7* 	g_pD3DDevice;
	
public:
	CRenderer_D3D();
	~CRenderer_D3D();

	void DrawVertexBuffer(CVertexBuffer* vbuf, uint vertexcount);
};

CRenderer_D3D::CRenderer_D3D()
{
	g_pD3D = NULL;
	g_pD3DDevice = NULL;
}

CRenderer_D3D::~CRenderer_D3D()
{
	if (g_pD3DDevice)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}
	
	if (g_pD3D)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

void CRenderer_D3D::DrawVertexBuffer(CVertexBuffer* vbuf, uint vertexcount)
{
	/*
	    STDMETHOD(DrawPrimitiveVB)(THIS_ D3DPRIMITIVETYPE primitive_type, struct IDirect3DVertexBuffer7 *vb,
            DWORD start_vertex, DWORD vertex_count, DWORD flags) PURE;
    */
	
	IDirect3DVertexBuffer7 *vb = NULL; // ( (CVertexBuffer_DX7*) vbuf)->vbuf;
	
	
	g_pD3DDevice->DrawPrimitiveVB(D3DPT_TRIANGLELIST, vb, 0, vertexcount, 0);
}

#endif
