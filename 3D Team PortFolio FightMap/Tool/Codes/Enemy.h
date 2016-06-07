#ifndef Enemy_h__
#define Enemy_h__
/********************************************************************
	created:	2015/05/04
	file path:	Yong\Tool\Code
	file base:	Enemy
	file ext:	h
	author:		Yong	
	purpose:	Test 용도의 플레이어
*********************************************************************/
#include "Tool_Defines.h"
#include "GameObject.h"

class CEnemy
	: public Engine::CGameObject
{
public:
	explicit CEnemy(void);
	~CEnemy(void);

public:
	virtual HRESULT Initialize(void);
	virtual void Update(void);
	virtual void Render(void);


public:
	static CEnemy* Create(void);
	void Render_Mesh(void);

public:
	HRESULT AddComponent(void);
	void KeyCheck(void);

public:
	Engine::CTransform*	GetEnemyInfo(void);
	_vec3	GetEnemyPos(void);

public:
	void SetConstantTable(void);

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

#endif // Enemy_h__
