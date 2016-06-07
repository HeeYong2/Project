#pragma once
#include "afxwin.h"
#include "Tool_Defines.h"
#include "afxcmn.h"
#include "Preview.h"
#include "Export_Function.h"


// CObjectPage 대화 상자입니다.

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

// 대화 상자 데이터입니다.
	enum { IDD = IDD_OBJECTPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
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


	//오브젝트 저장용
private:
	list<OBJINFO*>												m_ObjList;
	typedef list<OBJINFO*>										OBJLIST;

	//오브젝트 생성용
private:
	CMainFrame*								m_pMainFrm;
	map<_int, Engine::CLayer*>*				m_pMapLayer;

private:
	CButton 			m_ctrModify;							//수정버튼이 눌렸는지 
	CButton 			m_ctrPicking;							//피킹 체크박스가 눌렸는지
	CButton 			m_ctrCollider;							//충돌용 체크박스가 눌렸는지
	CButton				m_ctrAdd;								//Add버튼이 눌렸는지
	CButton	 			m_ctrMoveMinusZ;						//Z의 마이너스 이동값
	CButton	 			m_ctrMovePlusZ;							//Z의 플러스 이동값
	CButton	 			m_ctrMovePlusY;							//Y의 플러스 이동값
	CButton	 			m_ctrMoveMinusY;						//Y의 마이너스 이동값
	CButton	 			m_ctrMoveMinusX;						//X의 마이너스 이동값
	CButton	 			m_ctrMovePlusX;							//X의 플러스 이동값
	CButton				m_ctrPreview;							//Preview를 실행시키는 버튼
	CButton				m_Delete;								//Object를 지우자
	CSliderCtrl 		m_ctrScaleX;							//슬라이더 컨트롤의 ScaleX
	CSliderCtrl 		m_ctrScaleY;							//슬라이더 컨트롤의 ScaleY
	CSliderCtrl 		m_ctrScaleZ;							//슬라이더 컨트롤의 ScaleZ
	CSliderCtrl 		m_ctrAngleX;							//슬라이더 컨트롤의 AngleX
	CSliderCtrl 		m_ctrAngleY;							//슬라이더 컨트롤의 AngleY
	CSliderCtrl 		m_ctrAngleZ;							//슬라이더 컨트롤의 AngleZ
	CComboBox			m_ObjectType;							//어떤 타입의 오브젝트인지?
	CListBox			m_ListBox;								//Listbox용 변수

private:
	_int				m_iSelectComboNum;						//콤보박스 중 어떤타입을 선택했는지
	CString				m_strSelectName;						//리스트박스에서 누른 문자값
	CPreview			m_Preview;								//Preview의 함수

private:
	_float 				m_fScaleX;								//ScaleX 변수
	_float 				m_fScaleY;								//ScaleY 변수
	_float 				m_fScaleZ;								//ScaleZ 변수
	_float				m_fAngleX;								//AngleX 변수
	_float				m_fAngleY;								//AngleY 변수
	_float				m_fAngleZ;								//AngleZ 변수
	_float 				m_fPosX;								//PosX   변수
	_float 				m_fPosY;								//PosY   변수	
	_float 				m_fPosZ;								//PosZ   변수
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