#ifndef CRENDERER_H
#define CRENDERER_H

#include "ctypes.h"
#include "carray.h"
#include "cmath.h"

enum EPixelFormat
{
	PIXEL_FORMAT_UNKNOWN,
	PIXEL_FORMAT_RGBA8,
	
	// Should be last
	PIXEL_FORMAT_COUNT
};

enum EVisualType
{
    VISUAL_SPRITE,
    VISUAL_MODEL
};

enum ERenderFlags
{
	RENDER_2DFX_HIGHLIGHT = 1 << 1,

};

enum EVertexBufferFlags
{
	VERTEX_BUFFER_STATIC, 		/* Buffer content will never change. */
	VERTEX_BUFFER_DYNAMIC,		/* Buffer requires dynamic memory usage and allow writting in to it.  */
	VERTEX_BUFFER_MAX
};

enum EVertexFormat
{
	VERTEX_FORMAT_POSITION = 1 << 1,		// Float 3
	VERTEX_FORMAT_COLOR = 1 << 2,			// UINT
	VERTEX_FORMAT_TEXCOORD0 = 1 << 3,		// Float 2
	VERTEX_FORMAT_TEXCOORD1 = 1 << 4,		// Float 2
};

/* Forward declaration */
struct SPlatformContext;
class CRenderer;

/*
 * Viewport structure.
 */
struct SViewport
{
	int x, y, width, height;
};

/*
 * Rendering data structure.
 */
struct SRenderData
{
	Vec3 position;
	Vec3 rotation;
};

/*
 * Texture class.
 */
class CTexture
{
	friend class CRenderer;
public:
	CTexture();
	~CTexture();
	
	bool LoadFromFile(const char* path);
	
	int GetWidth() { return width; }
	int GetHeight() { return height; }
	
private:
	int width;
	int height;
	uint handle;
	EPixelFormat format;
};

/*
 * Texture containter class.
 */
class CTextureContainer
{
public:
	CTextureContainer();
	~CTextureContainer();

	CTexture* LoadTexture(const char* filename);

private:
	struct STextureEntry
	{
		char filename[256];
		CTexture* texture;
	};

	CArray<STextureEntry> textures;
};

extern CTextureContainer* texcontainer;

/*
 * Vertex buffer class.
 */
class CVertexBuffer
{
public:
	CVertexBuffer();
	virtual ~CVertexBuffer();

	virtual void Create( void* data, uint size, uint format, EVertexBufferFlags flags );
	virtual void Destroy();

	virtual void Bind();
	virtual void Unbind();

private:
	void* bufdata;
	uint bufsize;

protected:
	uint bufformat;
};

/*
 * Dynamic vertex buffer class.
 */
class CDynVertexBuffer
{
public:
	CDynVertexBuffer();
	~CDynVertexBuffer();

	void Lock();
	void Unlock();

	void Render();

private:
	
};

/*
 * Base visual class.
 */
class CVisual
{
public:
    CVisual();
    virtual ~CVisual();
    
    virtual void Render(const SRenderData* rnd_data);
    virtual void Load(const char* filename);
};

/*
 * Renderer class.
 */
class CRenderer
{
public:
    // class constructor
    CRenderer();
    // class destructor
    ~CRenderer();
    
    void Init(void* window);
    void Shutdown();
    
    // Rendering
	void DrawVertexBuffer(CVertexBuffer* vbuf, uint vertexcount);
	void DrawTile(CTexture* texture, float x, float y, float w, float h, float s1, float t1, float s2, float t2);
    void DrawTexture(CTexture* texture, float x, float y, float w, float h);
    void DrawTextureRot(CTexture* texture, float x, float y, float w, float h, float angle);
    void DrawTexture3D(CTexture* texture, float x, float y, float w, float h);

    void SetAlphaBlend(bool enable);
    
    void SetViewport(int x, int y, int width, int height);
    void SetTexture(CTexture* texture);

	// Render Flags
	void Set2DHighlight(bool enable) { enable ? flags |= RENDER_2DFX_HIGHLIGHT : flags &= ~RENDER_2DFX_HIGHLIGHT;  }

    
	void BeginFrame();
	void EndFrame();
	
	void Present();
    
    const SViewport& GetViewport() { return viewport; }
    
    // Visual factory
    CVisual* CreateVisual(EVisualType type);
    
    // Render object factory
    CTexture* CreateTexture();
	CVertexBuffer* CreateVertexBuffer();


private:
	SViewport viewport;
	SPlatformContext* context;
	uint flags;
};

extern CRenderer* renderer;

#endif // CRENDERER_H
