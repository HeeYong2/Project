#pragma once

#include "Tool_Defines.h"
#include "afxwin.h"
#include "Layer.h"
#include "..\..\Engine\Utility\Codes\NaviMesh.h"


// CNaviMeshPage 대화 상자입니다.

class CMainFrame;
class CNaviMeshPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CNaviMeshPage)

public:
	enum LAYERID {LAYER_ENVIRONMENT , LAYER_GAMELOGIC , LAYER_UI};

public:
	CNaviMeshPage();
	virtual ~CNaviMeshPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_NAVIMESHPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedDeleteLastCell();
	afx_msg void OnBnClickedDeleteAllCell();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnBnClickedRenderNavi();
	afx_msg void OnBnClickedPickingTerrain();
	afx_msg void OnBnClickedPickingMesh();

public:
	virtual BOOL OnInitDialog();

public:
	void AddMode(void);
	void ModifyMode(void);
	void Ready_NaviMesh(void);
	void SetRender(_bool isRender);

private:
	CButton		m_ctrAdd;					//Add 버튼
	CButton		m_ctrModify;				//Modify 버튼
	CButton m_ctrNaviRender;
	CButton m_ctrPickingTerrain;
	CButton m_ctrPickingMesh;

	_vec3		m_vPoint[3];				//Point배열
	_int		m_iCellCnt;					//Cell의 Cnt
	_int		m_iType;
	//네비메시 생성
private:
	vector<Engine::CNaviMesh*>						m_vecNaviMesh;
	typedef vector<Engine::CNaviMesh*>				VECNAVI;

private:
	Engine::CGameObject*							m_pNaviFlag;

private:
	map<_int, Engine::CLayer*>*						m_pMapLayer;
	typedef map<_int, Engine::CLayer*>				MAPLAYER;

private:
	typedef list<Engine::CGameObject*>							LISTOBJECT;

private:
	CMainFrame*										m_pMainFrm;
	_bool											m_isRender;
	_bool											m_isNaviRender;

public:
	_bool GetNaviRender(void);

	float m_fFirstNaviPosX;
	float m_fFirstNaviPosY;
	float m_fFirstNaviPosZ;
	float m_fSecNaviPosX;
	float m_fSecNaviPosY;
	float m_fSecNaviPosZ;
	float m_fThirdNaviPosX;
	float m_fThirdNaviPosY;
	float m_fThirdNaviPosZ;
	float m_fSelNaviPosX;
	float m_fSelNaviPosY;
	float m_fSelNaviPosZ;

	afx_msg void OnBnClickedLinkNavi();
	CButton m_ctrLinkNavi;
};