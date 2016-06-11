#pragma once

#include "Layer.h"
#include "Tool_Defines.h"

// CAnimationView ���Դϴ�.

class CAnimationView : public CView
{
	DECLARE_DYNCREATE(CAnimationView)

public:
	CAnimationView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CAnimationView();

public:
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	enum ANGLE {ANGLE_X ,ANGLE_Y ,ANGLE_Z ,ANGLE_END};

public:
	Engine::CComponent*						m_pMeshCom;
	Engine::CComponent*						m_pBufferCom;
	Engine::CTransform*						m_pInfo;

public:
	void Render(void);
	void Update(void);
	HRESULT	AddComponent(TCHAR*		szpName);

private:
	void Release(void);

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	void SetName(TCHAR* szpName, CListBox* pListBox);



public:
	_matrix									m_matWorld;
	TCHAR									m_szName[MAX_PATH];
	_bool									m_isOk;
	_bool									m_isPass;
	_vec3									m_vScale;
	_vec3									m_vPosition;
	_vec3									m_vDirection;
	_float									m_fAngle[ANGLE_END];

};


