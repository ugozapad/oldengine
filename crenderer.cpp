#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "crenderer.h"
#include "cfontmanager.h"

// Visuals
#include "cvisual_sprite.h"
#include "cmodel.h"

struct SPlatformContext
{
    HWND hWnd;
    HDC hDC;
    HGLRC hRC;        
};

CRenderer* renderer = NULL;

// class constructor
CRenderer::CRenderer()
{
	memset(&viewport, 0, sizeof(viewport));
	context = NULL;
	flags=0;
}

// class destructor
CRenderer::~CRenderer()
{
	memset(&viewport, 0, sizeof(viewport));
	context = NULL;
}

void CRenderer::Init(void* window)
{
	// allocate context
	context = (SPlatformContext*)malloc( sizeof(SPlatformContext) );
	context->hWnd = (HWND)window;
	
	// create opengl context
	PIXELFORMATDESCRIPTOR pfd;
    int iFormat;

    /* get the device context (DC) */
    context->hDC = GetDC (context->hWnd);

    /* set the pixel format for the DC */
    ZeroMemory (&pfd, sizeof (pfd));
    pfd.nSize = sizeof (pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;
    iFormat = ChoosePixelFormat (context->hDC, &pfd);
    SetPixelFormat (context->hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    context->hRC = wglCreateContext( context->hDC );
    wglMakeCurrent( context->hDC, context->hRC );
    
	/* Initialize texture container */
	texcontainer = new CTextureContainer();

	/* Initialize font manager */
	fontman.Init();

    /* Write GPU stuff */
    printf("%s %s\n", glGetString(GL_VENDOR), glGetString(GL_RENDERER));
    printf("OpenGL ver. %s\n", glGetString(GL_VERSION));
    
	/* Write renderer stuff */
	printf("sizeof(CRenderer)=%i\n", sizeof(CRenderer));
	printf("sizeof(CTexture)=%i\n", sizeof(CTexture));
	printf("sizeof(CTextureContainer)=%i\n", sizeof(CTextureContainer));
	printf("sizeof(CVertexBuffer)=%i\n", sizeof(CVertexBuffer));
	printf("sizeof(CVisual_Sprite)=%i\n", sizeof(CVisual_Sprite));
	printf("sizeof(CModel)=%i\n", sizeof(CModel));
	printf("sizeof(SModelHeader)=%i\n", sizeof(SModelHeader));

    /* reset opengl errors */
    glGetError();
    
    /* Enable OpenGL states */
    glEnable(GL_TEXTURE_2D);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void CRenderer::Shutdown()
{
	/* Destroy font manager */
	fontman.Shutdown();

	/* Destroy texture containter */
	delete texcontainer; texcontainer = NULL;

	// Release OpenGL
    wglMakeCurrent (NULL, NULL);
    wglDeleteContext (context->hRC);
    ReleaseDC (context->hWnd, context->hDC);
    
    // deallocate context
    free(context); context = NULL;
}

void CRenderer::DrawVertexBuffer(CVertexBuffer* vbuf, uint vertexcount)
{
	vbuf->Bind();
	glDrawArrays(GL_TRIANGLES, 0, vertexcount);
	vbuf->Unbind();
}

void CRenderer::DrawRectColoredWire(float x, float y, float w, float h, uint color)
{
	SetTexture(NULL);

	glPolygonMode(GL_BACK, GL_LINE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho((float)viewport.x, (float)viewport.width, (float)viewport.height, (float)viewport.y, -1.0f, 1.0f);

	glPushAttrib(GL_CURRENT_BIT);
	glColor4ubv((const GLubyte*)&color);

	glBegin(GL_QUADS);
		glVertex2f(x, y);
		glVertex2f(x + w, y);
		glVertex2f(x + w, y + h);
		glVertex2f(x, y + h);
	glEnd();

	glPopAttrib();

	glLoadIdentity();
}

void CRenderer::DrawTile(CTexture* texture, float x, float y, float w, float h, float s1, float t1, float s2, float t2)
{
	SetTexture(texture);

	glPolygonMode(GL_BACK, GL_FILL);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho((float)viewport.x, (float)viewport.width, (float)viewport.height, (float)viewport.y, -1.0f, 1.0f);
	
	glBegin(GL_QUADS);
  		glTexCoord2f(s1, t1); glVertex2f(x, y); 
    	glTexCoord2f(s2, t1); glVertex2f(x+w, y); 
    	glTexCoord2f(s2, t2); glVertex2f(x+w, y+h); 
    	glTexCoord2f(s1, t2); glVertex2f(x,y+h);
	glEnd();

	glLoadIdentity();
}

void CRenderer::DrawTexture(CTexture* texture, float x, float y, float w, float h)
{
	SetTexture(texture);
	
	glPolygonMode(GL_BACK, GL_FILL);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho((float)viewport.x, (float)viewport.width, (float)viewport.height, (float)viewport.y, -1.0f, 1.0f);
	
	glBegin(GL_QUADS);
  		glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y); 
    	glTexCoord2f(1.0f, 0.0f); glVertex2f(x+w, y); 
    	glTexCoord2f(1.0f, 1.0f); glVertex2f(x+w, y+h); 
    	glTexCoord2f(0.0f, 1.0f); glVertex2f(x,y+h);
	glEnd();

	glLoadIdentity();
}

void CRenderer::DrawTextureRot(CTexture* texture, float x, float y, float w, float h, float angle)
{
	SetTexture(texture);	
	
	glPolygonMode(GL_BACK, GL_FILL);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho((float)viewport.x, (float)viewport.width, (float)viewport.height, (float)viewport.y, -1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(angle, 0.0f, 0.0f, 1.0f );
	
	glBegin(GL_QUADS);
  		glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y); 
    	glTexCoord2f(1.0f, 0.0f); glVertex2f(x+w, y); 
    	glTexCoord2f(1.0f, 1.0f); glVertex2f(x+w, y+h); 
    	glTexCoord2f(0.0f, 1.0f); glVertex2f(x,y+h);
	glEnd();

	glLoadIdentity();
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

void CRenderer::DrawTexture3D(CTexture* texture, float x, float y, float w, float h)
{
	SetTexture(texture);	
	
	glPolygonMode(GL_BACK, GL_FILL);

	// Change model matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBegin(GL_QUADS);
  		glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y); 
    	glTexCoord2f(1.0f, 0.0f); glVertex2f(x+w, y); 
    	glTexCoord2f(1.0f, 1.0f); glVertex2f(x+w, y+h); 
    	glTexCoord2f(0.0f, 1.0f); glVertex2f(x,y+h);
	glEnd();
}

void CRenderer::SetAlphaBlend(bool enable)
{
	enable ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
}

void CRenderer::SetViewport(int x, int y, int width, int height)
{
	viewport.x = x;
	viewport.y = y;
	viewport.width = width;
	viewport.height = height;
	
	glViewport(x, y, width, height);
}

void CRenderer::SetTexture(CTexture* texture)
{
	glBindTexture(GL_TEXTURE_2D, texture ? texture->handle : 0);
}

void CRenderer::BeginFrame()
{
	// FRAME BEGIN	
    //glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
	glClearColor (0.0f, 0.0f, 1.0f, 1.0f);
    glClear (GL_COLOR_BUFFER_BIT);

}

void CRenderer::EndFrame()
{
}

void CRenderer::Present()
{
	assert(context);
	SwapBuffers(context->hDC);
}

// Visual factory
CVisual* CRenderer::CreateVisual(EVisualType type)
{
	CVisual* visual = NULL;
	
    switch (type)
    {
    case VISUAL_SPRITE:
    	visual = new CVisual_Sprite();
    	break;
	case VISUAL_MODEL:
		visual = new CModel();
    	break;
    }

	assert(visual);
    return visual;
}

CTexture* CRenderer::CreateTexture()
{
	return new CTexture();
}

CVertexBuffer* CRenderer::CreateVertexBuffer()
{
	return new CVertexBuffer();
}

void CRenderer::UpdateRendererState()
{
	if (flags & RENDER_BLACK_AS_ALPHA)
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	else
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

////////////////////
// base visual class

CVisual::CVisual()
{
}

CVisual::~CVisual()
{
}

void CVisual::Render(const SRenderData* rnd_data)
{
}

void CVisual::Load(const char* filename)
{
}

