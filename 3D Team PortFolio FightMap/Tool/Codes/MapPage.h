#pragma once

#include "Tool_Defines.h"
#include "Layer.h"
#include "MainFrm.h"
#include "afxcmn.h"
#include "afxwin.h"
// CMapPage 대화 상자입니다.


class CMapPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CMapPage)
public:
	enum LAYERID {LAYER_ENVIRONMENT , LAYER_GAMELOGIC , LAYER_UI};

public:
	CMapPage();
	virtual ~CMapPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MAPPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	HRESULT Ready_EnvironmentLayer(const _uint& wCntX, const _uint& wCntZ, const float& fItv);		//Terrain생성
	HRESULT Ready_Player(void);		
	HRESULT Ready_Enemy(void);																		//Test용 Player 생성
	//Test용 Player 생성
	HRESULT Ready_StaticCamera(void);																//Test용 StaticCam 생성
	HRESULT Ready_Terrain(const _uint& wCntX, const _uint& wCntZ, const float& fItv);				//Terrain1생성
	HRESULT Ready_UI(void);																			//Dynamic Cam 생성
	HRESULT Ready_SkyBox(CString strSkyName);														//SkyBox 생성
	virtual BOOL OnInitDialog();																	//Map 시트에서의 Initialize

	//Getter
public:
	_int 	GetHeight(void);																		//높이 조절용
	_int 	GetRange(void);																			//범위 조절용
	_int 	GetVtx1Size(void);																		//Terrain1의 VtxSize
	_int 	GetVtx2Size(void);																		//Terrain2의 VtxSize
	_int 	GetNum(void);																			//어떤 터레인을 선택했는지?
	_int	GetMixNum(void);																		//어떤 알파채널인지?
	_int	GetMode(void);																			//어떤 수정버튼이 눌렸는지 ?
	_vec3 	GetPos(void);																			//얼마나 이동시켰는지?
	_bool 	GetMove(void);																			//이동 버튼을 눌렀는지 확인
	_bool	GetTest(void);																			//Test용인지 ( Player가 Static Cam을 기반으로 움직임)
	_bool	GetSplatting(void);																		//Splatting이 눌렸는지
	_bool	GetSplattingMode(void);																	//Splatting Mode가 그리기인지 지우기인지

	//Setter
public:
	void SetMove(_bool isMove);
	void SetRadio(_int iSelectRadio);

public:
	afx_msg void OnNMCustomdrawHeight(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawRange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedWireFrame();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnBnClickedApplyTerrain();

public:
	afx_msg void OnBnClickedRadio();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedMoveApply();

public:
	afx_msg void OnBnClickedApplySkyBox();
	afx_msg void OnBnClickedLoadSkyBox();

public:
	afx_msg void OnBnClickedUpdown();
	afx_msg void OnBnClickedPlateau();
	afx_msg void OnBnClickedEditFlat();
	afx_msg void OnBnClickedTestMode();

public:
	afx_msg void OnBnClickedTextureLoad();
	afx_msg void OnBnClickedTextureApply();
	afx_msg void OnBnClickedSplatting();
	afx_msg void OnNMCustomdrawUVRatio(NMHDR *pNMHDR, LRESULT *pResult);

public:
	afx_msg void OnBnClickedMix1Load();
	afx_msg void OnBnClickedMix2Load();
	afx_msg void OnBnClickedMix3Load();
	afx_msg void OnBnClickedMix4Load();
	afx_msg void OnBnClickedMix1();
	afx_msg void OnBnClickedMix2();
	afx_msg void OnBnClickedMix3();
	afx_msg void OnBnClickedMix4();

public:
	afx_msg void OnBnClickedDrawSplatting();
	afx_msg void OnBnClickedEraseSplatting();
	afx_msg void OnBnClickedSplattingSave();
	afx_msg void OnBnClickedSplattingLoad();
	afx_msg void OnNMCustomdrawMixRatio(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedEditTerrainEdit();

public:
	wstring ConvertRelativePath(const wstring& szFullPath);
	_vec3   GetPlayerPos(void);
	_bool	GetEditTerrain(void);

private:
	void Release(void);

private:
	map<_int, Engine::CLayer*>*			m_pMapLayer;
	typedef map<_int, Engine::CLayer*>	MAPLAYER;
	CMainFrame*							m_pMainFrm;

private:
	list<VTXTEX*>						m_VtxList;
	list<VTXTEX*>						m_VtxList2;
	typedef list<VTXTEX*>				VTXLIST;			

private:
	_int m_iVtxSizeX;										//생성한 터레인의 사이즈X
	_int m_iVtxSizeZ;										//생성한 터레인의 사이즈Z
	_int m_iVtxItv;											//생성한 터레인의 간격
	_int m_iNum;											//어떤 터레인을 선택했는지
	_int m_iHeight;											//수정용 Height
	_int m_iRange;											//수정용 Range
	_int m_iVtx1Size;										//Terrain1의 사이즈
	_int m_iVtx2Size;										//Terrain2의 사이즈
	_int m_iSkyNum;											//몇번째 SkyBox 파일이 선택되었는지 판단
	_int m_iEditMode;										//어떤 Edit를 선택했는지 판단
	_int m_iMixNum;											//몇번 Mix가 선택 되었는지
	_int m_iMix1Num;										//몇번째 Terrain Texture를 그리는지?	
	_int m_iMix2Num;										//몇번째 Terrain Texture를 그리는지?	
	_int m_iMix3Num;										//몇번째 Terrain Texture를 그리는지?	
	_int m_iMix4Num;										//몇번째 Terrain Texture를 그리는지?	
	_int m_iTerrain1TextureNum[4];
	_int m_iTerrain2TextureNum[4];
	_int m_iBaseNum;
	_int m_iBaseTextureNum[2];
private:
	_float m_fMoveX;										//X방향으로 이동
	_float m_fMoveY;										//Y방향으로 이동
	_float m_fMoveZ;										//Z방향으로 이동
	_float m_fUVRatio;										//UV좌표
	_float m_fTerrain1MixRatio[4];
	_float m_fTerrain2MixRatio[4];
	_float m_fMixRatio;
	_float m_fTerrainUVRatio[2];
private:
	_bool	m_bMove;										//Terrain이 이동했는지 판단하는 bool형 변수
	_bool	m_isPlayer;										//Player가 만들어졌는지 판단하는 bool형 변수
	_bool	m_isSkyRender;									//SkyBox가 그려졌는지 판단하는 bool형 변수
	_bool	m_isSplatting;									//스플래팅을 눌렀는지 판단하는 bool형 변수
	_bool	m_isDrawingSplatting;							//스플래팅이 그리기인지 지우기 인지 판단
	_bool	m_isTest;										//Test버튼이 눌렸는지 판단하는 bool형 변수

private:
	CButton 	m_ctrEditUpdown;							//Up & Down 수정 버튼이 눌렸는지 확인
	CButton 	m_ctrEditPlateau;							//Plateau 수정 버튼이 눌렸는지 확인
	CButton 	m_ctrEditFlat;								//Flat 수정버튼이 눌렸는지 확인

public:
	CButton 	m_ctrTestMode;								//TestMode가 눌렸는지 확인(Public으로하자)

private:
	CButton 	m_WireFrame;								//WireFrame을 On / Off함
	CButton 	m_Terrain1;									//Terrain1를 생성하는 버튼
	CButton 	m_Terrain2;									//Terrain2를 생성하는 버튼
	CButton		m_ctrSplatting;								//Splatting을 눌렀는지 확인
	CButton 	m_ctrMix1;									//Mix1의 라디오버튼을 눌렀는지 확인
	CButton 	m_ctrMix2;									//Mix2의 라디오버튼을 눌렀는지 확인
	CButton 	m_ctrMix3;									//Mix3의 라디오버튼을 눌렀는지 확인
	CButton 	m_ctrMix4;									//Mix4의 라디오버튼을 눌렀는지 확인
	CButton 	m_ctrDrawSplatting;							//SPlatting이 그리기인지 
	CButton 	m_ctrEraseSplatting;						//Splatting이 지우기 인지 판다
	CButton		m_ctrTerrainEDit;

	CSliderCtrl m_ctrHeight;								//Height를 조절하는 슬라이더컨트롤
	CSliderCtrl m_ctrRange;									//Range를 조절하는 슬라이더컨트롤
	CSliderCtrl m_ctrUVRatio;								//UV를 조절하는 슬라이더컨트롤
	CSliderCtrl m_ctrMixRatio;								//Mix된 Uv를 조절하는 슬라이더

private:
	CString 	m_SkyName;									//SkyBox 교체시 입력받는 이름 , Edit박스의 이름이기도 함
	CString 	m_CreateSkyName;							//생성되는 SkyBox의 이름
	CString 	m_BaseTexture;								//Base Texture 교체시 입력받는 이름
	CString 	m_Mix1Name;									//Mix1의 이름			
	CString 	m_Mix2Name;									//Mix2의 이름
	CString 	m_Mix3Name;									//Mix3의 이름
	CString 	m_Mix4Name;									//Mix4의 이름

public:
	Engine::CGameObject*		m_pStaticCamera;			//Static Cam 생성 멤버변수
	Engine::CGameObject*		m_pDynamicCamera;			//Dynamic Cam 생성 멤버변수
	Engine::CGameObject*		m_pSkyBox;					//SkyBox 생성 멤버변수
	Engine::CGameObject*		m_pTerrain;					//Terrain1 생성 멤버변수
	Engine::CGameObject*		m_pSecondTerrain;			//Terrain2 생성 멤버변수
	Engine::CGameObject*		m_pPlayer;					//Test용 Player 멤버변수
	Engine::CGameObject*		m_pEnemy;					//Test용 Player 멤버변수


private:
	LPDIRECT3DTEXTURE9			m_pAlphaTexture[4];
	LPDIRECT3DSURFACE9			m_pTargetSurface[4];
	IDirect3DBaseTexture9*		m_pTexture;

private:
	wstring						m_strBaseTextureName[2];
	wstring						m_strTerrain1TextureName[4];		//1터레인
	wstring						m_strTerrain2TextureName[4];		//2터레인

private:
	TEXINFO						m_tTexInfo;

private:
	_float						m_fsliderRatio;
	_bool						m_isEditTerrain;
};