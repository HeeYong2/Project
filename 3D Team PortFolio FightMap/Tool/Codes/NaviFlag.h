#ifndef Flag_h__
#define NaviFlag_h__
/********************************************************************
	created:	2015/04/30
	file path:	Yong\Tool\Code
	file base:	Flag
	file ext:	h
	author:		Yong	
	purpose:	NaviMesh Flag
*********************************************************************/
#include "GameObject.h"
#include "Tool_Defines.h"

class CNaviFlag
	: public Engine::CGameObject
{
public:
	CNaviFlag::CNaviFlag(void);
	~CNaviFlag(void);

public:
	virtual HRESULT Initialize(void);
	virtual void Render(void);
	virtual void Update(void);
	static CNaviFlag*	Create(void);

public:
	HRESULT AddComponent(void);
	void Render_Texture(void);
	void Render_Buffer(void);

public:
	void SetPos(_vec3* vPos);

private:
	Engine::CTransform*		m_pInfo;
	Engine::CComponent*		m_pBufferCom;
	Engine::CComponent*		m_pTexterCom;

private:
	_bool					m_isRender;
	_int					m_iTexture;

public:
	void					SetTexture(_int iTexture);

public:
	void					SetRender(_bool isRender);
};

#endif // NaviFlag_h__
