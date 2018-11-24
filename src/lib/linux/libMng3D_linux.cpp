#include "lib.h"
#include "libMacros.h"
#include "libTypes.h"
#include "libMng3D.h"
#include "libMngGraphic.h"

//TODO#include <GL/gl.h>
//TODO#include <GL/glu.h>

CMng3D* CMng3D::singletonInstance= 0;

TVertex*
CVertexBuffer::Lock(void)
{
TVertex *ret;

	return ret= 0;
}

void
CVertexBuffer::Unlock(void)
{
}

u16*
CIndexBuffer::Lock(void)
{
u16 *ret;

	return ret= 0;
}

void
CIndexBuffer::Unlock(void)
{
}


//////
CMng3D::CMng3D():
m_width(0),
m_height(0),
m_isFS(0),
m_bufT(0)
{
	ASSERT2(!singletonInstance, "Another instance of CMngGraphic is already instanced");

	singletonInstance= this;
}

CMng3D::~CMng3D()
{
	SAFE_DELETE_ARRAY(m_bufT);

	Exit();
	singletonInstance= 0;
}

void
CMng3D::Init(void* p_hwnd, long FBwidth, long FBheight, long BBwidth, long BBheight, bool isFullSCreen, long p_pixelDepth)
{
	m_isFS	= isFullSCreen;
	m_width	= BBwidth;
	m_height= BBheight;

//	glViewport(0, 0, m_width-0, m_height-0);
//	glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
//	glClearDepth(1.0);
//	glDepthFunc(GL_LESS);
//	glEnable(GL_DEPTH_TEST);
//	glShadeModel(GL_SMOOTH);
//
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();

//	gluPerspective(45.0f,(GLfloat)m_width/(GLfloat)m_height,0.1f,100.0f);	// Calculate The Aspect Ratio Of The Window

//	glMatrixMode(GL_MODELVIEW);
}

void
CMng3D::Exit(void)
{
}

CVertexBuffer*
CMng3D::CreateVB (int nbVertex)
{
CVertexBuffer*	vb= new CVertexBuffer();

	vb->m_start	= m_cptV;	//in DX we only keep offset from start since vertex are in HW
	vb->m_nb	= nbVertex;
	m_cptV		+= nbVertex;

	return vb;
}

CIndexBuffer*
CMng3D::CreateIB(int nbIndex)
{
CIndexBuffer*	ib= new CIndexBuffer();

	ib->m_start	= m_cptI;	//in DX we only keep offset from start since vertex are in HW
	ib->m_nb	= nbIndex;
	m_cptI		+= nbIndex;

	return ib;
}

CTextureCoordonateBuffer*
CMng3D::CreateTB(int nbTC)
{
CTextureCoordonateBuffer*	tcb= new CTextureCoordonateBuffer();

	tcb->m_start= m_bufT + m_cptT;
	tcb->m_nb	= nbTC;
	m_cptT		+= nbTC;

	return tcb;
}

void
CMng3D::BufferGeo(int pnbV, int pnbT, int pnbI)
{
	m_cptMaxV	= pnbV;
	m_cptMaxI	= pnbI;
	m_cptMaxT	= pnbT;

	ResetGeo();

	if((m_cptMaxT= pnbT))
		m_bufT= new TTexcoord[m_cptMaxT];
}

void
CMng3D::ResetGeo(void)
{
	m_cptV		= 0;
	m_cptI		= 0;
	m_cptT		= 0;
}

void
CMng3D::Clear(int param)
{
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glLoadIdentity();
}

void
CMng3D::DrawPrimitiveIB(CIndexBuffer* p_ib, CVertexBuffer* p_vb, void* p_texture)
{
}

void
CMng3D::Flip(void)
{
//	glTranslatef(-1.5f,0.0f,-6.0f);		// Move Left 1.5 Units And Into The Screen 6.0

	// draw a triangle
//	glBegin(GL_POLYGON);				// start drawing a polygon
//	glVertex3f( 0.0f, 1.0f, 0.0f);		// Top
//	glVertex3f( 1.0f,-1.0f, 0.0f);		// Bottom Right
//	glVertex3f(-1.0f,-1.0f, 0.0f);		// Bottom Left
//	glEnd();					// we're done with the polygon

//	SDL_GL_SwapBuffers();
}
