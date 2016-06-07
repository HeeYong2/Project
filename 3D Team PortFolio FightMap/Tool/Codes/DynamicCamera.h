#ifndef DynamicCamera_h__
#define DynamicCamera_h__
/********************************************************************
	created:	2015/04/14
	file path:	Yong\Tool\Code
	file base:	Dynamic Camera
	file ext:	h
	author:		Yong	
	purpose:	Free Camera
*********************************************************************/
#include "Tool_Defines.h"
#include "Camera.h"

class CDynamicCamera 
	: public Engine::CCamera
{
public:
	explicit CDynamicCamera(void);
	virtual ~CDynamicCamera(void);

public:
	virtual HRESULT Initialize(LPDIRECT3DDEVICE9 pGraphicDev , const _vec3* pEye , const _vec3* pAt , const _vec3* pUp
		, _float fFov = D3DXToRadian(45.0f) , _float fAspect = float(WINSIZEX) / float(WINSIZEY) , float fNear = 0.1f , float fFar = 2000.f);

	virtual void Update(void);

public:
	void KeyCheck(void);

public:
	static CDynamicCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev , const _vec3* pEye , const _vec3* pAt , const _vec3* pUp
		, _float fFov = D3DXToRadian(45.0f) , _float fAspect = float(WINSIZEX) / float(WINSIZEY) , _float fNear = 0.1f , _float fFar = 2000.f);
};

#endif // DynamicCamera_h__
