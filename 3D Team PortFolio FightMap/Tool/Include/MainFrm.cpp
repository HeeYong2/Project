
// MainFrm.cpp : CMainFrame 클래스의 구현
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
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
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
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	//////////////////////////////////////////////////////////////////////////
	cs.lpszName = _T("3D 맵툴");		    //제목 바꾸기
	cs.style &= ~FWS_ADDTOTITLE;			//제목없음 없애기        
	cs.style &= ~WS_THICKFRAME;				//주석처리(//) 없애고 Test 해보삼=>마우스로 프로그램 크기 조절 못함
	//////////////////////////////////////////////////////////////////////////
	if(cs.hMenu != NULL)
	{
		::DestroyMenu(cs.hMenu);
		cs.hMenu = NULL;
	}
	return CFrameWnd::PreCreateWindow(cs);	//여기 사이가 메뉴바 없애기
	//////////////////////////////////////////////////////////////////////////

	return TRUE;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

// CMainFrame 진단

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

// CMainFrame 메시지 처리기

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// 기본 클래스가 실제 작업을 수행합니다.

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}
	// 모든 사용자 도구 모음에 사용자 지정 단추를 활성화합니다.
	return TRUE;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_MainSplitte.CreateStatic(this, 1, 2);	///앞에것이 행	뒤에것이 열

	m_MainSplitte.CreateView(0,0,RUNTIME_CLASS(CMyForm)
		, CSize(WINSIZEX / 2,WINSIZEY), pContext);

	m_MainSplitte.CreateView(0, 1, RUNTIME_CLASS(CToolView)
		, CSize(WINSIZEX, WINSIZEY), pContext);

	m_MainSplitte.SetColumnInfo(0 , 450 ,0);
	
	m_pMainView = (CToolView*)m_MainSplitte.GetPane(0,1);
	m_pMyForm = (CMyForm*)m_MainSplitte.GetPane(0,0);
	return TRUE;/*return CFrameWndEx::OnCreateClient(lpcs, pContext);*/
}