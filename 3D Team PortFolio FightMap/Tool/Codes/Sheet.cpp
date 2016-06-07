// Sheet.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "Sheet.h"


// CSheet

IMPLEMENT_DYNAMIC(CSheet, CPropertySheet)
//--------------------------------------------------------------------------------------------------------------------------------------------------------

CSheet::CSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

CSheet::CSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{

}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

CSheet::CSheet()
{
	AddPage(&m_MapPage);
	AddPage(&m_ObjectPage);
	AddPage(&m_NaviMeshPage);
	AddPage(&m_AnimationPage);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

CSheet::~CSheet()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------


BEGIN_MESSAGE_MAP(CSheet, CPropertySheet)
END_MESSAGE_MAP()


// CSheet 메시지 처리기입니다.
