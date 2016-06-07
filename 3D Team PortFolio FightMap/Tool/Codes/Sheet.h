#pragma once
#include "MapPage.h"
#include "ObjectPage.h"
#include "NaviMeshPage.h"
#include "AnimationPage.h"
#include "Tool_Defines.h"

// CSheet

class CSheet : public CPropertySheet
{
public:
	CSheet(void);
	DECLARE_DYNAMIC(CSheet)

public:
	CSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CSheet();

protected:
	DECLARE_MESSAGE_MAP()

public:
	CMapPage		m_MapPage;
	CObjectPage		m_ObjectPage;
	CNaviMeshPage	m_NaviMeshPage;
	CAnimationPage	m_AnimationPage;
};


