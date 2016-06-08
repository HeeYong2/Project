// Preview.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "Preview.h"
#include "ObjectView.h"

// CPreview 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPreview, CDialog)

CPreview::CPreview(CWnd* pParent /*=NULL*/)
	: CDialog(CPreview::IDD, pParent)
{
	m_ObjectView = NULL;
}

CPreview::~CPreview()
{
}

void CPreview::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPreview, CDialog)
END_MESSAGE_MAP()


// CPreview 메시지 처리기입니다.

BOOL CPreview::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ObjectView = new CObjectView;

	m_ObjectView->Create(NULL , L"ObjectView" , WS_CHILD | WS_BORDER | WS_VISIBLE
		, CRect(10 , 10 , 400 , 350) , this , 0);

	m_ObjectView->OnInitialUpdate();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CPreview::SetName(TCHAR*	szpName)
{
	if(m_ObjectView != NULL)
		m_ObjectView->SetName(szpName);
}