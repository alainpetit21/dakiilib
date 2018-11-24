#ifndef _3D_LINUX_H
#define _3D_LINUX_H


#include <math.h>

#include "libTypes.h"
#include "libMath.h"

typedef struct tagTVertex
{
	float			x;	//Origin	//3D Point Before Transformation calc
	float			y;	//Origin	//3D Point Before Transformation calc
	float			z;	//Origin	//3D Point Before Transformation calc
	float			tu;	//Origin	//3D Point Before Transformation calc
	float			tv;	//Origin	//3D Point Before Transformation calc
}TVertex;

typedef struct tagTTexcoord
{
	int		u,	v;		//Texture Coordonate
}TTexcoord;

class CVertexBuffer
{
public:
	TVertex*	Lock(void);
	void		Unlock(void);

	int				m_start;
	int				m_nb;
};

class CIndexBuffer
{
public:
	unsigned short*	Lock(void);
	void			Unlock(void);

	int				m_start;
	int				m_nb;
};

class CTextureCoordonateBuffer
{
public:
	TTexcoord*		Lock(void){return m_start;};
	void			Unlock(void){};

	TTexcoord		*m_start;
	int				m_nb;
};

class CMng3D
{
public:
	CMng3D();
	virtual	~CMng3D();

	void	Init	(void* p_hwnd, long FBwidth, long FBheight, long BBwidth, long BBheight, bool isFullSCreen, long p_pixelDepth);
	void	Exit	(void);

	CVertexBuffer*				CreateVB		(int nbVertex);
	CIndexBuffer*				CreateIB		(int nbIndex);
	CTextureCoordonateBuffer*	CreateTB		(int nbTC);

	void	BufferGeo		(int p_nbV, int p_nbT, int p_nbI);
	void 	ResetGeo		(void);

	//Run-Time 3DEngine
	void	Clear			(int param);
	void	DrawPrimitiveIB	(CIndexBuffer* p_ib, CVertexBuffer* p_vb, void* p_tex);
	void	Flip			(void);

	// Main objects used for creating and rendering the 3D scene
	unsigned int	m_width;
	unsigned int	m_height;
	unsigned int	m_bpp;
	bool			m_isFS;
	unsigned char	m_red;
	unsigned char	m_green;
	unsigned char	m_blue;

	//3D System
	TTexcoord		*m_bufT;
	unsigned int	m_cptV;
	unsigned int	m_cptMaxV;
	unsigned int	m_cptT;
	unsigned int	m_cptMaxT;
	unsigned int	m_cptI;
	unsigned int	m_cptMaxI;
	int				m_nb;

	static CMng3D* singletonInstance;
};

inline CMng3D*
GetMng3D(void)
{
	return CMng3D::singletonInstance? CMng3D::singletonInstance : new CMng3D;
}
#endif // _3D_LINUX_H
