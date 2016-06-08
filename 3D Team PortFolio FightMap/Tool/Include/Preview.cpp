// Preview.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "Preview.h"
#include "ObjectView.h"

// CPreview ��ȭ �����Դϴ�.

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


// CPreview �޽��� ó�����Դϴ�.

BOOL CPreview::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_ObjectView = new CObjectView;

	m_ObjectView->Create(NULL , L"ObjectView" , WS_CHILD | WS_BORDER | WS_VISIBLE
		, CRect(10 , 10 , 400 , 350) , this , 0);

	m_ObjectView->OnInitialUpdate();
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CPreview::SetName(TCHAR*	szpName)
{
	if(m_ObjectView != NULL)
		m_ObjectView->SetName(szpName);
}