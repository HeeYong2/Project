
// ToolView.h : CToolView 클래스의 인터페이스
//


#pragma once
#include "Layer.h"
#include "Tool_Defines.h"

class CToolDoc;
class CSheet;
class CTerrainTool;
class CMainFrame;
class CGameObject;
class CToolView : public CView
{
public:
	enum PAGETYPE	{MAPPAGE, OBJECTPAGE , NAVIPAGE };

protected: // serialization에서만 만들어집니다.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

// 특성입니다.
public:
	CToolDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

public:
	void Render(void);
	void Update(void);

public:
	void SetTerrain(Engine::CGameObject* pTerrain);
	void SetSecondTerrain(Engine::CGameObject* pTerrain);
	void SelectPage(void);
	void SetMapHeight(void);
	void Splatting(void);

public:
	void KeyCheck(void);
	void TerrainKeyCheck(void);
	void NaviKeyCheck(void);
	void CheckMove(void);

public:			//Getter
	list<Engine::CGameObject*>*				GetList(const TCHAR*	pObejctTag);

private:
	CSheet*				  					m_pSheet;
	Engine::CGameObject*  					m_pTerrain;
	Engine::CGameObject*  					m_pSecondTerrain;

private:
	LPDIRECT3DDEVICE9						m_pGraphicDev;
	CMainFrame*								m_pMainFrm;

public:
	map<_int , Engine::CLayer*>				m_mapLayer;
	typedef map<_int , Engine::CLayer*>		MAPLAYER;

private:									//Frame 띄우기
	_float									m_fTime;
	_matrix									m_MatWorld;
	TCHAR									szFps[128];

private:
	TCHAR									szTerrainNum[128];
	TCHAR									szEditMode[128];

	TCHAR									szPlayerPos[128];
	_vec3									m_vPlayerPos;

private:
	_int									m_iCount;		//프레임 Cnt
	_int									m_iPage;		//어떤 시트선택 되었는지 
	_int									m_iRange;		//맵 수정의 Range수치
	_int									m_iHeight;		//맵 수정의 Height수치
	_int									m_iVtx1Size;	//Terrain1의 VtxSize
	_int									m_iVtx2Size;	//Terrain2의 VtxSize
	_int									m_iNum;			//어떤 터레인이 선택 했는지
	_int									m_iEditMode;	//어떤 수정모드를 선택 했는지
	_int									m_iMixNum;		//몇번 믹스 텍스쳐를 사용했는지

private:
	_vec3									m_vPos;			//맵의 이동 좌표
	_bool									m_isMove;		//맵을 이동했는지
	_bool									m_isPass;		//MFC 호출순서 꼬임방지
	_bool									m_isTestMode;	//테스트 모드인지 판단
	_bool									m_isSplatting;	//스플래팅 모드인지 판단

private:
	LPDIRECT3DTEXTURE9						m_pTexture;
	_bool									m_isSplattingMode;
	_bool									m_isTerrainEdit;
	_int									m_iLoadTextureNum;

private:
	bool									m_isInputHome;

};

#ifndef _DEBUG  // ToolView.cpp의 디버그 버전
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif