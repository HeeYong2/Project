#ifndef Player_h__
#define Player_h__
/********************************************************************
	created:	2015/05/04
	file path:	Yong\Tool\Code
	file base:	Player
	file ext:	h
	author:		Yong	
	purpose:	Test 용도의 플레이어
*********************************************************************/
#include "Tool_Defines.h"
#include "GameObject.h"

class CPlayer
	: public Engine::CGameObject
{
public:
	explicit CPlayer(void);
	~CPlayer(void);

public:
	virtual HRESULT Initialize(void);
	virtual void Update(void);
	virtual void Render(void);


public:
	static CPlayer* Create(void);
	void Render_Mesh(void);

public:
	HRESULT AddComponent(void);
	void KeyCheck(void);

public:
	Engine::CTransform*	GetPlayerInfo(void);
	_vec3	GetPlayerPos(void);

public:
	void SetConstantTable(void);

public:
	void  SetNaviIndex(void);

private:
	LPD3DXEFFECT			m_pEffect;

private:
	Engine::CComponent*		m_pMeshCom;
	Engine::CTransform*		m_pInfo;

private:
	_ulong Release(void);
	_ulong m_dwNaviIdx;

private:
	_float	m_fSpeed;

};

#endif // Player_h__
