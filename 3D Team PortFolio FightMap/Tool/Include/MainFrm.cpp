
// MainFrm.cpp : CMainFrame Ŭ������ ����
//

#include "stdafx.h"
#include "Tool.h"

#include "MainFrm.h"
#include "ToolView.h"
#include "Tool_Defines.h"
#include "../Codes/MyForm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnUpdateApplicationLook)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ���� �� ǥ�ñ�
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2005);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

CMainFrame::~CMainFrame()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	//////////////////////////////////////////////////////////////////////////
	cs.lpszName = _T("3D ����");		    //���� �ٲٱ�
	cs.style &= ~FWS_ADDTOTITLE;			//������� ���ֱ�        
	cs.style &= ~WS_THICKFRAME;				//�ּ�ó��(//) ���ְ� Test �غ���=>���콺�� ���α׷� ũ�� ���� ����
	//////////////////////////////////////////////////////////////////////////
	if(cs.hMenu != NULL)
	{
		::DestroyMenu(cs.hMenu);
		cs.hMenu = NULL;
	}
	return CFrameWnd::PreCreateWindow(cs);	//���� ���̰� �޴��� ���ֱ�
	//////////////////////////////////////////////////////////////////////////

	return TRUE;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

// CMainFrame ����

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG

//--------------------------------------------------------------------------------------------------------------------------------------------------------

// CMainFrame �޽��� ó����

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// �⺻ Ŭ������ ���� �۾��� �����մϴ�.

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}
	// ��� ����� ���� ������ ����� ���� ���߸� Ȱ��ȭ�մϴ�.
	return TRUE;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	m_MainSplitte.CreateStatic(this, 1, 2);	///�տ����� ��	�ڿ����� ��

	m_MainSplitte.CreateView(0,0,RUNTIME_CLASS(CMyForm)
		, CSize(WINSIZEX / 2,WINSIZEY), pContext);

	m_MainSplitte.CreateView(0, 1, RUNTIME_CLASS(CToolView)
		, CSize(WINSIZEX, WINSIZEY), pContext);

	m_MainSplitte.SetColumnInfo(0 , 450 ,0);
	
	m_pMainView = (CToolView*)m_MainSplitte.GetPane(0,1);
	m_pMyForm = (CMyForm*)m_MainSplitte.GetPane(0,0);
	return TRUE;/*return CFrameWndEx::OnCreateClient(lpcs, pContext);*/
}