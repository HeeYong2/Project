#ifndef StaticCamera_h__
#define StaticCamera_h__
/********************************************************************
	created:	2015/05/04
	file path:	Yong\Tool\Utility\Code
	file base:	StaticCamera
	file ext:	h
	author:		Yong
	purpose:	Static Camera
*********************************************************************/
#include "Tool_Defines.h"
#include "Camera.h"
#include "Transform.h"

class CStaticCamera
	: public Engine::CCamera
{
public:
	explicit CStaticCamera(void);
	virtual ~CStaticCamera(void);

public:
	virtual HRESULT Initialize(LPDIRECT3DDEVICE9 pGraphicDev , const _vec3* pEye , const _vec3* pAt , const _vec3* pUp
		, _float fFov = D3DXToRadian(45.0f) , _float fAspect = float(WINSIZEX) / float(WINSIZEY) , float fNear = 0.1f , float fFar = 2000.f);

	virtual void Update(void);

public:
	static CStaticCamera* Create(LPDIRECT3DDEVICE9 pGraphicDev , const _vec3* pEye , const _vec3* pAt , const _vec3* pUp
		, _float fFov = D3DXToRadian(45.0f) , _float fAspect = float(WINSIZEX) / float(WINSIZEY) , _float fNear = 0.1f , _float fFar = 2000.f);

public:
	void SetTransCamera(Engine::CTransform* pInfo);
	void SetCamera(void);
	void SetPlayerKey(void);

private:
	_float		m_fCamDistance;
	_float		m_fCamAngle;
};
#endif // StaticCamera_h__
