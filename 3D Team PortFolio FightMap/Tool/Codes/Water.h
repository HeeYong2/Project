#ifndef Water_h__
#define Water_h__

/********************************************************************
	created:	2015/05/15
	file path:	Yong\Tool\Code
	file base:	Water
	file ext:	h
	author:		Yong	
	purpose:	Water
*********************************************************************/
#include "GameObject.h"
#include "Tool_Defines.h"

class CWater
	: public Engine::CGameObject
{
public:
	explicit CWater(void);
	~CWater(void);

public:
	virtual HRESULT Initialize(void);
	virtual void Update(void);
	virtual void Render(void);
	virtual _ulong Release(void);

public:
	static CWater*	Create(void);

private:
	HRESULT Add_Component(void);
	void Set_ConstantTable(void);

private:
	Engine::CTransform*		m_pInfo;
	Engine::CComponent*		m_pBuffer;
	Engine::CComponent*		m_pTexture;

private:
	LPD3DXEFFECT			m_pEffect;

};
#endif // Water_h__
