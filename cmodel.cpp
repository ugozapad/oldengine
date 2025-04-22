#include <stdio.h>
#include "cmodel.h"

CModel::CModel()
{
	vbuf = NULL;
	vbuf_pos = NULL;
	vbuf_texcoord = NULL;
	tex = NULL;
	numVert = 0;
}

CModel::~CModel()
{
	if (vbuf)
	{
		delete vbuf; vbuf = NULL;
	}
}

void CModel::Render(const SRenderData* rnd_data)
{
	renderer->PushTransformMatrix();

	renderer->DrawVertexBuffer(vbuf, numVert);

	renderer->PopTransformMatrix();
}

void CModel::Load(const char* filename)
{
	FILE* file = fopen(filename, "rb");
	if (file == NULL)
	{
		printf("Cannot load file %s\n", filename);
		return;
	}

	SModelHeader header;
	fread(&header, sizeof(header), 1, file);

	if (header.version != MODEL_VERSION)
	{
		printf("Model %s version %i is older than %i\n", 
			filename, header.version, MODEL_VERSION);

		return;
	}
	
	pos.SetSize(header.vertexcount);
	texcoord.SetSize(header.vertexcount);

	fread(&pos[0], header.vertexcount * sizeof(Vec3), 1, file);
	fread(&texcoord[0], header.vertexcount * sizeof(Vec2), 1, file);
	
	fclose(file);

	tex = texcontainer->LoadTexture(header.texture);

	// generate vertex buffer
	CArray<SModelVertex> vertexBuffer;
	vertexBuffer.SetSize(header.vertexcount);

	for (int i = 0; i < header.vertexcount; i++)
	{
		vertexBuffer[i].x = pos[i].x;
		vertexBuffer[i].y = pos[i].y;
		vertexBuffer[i].z = pos[i].z;
		vertexBuffer[i].s = texcoord[i].x;
		vertexBuffer[i].t = texcoord[i].y;
	}

	numVert = vertexBuffer.GetSize();

	vbuf = renderer->CreateVertexBuffer();
	vbuf->Create(&vertexBuffer[0], 
		vertexBuffer.GetSize() * sizeof(SModelHeader),
		VERTEX_FORMAT_POSITION | VERTEX_FORMAT_TEXCOORD0,
		VERTEX_BUFFER_STATIC);
}

// Geometry and collision functions

bool CModel::IsRayIntersect(const Vec3& rayBegin, const Vec3& rayDir, bool* pSide)
{
	return false;
}
