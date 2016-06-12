#pragma once


// CObjectView 뷰입니다.

#include "Layer.h"
#include "Tool_Defines.h"

class CObjectView : public CView
{
	DECLARE_DYNCREATE(CObjectView)

public:
	CObjectView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CObjectView();

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void OnInitialUpdate();

public:
	enum ANGLE {ANGLE_X ,ANGLE_Y ,ANGLE_Z ,ANGLE_END};

public:
	void Render(void);
	void Update(void);

public:
	void SetName(TCHAR*		szpName);	
	HRESULT AddComponent(TCHAR*		szpName);

public:
	map<_int , Engine::CLayer*>				m_mapLayer;
	typedef map<_int , Engine::CLayer*>		MAPLAYER;

private:
	_int									m_iSizeX;
	_int									m_iSizeY;

private:
	_bool									m_isPass;
	TCHAR									m_szName[MAX_PATH];
	Engine::CComponent*						m_pMeshCom;
	Engine::CTransform*						m_pInfo;

private:
	Engine::CGameObject*					m_pGameObject;

public:
	_vec3									m_vScale;
	_vec3									m_vPosition;
	_vec3									m_vDirection;
	_matrix									m_matWorld;
	_float									m_fAngle[ANGLE_END];

private:
	void Release(void);

public:
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};