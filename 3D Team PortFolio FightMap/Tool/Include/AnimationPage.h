#pragma once

#include "Tool_Defines.h"
#include "Layer.h"
#include "MainFrm.h"
#include "afxwin.h"
#include "AnimationView.h"
// CAnimationView 대화 상자입니다.

class CAnimationPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CAnimationPage)

public:
	CAnimationPage();
	virtual ~CAnimationPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ANIMATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:
	HRESULT Ready_UI(void);
	afx_msg void OnLbnSelchangeList();
	afx_msg void OnCbnSelchangeObjectType();

public:
	CAnimationView*			m_pAnimationView;
	CString					m_strSelectName;
	CListBox				m_ListBox;
	CComboBox				m_ObjectType;
	_int					m_iSelectComboNum;						//콤보박스 중 어떤타입을 선택했는지
	_bool					m_isOk;
	_bool					m_isPass;
	TCHAR					m_szName[MAX_PATH];

public:
	map<const TCHAR* , list<const TCHAR*>>			m_MeshPathMap;
	typedef map<const TCHAR* , list<const TCHAR*>>	MAPLIST;
	typedef list<const TCHAR*>						LISTPATH;
	
public:
	list<MESHPATH*>									m_MeshPathlist;
	typedef list<MESHPATH*>							PATHLIST;

public:
	list<MESHPATH*>               m_ObjectPathList;

public:
	list<const TCHAR*>* Find_MeshTag(const TCHAR* pGroupTag);
	void	Set_MeshList(list<const TCHAR*>* pPathList);
	void	LoadMesh(const TCHAR* pMeshTag, const TCHAR* pFullPath ,const TCHAR* pFileTag);
	void	Load_MeshList(void);
	void	Find_MeshFilePath(const TCHAR* pFilePath , const TCHAR* pSavetxtpath , PATHLIST& rPathList);
	void	Set_MeshTagList(PATHLIST* pPathList , MAPLIST* pMapTag);
	void	Save_MeshPath(const TCHAR* pSavetxtPath , PATHLIST& rPathList);


};