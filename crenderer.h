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
	RENDER_BLACK_AS_ALPHA = 1 << 2,
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
	float scale;
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
	void DrawLine(float x1, float y1, float x2, float y2, uint color);
	void DrawRectColoredWire(float x, float y, float w, float h, uint color);
	void DrawTile(CTexture* texture, float x, float y, float w, float h, float s1, float t1, float s2, float t2);
    void DrawTexture(CTexture* texture, float x, float y, float w, float h);
    void DrawTexture3D(CTexture* texture, float x, float y, float w, float h);

	// Special Effects
	void DrawScreenOverlay(float r, float g, float b, float a);

	// Matrix API
	void PushTransformMatrix();
	void Translate(float x, float y, float z);
	void RotateZ(float angle);
	void Scale(float scale);
	void PopTransformMatrix();

    void SetAlphaBlend(bool enable);
    
    void SetViewport(int x, int y, int width, int height);
    void SetTexture(CTexture* texture);

	// Render Flags
	void Set2DHighlight(bool enable) { enable ? flags |= RENDER_2DFX_HIGHLIGHT : flags &= ~RENDER_2DFX_HIGHLIGHT; UpdateRendererState(); }
	void SetBlackAlpha(bool enable) { enable ? flags |= RENDER_BLACK_AS_ALPHA : flags &= ~RENDER_BLACK_AS_ALPHA; UpdateRendererState(); }
    
	void BeginFrame();
	void EndFrame();
	
	void Present();
    
    const SViewport& GetViewport() { return viewport; }
    
	// WinAPI
	void* Win32_GetWindow();

    // Visual factory
    CVisual* CreateVisual(EVisualType type);
    
    // Render object factory
    CTexture* CreateTexture();
	CVertexBuffer* CreateVertexBuffer();

private:
	void UpdateRendererState();

private:
	SViewport viewport;
	SPlatformContext* context;
	uint flags;
};

extern CRenderer* renderer;

/*
 * Debug utils class.
 */
class CDebugUtils
{
public:
	CDebugUtils();
	~CDebugUtils();

	void DrawLine(float x1, float y1, float x2, float y2, uint color);
	
	void Flush();

private:
	struct SDbgLine
	{
		float x1;
		float y1;
		float x2;
		float y2;
		uint color;
	};

	CArray<SDbgLine> lines;
};

extern CDebugUtils* debugUtils;

#endif // CRENDERER_H
