
// ToolView.h : CToolView Ŭ������ �������̽�
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

protected: // serialization������ ��������ϴ�.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

// Ư���Դϴ�.
public:
	CToolDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
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

private:									//Frame ����
	_float									m_fTime;
	_matrix									m_MatWorld;
	TCHAR									szFps[128];

private:
	TCHAR									szTerrainNum[128];
	TCHAR									szEditMode[128];

	TCHAR									szPlayerPos[128];
	_vec3									m_vPlayerPos;

private:
	_int									m_iCount;		//������ Cnt
	_int									m_iPage;		//� ��Ʈ���� �Ǿ����� 
	_int									m_iRange;		//�� ������ Range��ġ
	_int									m_iHeight;		//�� ������ Height��ġ
	_int									m_iVtx1Size;	//Terrain1�� VtxSize
	_int									m_iVtx2Size;	//Terrain2�� VtxSize
	_int									m_iNum;			//� �ͷ����� ���� �ߴ���
	_int									m_iEditMode;	//� ������带 ���� �ߴ���
	_int									m_iMixNum;		//��� �ͽ� �ؽ��ĸ� ����ߴ���

private:
	_vec3									m_vPos;			//���� �̵� ��ǥ
	_bool									m_isMove;		//���� �̵��ߴ���
	_bool									m_isPass;		//MFC ȣ����� ���ӹ���
	_bool									m_isTestMode;	//�׽�Ʈ ������� �Ǵ�
	_bool									m_isSplatting;	//���÷��� ������� �Ǵ�

private:
	LPDIRECT3DTEXTURE9						m_pTexture;
	_bool									m_isSplattingMode;
	_bool									m_isTerrainEdit;
	_int									m_iLoadTextureNum;

private:
	bool									m_isInputHome;

};

#ifndef _DEBUG  // ToolView.cpp�� ����� ����
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif