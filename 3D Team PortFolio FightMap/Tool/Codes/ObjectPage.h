#pragma once
#include "afxwin.h"
#include "Tool_Defines.h"
#include "afxcmn.h"
#include "Preview.h"
#include "Export_Function.h"


// CObjectPage ��ȭ �����Դϴ�.

class CResource;
class CMainFrame;
class Engine::CGameObject;
class CObjectPage : public CPropertyPage
{
public:
	enum LAYERID {LAYER_ENVIRONMENT , LAYER_GAMELOGIC , LAYER_UI};
	
	DECLARE_DYNAMIC(CObjectPage)

public:
	CObjectPage();
	virtual ~CObjectPage();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_OBJECTPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	DECLARE_MESSAGE_MAP()

public:
	HRESULT Ready_EnvironmentLayer(OBJINFO* ObjectInfo);

public:
	virtual BOOL OnInitDialog();
	void Release(void);

public:
	afx_msg void OnLbnSelchangeList();
	afx_msg void OnCbnSelchangeObjectType();
	afx_msg void OnBnClickedPickingBox();
	afx_msg void OnBnClickedColliderBox();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnBnClickedMoveMinusZ();
	afx_msg void OnBnClickedMovePlusZ();
	afx_msg void OnBnClickedMovePlusY();
	afx_msg void OnBnClickedMoveMinusY();
	afx_msg void OnBnClickedMoveMinusX();
	afx_msg void OnBnClickedMovePlusX();
	afx_msg void OnBnClickedPreview();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnNMCustomdrawScaleX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawScaleY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawScaleZ(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawAngleX(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawAngleY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawAngleZ(NMHDR *pNMHDR, LRESULT *pResult);

public:
	void	LoadMesh(const TCHAR* pMeshTag, const TCHAR* pFullPath ,const TCHAR* pFileTag);
	void	GetType(void);

private:
	map<const TCHAR*, Engine::CResource*>*			m_pMapResource;
	typedef map<const TCHAR*, Engine::CResource*>	MAPRESOURCE;

private:
	map<const TCHAR* , list<const TCHAR*>>			m_MeshPathMap;
	typedef map<const TCHAR* , list<const TCHAR*>>	MAPLIST;
	typedef list<const TCHAR*>						LISTPATH;
	list<MESHPATH*>									m_MeshPathlist;
	typedef list<MESHPATH*>							PATHLIST;

private:
	OBJINFO											m_tObjectInfo;
	Engine::CTransform*								m_pObjectInfo;

private:
 	map<const TCHAR*, list<Engine::CGameObject*>>				m_mapObject;
 	typedef list<Engine::CGameObject*>							LISTOBJECT;
 	typedef map<const TCHAR*, list<Engine::CGameObject*>>		MAPOBJECT;

private:
	map<_int , Engine::CLayer*>				m_mapLayer;
	typedef map<_int , Engine::CLayer*>		MAPLAYER;


	//������Ʈ �����
private:
	list<OBJINFO*>												m_ObjList;
	typedef list<OBJINFO*>										OBJLIST;

	//������Ʈ ������
private:
	CMainFrame*								m_pMainFrm;
	map<_int, Engine::CLayer*>*				m_pMapLayer;

private:
	CButton 			m_ctrModify;							//������ư�� ���ȴ��� 
	CButton 			m_ctrPicking;							//��ŷ üũ�ڽ��� ���ȴ���
	CButton 			m_ctrCollider;							//�浹�� üũ�ڽ��� ���ȴ���
	CButton				m_ctrAdd;								//Add��ư�� ���ȴ���
	CButton	 			m_ctrMoveMinusZ;						//Z�� ���̳ʽ� �̵���
	CButton	 			m_ctrMovePlusZ;							//Z�� �÷��� �̵���
	CButton	 			m_ctrMovePlusY;							//Y�� �÷��� �̵���
	CButton	 			m_ctrMoveMinusY;						//Y�� ���̳ʽ� �̵���
	CButton	 			m_ctrMoveMinusX;						//X�� ���̳ʽ� �̵���
	CButton	 			m_ctrMovePlusX;							//X�� �÷��� �̵���
	CButton				m_ctrPreview;							//Preview�� �����Ű�� ��ư
	CButton				m_Delete;								//Object�� ������
	CSliderCtrl 		m_ctrScaleX;							//�����̴� ��Ʈ���� ScaleX
	CSliderCtrl 		m_ctrScaleY;							//�����̴� ��Ʈ���� ScaleY
	CSliderCtrl 		m_ctrScaleZ;							//�����̴� ��Ʈ���� ScaleZ
	CSliderCtrl 		m_ctrAngleX;							//�����̴� ��Ʈ���� AngleX
	CSliderCtrl 		m_ctrAngleY;							//�����̴� ��Ʈ���� AngleY
	CSliderCtrl 		m_ctrAngleZ;							//�����̴� ��Ʈ���� AngleZ
	CComboBox			m_ObjectType;							//� Ÿ���� ������Ʈ����?
	CListBox			m_ListBox;								//Listbox�� ����

private:
	_int				m_iSelectComboNum;						//�޺��ڽ� �� �Ÿ���� �����ߴ���
	CString				m_strSelectName;						//����Ʈ�ڽ����� ���� ���ڰ�
	CPreview			m_Preview;								//Preview�� �Լ�

private:
	_float 				m_fScaleX;								//ScaleX ����
	_float 				m_fScaleY;								//ScaleY ����
	_float 				m_fScaleZ;								//ScaleZ ����
	_float				m_fAngleX;								//AngleX ����
	_float				m_fAngleY;								//AngleY ����
	_float				m_fAngleZ;								//AngleZ ����
	_float 				m_fPosX;								//PosX   ����
	_float 				m_fPosY;								//PosY   ����	
	_float 				m_fPosZ;								//PosZ   ����
	_float				m_fScaleRatio;

public:
	void Load_MeshList(void);
	void Find_MeshFilePath(const TCHAR* pFilePath , const TCHAR* pSavetxtpath , PATHLIST& rPathList);
	void Save_MeshPath(const TCHAR* pSavetxtPath , PATHLIST& rPathList);
	void Set_MeshTagList(PATHLIST* pPathList , MAPLIST* pMapTag);
	list<const TCHAR*>* Find_MeshTag(const TCHAR* pGroupTag);
	void Set_MeshList(list<const TCHAR*>* pPathList);

private:
	list<MESHPATH*>               m_ObjectPathList;
public:
	afx_msg void OnBnClickedObjectWireFrame();
	afx_msg void OnBnClickedBillBorad();

private:
	_bool		m_isInstancing;
	_bool		m_isNeedUV;
	CButton 	m_ctrObjectWireFrame;
	CButton 	m_ctrBillBorad;
	Engine::CGameObject*		m_pToolObject;

private:
	multimap<const TCHAR* , vector<int>>	m_MapInstancing;
	vector<int>								m_VecInt;
	typedef multimap<const TCHAR* , vector<int>> MAPINSTANCING;

	
public:
	afx_msg void OnBnClickedNeedUV();
	CButton m_ctrNeedUV;
};