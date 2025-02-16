#ifndef CMODEL_H
#define CMODEL_H

#include "crenderer.h"
#include "cmath.h"

/**
*   Model File Compiled:
*		Header:
*			byte version
*			uint16 vertexcount
*			char texture[61]
*		Data:
*			float x, y, z
*			float s, t
*/

#define MODEL_VERSION 1

/**
* Model header.
*/
struct SModelHeader
{
	byte version;
	uint16 vertexcount;
	char texture[59];
};

/**
* Model vertex.
*/
struct SModelVertex
{
	float x, y, z;
	float s, t;
};

/**
* Static or base model class.
*/
class CModel : public CVisual
{
public:
	CModel();
	virtual ~CModel();

	virtual void Render(const SRenderData* rnd_data);
    virtual void Load(const char* filename);

	// Geometry and collision functions
	virtual bool IsRayIntersect(const Vec3& rayBegin, const Vec3& rayDir, bool* pSide);

private:
	CVertexBuffer* vbuf;
	CVertexBuffer* vbuf_pos;
	CVertexBuffer* vbuf_texcoord;
	CTexture* tex;

	CArray<Vec3> pos;
	CArray<Vec2> texcoord;
};

#endif