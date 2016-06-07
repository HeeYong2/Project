#ifndef SkyBox_h__
#define SkyBox_h__
/********************************************************************
	created:	2015/05/23
	file path:	Yong\Tool\Code
	file base:	SkyBox
	file ext:	h
	author:		Yong	
	purpose:	SkyBox
*********************************************************************/

#include "Tool_Defines.h"
#include "GameObject.h"

class CSkyBox
	: public Engine::CGameObject

{
public:
	explicit CSkyBox(void);
	~CSkyBox(void);

public:
	virtual HRESULT Initialize(CString strSkyName);
	virtual void Updata(void);
	virtual void Render(void);
	virtual _ulong Release(void);

public:
	static CSkyBox*	Create(CString strSkyName);
	HRESULT AddComponent(CString strSkyName);

public:
	void SetSkyNum(_int iSkyNum);
	void SetConstantTable(void);

private:
	Engine::CComponent*			m_pMeshCom;
	LPD3DXEFFECT				m_pEffect;

private:
	_int						m_iSkyNum;
	_float						m_fAngle;
};

#endif // SkyBox_h__