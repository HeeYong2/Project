#pragma once

#include "Tool_Defines.h"
#include "Layer.h"
#include "MainFrm.h"
#include "afxwin.h"
#include "AnimationView.h"
// CAnimationView ��ȭ �����Դϴ�.

class CAnimationPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CAnimationPage)

public:
	CAnimationPage();
	virtual ~CAnimationPage();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ANIMATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
	_int					m_iSelectComboNum;						//�޺��ڽ� �� �Ÿ���� �����ߴ���
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